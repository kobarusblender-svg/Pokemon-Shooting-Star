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
static const u16 sRotatingBall_Pal[] = INCGFX_U16("graphics/bag/rotating_ball.png", ".gbapal");
static const u8 sRotatingBall_Gfx[] = INCGFX_U8("graphics/bag/rotating_ball.png", ".4bpp");
static const u8 sCherryUnused[] = INCGFX_U8("graphics/unused/cherry.png", ".4bpp");
static const u16 sCherryUnused_Pal[] = INCGFX_U16("graphics/unused/cherry.png", ".gbapal");

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
    .affineAnims = sBagAffineAnimCmds,
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
    .affineAnims = sBerryPicRotatingAnimCmds,
};

const struct CompressedSpriteSheet gBerryFlowerSpriteSheet[] = // MOD CONTEST Berry Flower Sprites. Don't ask, I don't know.
{
    [BERRY_ID_CHERI]          = {gFlowerPic_Cheri,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Cheri},
    [BERRY_ID_CHESTO]          = {gFlowerPic_Chesto, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Chesto},
    [BERRY_ID_PECHA]          = {gFlowerPic_Pecha,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pecha},
    [BERRY_ID_RAWST]          = {gFlowerPic_Rawst,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rawst},
    [BERRY_ID_ASPEAR]          = {gFlowerPic_Aspear, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Aspear},
    [BERRY_ID_LEPPA]          = {gFlowerPic_Leppa,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Leppa},
    [BERRY_ID_ORAN]          = {gFlowerPic_Oran,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Oran},
    [BERRY_ID_PERSIM]          = {gFlowerPic_Persim, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Persim},
    [BERRY_ID_LUM]          = {gFlowerPic_Lum,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Lum},
    [BERRY_ID_SITRUS]          = {gFlowerPic_Sitrus, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Sitrus},
    [BERRY_ID_FIGY]          = {gFlowerPic_Figy,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Figy},
    [BERRY_ID_WIKI]          = {gFlowerPic_Wiki,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Wiki},
    [BERRY_ID_MAGO]          = {gFlowerPic_Mago,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Mago},
    [BERRY_ID_AGUAV]          = {gFlowerPic_Aguav,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Aguav},
    [BERRY_ID_IAPAPA]          = {gFlowerPic_Iapapa, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Iapapa},
    [BERRY_ID_RAZZ]          = {gFlowerPic_Razz,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Razz},
    [BERRY_ID_BLUK]          = {gFlowerPic_Bluk,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Bluk},
    [BERRY_ID_NANAB]          = {gFlowerPic_Nanab,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Nanab},
    [BERRY_ID_WEPEAR]          = {gFlowerPic_Wepear, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Wepear},
    [BERRY_ID_PINAP]          = {gFlowerPic_Pinap,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pinap},
    [BERRY_ID_POMEG]          = {gFlowerPic_Pomeg,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pomeg},
    [BERRY_ID_KELPSY]          = {gFlowerPic_Kelpsy, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kelpsy},
    [BERRY_ID_QUALOT]          = {gFlowerPic_Qualot, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Qualot},
    [BERRY_ID_HONDEW]          = {gFlowerPic_Hondew, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Hondew},
    [BERRY_ID_GREPA]          = {gFlowerPic_Grepa,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Grepa},
    [BERRY_ID_TAMATO]          = {gFlowerPic_Tamato, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Tamato},
    [BERRY_ID_CORNN]          = {gFlowerPic_Cornn,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Cornn},
    [BERRY_ID_MAGOST]          = {gFlowerPic_Magost, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Magost},
    [BERRY_ID_RABUTA]          = {gFlowerPic_Rabuta, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rabuta},
    [BERRY_ID_NOMEL]          = {gFlowerPic_Nomel,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Nomel},
    [BERRY_ID_SPELON]          = {gFlowerPic_Spelon, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Spelon},
    [BERRY_ID_PAMTRE]          = {gFlowerPic_Pamtre, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pamtre},
    [BERRY_ID_WATMEL]          = {gFlowerPic_Watmel, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Watmel},
    [BERRY_ID_DURIN]          = {gFlowerPic_Durin,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Durin},
    [BERRY_ID_BELUE]          = {gFlowerPic_Belue,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Belue},
    [BERRY_ID_PUMKIN]          = {gFlowerPic_Pumkin,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Pumkin},// MOD CONTEST E-reader berry graphics:
    [BERRY_ID_DRASH]          = {gFlowerPic_Drash,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Drash},//
    [BERRY_ID_EGGANT]          = {gFlowerPic_Eggant,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Eggant},//
    [BERRY_ID_STRIB]          = {gFlowerPic_Strib,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Strib},//
    [BERRY_ID_CHILANER]          = {gFlowerPic_ChilanER, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_ChilanER},//
    [BERRY_ID_NUTPEA]          = {gFlowerPic_Nutpea,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Nutpea},//
    [BERRY_ID_NOION]          = {gFlowerPic_Noion,    0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Noion},//(Japan exclusive Ginema berry)
    [BERRY_ID_ROKA]          = {gFlowerPic_Roka,     0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Roka},//(Japan exclusive Kuo berry)
    [BERRY_ID_SKUASH]          = {gFlowerPic_Skuash,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Skuash},//(Japan exclusive Yago berry)
    [BERRY_ID_PAPRIK]          = {gFlowerPic_Paprik,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Paprik},//(Japan exclusive Touga berry)
    [BERRY_ID_MALIUM]          = {gFlowerPic_Malium,   0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Malium},//(Japan exclusive Niniku berry)
    [BERRY_ID_TOPO]          = {gFlowerPic_Topo,     0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Topo},//(Japan exclusive Topo berry)
    //[BERRY_ID_CHILAN]          = {gFlowerPic_Chilan, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Chilan},
    //[BERRY_ID_OCCA]          = {gFlowerPic_Occa, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Occa},
    //[BERRY_ID_PASSHO]          = {gFlowerPic_Passho, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Passho},
    //[BERRY_ID_WACAN]          = {gFlowerPic_Wacan, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Wacan},
    //[BERRY_ID_RINDO]          = {gFlowerPic_Rindo, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rindo},
    //[BERRY_ID_YACHE]          = {gFlowerPic_Yache, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Yache},
    //[BERRY_ID_CHOPLE]          = {gFlowerPic_Chople, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Chople},
    //[BERRY_ID_KEBIA]          = {gFlowerPic_Kebia, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kebia},
    //[BERRY_ID_SHUCA]          = {gFlowerPic_Shuca, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Shuca},
    //[BERRY_ID_COBA]          = {gFlowerPic_Coba, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Coba},
    //[BERRY_ID_PAYAPA]          = {gFlowerPic_Payapa, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Payapa},
    //[BERRY_ID_TANGA]          = {gFlowerPic_Tanga, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Tanga},
    //[BERRY_ID_CHARTI]          = {gFlowerPic_Charti, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Charti},
    //[BERRY_ID_KASIB]          = {gFlowerPic_Kasib, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kasib},
    //[BERRY_ID_HABAN]          = {gFlowerPic_Haban, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Haban},
    //[BERRY_ID_COLBUR]          = {gFlowerPic_Colbur, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Colbur},
    //[BERRY_ID_BABIRI]          = {gFlowerPic_Babiri, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Babiri},
    //[BERRY_ID_ROSELI]          = {gFlowerPic_Roseli, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Roseli},
    [BERRY_ID_LIECHI]          = {gFlowerPic_Liechi, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Liechi},
    [BERRY_ID_GANLON]          = {gFlowerPic_Ganlon, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Ganlon},
    [BERRY_ID_SALAC]          = {gFlowerPic_Salac,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Salac},
    [BERRY_ID_PETAYA]          = {gFlowerPic_Petaya, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Petaya},
    [BERRY_ID_APICOT]          = {gFlowerPic_Apicot, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Apicot},
    [BERRY_ID_LANSAT]          = {gFlowerPic_Lansat, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Lansat},
    [BERRY_ID_STARF]          = {gFlowerPic_Starf,  0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Starf},
    [BERRY_ID_ENIGMA]          = {gFlowerPic_Enigma, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Enigma},
    //[BERRY_ID_MICLE]          = {gFlowerPic_Micle, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Micle},
    //[BERRY_ID_CUSTAP]          = {gFlowerPic_Custap, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Custap},
    //[BERRY_ID_JABOCA]          = {gFlowerPic_Jaboca, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Jaboca},
    //[BERRY_ID_ROWAP]          = {gFlowerPic_Rowap, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Rowap},
    //[BERRY_ID_KEE]          = {gFlowerPic_Kee, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Kee},
    //[BERRY_ID_MARANGA]          = {gFlowerPic_Maranga, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Maranga},
    [BERRY_ID_ENGIMA_E_READER] = {gFlowerPic_Enigma, 0x800, TAG_BERRY_FLOWER_GFX}, //gFlowerPalette_Enigma},
};

const struct SpritePalette gBerryFlowerPaletteTable[] = // MOD CONTEST Berry Flower palettes
{
    [BERRY_ID_CHERI]          = {gFlowerPalette_Cheri, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_CHESTO]          = {gFlowerPalette_Chesto, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_PECHA]          = {gFlowerPalette_Pecha, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_RAWST]          = {gFlowerPalette_Rawst, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_ASPEAR]          = {gFlowerPalette_Aspear, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_LEPPA]          = {gFlowerPalette_Leppa, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_ORAN]          = {gFlowerPalette_Oran, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_PERSIM]          = {gFlowerPalette_Persim, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_LUM]          = {gFlowerPalette_Lum, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_SITRUS]          = {gFlowerPalette_Sitrus, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_FIGY]          = {gFlowerPalette_Figy, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_WIKI]          = {gFlowerPalette_Wiki, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_MAGO]          = {gFlowerPalette_Mago, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_AGUAV]          = {gFlowerPalette_Aguav, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_IAPAPA]          = {gFlowerPalette_Iapapa, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_RAZZ]          = {gFlowerPalette_Razz, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_BLUK]          = {gFlowerPalette_Bluk, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_NANAB]          = {gFlowerPalette_Nanab, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_WEPEAR]          = {gFlowerPalette_Wepear, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_PINAP]          = {gFlowerPalette_Pinap, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_POMEG]          = {gFlowerPalette_Pomeg, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_KELPSY]          = {gFlowerPalette_Kelpsy, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_QUALOT]          = {gFlowerPalette_Qualot, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_HONDEW]          = {gFlowerPalette_Hondew, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_GREPA]          = {gFlowerPalette_Grepa, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_TAMATO]          = {gFlowerPalette_Tamato, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_CORNN]          = {gFlowerPalette_Cornn, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_MAGOST]          = {gFlowerPalette_Magost, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_RABUTA]          = {gFlowerPalette_Rabuta, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_NOMEL]          = {gFlowerPalette_Nomel, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_SPELON]          = {gFlowerPalette_Spelon, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_PAMTRE]          = {gFlowerPalette_Pamtre, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_WATMEL]          = {gFlowerPalette_Watmel, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_DURIN]          = {gFlowerPalette_Durin, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_BELUE]          = {gFlowerPalette_Belue, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_PUMKIN]          = {gFlowerPalette_Pumkin, TAG_BERRY_FLOWER_GFX},// MOD CONTEST E-reader berry graphics:
    [BERRY_ID_DRASH]          = {gFlowerPalette_Drash, TAG_BERRY_FLOWER_GFX},//
    [BERRY_ID_EGGANT]          = {gFlowerPalette_Eggant, TAG_BERRY_FLOWER_GFX},//
    [BERRY_ID_STRIB]          = {gFlowerPalette_Strib, TAG_BERRY_FLOWER_GFX},//
    [BERRY_ID_CHILANER]          = {gFlowerPalette_ChilanER, TAG_BERRY_FLOWER_GFX},//
    [BERRY_ID_NUTPEA]          = {gFlowerPalette_Nutpea, TAG_BERRY_FLOWER_GFX},//
    [BERRY_ID_NOION]          = {gFlowerPalette_Noion, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Ginema berry)
    [BERRY_ID_ROKA]          = {gFlowerPalette_Roka, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Kuo berry)
    [BERRY_ID_SKUASH]          = {gFlowerPalette_Skuash, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Yago berry)
    [BERRY_ID_PAPRIK]          = {gFlowerPalette_Paprik, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Touga berry)
    [BERRY_ID_MALIUM]          = {gFlowerPalette_Malium, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Niniku berry)
    [BERRY_ID_TOPO]          = {gFlowerPalette_Topo, TAG_BERRY_FLOWER_GFX},//(Japan exclusive Topo berry)
    //[BERRY_ID_CHILAN]          = {gFlowerPalette_Chilan, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_OCCA]          = {gFlowerPalette_Occa, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_PASSHO]          = {gFlowerPalette_Passho, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_WACAN]          = {gFlowerPalette_Wacan, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_RINDO]          = {gFlowerPalette_Rindo, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_YACHE]          = {gFlowerPalette_Yache, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_CHOPLE]          = {gFlowerPalette_Chople, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_KEBIA]          = {gFlowerPalette_Kebia, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_SHUCA]          = {gFlowerPalette_Shuca, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_COBA]          = {gFlowerPalette_Coba, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_PAYAPA]          = {gFlowerPalette_Payapa, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_TANGA]          = {gFlowerPalette_Tanga, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_CHARTI]          = {gFlowerPalette_Charti, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_KASIB]          = {gFlowerPalette_Kasib, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_HABAN]          = {gFlowerPalette_Haban, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_COLBUR]          = {gFlowerPalette_Colbur, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_BABIRI]          = {gFlowerPalette_Babiri, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_ROSELI]          = {gFlowerPalette_Roseli, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_LIECHI]          = {gFlowerPalette_Liechi, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_GANLON]          = {gFlowerPalette_Ganlon, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_SALAC]          = {gFlowerPalette_Salac, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_PETAYA]          = {gFlowerPalette_Petaya, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_APICOT]          = {gFlowerPalette_Apicot, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_LANSAT]          = {gFlowerPalette_Lansat, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_STARF]          = {gFlowerPalette_Starf, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_ENIGMA]          = {gFlowerPalette_Enigma, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_MICLE]          = {gFlowerPalette_Micle, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_CUSTAP]          = {gFlowerPalette_Custap, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_JABOCA]          = {gFlowerPalette_Jaboca, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_ROWAP]          = {gFlowerPalette_Rowap, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_KEE]          = {gFlowerPalette_Kee, TAG_BERRY_FLOWER_GFX},
    //[BERRY_ID_MARANGA]          = {gFlowerPalette_Maranga, TAG_BERRY_FLOWER_GFX},
    [BERRY_ID_ENGIMA_E_READER] = {gFlowerPalette_Enigma, TAG_BERRY_FLOWER_GFX},
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

    pal.data = gBerries[berryId].berryPal;
    pal.tag = TAG_BERRY_PIC_PAL + berryId;
    LoadSpritePalette(&pal);
    struct BerryDynamicGfx *gfxAlloced = Alloc(sizeof(struct BerryDynamicGfx));
    void *buffer = malloc_and_decompress(gBerries[berryId].berryPic, NULL);
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

u8 CreateFlavorGrafSprite(s16 x, s16 y, s16 tile) // MOD CONTEST this one creates the berry flavor grafic sprite. It's too big, so it's loaded with four different pieces.
{   
    u8 CurrentTile;

    switch(tile)
    {
        case 0:
            CurrentTile = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x - 32, y, 1);
            StartSpriteAnim(&gSprites[CurrentTile], 0);
            break;
        case 1:
            CurrentTile = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x + 32, y, 1);
            StartSpriteAnim(&gSprites[CurrentTile], 1);
            break;
        case 2:
            CurrentTile = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x - 32, y + 64, 1);
            StartSpriteAnim(&gSprites[CurrentTile], 2);
            break;
        case 3:
            CurrentTile = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x + 32, y + 64, 1);
            StartSpriteAnim(&gSprites[CurrentTile], 3);
            break;
        default:
            CurrentTile = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x - 32, y, 1);
            StartSpriteAnim(&gSprites[CurrentTile], 0);
            break;
    }
    return CurrentTile;
}

void CalculateAndloadProfilePalette(s16 BerryNumber)
{
    const struct BerryInfo *berryInfo = GetBerryInfo(BerryNumber);
    u8 HighestValue = 0;
    u8 Quantity = 0;

    if((berryInfo->spicy >= berryInfo->dry) && (berryInfo->spicy >= berryInfo->sweet) && (berryInfo->spicy >= berryInfo->bitter) && (berryInfo->spicy >= berryInfo->sour))
    {
        HighestValue = berryInfo->spicy;
        Quantity++;
        if(berryInfo->dry == HighestValue)
            Quantity++;
        if(berryInfo->sweet == HighestValue)
            Quantity++;
        if(berryInfo->bitter == HighestValue)
            Quantity++;
        if(berryInfo->sour == HighestValue)
            Quantity++;

        if(Quantity > 3){ //All
            LoadSpritePalette(&gFlavorBalancedPaletteTable);
        }
        else if(Quantity > 2){
            if(berryInfo->sweet == HighestValue && berryInfo->sour != HighestValue){//Spicy+Sweet
            LoadSpritePalette(&gFlavorSpicySweetPaletteTable);
            }
            else if(berryInfo->bitter == HighestValue){ //Spicy+Bitter
            LoadSpritePalette(&gFlavorSpicyBitterPaletteTable);
            }
            else{ //Sour+Sweet 
            LoadSpritePalette(&gFlavorSweetSourPaletteTable);
            }
        }
        else if(Quantity > 1){
            if(berryInfo->dry == HighestValue){//Spicy+Dry
                LoadSpritePalette(&gFlavorSpicyDryPaletteTable);
            }
            else if(berryInfo->sweet == HighestValue){//Spicy+Sweet
                LoadSpritePalette(&gFlavorSpicySweetPaletteTable);
            }
            else if(berryInfo->bitter == HighestValue){//Spicy+Bitter
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
    else if((berryInfo->dry >= berryInfo->sweet) && (berryInfo->dry >= berryInfo->bitter) && (berryInfo->dry >= berryInfo->sour))
    {
        HighestValue = berryInfo->dry;
        Quantity++;
        if(berryInfo->sweet == HighestValue)
            Quantity++;
        if(berryInfo->bitter == HighestValue)
            Quantity++;
        if(berryInfo->sour == HighestValue)
            Quantity++;

        if(Quantity > 3){ //All
            LoadSpritePalette(&gFlavorBalancedPaletteTable);
        }
        else if(Quantity > 2){
            if(HighestValue == berryInfo->sweet){//Dry+Sour
                LoadSpritePalette(&gFlavorDrySourPaletteTable);
            }
            else{//Dry+Bitter
                LoadSpritePalette(&gFlavorDryBitterPaletteTable);
            }
        }
        else if(Quantity > 1){
            if(berryInfo->sweet == HighestValue){ //Dry + Sweet
                LoadSpritePalette(&gFlavorDrySweetPaletteTable);
            }
            else if(berryInfo->bitter == HighestValue){ //Dry + Bitter
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
    else if((berryInfo->sweet >= berryInfo->bitter) && (berryInfo->sweet >= berryInfo->sour))
    {
        HighestValue = berryInfo->sweet;
        if(HighestValue == berryInfo->bitter){//Sweet+Bitter !
            LoadSpritePalette(&gFlavorSweetBitterPaletteTable);
        }
        else if(HighestValue == berryInfo->sour){ //Sweet+Sour
            LoadSpritePalette(&gFlavorSweetSourPaletteTable);
        }
        else{//Sweet
            LoadSpritePalette(&gFlavorSweetPaletteTable);
        }
    }
    else if((berryInfo->bitter >= berryInfo->sour)){ 
        if (berryInfo->bitter == berryInfo->sour){//Bitter+Sour
            HighestValue = berryInfo->bitter;
            LoadSpritePalette(&gFlavorBitterSourPaletteTable);
        }
        else{//Bitter
            HighestValue = berryInfo->sour;
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
    const struct BerryInfo *berryInfo = GetBerryInfo(BerryNumber);

    switch(berryInfo->graficGroup){
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
    const struct BerryInfo *berryInfo = GetBerryInfo(BerryNumber);

    u8 SpriteId;

    if(BerryNumber == (BERRY_ID_ENIGMA)){ //Enigma Berry
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x - 32, y, 1);
        StartSpriteAnim(&gSprites[SpriteId], 2);
    }
    else 
    {
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x + 5, y + 38, 1);//Regular berry profile 1 
        StartSpriteAnim(&gSprites[SpriteId], berryInfo->tileNumber1+1);
    }
    return SpriteId;
}
    
u8 CreateFlavorProfileSprite2(s16 BerryNumber, s16 x, s16 y)
{ 
    const struct BerryInfo *berryInfo = GetBerryInfo(BerryNumber);

    u8 SpriteId;

    if(BerryNumber == (BERRY_ID_ENIGMA)){ //Enigma Berry
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x + 32, y, 1);
        StartSpriteAnim(&gSprites[SpriteId], 3);
    }
    else 
    {
        SpriteId = CreateSprite(&sBerryFlavorProfileSpriteTemplate, x + 5, y + 38, 1); //Regular berry profile 2 (for combined versions)
                StartSpriteAnim(&gSprites[SpriteId], berryInfo->tileNumber2+1);
    }
    return SpriteId;
}
    
u8 CreateFlavorProfileSprite3(s16 BerryNumber, s16 x, s16 y)
{ 

    u8 SpriteId;

    if(BerryNumber == (BERRY_ID_ENIGMA))
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

    if(BerryNumber == (BERRY_ID_ENIGMA))
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