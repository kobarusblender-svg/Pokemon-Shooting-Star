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

    /*u8 UPleftSpriteId = CreateSprite(&sBerryFlavorGrafSpriteTemplate, x - 32, y, 1); //added 1 to each subpriority to make it work with the Enigma profile sprite
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
    return UPleftSpriteId;*/ //MOD CONTEST OLD CODE
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
/*
u8 CreateBerryFlowerSprite(s16 BerryNumber) //MOD CONTEST this handles the berry flower sprites
{
    struct CompressedSpriteSheet pic;
    struct SpritePalette pal;
    
    pal.data = gBerries[BerryNumber].flowerPal;
    pal.tag = TAG_BERRY_FLOWER_GFX;
    LoadSpritePalette(&pal);

    
    pic.data = gBerries[BerryNumber].flowerPic;
    pic.tag = TAG_BERRY_FLOWER_GFX;
    LoadCompressedSpriteSheet(&pic);

    return CreateSprite(&sBerryFlowerSpriteTemplate, 56, 64, 0);
}*/


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

    if(BerryNumber == (ItemIdToBerryType(ITEM_ENIGMA_BERRY))){ //Enigma Berry
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

    if(BerryNumber == (ItemIdToBerryType(ITEM_ENIGMA_BERRY))){ //Enigma Berry
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