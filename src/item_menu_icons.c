#include "global.h"
#include "berry.h"
#include "decompress.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_icon.h"
#include "item_menu_icons.h"
#include "malloc.h"
#include "menu_helpers.h"
#include "menu.h"
#include "sprite.h"
#include "window.h"
#include "util.h"
#include "constants/items.h"

enum {
    TAG_BAG_GFX = 100,
    TAG_ROTATING_BALL_GFX,
    TAG_ITEM_ICON,
    TAG_ITEM_ICON_ALT,
};

enum { //MOD CONTEST This one lists every berry season sprite (Yes, it was better than loading 16 sprites along with the table) and the graf.
    TAG_BERRY_SEASONS_PAL,
    TAG_BERRY_SEASONS_TABLE_GFX,
    TAG_BERRY_SEASONS_X_GFX,
    TAG_BERRY_SEASONS_QUESTION_GFX,
    TAG_BERRY_FLAVOR_GRAF_GFX,
    TAG_BERRY_FLOWER_GFX,
    TAG_FLAVOR_PROFILE, // Yes, they all share the same tag, not like they will load at the same time anyway.
    TAG_FLAVOR_PROFILE_ENIGMA, //This one is too big so, it get's it's own sprite
};

#define TAG_BERRY_CHECK_CIRCLE_GFX 10000
#define TAG_BERRY_PIC_PAL 30020

struct TilesPal
{
    const u32 *tiles;
    const u16 *pal;
};

// this file's functions
static void SpriteCB_BagVisualSwitchingPockets(struct Sprite *sprite);
static void SpriteCB_ShakeBagSprite(struct Sprite *sprite);
static void SpriteCB_SwitchPocketRotatingBallInit(struct Sprite *sprite);
static void SpriteCB_SwitchPocketRotatingBallContinue(struct Sprite *sprite);
static void SpriteCB_BerryFlavorGrafUPright(struct Sprite *sprite);//
static void SpriteCB_BerryFlavorGrafDOWNleft(struct Sprite *sprite);//
static void SpriteCB_BerryFlavorGrafDOWNright(struct Sprite *sprite);//

// static const rom data
static const u16 sRotatingBall_Pal[] = INCBIN_U16("graphics/bag/rotating_ball.gbapal");
static const u8 sRotatingBall_Gfx[] = INCBIN_U8("graphics/bag/rotating_ball.4bpp");
static const u8 sCherryUnused[] = INCBIN_U8("graphics/unused/cherry.4bpp");
static const u16 sCherryUnused_Pal[] = INCBIN_U16("graphics/unused/cherry.gbapal");

static const struct OamData sBagOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sSpriteAnim_Bag_Closed[] =
{
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_Items[] =
{
    ANIMCMD_FRAME(64, 4),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_KeyItems[] =
{
    ANIMCMD_FRAME(128, 4),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_Pokeballs[] =
{
    ANIMCMD_FRAME(192, 4),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_TMsHMs[] =
{
    ANIMCMD_FRAME(256, 4),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_Bag_Berries[] =
{
    ANIMCMD_FRAME(320, 4),
    ANIMCMD_END
};

static const union AnimCmd *const sBagSpriteAnimTable[] =
{
    [POCKET_ITEMS]      = sSpriteAnim_Bag_Items,
    [POCKET_POKE_BALLS] = sSpriteAnim_Bag_Pokeballs,
    [POCKET_TM_HM]      = sSpriteAnim_Bag_TMsHMs,
    [POCKET_BERRIES]    = sSpriteAnim_Bag_Berries,
    [POCKET_KEY_ITEMS]  = sSpriteAnim_Bag_KeyItems,
    [POCKET_DUMMY]       = sSpriteAnim_Bag_Closed,
};

static const union AffineAnimCmd sSpriteAffineAnim_BagNormal[] =
{
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_BagShake[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 254, 2),
    AFFINEANIMCMD_FRAME(0, 0, 2, 4),
    AFFINEANIMCMD_FRAME(0, 0, 254, 4),
    AFFINEANIMCMD_FRAME(0, 0, 2, 2),
    AFFINEANIMCMD_END
};

enum {
    ANIM_BAG_NORMAL,
    ANIM_BAG_SHAKE,
};

static const union AffineAnimCmd *const sBagAffineAnimCmds[] =
{
    [ANIM_BAG_NORMAL] = sSpriteAffineAnim_BagNormal,
    [ANIM_BAG_SHAKE]  = sSpriteAffineAnim_BagShake
};

const struct CompressedSpriteSheet gBagMaleSpriteSheet =
{
    gBagMaleTiles, 0x3000, TAG_BAG_GFX
};

const struct CompressedSpriteSheet gBagFemaleSpriteSheet =
{
    gBagFemaleTiles, 0x3000, TAG_BAG_GFX
};

const struct SpritePalette gBagPaletteTable =
{
    gBagPalette, TAG_BAG_GFX
};

static const struct SpriteTemplate sBagSpriteTemplate =
{
    .tileTag = TAG_BAG_GFX,
    .paletteTag = TAG_BAG_GFX,
    .oam = &sBagOamData,
    .anims = sBagSpriteAnimTable,
    .images = NULL,
    .affineAnims = sBagAffineAnimCmds,
    .callback = SpriteCallbackDummy,
};

static const struct OamData sRotatingBallOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 4,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sSpriteAffineAnim_RotatingBallStationary[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sRotatingBallSpriteAnimTable[] =
{
    sSpriteAffineAnim_RotatingBallStationary
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingBallRotation1[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 8, 16),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_RotatingBallRotation2[] =
{
    AFFINEANIMCMD_FRAME(0, 0, 248, 16),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sRotatingBallAnimCmds[] =
{
    sSpriteAffineAnim_RotatingBallRotation1,
};

static const union AffineAnimCmd *const sRotatingBallAnimCmds_FullRotation[] =
{
    sSpriteAffineAnim_RotatingBallRotation2,
};

static const struct SpriteSheet sRotatingBallTable =
{
    sRotatingBall_Gfx, sizeof(sRotatingBall_Gfx), TAG_ROTATING_BALL_GFX
};

static const struct SpritePalette sRotatingBallPaletteTable =
{
    sRotatingBall_Pal, TAG_ROTATING_BALL_GFX
};

static const struct SpriteTemplate sRotatingBallSpriteTemplate =
{
    .tileTag = TAG_ROTATING_BALL_GFX,
    .paletteTag = TAG_ROTATING_BALL_GFX,
    .oam = &sRotatingBallOamData,
    .anims = sRotatingBallSpriteAnimTable,
    .images = NULL,
    .affineAnims = sRotatingBallAnimCmds,
    .callback = SpriteCB_SwitchPocketRotatingBallInit,
};

static const struct OamData sBerryPicOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 7,
    .affineParam = 0
};

static const struct OamData sBerryPicRotatingOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 7,
    .affineParam = 0
};

static const union AnimCmd sAnim_BerryPic[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sBerryPicSpriteAnimTable[] =
{
    sAnim_BerryPic
};

static const struct SpriteTemplate sBerryPicSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = TAG_BERRY_PIC_PAL,
    .oam = &sBerryPicOamData,
    .anims = sBerryPicSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const union AffineAnimCmd sSpriteAffineAnim_BerryPicRotation1[] =
{
    AFFINEANIMCMD_FRAME(-1, -1, 253, 96),
    AFFINEANIMCMD_FRAME(0, 0, 0, 16),
    AFFINEANIMCMD_FRAME(-2, -2, 255, 64),
    AFFINEANIMCMD_FRAME(-8, 0, 0, 16),
    AFFINEANIMCMD_FRAME(0, -8, 0, 16),
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd sSpriteAffineAnim_BerryPicRotation2[] =
{
    AFFINEANIMCMD_FRAME(-1, -1, 3, 96),
    AFFINEANIMCMD_FRAME(0, 0, 0, 16),
    AFFINEANIMCMD_FRAME(-2, -2, 1, 64),
    AFFINEANIMCMD_FRAME(-8, 0, 0, 16),
    AFFINEANIMCMD_FRAME(0, -8, 0, 16),
    AFFINEANIMCMD_FRAME(256, 256, 0, 0),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd *const sBerryPicRotatingAnimCmds[] =
{
    sSpriteAffineAnim_BerryPicRotation1,
    sSpriteAffineAnim_BerryPicRotation2
};

static const struct SpriteTemplate sBerryPicRotatingSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = TAG_BERRY_PIC_PAL,
    .oam = &sBerryPicRotatingOamData,
    .anims = sBerryPicSpriteAnimTable,
    .images = NULL,
    .affineAnims = sBerryPicRotatingAnimCmds,
    .callback = SpriteCallbackDummy,
};

const struct CompressedSpriteSheet gBerryFlowerSpriteSheet[] = // MOD CONTEST Berry Flower Sprites. Don't ask, I don't know.
{
    [ITEM_TO_BERRY(ITEM_CHERI_BERRY)]          = {gFlowerPic_Cheri,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Cheri},
    [ITEM_TO_BERRY(ITEM_CHESTO_BERRY)]          = {gFlowerPic_Chesto, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Chesto},
    [ITEM_TO_BERRY(ITEM_PECHA_BERRY)]          = {gFlowerPic_Pecha,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pecha},
    [ITEM_TO_BERRY(ITEM_RAWST_BERRY)]          = {gFlowerPic_Rawst,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rawst},
    [ITEM_TO_BERRY(ITEM_ASPEAR_BERRY)]          = {gFlowerPic_Aspear, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Aspear},
    [ITEM_TO_BERRY(ITEM_LEPPA_BERRY)]          = {gFlowerPic_Leppa,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Leppa},
    [ITEM_TO_BERRY(ITEM_ORAN_BERRY)]          = {gFlowerPic_Oran,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Oran},
    [ITEM_TO_BERRY(ITEM_PERSIM_BERRY)]          = {gFlowerPic_Persim, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Persim},
    [ITEM_TO_BERRY(ITEM_LUM_BERRY)]          = {gFlowerPic_Lum,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Lum},
    [ITEM_TO_BERRY(ITEM_SITRUS_BERRY)]          = {gFlowerPic_Sitrus, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Sitrus},
    [ITEM_TO_BERRY(ITEM_FIGY_BERRY)]          = {gFlowerPic_Figy,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Figy},
    [ITEM_TO_BERRY(ITEM_WIKI_BERRY)]          = {gFlowerPic_Wiki,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Wiki},
    [ITEM_TO_BERRY(ITEM_MAGO_BERRY)]          = {gFlowerPic_Mago,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Mago},
    [ITEM_TO_BERRY(ITEM_AGUAV_BERRY)]          = {gFlowerPic_Aguav,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Aguav},
    [ITEM_TO_BERRY(ITEM_IAPAPA_BERRY)]          = {gFlowerPic_Iapapa, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Iapapa},
    [ITEM_TO_BERRY(ITEM_RAZZ_BERRY)]          = {gFlowerPic_Razz,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Razz},
    [ITEM_TO_BERRY(ITEM_BLUK_BERRY)]          = {gFlowerPic_Bluk,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Bluk},
    [ITEM_TO_BERRY(ITEM_NANAB_BERRY)]          = {gFlowerPic_Nanab,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Nanab},
    [ITEM_TO_BERRY(ITEM_WEPEAR_BERRY)]          = {gFlowerPic_Wepear, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Wepear},
    [ITEM_TO_BERRY(ITEM_PINAP_BERRY)]          = {gFlowerPic_Pinap,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pinap},
    [ITEM_TO_BERRY(ITEM_POMEG_BERRY)]          = {gFlowerPic_Pomeg,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pomeg},
    [ITEM_TO_BERRY(ITEM_KELPSY_BERRY)]          = {gFlowerPic_Kelpsy, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kelpsy},
    [ITEM_TO_BERRY(ITEM_QUALOT_BERRY)]          = {gFlowerPic_Qualot, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Qualot},
    [ITEM_TO_BERRY(ITEM_HONDEW_BERRY)]          = {gFlowerPic_Hondew, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Hondew},
    [ITEM_TO_BERRY(ITEM_GREPA_BERRY)]          = {gFlowerPic_Grepa,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Grepa},
    [ITEM_TO_BERRY(ITEM_TAMATO_BERRY)]          = {gFlowerPic_Tamato, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Tamato},
    [ITEM_TO_BERRY(ITEM_CORNN_BERRY)]          = {gFlowerPic_Cornn,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Cornn},
    [ITEM_TO_BERRY(ITEM_MAGOST_BERRY)]          = {gFlowerPic_Magost, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Magost},
    [ITEM_TO_BERRY(ITEM_RABUTA_BERRY)]          = {gFlowerPic_Rabuta, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rabuta},
    [ITEM_TO_BERRY(ITEM_NOMEL_BERRY)]          = {gFlowerPic_Nomel,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Nomel},
    [ITEM_TO_BERRY(ITEM_SPELON_BERRY)]          = {gFlowerPic_Spelon, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Spelon},
    [ITEM_TO_BERRY(ITEM_PAMTRE_BERRY)]          = {gFlowerPic_Pamtre, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pamtre},
    [ITEM_TO_BERRY(ITEM_WATMEL_BERRY)]          = {gFlowerPic_Watmel, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Watmel},
    [ITEM_TO_BERRY(ITEM_DURIN_BERRY)]          = {gFlowerPic_Durin,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Durin},
    [ITEM_TO_BERRY(ITEM_BELUE_BERRY)]          = {gFlowerPic_Belue,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Belue},
    [ITEM_TO_BERRY(ITEM_PUMKIN_BERRY)]          = {gFlowerPic_Pumkin,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pumkin},// MOD CONTEST E-reader berry graphics:
    [ITEM_TO_BERRY(ITEM_DRASH_BERRY)]          = {gFlowerPic_Drash,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Drash},//
    [ITEM_TO_BERRY(ITEM_EGGANT_BERRY)]          = {gFlowerPic_Eggant,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Eggant},//
    [ITEM_TO_BERRY(ITEM_STRIB_BERRY)]          = {gFlowerPic_Strib,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Strib},//
    [ITEM_TO_BERRY(ITEM_CHILANER_BERRY)]          = {gFlowerPic_ChilanER, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_ChilanER},//
    [ITEM_TO_BERRY(ITEM_NUTPEA_BERRY)]          = {gFlowerPic_Nutpea,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Nutpea},//
    [ITEM_TO_BERRY(ITEM_NOION_BERRY)]          = {gFlowerPic_Noion,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Noion},//(Japan exclusive Ginema berry)
    [ITEM_TO_BERRY(ITEM_ROKA_BERRY)]          = {gFlowerPic_Roka,     0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Roka},//(Japan exclusive Kuo berry)
    [ITEM_TO_BERRY(ITEM_SKUASH_BERRY)]          = {gFlowerPic_Skuash,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Skuash},//(Japan exclusive Yago berry)
    [ITEM_TO_BERRY(ITEM_PAPRIK_BERRY)]          = {gFlowerPic_Paprik,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Paprik},//(Japan exclusive Touga berry)
    [ITEM_TO_BERRY(ITEM_MALIUM_BERRY)]          = {gFlowerPic_Malium,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Malium},//(Japan exclusive Niniku berry)
    [ITEM_TO_BERRY(ITEM_TOPO_BERRY)]          = {gFlowerPic_Topo,     0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Topo},//(Japan exclusive Topo berry)
    //[ITEM_TO_BERRY(ITEM_CHILAN_BERRY)]          = {gFlowerPic_Chilan, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Chilan},
    //[ITEM_TO_BERRY(ITEM_OCCA_BERRY)]          = {gFlowerPic_Occa, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Occa},
    //[ITEM_TO_BERRY(ITEM_PASSHO_BERRY)]          = {gFlowerPic_Passho, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Passho},
    //[ITEM_TO_BERRY(ITEM_WACAN_BERRY)]          = {gFlowerPic_Wacan, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Wacan},
    //[ITEM_TO_BERRY(ITEM_RINDO_BERRY)]          = {gFlowerPic_Rindo, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rindo},
    //[ITEM_TO_BERRY(ITEM_YACHE_BERRY)]          = {gFlowerPic_Yache, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Yache},
    //[ITEM_TO_BERRY(ITEM_CHOPLE_BERRY)]          = {gFlowerPic_Chople, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Chople},
    //[ITEM_TO_BERRY(ITEM_KEBIA_BERRY)]          = {gFlowerPic_Kebia, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kebia},
    //[ITEM_TO_BERRY(ITEM_SHUCA_BERRY)]          = {gFlowerPic_Shuca, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Shuca},
    //[ITEM_TO_BERRY(ITEM_COBA_BERRY)]          = {gFlowerPic_Coba, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Coba},
    //[ITEM_TO_BERRY(ITEM_PAYAPA_BERRY)]          = {gFlowerPic_Payapa, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Payapa},
    //[ITEM_TO_BERRY(ITEM_TANGA_BERRY)]          = {gFlowerPic_Tanga, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Tanga},
    //[ITEM_TO_BERRY(ITEM_CHARTI_BERRY)]          = {gFlowerPic_Charti, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Charti},
    //[ITEM_TO_BERRY(ITEM_KASIB_BERRY)]          = {gFlowerPic_Kasib, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kasib},
    //[ITEM_TO_BERRY(ITEM_HABAN_BERRY)]          = {gFlowerPic_Haban, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Haban},
    //[ITEM_TO_BERRY(ITEM_COLBUR_BERRY)]          = {gFlowerPic_Colbur, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Colbur},
    //[ITEM_TO_BERRY(ITEM_BABIRI_BERRY)]          = {gFlowerPic_Babiri, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Babiri},
    //[ITEM_TO_BERRY(ITEM_ROSELI_BERRY)]          = {gFlowerPic_Roseli, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Roseli},
    [ITEM_TO_BERRY(ITEM_LIECHI_BERRY)]          = {gFlowerPic_Liechi, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Liechi},
    [ITEM_TO_BERRY(ITEM_GANLON_BERRY)]          = {gFlowerPic_Ganlon, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Ganlon},
    [ITEM_TO_BERRY(ITEM_SALAC_BERRY)]          = {gFlowerPic_Salac,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Salac},
    [ITEM_TO_BERRY(ITEM_PETAYA_BERRY)]          = {gFlowerPic_Petaya, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Petaya},
    [ITEM_TO_BERRY(ITEM_APICOT_BERRY)]          = {gFlowerPic_Apicot, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Apicot},
    [ITEM_TO_BERRY(ITEM_LANSAT_BERRY)]          = {gFlowerPic_Lansat, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Lansat},
    [ITEM_TO_BERRY(ITEM_STARF_BERRY)]          = {gFlowerPic_Starf,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Starf},
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY)]          = {gFlowerPic_Enigma, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Enigma},
    //[ITEM_TO_BERRY(ITEM_MICLE_BERRY)]          = {gFlowerPic_Micle, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Micle},
    //[ITEM_TO_BERRY(ITEM_CUSTAP_BERRY)]          = {gFlowerPic_Custap, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Custap},
    //[ITEM_TO_BERRY(ITEM_JABOCA_BERRY)]          = {gFlowerPic_Jaboca, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Jaboca},
    //[ITEM_TO_BERRY(ITEM_ROWAP_BERRY)]          = {gFlowerPic_Rowap, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rowap},
    //[ITEM_TO_BERRY(ITEM_KEE_BERRY)]          = {gFlowerPic_Kee, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kee},
    //[ITEM_TO_BERRY(ITEM_MARANGA_BERRY)]          = {gFlowerPic_Maranga, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Maranga},
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY_E_READER)] = {gFlowerPic_Enigma, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Enigma},
};

const struct SpritePalette gBerryFlowerPaletteTable[] = // MOD CONTEST Berry Flower palettes
{
    [ITEM_TO_BERRY(ITEM_CHERI_BERRY)]          = {gFlowerPalette_Cheri, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_CHESTO_BERRY)]          = {gFlowerPalette_Chesto, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_PECHA_BERRY)]          = {gFlowerPalette_Pecha, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_RAWST_BERRY)]          = {gFlowerPalette_Rawst, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_ASPEAR_BERRY)]          = {gFlowerPalette_Aspear, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_LEPPA_BERRY)]          = {gFlowerPalette_Leppa, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_ORAN_BERRY)]          = {gFlowerPalette_Oran, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_PERSIM_BERRY)]          = {gFlowerPalette_Persim, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_LUM_BERRY)]          = {gFlowerPalette_Lum, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_SITRUS_BERRY)]          = {gFlowerPalette_Sitrus, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_FIGY_BERRY)]          = {gFlowerPalette_Figy, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_WIKI_BERRY)]          = {gFlowerPalette_Wiki, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_MAGO_BERRY)]          = {gFlowerPalette_Mago, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_AGUAV_BERRY)]          = {gFlowerPalette_Aguav, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_IAPAPA_BERRY)]          = {gFlowerPalette_Iapapa, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_RAZZ_BERRY)]          = {gFlowerPalette_Razz, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_BLUK_BERRY)]          = {gFlowerPalette_Bluk, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_NANAB_BERRY)]          = {gFlowerPalette_Nanab, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_WEPEAR_BERRY)]          = {gFlowerPalette_Wepear, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_PINAP_BERRY)]          = {gFlowerPalette_Pinap, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_POMEG_BERRY)]          = {gFlowerPalette_Pomeg, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_KELPSY_BERRY)]          = {gFlowerPalette_Kelpsy, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_QUALOT_BERRY)]          = {gFlowerPalette_Qualot, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_HONDEW_BERRY)]          = {gFlowerPalette_Hondew, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_GREPA_BERRY)]          = {gFlowerPalette_Grepa, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_TAMATO_BERRY)]          = {gFlowerPalette_Tamato, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_CORNN_BERRY)]          = {gFlowerPalette_Cornn, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_MAGOST_BERRY)]          = {gFlowerPalette_Magost, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_RABUTA_BERRY)]          = {gFlowerPalette_Rabuta, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_NOMEL_BERRY)]          = {gFlowerPalette_Nomel, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_SPELON_BERRY)]          = {gFlowerPalette_Spelon, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_PAMTRE_BERRY)]          = {gFlowerPalette_Pamtre, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_WATMEL_BERRY)]          = {gFlowerPalette_Watmel, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_DURIN_BERRY)]          = {gFlowerPalette_Durin, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_BELUE_BERRY)]          = {gFlowerPalette_Belue, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_PUMKIN_BERRY)]          = {gFlowerPalette_Pumkin, TAG_BERRY_FLOWER_GFX},// MOD CONTEST E-reader berry graphics:
    [ITEM_TO_BERRY(ITEM_DRASH_BERRY)]          = {gFlowerPalette_Drash, TAG_BERRY_FLOWER_GFX},//
    [ITEM_TO_BERRY(ITEM_EGGANT_BERRY)]          = {gFlowerPalette_Eggant, TAG_BERRY_FLOWER_GFX},//
    [ITEM_TO_BERRY(ITEM_STRIB_BERRY)]          = {gFlowerPalette_Strib, TAG_BERRY_FLOWER_GFX},//
    [ITEM_TO_BERRY(ITEM_CHILANER_BERRY)]          = {gFlowerPalette_ChilanER, TAG_BERRY_FLOWER_GFX},//
    [ITEM_TO_BERRY(ITEM_NUTPEA_BERRY)]          = {gFlowerPalette_Nutpea, TAG_BERRY_FLOWER_GFX},//
    [ITEM_TO_BERRY(ITEM_NOION_BERRY)]          = {gFlowerPalette_Noion, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Ginema berry)
    [ITEM_TO_BERRY(ITEM_ROKA_BERRY)]          = {gFlowerPalette_Roka, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Kuo berry)
    [ITEM_TO_BERRY(ITEM_SKUASH_BERRY)]          = {gFlowerPalette_Skuash, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Yago berry)
    [ITEM_TO_BERRY(ITEM_PAPRIK_BERRY)]          = {gFlowerPalette_Paprik, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Touga berry)
    [ITEM_TO_BERRY(ITEM_MALIUM_BERRY)]          = {gFlowerPalette_Malium, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Niniku berry)
    [ITEM_TO_BERRY(ITEM_TOPO_BERRY)]          = {gFlowerPalette_Topo, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Topo berry)
    //[ITEM_TO_BERRY(ITEM_CHILAN_BERRY)]          = {gFlowerPalette_Chilan, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_OCCA_BERRY)]          = {gFlowerPalette_Occa, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_PASSHO_BERRY)]          = {gFlowerPalette_Passho, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_WACAN_BERRY)]          = {gFlowerPalette_Wacan, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_RINDO_BERRY)]          = {gFlowerPalette_Rindo, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_YACHE_BERRY)]          = {gFlowerPalette_Yache, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_CHOPLE_BERRY)]          = {gFlowerPalette_Chople, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_KEBIA_BERRY)]          = {gFlowerPalette_Kebia, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_SHUCA_BERRY)]          = {gFlowerPalette_Shuca, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_COBA_BERRY)]          = {gFlowerPalette_Coba, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_PAYAPA_BERRY)]          = {gFlowerPalette_Payapa, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_TANGA_BERRY)]          = {gFlowerPalette_Tanga, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_CHARTI_BERRY)]          = {gFlowerPalette_Charti, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_KASIB_BERRY)]          = {gFlowerPalette_Kasib, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_HABAN_BERRY)]          = {gFlowerPalette_Haban, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_COLBUR_BERRY)]          = {gFlowerPalette_Colbur, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_BABIRI_BERRY)]          = {gFlowerPalette_Babiri, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_ROSELI_BERRY)]          = {gFlowerPalette_Roseli, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_LIECHI_BERRY)]          = {gFlowerPalette_Liechi, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_GANLON_BERRY)]          = {gFlowerPalette_Ganlon, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_SALAC_BERRY)]          = {gFlowerPalette_Salac, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_PETAYA_BERRY)]          = {gFlowerPalette_Petaya, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_APICOT_BERRY)]          = {gFlowerPalette_Apicot, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_LANSAT_BERRY)]          = {gFlowerPalette_Lansat, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_STARF_BERRY)]          = {gFlowerPalette_Starf, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY)]          = {gFlowerPalette_Enigma, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_MICLE_BERRY)]          = {gFlowerPalette_Micle, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_CUSTAP_BERRY)]          = {gFlowerPalette_Custap, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_JABOCA_BERRY)]          = {gFlowerPalette_Jaboca, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_ROWAP_BERRY)]          = {gFlowerPalette_Rowap, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_KEE_BERRY)]          = {gFlowerPalette_Kee, TAG_BERRY_FLOWER_GFX},
    //[ITEM_TO_BERRY(ITEM_MARANGA_BERRY)]          = {gFlowerPalette_Maranga, TAG_BERRY_FLOWER_GFX},
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY_E_READER)] = {gFlowerPalette_Enigma, TAG_BERRY_FLOWER_GFX},
};


static const struct TilesPal sBerryPicTable[] =
{
    [ITEM_TO_BERRY(ITEM_CHERI_BERRY)  - 1]          = {gBerryPic_Cheri,  gBerryPalette_Cheri},
    [ITEM_TO_BERRY(ITEM_CHESTO_BERRY) - 1]          = {gBerryPic_Chesto, gBerryPalette_Chesto},
    [ITEM_TO_BERRY(ITEM_PECHA_BERRY)  - 1]          = {gBerryPic_Pecha,  gBerryPalette_Pecha},
    [ITEM_TO_BERRY(ITEM_RAWST_BERRY)  - 1]          = {gBerryPic_Rawst,  gBerryPalette_Rawst},
    [ITEM_TO_BERRY(ITEM_ASPEAR_BERRY) - 1]          = {gBerryPic_Aspear, gBerryPalette_Aspear},
    [ITEM_TO_BERRY(ITEM_LEPPA_BERRY)  - 1]          = {gBerryPic_Leppa,  gBerryPalette_Leppa},
    [ITEM_TO_BERRY(ITEM_ORAN_BERRY)   - 1]          = {gBerryPic_Oran,   gBerryPalette_Oran},
    [ITEM_TO_BERRY(ITEM_PERSIM_BERRY) - 1]          = {gBerryPic_Persim, gBerryPalette_Persim},
    [ITEM_TO_BERRY(ITEM_LUM_BERRY)    - 1]          = {gBerryPic_Lum,    gBerryPalette_Lum},
    [ITEM_TO_BERRY(ITEM_SITRUS_BERRY) - 1]          = {gBerryPic_Sitrus, gBerryPalette_Sitrus},
    [ITEM_TO_BERRY(ITEM_FIGY_BERRY)   - 1]          = {gBerryPic_Figy,   gBerryPalette_Figy},
    [ITEM_TO_BERRY(ITEM_WIKI_BERRY)   - 1]          = {gBerryPic_Wiki,   gBerryPalette_Wiki},
    [ITEM_TO_BERRY(ITEM_MAGO_BERRY)   - 1]          = {gBerryPic_Mago,   gBerryPalette_Mago},
    [ITEM_TO_BERRY(ITEM_AGUAV_BERRY)  - 1]          = {gBerryPic_Aguav,  gBerryPalette_Aguav},
    [ITEM_TO_BERRY(ITEM_IAPAPA_BERRY) - 1]          = {gBerryPic_Iapapa, gBerryPalette_Iapapa},
    [ITEM_TO_BERRY(ITEM_RAZZ_BERRY)   - 1]          = {gBerryPic_Razz,   gBerryPalette_Razz},
    [ITEM_TO_BERRY(ITEM_BLUK_BERRY)   - 1]          = {gBerryPic_Bluk,   gBerryPalette_Bluk},
    [ITEM_TO_BERRY(ITEM_NANAB_BERRY)  - 1]          = {gBerryPic_Nanab,  gBerryPalette_Nanab},
    [ITEM_TO_BERRY(ITEM_WEPEAR_BERRY) - 1]          = {gBerryPic_Wepear, gBerryPalette_Wepear},
    [ITEM_TO_BERRY(ITEM_PINAP_BERRY)  - 1]          = {gBerryPic_Pinap,  gBerryPalette_Pinap},
    [ITEM_TO_BERRY(ITEM_POMEG_BERRY)  - 1]          = {gBerryPic_Pomeg,  gBerryPalette_Pomeg},
    [ITEM_TO_BERRY(ITEM_KELPSY_BERRY) - 1]          = {gBerryPic_Kelpsy, gBerryPalette_Kelpsy},
    [ITEM_TO_BERRY(ITEM_QUALOT_BERRY) - 1]          = {gBerryPic_Qualot, gBerryPalette_Qualot},
    [ITEM_TO_BERRY(ITEM_HONDEW_BERRY) - 1]          = {gBerryPic_Hondew, gBerryPalette_Hondew},
    [ITEM_TO_BERRY(ITEM_GREPA_BERRY)  - 1]          = {gBerryPic_Grepa,  gBerryPalette_Grepa},
    [ITEM_TO_BERRY(ITEM_TAMATO_BERRY) - 1]          = {gBerryPic_Tamato, gBerryPalette_Tamato},
    [ITEM_TO_BERRY(ITEM_CORNN_BERRY)  - 1]          = {gBerryPic_Cornn,  gBerryPalette_Cornn},
    [ITEM_TO_BERRY(ITEM_MAGOST_BERRY) - 1]          = {gBerryPic_Magost, gBerryPalette_Magost},
    [ITEM_TO_BERRY(ITEM_RABUTA_BERRY) - 1]          = {gBerryPic_Rabuta, gBerryPalette_Rabuta},
    [ITEM_TO_BERRY(ITEM_NOMEL_BERRY)  - 1]          = {gBerryPic_Nomel,  gBerryPalette_Nomel},
    [ITEM_TO_BERRY(ITEM_SPELON_BERRY) - 1]          = {gBerryPic_Spelon, gBerryPalette_Spelon},
    [ITEM_TO_BERRY(ITEM_PAMTRE_BERRY) - 1]          = {gBerryPic_Pamtre, gBerryPalette_Pamtre},
    [ITEM_TO_BERRY(ITEM_WATMEL_BERRY) - 1]          = {gBerryPic_Watmel, gBerryPalette_Watmel},
    [ITEM_TO_BERRY(ITEM_DURIN_BERRY)  - 1]          = {gBerryPic_Durin,  gBerryPalette_Durin},
    [ITEM_TO_BERRY(ITEM_BELUE_BERRY)  - 1]          = {gBerryPic_Belue,  gBerryPalette_Belue},
    [ITEM_TO_BERRY(ITEM_PUMKIN_BERRY)   - 1]          = {gBerryPic_Pumkin, gBerryPalette_Pumkin},// MOD CONTEST E-reader berry graphics:
    [ITEM_TO_BERRY(ITEM_DRASH_BERRY)    - 1]          = {gBerryPic_Drash, gBerryPalette_Drash},//
    [ITEM_TO_BERRY(ITEM_EGGANT_BERRY)   - 1]          = {gBerryPic_Eggant, gBerryPalette_Eggant},//
    [ITEM_TO_BERRY(ITEM_STRIB_BERRY)    - 1]          = {gBerryPic_Strib, gBerryPalette_Strib},//
    [ITEM_TO_BERRY(ITEM_CHILANER_BERRY) - 1]          = {gBerryPic_ChilanER, gBerryPalette_ChilanER},//
    [ITEM_TO_BERRY(ITEM_NUTPEA_BERRY)   - 1]          = {gBerryPic_Nutpea, gBerryPalette_Nutpea},//
    [ITEM_TO_BERRY(ITEM_NOION_BERRY)    - 1]          = {gBerryPic_Noion, gBerryPalette_Noion},//(Japan exclusive Ginema berry)
    [ITEM_TO_BERRY(ITEM_ROKA_BERRY)     - 1]          = {gBerryPic_Roka, gBerryPalette_Roka},//(Japan exclusive Kuo berry)
    [ITEM_TO_BERRY(ITEM_SKUASH_BERRY)   - 1]          = {gBerryPic_Skuash, gBerryPalette_Skuash},//(Japan exclusive Yago berry)
    [ITEM_TO_BERRY(ITEM_PAPRIK_BERRY)   - 1]          = {gBerryPic_Paprik, gBerryPalette_Paprik},//(Japan exclusive Touga berry)
    [ITEM_TO_BERRY(ITEM_MALIUM_BERRY)   - 1]          = {gBerryPic_Malium, gBerryPalette_Malium},//(Japan exclusive Niniku berry)
    [ITEM_TO_BERRY(ITEM_TOPO_BERRY)     - 1]          = {gBerryPic_Topo, gBerryPalette_Topo},//(Japan exclusive Topo berry)
    //[ITEM_TO_BERRY(ITEM_CHILAN_BERRY) - 1]          = {gBerryPic_Chilan, gBerryPalette_Chilan},
    //[ITEM_TO_BERRY(ITEM_OCCA_BERRY)   - 1]          = {gBerryPic_Occa, gBerryPalette_Occa},
    //[ITEM_TO_BERRY(ITEM_PASSHO_BERRY) - 1]          = {gBerryPic_Passho, gBerryPalette_Passho},
    //[ITEM_TO_BERRY(ITEM_WACAN_BERRY)  - 1]          = {gBerryPic_Wacan, gBerryPalette_Wacan},
    //[ITEM_TO_BERRY(ITEM_RINDO_BERRY)  - 1]          = {gBerryPic_Rindo, gBerryPalette_Rindo},
    //[ITEM_TO_BERRY(ITEM_YACHE_BERRY)  - 1]          = {gBerryPic_Yache, gBerryPalette_Yache},
    //[ITEM_TO_BERRY(ITEM_CHOPLE_BERRY) - 1]          = {gBerryPic_Chople, gBerryPalette_Chople},
    //[ITEM_TO_BERRY(ITEM_KEBIA_BERRY)  - 1]          = {gBerryPic_Kebia, gBerryPalette_Kebia},
    //[ITEM_TO_BERRY(ITEM_SHUCA_BERRY)  - 1]          = {gBerryPic_Shuca, gBerryPalette_Shuca},
    //[ITEM_TO_BERRY(ITEM_COBA_BERRY)   - 1]          = {gBerryPic_Coba, gBerryPalette_Coba},
    //[ITEM_TO_BERRY(ITEM_PAYAPA_BERRY) - 1]          = {gBerryPic_Payapa, gBerryPalette_Payapa},
    //[ITEM_TO_BERRY(ITEM_TANGA_BERRY)  - 1]          = {gBerryPic_Tanga, gBerryPalette_Tanga},
    //[ITEM_TO_BERRY(ITEM_CHARTI_BERRY) - 1]          = {gBerryPic_Charti, gBerryPalette_Charti},
    //[ITEM_TO_BERRY(ITEM_KASIB_BERRY)  - 1]          = {gBerryPic_Kasib, gBerryPalette_Kasib},
    //[ITEM_TO_BERRY(ITEM_HABAN_BERRY)  - 1]          = {gBerryPic_Haban, gBerryPalette_Haban},
    //[ITEM_TO_BERRY(ITEM_COLBUR_BERRY) - 1]          = {gBerryPic_Colbur, gBerryPalette_Colbur},
    //[ITEM_TO_BERRY(ITEM_BABIRI_BERRY) - 1]          = {gBerryPic_Babiri, gBerryPalette_Babiri},
    //[ITEM_TO_BERRY(ITEM_ROSELI_BERRY) - 1]          = {gBerryPic_Roseli, gBerryPalette_Roseli},
    [ITEM_TO_BERRY(ITEM_LIECHI_BERRY) - 1]          = {gBerryPic_Liechi, gBerryPalette_Liechi},
    [ITEM_TO_BERRY(ITEM_GANLON_BERRY) - 1]          = {gBerryPic_Ganlon, gBerryPalette_Ganlon},
    [ITEM_TO_BERRY(ITEM_SALAC_BERRY)  - 1]          = {gBerryPic_Salac,  gBerryPalette_Salac},
    [ITEM_TO_BERRY(ITEM_PETAYA_BERRY) - 1]          = {gBerryPic_Petaya, gBerryPalette_Petaya},
    [ITEM_TO_BERRY(ITEM_APICOT_BERRY) - 1]          = {gBerryPic_Apicot, gBerryPalette_Apicot},
    [ITEM_TO_BERRY(ITEM_LANSAT_BERRY) - 1]          = {gBerryPic_Lansat, gBerryPalette_Lansat},
    [ITEM_TO_BERRY(ITEM_STARF_BERRY)  - 1]          = {gBerryPic_Starf,  gBerryPalette_Starf},
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY) - 1]          = {gBerryPic_Enigma, gBerryPalette_Enigma},
    //[ITEM_TO_BERRY(ITEM_MICLE_BERRY)  - 1]          = {gBerryPic_Micle, gBerryPalette_Micle},
    //[ITEM_TO_BERRY(ITEM_CUSTAP_BERRY) - 1]          = {gBerryPic_Custap, gBerryPalette_Custap},
    //[ITEM_TO_BERRY(ITEM_JABOCA_BERRY) - 1]          = {gBerryPic_Jaboca, gBerryPalette_Jaboca},
    //[ITEM_TO_BERRY(ITEM_ROWAP_BERRY)  - 1]          = {gBerryPic_Rowap, gBerryPalette_Rowap},
    //[ITEM_TO_BERRY(ITEM_KEE_BERRY)    - 1]          = {gBerryPic_Kee, gBerryPalette_Kee},
    //[ITEM_TO_BERRY(ITEM_MARANGA_BERRY)- 1]          = {gBerryPic_Maranga, gBerryPalette_Maranga},
    [ITEM_TO_BERRY(ITEM_ENIGMA_BERRY_E_READER) - 1] = {gBerryPic_Enigma, gBerryPalette_Enigma},
};

const struct CompressedSpriteSheet gBerryCheckCircleSpriteSheet =
{
    gBerryCheckCircle_Gfx, 0x800, TAG_BERRY_CHECK_CIRCLE_GFX
};

const struct CompressedSpriteSheet gBerrySeasonSpriteSheet = //MOD CONTEST
{
    gBerrySeasonPic, 0x800, TAG_BERRY_SEASONS_TABLE_GFX
};

const struct CompressedSpriteSheet gBerrySeasonsQuestionSpriteSheet = //
{
    gBerrySeasonQuestionPic, 0x400, TAG_BERRY_SEASONS_QUESTION_GFX
};

const struct CompressedSpriteSheet gBerrySeasonXSpriteSheet = //
{
    gBerrySeasonXPic, 0x400, TAG_BERRY_SEASONS_X_GFX
};

const struct CompressedSpriteSheet gBerryFlavorGrafSpriteSheet = //
{
    gBerryFlavorGrafPic, 0x2800, TAG_BERRY_FLAVOR_GRAF_GFX
};

const struct CompressedSpriteSheet gFlavorProfileG1SpriteSheet = // Flavor Profile SpriteSheets:
{
    gBerryFlavorProfileG1Pic, 0x2800, TAG_FLAVOR_PROFILE
};

const struct CompressedSpriteSheet gFlavorProfileG2SpriteSheet = //
{
    gBerryFlavorProfileG2Pic, 0x2800, TAG_FLAVOR_PROFILE
};

const struct CompressedSpriteSheet gFlavorProfileG3SpriteSheet = //
{
    gBerryFlavorProfileG3Pic, 0x2800, TAG_FLAVOR_PROFILE
};

const struct CompressedSpriteSheet gFlavorProfileG4SpriteSheet = //
{
    gBerryFlavorProfileG4Pic, 0x2800, TAG_FLAVOR_PROFILE
};

const struct CompressedSpriteSheet gFlavorProfileG5SpriteSheet = //
{
    gBerryFlavorProfileG5Pic, 0x2800, TAG_FLAVOR_PROFILE
};

const struct CompressedSpriteSheet gFlavorProfileG6SpriteSheet = //
{
    gBerryFlavorProfileG6Pic, 0x2800, TAG_FLAVOR_PROFILE
};

const struct CompressedSpriteSheet gFlavorProfileG7SpriteSheet = //
{
    gBerryFlavorProfileG7Pic, 0x2800, TAG_FLAVOR_PROFILE
};

const struct CompressedSpriteSheet gFlavorProfileG8SpriteSheet = //
{
    gBerryFlavorProfileG8Pic, 0x2800, TAG_FLAVOR_PROFILE
};


const struct SpritePalette gFlavorSpicyPaletteTable = //
{
    gFlavorSpicyPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorDryPaletteTable = //
{
    gFlavorDryPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSweetPaletteTable = //
{
    gFlavorSweetPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorBitterPaletteTable = //
{
    gFlavorBitterPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSourPaletteTable = //
{
    gFlavorSourPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSpicyDryPaletteTable = //
{
    gFlavorSpicyDryPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSpicySweetPaletteTable = //
{
    gFlavorSpicySweetPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSpicyBitterPaletteTable = //
{
    gFlavorSpicyBitterPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSpicySourPaletteTable = //
{
    gFlavorSpicySourPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorDrySweetPaletteTable = //
{
    gFlavorDrySweetPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorDryBitterPaletteTable = //
{
    gFlavorDryBitterPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorDrySourPaletteTable = //
{
    gFlavorDrySourPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSweetBitterPaletteTable = //
{
    gFlavorSweetBitterPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorSweetSourPaletteTable = //
{
    gFlavorSweetSourPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorBitterSourPaletteTable = //
{
    gFlavorBitterSourPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gFlavorBalancedPaletteTable = //
{
    gFlavorBalancedPalette, TAG_FLAVOR_PROFILE
};

const struct SpritePalette gBerryCheckCirclePaletteTable =
{
    gBerryCheck_Pal, TAG_BERRY_CHECK_CIRCLE_GFX
};

const struct SpritePalette gBerrySeasonsPaletteTable = //
{
    gBerrySeasonPalette, TAG_BERRY_SEASONS_PAL
};

const struct SpritePalette gBerryFlavorGrafPaletteTable = //
{
    gBerryFlavorGrafPalette, TAG_BERRY_FLAVOR_GRAF_GFX
};

static const struct OamData sBerryCheckCircleOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sBerryFlowerOamData =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64), //48x48, the size of pokemon berries, doesn't exist?? I call bull here. Why?!
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sBerrySeasonsOamData = //MOD CONTEST Defines how the Sprite Sprites.
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64), 
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sBerryQuestionMarksOamData = //MOD CONTEST Defines how the Sprite Sprites.
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32), 
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

//.hFlip = TRUE poner en anim_frame como tercer argumento.
static const struct OamData sFlavorProfileOamData = //
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64), 
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
};

static const struct OamData sFlavorProfile5OamData = // For the Enigma Berry
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .y = DISPLAY_HEIGHT,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 1
};

static const struct OamData sBerrySeasonsXOamData = //
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16), 
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};


static const struct OamData sBerryFlavorGrafOamData = //
{
    .y = DISPLAY_HEIGHT,
    .objMode = ST_OAM_OBJ_BLEND,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 2
};

static const union AnimCmd sSpriteAnim_BerryCheckCircle[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_BerrySeasons[] =//
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_BerryFlavorGraf1[] = //
{
    ANIMCMD_FRAME( 0, 0),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_BerryFlavorGraf2[] =//
{
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_BerryFlavorGraf3[] = //
{
    ANIMCMD_FRAME(128, 0),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_BerryFlavorGraf4[] = //
{
    ANIMCMD_FRAME(192, 0),
    ANIMCMD_END
};


static const union AnimCmd sSpriteAnim_FlavorProfile1[] =//
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile2[] =//
{
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile3[] =//
{
    ANIMCMD_FRAME(128, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile4[] =//
{
    ANIMCMD_FRAME(192, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile5[] =//
{
    ANIMCMD_FRAME(0, -1, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile7[] =//
{
    ANIMCMD_FRAME(64, -1, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile6[] =//
{
    ANIMCMD_FRAME(128, -1, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile8[] =//
{
    ANIMCMD_FRAME(192, -1, .hFlip = TRUE),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_FlavorProfile0[] =// This one will always load an empty sprite, so the flavors don't load RAM junk (Why can't them just unload??)
{
    ANIMCMD_FRAME(256, 0),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_BerryFlavorProfileE1[] = // these are for the enigma profile tiles
{
    ANIMCMD_FRAME( 0, 0),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_BerryFlavorProfileE2[] =//
{
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_BerryFlavorProfileE3[] = //
{
    ANIMCMD_FRAME(128, 0),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_BerryFlavorProfileE4[] = //
{
    ANIMCMD_FRAME(192, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sBerryCheckCircleSpriteAnimTable[] =
{
    sSpriteAnim_BerryCheckCircle
};


static const union AnimCmd *const sBerrySeasonsSpriteAnimTable[] = //
{
    sSpriteAnim_BerrySeasons
};

static const union AnimCmd *const sFlavorProfilesSpriteAnimTable[] = //
{ 
    sSpriteAnim_FlavorProfile0,
    sSpriteAnim_FlavorProfile0,
    sSpriteAnim_FlavorProfile1, //regular ones
    sSpriteAnim_FlavorProfile2,
    sSpriteAnim_FlavorProfile3,
    sSpriteAnim_FlavorProfile4,
    sSpriteAnim_FlavorProfile5, //flip versions
    sSpriteAnim_FlavorProfile6,
    sSpriteAnim_FlavorProfile7,
    sSpriteAnim_FlavorProfile8

};

static const union AnimCmd *const sBerryFlavorGrafSpriteAnimTable[] = //
{
    sSpriteAnim_BerryFlavorGraf1,
    sSpriteAnim_BerryFlavorGraf2,
    sSpriteAnim_BerryFlavorGraf3,
    sSpriteAnim_BerryFlavorGraf4
};


static const struct SpriteTemplate sBerryCheckCircleSpriteTemplate =
{
    .tileTag = TAG_BERRY_CHECK_CIRCLE_GFX,
    .paletteTag = TAG_BERRY_CHECK_CIRCLE_GFX,
    .oam = &sBerryCheckCircleOamData,
    .anims = sBerryCheckCircleSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sBerryFlowerSpriteTemplate =
{
    .tileTag = TAG_BERRY_FLOWER_GFX,
    .paletteTag = TAG_BERRY_FLOWER_GFX,
    .oam = &sBerrySeasonsOamData,
    .anims = sBerrySeasonsSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sBerrySeasonsSpriteTemplate = //
{
    .tileTag = TAG_BERRY_SEASONS_TABLE_GFX,
    .paletteTag = TAG_BERRY_SEASONS_PAL,
    .oam = &sBerrySeasonsOamData,
    .anims = sBerrySeasonsSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sBerrySeasonsQuestionSpriteTemplate = //
{
    .tileTag = TAG_BERRY_SEASONS_QUESTION_GFX,
    .paletteTag = TAG_BERRY_SEASONS_PAL,
    .oam = &sBerryQuestionMarksOamData,
    .anims = sBerrySeasonsSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sBerrySeasonsXSpriteTemplate = //
{
    .tileTag = TAG_BERRY_SEASONS_X_GFX,
    .paletteTag = TAG_BERRY_SEASONS_PAL,
    .oam = &sBerrySeasonsXOamData,
    .anims = sBerrySeasonsSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sBerryFlavorGrafSpriteTemplate = //
{
    .tileTag = TAG_BERRY_FLAVOR_GRAF_GFX,
    .paletteTag = TAG_BERRY_FLAVOR_GRAF_GFX,
    .oam = &sBerryFlavorGrafOamData,
    .anims = sBerryFlavorGrafSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

//Since the sprite is loaded separately, the only thing that changes for the Profile sprites here is the Oam Data and the rotation.
static const struct SpriteTemplate sBerryFlavorProfileSpriteTemplate =  //
{
    .tileTag = TAG_FLAVOR_PROFILE,
    .paletteTag = TAG_FLAVOR_PROFILE,
    .oam = &sFlavorProfileOamData,
    .anims = sFlavorProfilesSpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

const struct CompressedSpriteSheet gSpriteSheet_FlavorGraf[] = //
{
    {
        .data = gBerryFlavorGrafPic,
        .size = 0x1000,
        .tag = TAG_BERRY_FLAVOR_GRAF_GFX
    },
    {}
};

// code
void RemoveBagSprite(u8 id)
{
    u8 *spriteId = &gBagMenu->spriteIds[id];
    if (*spriteId != SPRITE_NONE)
    {
        FreeSpriteTilesByTag(id + TAG_BAG_GFX);
        FreeSpritePaletteByTag(id + TAG_BAG_GFX);
        FreeSpriteOamMatrix(&gSprites[*spriteId]);
        DestroySprite(&gSprites[*spriteId]);
        *spriteId = SPRITE_NONE;
    }
}

void AddBagVisualSprite(u8 bagPocketId)
{
    u8 *spriteId = &gBagMenu->spriteIds[ITEMMENUSPRITE_BAG];
    *spriteId = CreateSprite(&sBagSpriteTemplate, 68, 66, 0);
    SetBagVisualPocketId(bagPocketId, FALSE);
}

#define sPocketId data[0]

void SetBagVisualPocketId(u8 bagPocketId, bool8 isSwitchingPockets)
{
    struct Sprite *sprite = &gSprites[gBagMenu->spriteIds[ITEMMENUSPRITE_BAG]];
    if (isSwitchingPockets)
    {
        sprite->y2 = -5;
        sprite->callback = SpriteCB_BagVisualSwitchingPockets;
        sprite->sPocketId = bagPocketId;
        StartSpriteAnim(sprite, POCKET_DUMMY);
    }
    else
    {
        StartSpriteAnim(sprite, bagPocketId);
    }
}

static void SpriteCB_BagVisualSwitchingPockets(struct Sprite *sprite)
{
    if (sprite->y2 != 0)
    {
        sprite->y2++;
    }
    else
    {
        StartSpriteAnim(sprite, sprite->sPocketId);
        sprite->callback = SpriteCallbackDummy;
    }
}

#undef sPocketId

void ShakeBagSprite(void)
{
    struct Sprite *sprite = &gSprites[gBagMenu->spriteIds[ITEMMENUSPRITE_BAG]];
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, ANIM_BAG_SHAKE);
        sprite->callback = SpriteCB_ShakeBagSprite;
    }
}

static void SpriteCB_ShakeBagSprite(struct Sprite *sprite)
{
    // Wait for shaking to end
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, ANIM_BAG_NORMAL);
        sprite->callback = SpriteCallbackDummy;
    }
}

void AddSwitchPocketRotatingBallSprite(s16 rotationDirection)
{
    u8 *spriteId = &gBagMenu->spriteIds[ITEMMENUSPRITE_BALL];
    LoadSpriteSheet(&sRotatingBallTable);
    LoadSpritePalette(&sRotatingBallPaletteTable);
    *spriteId = CreateSprite(&sRotatingBallSpriteTemplate, 16, 16, 0);
    gSprites[*spriteId].data[0] = rotationDirection;
}

static void UpdateSwitchPocketRotatingBallCoords(struct Sprite *sprite)
{
    sprite->centerToCornerVecX = sprite->data[1] - ((sprite->data[3] + 1) & 1);
    sprite->centerToCornerVecY = sprite->data[1] - ((sprite->data[3] + 1) & 1);
}

static void SpriteCB_SwitchPocketRotatingBallInit(struct Sprite *sprite)
{
    sprite->oam.affineMode = ST_OAM_AFFINE_NORMAL;
    if (sprite->data[0] == -1)
        sprite->affineAnims = sRotatingBallAnimCmds;
    else
        sprite->affineAnims = sRotatingBallAnimCmds_FullRotation;

    InitSpriteAffineAnim(sprite);
    sprite->data[1] = sprite->centerToCornerVecX;
    sprite->data[1] = sprite->centerToCornerVecY;
    UpdateSwitchPocketRotatingBallCoords(sprite);
    sprite->callback = SpriteCB_SwitchPocketRotatingBallContinue;
}

static void SpriteCB_SwitchPocketRotatingBallContinue(struct Sprite *sprite)
{
    sprite->data[3]++;
    UpdateSwitchPocketRotatingBallCoords(sprite);
    if (sprite->data[3] == 16)
        RemoveBagSprite(ITEMMENUSPRITE_BALL);
}

void AddBagItemIconSprite(enum Item itemId, u8 id)
{
    u8 *spriteId = &gBagMenu->spriteIds[id + ITEMMENUSPRITE_ITEM];
    if (*spriteId == SPRITE_NONE)
    {
        u8 iconSpriteId;

        // Either TAG_ITEM_ICON or TAG_ITEM_ICON_ALT
        FreeSpriteTilesByTag(id + TAG_ITEM_ICON);
        FreeSpritePaletteByTag(id + TAG_ITEM_ICON);
        iconSpriteId = AddItemIconSprite(id + TAG_ITEM_ICON, id + TAG_ITEM_ICON, itemId);
        if (iconSpriteId != MAX_SPRITES)
        {
            *spriteId = iconSpriteId;
            gSprites[iconSpriteId].x2 = 24;
            gSprites[iconSpriteId].y2 = 88;
        }
    }
}

void RemoveBagItemIconSprite(u8 id)
{
// BUG: For one frame, the item you scroll to in the Bag menu
// will have an incorrect palette and may be seen as a flicker.
#ifdef BUGFIX
    u8 *spriteId = &gBagMenu->spriteIds[ITEMMENUSPRITE_ITEM];

    if (spriteId[id ^ 1] != SPRITE_NONE)
        gSprites[spriteId[id ^ 1]].invisible = TRUE;

    if (spriteId[id] != SPRITE_NONE)
    {
        DestroySpriteAndFreeResources(&gSprites[spriteId[id]]);
        spriteId[id] = SPRITE_NONE;
    }
#else
    RemoveBagSprite(id + ITEMMENUSPRITE_ITEM);
#endif
}

void CreateItemMenuSwapLine(void)
{
    CreateSwapLineSprites(&gBagMenu->spriteIds[ITEMMENUSPRITE_SWAP_LINE], ITEMMENU_SWAP_LINE_LENGTH);
}

void SetItemMenuSwapLineInvisibility(bool8 invisible)
{
    SetSwapLineSpritesInvisibility(&gBagMenu->spriteIds[ITEMMENUSPRITE_SWAP_LINE], ITEMMENU_SWAP_LINE_LENGTH, invisible);
}

void UpdateItemMenuSwapLinePos(u8 y)
{
    UpdateSwapLineSpritesPos(&gBagMenu->spriteIds[ITEMMENUSPRITE_SWAP_LINE], ITEMMENU_SWAP_LINE_LENGTH | SWAP_LINE_HAS_MARGIN, 120, (y + 1) * 16);
}

static void ArrangeBerryGfx(void *src, void *dest)
{
    u8 i, j;

    memset(dest, 0, 0x800);

    // Create top margin
    dest += 0x100;

    for (i = 0; i < 6; i++)
    {
        // Create left margin
        dest += 0x20;

        // Copy one row of berry's icon
        for (j = 0; j < 6; j++)
        {
            memcpy(dest, src, 0x20);
            dest += 0x20;
            src += 0x20;
        }

        // Create right margin
        if (i != 5)
            dest += 0x20;
    }
}

#define BERRY_SPRITE_SIZE ((64*64)/2) // 0x800

struct BerryDynamicGfx
{
    ALIGNED(4) u8 gfx[BERRY_SPRITE_SIZE];
    struct SpriteFrameImage images[1];
};

static struct BerryDynamicGfx *LoadBerryGfx(u8 berryId)
{
    struct SpritePalette pal;

    pal.data = sBerryPicTable[berryId].pal;
    pal.tag = TAG_BERRY_PIC_PAL + berryId;
    LoadSpritePalette(&pal);
    struct BerryDynamicGfx *gfxAlloced = Alloc(sizeof(struct BerryDynamicGfx));
    void *buffer = malloc_and_decompress(sBerryPicTable[berryId].tiles, NULL);
    ArrangeBerryGfx(buffer, gfxAlloced->gfx);
    Free(buffer);

    return gfxAlloced;
}

static u32 CreateBerrySprite(const struct SpriteTemplate *sprTemplate, u32 berryId, s32 x, s32 y)
{
    u32 spriteId;
    struct BerryDynamicGfx *dynamicGfx = LoadBerryGfx(berryId);
    struct SpriteTemplate newSprTemplate = *sprTemplate;

    newSprTemplate.paletteTag += berryId;
    newSprTemplate.images = dynamicGfx->images;

    dynamicGfx->images[0].data = dynamicGfx->gfx;
    dynamicGfx->images[0].size = BERRY_SPRITE_SIZE;
    dynamicGfx->images[0].relativeFrames = FALSE;

    spriteId = CreateSprite(&newSprTemplate, x, y, 0);
    StoreWordInTwoHalfwords((u16 *) &gSprites[spriteId].data[BERRY_ICON_GFX_PTR_DATA_ID], (u32) dynamicGfx);
    return spriteId;
}

u32 CreateBerryTagSprite(u32 id, s32 x, s32 y)
{
    return CreateBerrySprite(&sBerryPicSpriteTemplate, id, x, y);
}

// For throwing berries into the Berry Blender
u32 CreateSpinningBerrySprite(u32 berryId, s32 x, s32 y, bool32 startAffine)
{
    u32 spriteId = CreateBerrySprite(&sBerryPicRotatingSpriteTemplate, berryId, x, y);
    if (startAffine)
        StartSpriteAffineAnim(&gSprites[spriteId], 1);

    return spriteId;
}

void DestroyBerryIconSprite(u32 spriteId, u32 berryId, bool32 freePal)
{
    DestroyBerryIconSpritePtr(&gSprites[spriteId], berryId, freePal);
}

void DestroyBerryIconSpritePtr(struct Sprite *sprite, u32 berryId, bool32 freePal)
{
    u32 gfxBuffer;

    LoadWordFromTwoHalfwords((u16 *) &sprite->data[BERRY_ICON_GFX_PTR_DATA_ID], &gfxBuffer);
    Free((void *)gfxBuffer);
    DestroySprite(sprite);
    if (freePal)
        FreeBerryIconSpritePalette(berryId);
}

void FreeBerryIconSpritePalette(u32 berryId)
{
    FreeSpritePaletteByTag(TAG_BERRY_PIC_PAL + berryId);
}

u8 CreateBerryFlavorCircleSprite(s16 x)
{
    return CreateSprite(&sBerryCheckCircleSpriteTemplate, x, 116, 0);
}


u8 CreateSeasonXSprite(s16 x, s16 y) //
{
    return CreateSprite(&sBerrySeasonsXSpriteTemplate, 55 + x, 71 + y, 0); 
}

u8 CreateSeasonQuestionMSprite() //
{
    return CreateSprite(&sBerrySeasonsQuestionSpriteTemplate, 69, 70, 0); 
}

#define sUPLeftSpriteId data[0] //All of this for the flavor grafic sprite...

static void SpriteCB_BerryFlavorGrafUPright(struct Sprite *sprite)//
{
    sprite->invisible = gSprites[sprite->sUPLeftSpriteId].invisible;
    sprite->y = gSprites[sprite->sUPLeftSpriteId].y;
    sprite->x2 = gSprites[sprite->sUPLeftSpriteId].x2;
    sprite->y2 = gSprites[sprite->sUPLeftSpriteId].y2;
}

static void SpriteCB_BerryFlavorGrafDOWNleft(struct Sprite *sprite)//
{
    sprite->invisible = gSprites[sprite->sUPLeftSpriteId].invisible;
    sprite->x2 = gSprites[sprite->sUPLeftSpriteId].x2;
    sprite->y2 = gSprites[sprite->sUPLeftSpriteId].y2;
}

static void SpriteCB_BerryFlavorGrafDOWNright(struct Sprite *sprite)//
{
    sprite->invisible = gSprites[sprite->sUPLeftSpriteId].invisible;
    sprite->x2 = gSprites[sprite->sUPLeftSpriteId].x2;
    sprite->y2 = gSprites[sprite->sUPLeftSpriteId].y2;
}

u8 CreateFlavorGrafSprite(s16 x, s16 y) // MOD CONTEST this one creates the berry flavor grafic sprite. It's too big, so it's loaded with four different pieces.
{
    u8 UPleftSpriteId = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x - 32, y, 1); //added 1 to each subpriority to make it work with the Enigma profile sprite
    u8 UPrightSpriteId = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x + 32, y, 1);
    u8 DOWNleftSpriteId = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x - 32, y + 64, 1);
    u8 DOWNrightSpriteId = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x + 32, y + 64, 1);
    gSprites[UPrightSpriteId].sUPLeftSpriteId = UPleftSpriteId;
    gSprites[DOWNleftSpriteId].sUPLeftSpriteId = UPleftSpriteId;
    gSprites[DOWNrightSpriteId].sUPLeftSpriteId = UPleftSpriteId;
    StartSpriteAnim(&gSprites[UPrightSpriteId], 1); //tells what tile of the spritesheet is each.
    StartSpriteAnim(&gSprites[DOWNleftSpriteId], 2);
    StartSpriteAnim(&gSprites[DOWNrightSpriteId], 3);
    gSprites[UPrightSpriteId].callback = &SpriteCB_BerryFlavorGrafUPright;
    gSprites[DOWNleftSpriteId].callback = &SpriteCB_BerryFlavorGrafDOWNleft;
    gSprites[DOWNrightSpriteId].callback = &SpriteCB_BerryFlavorGrafDOWNright;
    return UPleftSpriteId;
}

void CalculateAndloadProfilePalette(s16 BerryNumber)
{
    const struct Berry *berry = GetBerryInfo(BerryNumber);
    u8 HighestValue = 0;
    u8 Quantity = 0;

    if((berry->spicy >= berry->dry) && (berry->spicy >= berry->sweet) && (berry->spicy >= berry->bitter) && (berry->spicy >= berry->sour))
    {
        HighestValue = berry->spicy;
        Quantity++;
        if(berry->dry == HighestValue)
            Quantity++;
        if(berry->sweet == HighestValue)
            Quantity++;
        if(berry->bitter == HighestValue)
            Quantity++;
        if(berry->sour == HighestValue)
            Quantity++;

        if(Quantity > 3){ //All
            LoadSpritePalette(&gFlavorBalancedPaletteTable);
        }
        else if(Quantity > 2){
            if(berry->sweet == HighestValue && berry->sour != HighestValue){//Spicy+Sweet
            LoadSpritePalette(&gFlavorSpicySweetPaletteTable);
            }
            else if(berry->bitter == HighestValue){ //Spicy+Bitter
            LoadSpritePalette(&gFlavorSpicyBitterPaletteTable);
            }
            else{ //Sour+Sweet 
            LoadSpritePalette(&gFlavorSweetSourPaletteTable);
            }
        }
        else if(Quantity > 1){
            if(berry->dry == HighestValue){//Spicy+Dry
                LoadSpritePalette(&gFlavorSpicyDryPaletteTable);
            }
            else if(berry->sweet == HighestValue){//Spicy+Sweet
                LoadSpritePalette(&gFlavorSpicySweetPaletteTable);
            }
            else if(berry->bitter == HighestValue){//Spicy+Bitter
                LoadSpritePalette(&gFlavorSpicyBitterPaletteTable);
            }
            else{//Spicy+Sour
                LoadSpritePalette(&gFlavorSpicySourPaletteTable);
            }
        }
        else{//Spicy
                LoadSpritePalette(&gFlavorSpicyPaletteTable);
        }
    }
    else if((berry->dry >= berry->sweet) && (berry->dry >= berry->bitter) && (berry->dry >= berry->sour))
    {
        HighestValue = berry->dry;
        Quantity++;
        if(berry->sweet == HighestValue)
            Quantity++;
        if(berry->bitter == HighestValue)
            Quantity++;
        if(berry->sour == HighestValue)
            Quantity++;

        if(Quantity > 3){ //All
            LoadSpritePalette(&gFlavorBalancedPaletteTable);
        }
        else if(Quantity > 2){
            if(HighestValue == berry->sweet){//Dry+Sour
                LoadSpritePalette(&gFlavorDrySourPaletteTable);
            }
            else{//Dry+Bitter
                LoadSpritePalette(&gFlavorDryBitterPaletteTable);
            }
        }
        else if(Quantity > 1){
            if(berry->sweet == HighestValue){ //Dry + Sweet
                LoadSpritePalette(&gFlavorDrySweetPaletteTable);
            }
            else if(berry->bitter == HighestValue){ //Dry + Bitter
                LoadSpritePalette(&gFlavorDryBitterPaletteTable);
            }
            else{ //Dry + Sour
                LoadSpritePalette(&gFlavorDrySourPaletteTable);
            }
        }
        else{//Dry !
                LoadSpritePalette(&gFlavorDryPaletteTable);
        }
    }
    else if((berry->sweet >= berry->bitter) && (berry->sweet >= berry->sour))
    {
        HighestValue = berry->sweet;
        if(HighestValue == berry->bitter){//Sweet+Bitter !
            LoadSpritePalette(&gFlavorSweetBitterPaletteTable);
        }
        else if(HighestValue == berry->sour){ //Sweet+Sour
            LoadSpritePalette(&gFlavorSweetSourPaletteTable);
        }
        else{//Sweet
            LoadSpritePalette(&gFlavorSweetPaletteTable);
        }
    }
    else if((berry->bitter >= berry->sour)){ 
        if (berry->bitter == berry->sour){//Bitter+Sour
            HighestValue = berry->bitter;
            LoadSpritePalette(&gFlavorBitterSourPaletteTable);
        }
        else{//Bitter
            HighestValue = berry->sour;
            LoadSpritePalette(&gFlavorBitterPaletteTable);
        }
    }
    else{//Sour
            LoadSpritePalette(&gFlavorSourPaletteTable);
    }
}

u8 CreateBerryFlowerSprite(u32 BerryNumber) //MOD CONTEST this handles the berry flower sprites
{
    LoadSpritePalette(&gBerryFlowerPaletteTable[BerryNumber]);
    LoadCompressedSpriteSheet(&gBerryFlowerSpriteSheet[BerryNumber]);
    return CreateSprite(&sBerryFlowerSpriteTemplate, 56, 64, 0);
}

u8 CreateBerrySeasonsSprite(void) //MOD CONTEST
{
    LoadSpritePalette(&gBerrySeasonsPaletteTable);
    LoadCompressedSpriteSheet(&gBerrySeasonSpriteSheet);
    return CreateSprite(&sBerrySeasonsSpriteTemplate, 69, 70, 0); 
}

void LoadFlavorProfileSpriteData(s16 BerryNumber) //this one returns the sprite that get's loaded on memory, and unloads the previous one.
{
    const struct Berry *berry = GetBerryInfo(BerryNumber);

    switch(berry->graficGroup){
        case 1:
            LoadCompressedSpriteSheet(&gFlavorProfileG1SpriteSheet);
            break;
        case 2:
            LoadCompressedSpriteSheet(&gFlavorProfileG2SpriteSheet);
            break;
        case 3:
            LoadCompressedSpriteSheet(&gFlavorProfileG3SpriteSheet);
            break;
        case 4:
            LoadCompressedSpriteSheet(&gFlavorProfileG4SpriteSheet);
            break;
        case 5:
            LoadCompressedSpriteSheet(&gFlavorProfileG5SpriteSheet);
            break;
        case 6:
            LoadCompressedSpriteSheet(&gFlavorProfileG6SpriteSheet);
            break;
        case 7:
            LoadCompressedSpriteSheet(&gFlavorProfileG7SpriteSheet); 
            break;
        default:
            LoadCompressedSpriteSheet(&gFlavorProfileG8SpriteSheet);
            break;
    }
}
    
u8 CreateFlavorProfileSprite1(s16 BerryNumber, s16 x, s16 y) // MOD CONTEST this one creates the berry flavor grafic sprite. It's loaded with four different pieces, since it varies on size and complexity.
{ 
    const struct Berry *berry = GetBerryInfo(BerryNumber);

    u8 SpriteId;

    if(BerryNumber == (ItemIdToBerryType(ITEM_ENIGMA_BERRY))){ //Enigma Berry
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x - 32, y, 1);
        StartSpriteAnim(&gSprites[SpriteId], 2);
    }
    else 
    {
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x + 5, y + 38, 1);//Regular berry profile 1 
        StartSpriteAnim(&gSprites[SpriteId], berry->tileNumber1+1);
    }
    return SpriteId;
}
    
u8 CreateFlavorProfileSprite2(s16 BerryNumber, s16 x, s16 y)
{ 
    const struct Berry *berry = GetBerryInfo(BerryNumber);

    u8 SpriteId;

    if(BerryNumber == (ItemIdToBerryType(ITEM_ENIGMA_BERRY))){ //Enigma Berry
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x + 32, y, 1);
        StartSpriteAnim(&gSprites[SpriteId], 3);
    }
    else 
    {
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x + 5, y + 38, 1); //Regular berry profile 2 (for combined versions)
                StartSpriteAnim(&gSprites[SpriteId], berry->tileNumber2+1);
    }
    return SpriteId;
}
    
u8 CreateFlavorProfileSprite3(s16 BerryNumber, s16 x, s16 y)
{ 

    u8 SpriteId;

    if(BerryNumber == (ItemIdToBerryType(ITEM_ENIGMA_BERRY)))
    { //Enigma Berry
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x - 32, y + 64, 1); //Enigma Berry
        StartSpriteAnim(&gSprites[SpriteId], 4);
    }
    else
    {
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x, y, 1); //Enigma Berry
        StartSpriteAnim(&gSprites[SpriteId], 1);
    }
    return SpriteId;
}
    
u8 CreateFlavorProfileSprite4(s16 BerryNumber, s16 x, s16 y)
{ 
    u8 SpriteId;

    if(BerryNumber == (ItemIdToBerryType(ITEM_ENIGMA_BERRY)))
    { //Enigma Berry
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x + 32, y + 64, 1); //Enigma Berry
        StartSpriteAnim(&gSprites[SpriteId], 5);
    }
    else
    {
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x, y, 1); //Enigma Berry
        StartSpriteAnim(&gSprites[SpriteId], 1);
    }
    return SpriteId;
}