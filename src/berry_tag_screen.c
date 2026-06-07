#include "global.h"
#include "berry_tag_screen.h"
#include "berry.h"
#include "decompress.h"
#include "event_object_movement.h"
#include "item_menu.h"
#include "item.h"
#include "item_use.h"
#include "main.h"
#include "menu.h"
#include "text.h"
#include "window.h"
#include "task.h"
#include "menu_helpers.h"
#include "palette.h"
#include "overworld.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "bg.h"
#include "malloc.h"
#include "scanline_effect.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item_menu_icons.h"
#include "decompress.h"
#include "international_string_util.h"
#include "constants/items.h"
#include "constants/rgb.h"
#include "constants/songs.h"

// There are 4 windows used in berry tag screen.
enum //MOD CONTEST
{
    WIN_BERRY_NAME,
    WIN_SIZE_FIRM,
    WIN_DESC,
    WIN_BERRY_TAG,
    WIN_BERRY_DATA //MOD CONTEST New window, shows how many berries it yields, growing time and feel. Maybe I'll fancy a planting season as flavor text. 
};

struct BerryTagScreenStruct //MOD CONTEST
{
    u16 tilemapBuffers[5][0x400]; //Previously 4
    u16 berryId;
    u16 currentSpriteBerryId;
    u8 berrySpriteId;
    u8 flavorCircleIds[FLAVOR_COUNT];
    u16 gfxState;
    s8 flipBlendY;// MOD CONTEST
    u16 tagTop;//
    bool8 onBack; //
    u8 flipDrawState;//
    u8 mainState;//
    u8 printState;//
    u8 seasonSpriteId;//
    u8 flavorGrafSpriteId;//
    u8 flipping;//
    u8 flavorXIds[8]; //
    u8 seasonQuestionSpriteId;//
    u8 flavorProfileSpriteIds[4];//
    //u8 flowerSprite; //
};

// EWRAM vars
static EWRAM_DATA struct BerryTagScreenStruct *sBerryTag = NULL;

// const rom data
static const struct BgTemplate sBackgroundTemplates[] = //MOD CONTEST THIS HANDLES THE BACKGROUND SPRITES (including big ones like the whole tag)
{
  {
      .bg = 0,
      .charBaseIndex = 0,
      .mapBaseIndex = 30,
      .screenSize = 0,
      .paletteMode = 0,
      .priority = 0, //Berry tag title
      .baseTile = 0
  },
  {
      .bg = 1,
      .charBaseIndex = 1, //previously 0, it was bad.
      .mapBaseIndex = 29,
      .screenSize = 0,
      .paletteMode = 0,
      .priority = 1, //BerryTag itself
      .baseTile = 0
  },
  {
      .bg = 2,
      .charBaseIndex = 0,
      .mapBaseIndex = 28,
      .screenSize = 0,
      .paletteMode = 0,
      .priority = 2, //something on the BG
      .baseTile = 0
  },
  {
      .bg = 3,
      .charBaseIndex = 0,
      .mapBaseIndex = 27,
      .screenSize = 0,
      .paletteMode = 0,
      .priority = 3, // base Background
      .baseTile = 0
  }
};

static const u16 sFontPalette[] = INCBIN_U16("graphics/bag/berry_tag_screen.gbapal");

static const u8 sTextColors[2][3] =
{
    {0, 2, 3},
    {15, 14, 13}
};

static const struct WindowTemplate sWindowTemplates[] =
{
    [WIN_BERRY_NAME] = {
        .bg = 1,
        .tilemapLeft = 11,
        .tilemapTop = 4,
        .width = 8,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 69,
    },
    [WIN_SIZE_FIRM] = {
        .bg = 1,
        .tilemapLeft = 11,
        .tilemapTop = 7,
        .width = 18,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 85,
    },
    [WIN_DESC] = {
        .bg = 1,
        .tilemapLeft = 4,
        .tilemapTop = 14,
        .width = 25,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 157,
    },
    [WIN_BERRY_TAG] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 0,
        .width = 8,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 257,
    },
    [WIN_BERRY_DATA] = {
        .bg = 1,
        .tilemapLeft = 4,
        .tilemapTop = 11,
        .width = 12,
        .height = 7,
        .paletteNum = 15,
        .baseBlock = 257,
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 *const sBerryFirmnessStrings[] =
{
    [BERRY_FIRMNESS_UNKNOWN]    = COMPOUND_STRING("???"),
    [BERRY_FIRMNESS_VERY_SOFT]  = COMPOUND_STRING("Very soft"),
    [BERRY_FIRMNESS_SOFT]       = COMPOUND_STRING("Soft"),
    [BERRY_FIRMNESS_HARD]       = COMPOUND_STRING("Hard"),
    [BERRY_FIRMNESS_VERY_HARD]  = COMPOUND_STRING("Very hard"),
    [BERRY_FIRMNESS_SUPER_HARD] = COMPOUND_STRING("Super hard")
};

static const u8 *const sBerryFeelStrings[] = //MOD CONTEST
{
    [BERRY_FEEL_UNKNOWN]    = COMPOUND_STRING("FEEL / ???"),
    [BERRY_FEEL_SILKY]      = COMPOUND_STRING("FEEL / Silky"),
    [BERRY_FEEL_SMOOTH]     = COMPOUND_STRING("FEEL / Smooth"),
    [BERRY_FEEL_TENDER]     = COMPOUND_STRING("FEEL / Tender"),
    [BERRY_FEEL_CHEWY]      = COMPOUND_STRING("FEEL / Chewy"),
    [BERRY_FEEL_GRITTY]     = COMPOUND_STRING("FEEL / Gritty"),
    [BERRY_FEEL_CRUMBLY]    = COMPOUND_STRING("FEEL / Crumbly"),
    [BERRY_FEEL_DENSE]      = COMPOUND_STRING("FEEL / Dense")
};


static const u8 *const sBerryGrowthStrings[] = //MOD CONTEST
{
    [BERRY_GROWTH_UNKNOWN]      = COMPOUND_STRING("GROWTH / ???"),
    [BERRY_GROWTH_FAST]         = COMPOUND_STRING("GROWTH / Fast"),
    [BERRY_GROWTH_QUICK]        = COMPOUND_STRING("GROWTH / Quick"),
    [BERRY_GROWTH_EVEN]         = COMPOUND_STRING("GROWTH / Even"),
    [BERRY_GROWTH_SLOW]         = COMPOUND_STRING("GROWTH / Slow"),
    [BERRY_GROWTH_LATE]         = COMPOUND_STRING("GROWTH / Late")
};

static const u8 *const sBerryYieldStrings[] = //MOD CONTEST
{
    [BERRY_YIELD_UNKNOWN]       = COMPOUND_STRING("YIELD / ???"),
    [BERRY_YIELD_A_PAIR]        = COMPOUND_STRING("YIELD / A pair"),
    [BERRY_YIELD_A_FEW]         = COMPOUND_STRING("YIELD / A few"),
    [BERRY_YIELD_SOME]          = COMPOUND_STRING("YIELD / Some"),
    [BERRY_YIELD_LOTS]          = COMPOUND_STRING("YIELD / Lots"),
    [BERRY_YIELD_PLENTY]        = COMPOUND_STRING("YIELD / Plenty")
};


// this file's functions
static void CB2_InitBerryTagScreen(void);
static void HandleInitBackgrounds(void);
static void HandleInitWindows(void);
static void AddBerryTagTextToBg0(void);
static void PrintAllBerryData(void);
static void CreateBerrySprite(void);
static void CreateFlavorCircleSprites(void);
static void SetFlavorCirclesVisiblity(void);
static void PrintBerryNumberAndName(void);
static void PrintBerrySize(void);
static void PrintBerryFirmness(void);
static void PrintBerryDescription1(void);
static void PrintBerryDescription2(void);
static bool8 InitBerryTagScreen(void);
static bool8 LoadBerryTagGfx(void);
static void Task_HandleInput(u8 taskId);
static void Task_CloseBerryTagScreen(u8 taskId);
static void Task_DisplayAnotherBerry(u8 taskId);
static void TryChangeDisplayedBerry(u8 taskId, s8 toMove);

//MOD CONTEST All following commented voids are about flipping the tag
static void HandleBagCursorPositionChange(s8 toMove);
static void Task_DoTagFlipTask(u8); //
static bool8 Task_BeginTagFlip(struct Task *task);//
static bool8 Task_AnimateTagFlipDown(struct Task *task);//
static bool8 Task_AnimateTagFlipUp(struct Task *task);//
static bool8 Task_EndTagFlip(struct Task *task);//
static void UpdateTagFlipRegs(u16); //
static void PrintBerryFeel(void); //
static void PrintBerryGrowth(void); //
static void PrintBerryYield(void); //
static void Task_DoTagFlipTask(u8 taskId); //
static void FlipBerryTag(void); //
static void DrawTagFrontOrBack(u16 *); //
static void PrintAllBerryDataFlip(void); //
static void CreateSeasonsSprite(void); //
static void DestroySeasonsSprite(void); //
static void HandleSeasonSpriteSheets(void); //
static void DestroyBerrySprite(void);//
static void DestroyFlavorCircleSprites(void);//
static void ClearSideRelevantWindows(void); //
static void SetBerrySpriteVisibility(void); //
static void CreateFlavorGraficSprite(void); //
static void DestroyFlavorGraficSprite(void); //
static void DestroyFlavorXSprites(void); //
static void SetFlavorXVisiblity(void); //
static void CreateFlavorXSprites(void); //
static void CreateNewFlavorProfileSprite(void); //
static void DestroyFlavorProfileSprite(void); //
static bool8 Task_DrawFlippedTagSide(struct Task *task);//

static const u8 sText_SizeSlash[] = _("SIZE /");
static const u8 sText_FirmSlash[] = _("FIRM /");
static const u8 sText_Var1DotVar2[] = _("{STR_VAR_1}.{STR_VAR_2}”");
static const u8 sText_NumberVar1Var2[] = _("{NO}{STR_VAR_1} {STR_VAR_2}");
static const u8 sText_BerryTag[] = _("BERRY TAG");
static const u8 sText_ThreeMarks[] = _("???");

// code
void DoBerryTagScreen(void)
{
    sBerryTag = AllocZeroed(sizeof(*sBerryTag));
    sBerryTag->berryId = ItemIdToBerryType(gSpecialVar_ItemId);
    SetMainCallback2(CB2_InitBerryTagScreen);
}

static void CB2_BerryTagScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VblankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_InitBerryTagScreen(void)
{
    while (1)
    {
        if (MenuHelpers_ShouldWaitForLinkRecv() == TRUE)
            break;
        if (InitBerryTagScreen() == TRUE)
            break;
        if (MenuHelpers_IsLinkActive() == TRUE)
            break;
    }
}

#define tFlipState data[0] //MOD CONTEST Flip thing
#define tBgOp   data[1]
#define tTagTop   data[2] //MOD CONTEST Flip thing

static void FlipBerryTag(void) //This flips the berry tag
{
    u8 taskId = CreateTask(Task_DoTagFlipTask, 0);
    Task_DoTagFlipTask(taskId);
}

static bool8 (*const sBerryTagFlipTasks[])(struct Task *) = //MOD CONTEST TODO Berry tag flips with this in the function below
{
    Task_BeginTagFlip, 
    Task_AnimateTagFlipDown, 
    Task_DrawFlippedTagSide, 
    Task_AnimateTagFlipUp, 
    Task_EndTagFlip, 
};

static bool8 Task_DrawFlippedTagSide(struct Task *task)
{
    DrawTagFrontOrBack(sBerryTag->tilemapBuffers[4]); // This swaps the tilemap

    if(sBerryTag->onBack == TRUE)
        sBerryTag->onBack = FALSE;
    else
        sBerryTag->onBack = TRUE;

    task->tFlipState++; //this loads the next task
    return FALSE;
}


static void Task_DoTagFlipTask(u8 taskId) // MOD CONTEST this handles the flip tasks when called with the set taskID, the commented code should be copied to the end tag flip.
{
    while(sBerryTagFlipTasks[gTasks[taskId].tFlipState](&gTasks[taskId])) 
        ;
}

static void SetBerrySpriteVisibility(void){ //MOD CONTEST this one sets the visibility for the berry when flipping.
    if(!sBerryTag->flipping){
        SetFlavorXVisiblity();
        SetFlavorCirclesVisiblity();
        if(!sBerryTag->onBack)
        {
            gSprites[sBerryTag->berrySpriteId].invisible = FALSE;
            gSprites[sBerryTag->seasonSpriteId].invisible = FALSE; //Modify this to make it dependent on flags for each berry
            gSprites[sBerryTag->flavorGrafSpriteId].invisible = TRUE;
            gSprites[sBerryTag->flavorProfileSpriteIds[0]].invisible = TRUE;
            gSprites[sBerryTag->flavorProfileSpriteIds[1]].invisible = TRUE;
            gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = TRUE;
            gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = TRUE;
        }
        else
        {
            gSprites[sBerryTag->berrySpriteId].invisible = TRUE;
            gSprites[sBerryTag->seasonSpriteId].invisible = FALSE;
            gSprites[sBerryTag->flavorGrafSpriteId].invisible = FALSE;
            gSprites[sBerryTag->flavorProfileSpriteIds[0]].invisible = FALSE;
            gSprites[sBerryTag->flavorProfileSpriteIds[1]].invisible = FALSE;
            if (sBerryTag->berryId == (ItemIdToBerryType(ITEM_ENIGMA_BERRY))){
            gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = FALSE;
            gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = FALSE;
            }
            else{
            gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = TRUE;
            gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = TRUE;
            }
        }
    }
    else{
        SetFlavorXVisiblity();
        SetFlavorCirclesVisiblity();
        gSprites[sBerryTag->berrySpriteId].invisible = TRUE;
        gSprites[sBerryTag->seasonSpriteId].invisible = TRUE;
        gSprites[sBerryTag->flavorGrafSpriteId].invisible = TRUE;
        gSprites[sBerryTag->flavorProfileSpriteIds[0]].invisible = TRUE;
        gSprites[sBerryTag->flavorProfileSpriteIds[1]].invisible = TRUE;
        gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = TRUE;
        gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = TRUE;
    }
}

static bool8 InitBerryTagScreen(void) //MOD CONTEST the original one, I'm gonna make a flipped version to set everything first, then make them switch between them.
{   
    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ResetVramOamAndBgCntRegs();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        gPaletteFade.bufferTransferDisabled = 1;
        gMain.state++;
        break;
    case 3:
        ResetSpriteData();
        gMain.state++;
        break;
    case 4:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 5:
        if (!MenuHelpers_IsLinkActive())
            ResetTasks();
        gMain.state++;
        break;
    case 6: 
        HandleInitBackgrounds();
        sBerryTag->gfxState = 0;
        gMain.state++;
        break;
    case 7:
        if (LoadBerryTagGfx())
        {
            gMain.state++;
        }
        break;
    case 8:
        HandleInitWindows();
        gMain.state++;
        break;
    case 9:
        AddBerryTagTextToBg0();
        gMain.state++;
        break;
    case 10:
        if(!sBerryTag->onBack){
        PrintAllBerryData();
        }
        else{
        PrintAllBerryDataFlip(); //MOD CONTEST Flip version, just in case, so the flip side is loaded too.
        }   
        sBerryTag->flavorProfileSpriteIds[2] = CreateFlavorProfileSprite3(sBerryTag->berryId, 177, 66); //Loads the two extra enigma sprites so they can be used later.
        sBerryTag->flavorProfileSpriteIds[3] = CreateFlavorProfileSprite4(sBerryTag->berryId, 177, 66);
        SetBerrySpriteVisibility(); //MOD CONTEST this makes side relevant graphics hide when the tag is first loaded.
        gMain.state++;
        break;
    case 11:
        CreateBerrySprite();
        gMain.state++;
        break;
    case 12:
        CreateFlavorCircleSprites();
        SetFlavorCirclesVisiblity();
        CreateFlavorXSprites();
        SetFlavorXVisiblity();
        gMain.state++;
        break;
    case 13:
        CreateTask(Task_HandleInput, 0);
        gMain.state++;
        break;
    case 14:
        BlendPalettes(PALETTES_ALL, 0x10, RGB_BLACK);
        gMain.state++;
        break;
    case 15:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        gPaletteFade.bufferTransferDisabled = 0;
        gMain.state++;
        break;
    default: // done
        SetVBlankCallback(VblankCB);
        SetMainCallback2(CB2_BerryTagScreen);
        return TRUE;
    }

    return FALSE;
}


static void HandleInitBackgrounds(void)
{
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBackgroundTemplates, ARRAY_COUNT(sBackgroundTemplates)); //MOD CONTEST This makes the backgrounds and layers them!
    SetBgTilemapBuffer(2, sBerryTag->tilemapBuffers[0]);
    SetBgTilemapBuffer(3, sBerryTag->tilemapBuffers[1]);
    ResetAllBgsCoordinates();
    ScheduleBgCopyTilemapToVram(2);
    ScheduleBgCopyTilemapToVram(3);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ | WININ_WIN0_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ);
    SetGpuReg(REG_OFFSET_WIN0V, DISPLAY_HEIGHT);
    SetGpuReg(REG_OFFSET_WIN0H, DISPLAY_WIDTH);
    EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_HBLANK);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}

#define BG_TILE 0x42
static bool8 LoadBerryTagGfx(void)
{
    u16 i;

    switch (sBerryTag->gfxState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(2, gBerryCheck_Gfx, 0, 0, 0);
        sBerryTag->gfxState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(gBerryTag_Gfx, sBerryTag->tilemapBuffers[0]); //Original function, loads the berry tag graphics so it can work.
            DecompressDataWithHeaderWram(gBerryTagFlip_Gfx, sBerryTag->tilemapBuffers[4]); //loads the flip tilemap to a diferent part of memory for switching.
            sBerryTag->gfxState++;
        }
        break;
    case 2:
        DecompressDataWithHeaderWram(gBerryTag_Tilemap, sBerryTag->tilemapBuffers[2]);
        sBerryTag->gfxState++;
        break;
    case 3:
        // Palette of the bg tiles changes depending on the player's gender
        if (gSaveBlock2Ptr->playerGender == MALE)
        {
            for (i = 0; i < ARRAY_COUNT(sBerryTag->tilemapBuffers[1]); i++)
                sBerryTag->tilemapBuffers[1][i] = (4 << 12) | BG_TILE;
        }
        else
        {
            for (i = 0; i < ARRAY_COUNT(sBerryTag->tilemapBuffers[1]); i++)
                sBerryTag->tilemapBuffers[1][i] = (5 << 12) | BG_TILE;
        }
        sBerryTag->gfxState++;
        break;
    case 4:
        LoadPalette(gBerryCheck_Pal, BG_PLTT_ID(0), 6 * PLTT_SIZE_4BPP);
        sBerryTag->gfxState++;
        break;
    case 5:
        LoadCompressedSpriteSheet(&gBerryCheckCircleSpriteSheet);
        HandleSeasonSpriteSheets();
        CreateSeasonsSprite();
        CreateFlavorGraficSprite();
        CreateNewFlavorProfileSprite();
        sBerryTag->gfxState++;
        break;
    default:
        LoadSpritePalette(&gBerryCheckCirclePaletteTable);
        return TRUE; // done
    }
    return FALSE;
}


static void HandleSeasonSpriteSheets(void) //
{
    LoadCompressedSpriteSheet(&gBerrySeasonXSpriteSheet);
    LoadCompressedSpriteSheet(&gBerrySeasonsQuestionSpriteSheet);
    LoadCompressedSpriteSheet(&gBerryFlavorGrafSpriteSheet);
    LoadSpritePalette(&gBerrySeasonsPaletteTable);
    LoadSpritePalette(&gBerryFlavorGrafPaletteTable);
}

static void ClearSideRelevantWindows(void) //This deletes the windows needed to switch sides before loading new ones
{
        if(!sBerryTag->onBack){ 
            ClearWindowTilemap(4);
            PutWindowTilemap(1);
            PutWindowTilemap(2);
        }
        else{
            ClearWindowTilemap(1);
            ClearWindowTilemap(2);
            PutWindowTilemap(4); 
        }

    ScheduleBgCopyTilemapToVram(1);
}


static void HandleInitWindows(void) //MOD CONTEST FIXINGIT
{
    InitWindows(sWindowTemplates);
    LoadPalette(sFontPalette, BG_PLTT_ID(15), sizeof(sFontPalette));
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
    PutWindowTilemap(3);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
}

static void PrintTextInBerryTagScreen(u8 windowId, const u8 *text, u8 x, u8 y, s32 speed, u8 colorStructId)
{
    AddTextPrinterParameterized4(windowId, FONT_NORMAL, x, y, 0, 0, sTextColors[colorStructId], speed, text);
}

static void AddBerryTagTextToBg0(void)
{
    memcpy(GetBgTilemapBuffer(0), sBerryTag->tilemapBuffers[2], sizeof(sBerryTag->tilemapBuffers[2]));
    FillWindowPixelBuffer(WIN_BERRY_TAG, PIXEL_FILL(15));
    PrintTextInBerryTagScreen(WIN_BERRY_TAG, sText_BerryTag, GetStringCenterAlignXOffset(FONT_NORMAL, sText_BerryTag, 0x40), 1, 0, 1);
    PutWindowTilemap(WIN_BERRY_TAG);
    ScheduleBgCopyTilemapToVram(0);
}

static void PrintAllBerryData(void)
{
    PrintBerryNumberAndName();
    PrintBerrySize();
    PrintBerryFirmness();
    PrintBerryDescription1();
    PrintBerryDescription2();
    LoadCompressedSpriteSheet(&gBerryCheckCircleSpriteSheet);//
    DestroySeasonsSprite();//these two last ones switch between the berry flower graphic and the berry season grid
    CreateSeasonsSprite();//
}

static void PrintAllBerryDataFlip(void) //Same function as above, but for flipped berry tag
{
    PrintBerryFeel(); 
    PrintBerryGrowth(); 
    PrintBerryYield(); 
    DestroySeasonsSprite();//
    CreateSeasonsSprite();//
}


static bool8 Task_BeginTagFlip(struct Task *task) //MOD CONTEST TODO These following functions handled the ID flip, gotta change it for the berry tag
{
    u32 i;

    DestroyTask(FindTaskIdByFunc(Task_HandleInput));
    HideBg(1); 
    ScanlineEffect_Stop(); //
    ScanlineEffect_Clear();
    for (i = 0; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[1][i] = 0; //Gotta see what this actually does

    sBerryTag->flipping = TRUE;
    SetBerrySpriteVisibility();
    PlaySE(SE_RG_CARD_FLIP);

    task->tFlipState++; //this loads the next task
    return FALSE;
}

#define TAG_FLIP_Y ((DISPLAY_HEIGHT / 2) - 6)

static bool8 Task_AnimateTagFlipDown(struct Task *task) //
{
    
    u32 tagHeight, r5, r10, tagTop, r6, var_24, tagBottom, var;
    s16 i;

    if (task->tTagTop >= TAG_FLIP_Y) 
        task->tTagTop = TAG_FLIP_Y;
    else
        task->tTagTop += 7;

    sBerryTag->tagTop = task->tTagTop;
    UpdateTagFlipRegs(task->tTagTop);

    tagTop = task->tTagTop; //all these finish registering the sprite's height, guessing here.
    tagBottom = DISPLAY_HEIGHT - tagTop;
    tagHeight = tagBottom - tagTop;
    r6 = -tagTop << 16;
    r5 = (DISPLAY_HEIGHT << 16) / tagHeight;
    r5 -= 1 << 16;
    var_24 = r6;
    var_24 += r5 * tagHeight;
    r10 = r5 / tagHeight;
    r5 *= 2;

    for (i = 0; i < tagTop; i++) //these animate the part where the tag collapses by increasing the low border and decreasing the high one.
        gScanlineEffectRegBuffers[0][i] = -i;
    for (; i < (s16)tagBottom; i++)
    {
        var = r6 >> 16;
        r6 += r5;
        r5 -= r10;
        gScanlineEffectRegBuffers[0][i] = var;//
    }
    var = var_24 >> 16;
    for (; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[0][i] = var;//

    if (task->tTagTop >= TAG_FLIP_Y)
        task->tFlipState++;
    return FALSE;
}


static void DrawTagFrontOrBack(u16 *ptr) //MOD CONTEST this one prints the front or back tilemaps when the tilemapBuffers[4/5] is set on the ptr Var.
{
    s16 i, j;
    u16 *dst = sBerryTag->tilemapBuffers[0];
    u16 Swap; //this VAR wil store exactly one tile value to be saved, so both tilemaps can swap tiles at the same time, saving 1 tilemapBuffer and NOT crashing the game :).
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 32; j++)
        {
            Swap = dst[32 * i + j];
            dst[32 * i + j] = ptr[32 * i + j];
            ptr[32 * i + j] = Swap;
        }
    }
    CopyBgTilemapBufferToVram(2);
}

static void UpdateTagFlipRegs(u16 tagTop) // Oh, wait, this creates a window that simulates the background flipping? Whaaaaaat?
{
    s8 blendY = (tagTop + 40) / 10;

    if (blendY <= 4)
        blendY = 0;
    sBerryTag->flipBlendY = blendY;
    SetGpuReg(REG_OFFSET_BLDY, sBerryTag->flipBlendY);
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(sBerryTag->tagTop, DISPLAY_HEIGHT - sBerryTag->tagTop));
}

static bool8 Task_AnimateTagFlipUp(struct Task *task) //
{
    
    u32 tagHeight, r5, r10, tagTop, r6, var_24, tagBottom, var;
    s16 i;

    if (task->tTagTop <= 5)
        task->tTagTop = 0;
    else
        task->tTagTop -= 5;

    sBerryTag->tagTop = task->tTagTop;
    UpdateTagFlipRegs(task->tTagTop); //This calls for the function rignt above this one

    tagTop = task->tTagTop;
    tagBottom = DISPLAY_HEIGHT - tagTop;
    tagHeight = tagBottom - tagTop;
    r6 = -tagTop << 16;
    r5 = (DISPLAY_HEIGHT << 16) / tagHeight;
    r5 -= 1 << 16;
    var_24 = r6;
    var_24 += r5 * tagHeight;
    r10 = r5 / tagHeight;
    r5 /= 2;

    for (i = 0; i < tagTop; i++) //All the same as flip down but inverted, this one finishes the flip.
        gScanlineEffectRegBuffers[0][i] = -i;
    for (; i < (s16)tagBottom; i++)
    {
        var = r6 >> 16;
        r6 += r5;
        r5 += r10;
        gScanlineEffectRegBuffers[0][i] = var;
    }
    var = var_24 >> 16;
    for (; i < DISPLAY_HEIGHT; i++)
        gScanlineEffectRegBuffers[0][i] = var;

    if (task->tTagTop <= 0)
        task->tFlipState++;
    return FALSE;
}

static bool8 Task_EndTagFlip(struct Task *task) //
{

    sBerryTag->flipping = FALSE;
    if(sBerryTag->onBack)
    {
        PrintAllBerryDataFlip();
    }
    else{
        PrintAllBerryData();
    }

    ClearSideRelevantWindows();
    SetBerrySpriteVisibility();
    ShowBg(1);
    
    DestroyTask(FindTaskIdByFunc(Task_DoTagFlipTask));
    CreateTask(Task_HandleInput, 0);
    return FALSE;
}

static void PrintBerryNumberAndName(void)
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);
    ConvertIntToDecimalStringN(gStringVar1, sBerryTag->berryId, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopy(gStringVar2, berry->name);
    StringExpandPlaceholders(gStringVar4, sText_NumberVar1Var2);
    PrintTextInBerryTagScreen(WIN_BERRY_NAME, gStringVar4, 0, 1, 0, 0);
}

static void PrintBerrySize(void)
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);
    AddTextPrinterParameterized(WIN_SIZE_FIRM, FONT_NORMAL, sText_SizeSlash, 0, 1, TEXT_SKIP_DRAW, NULL);
    if (berry->size != 0)
    {
        u32 inches, fraction; //MOD CONTEST TODO turn this to CM for spanish (ez)

        inches = 1000 * berry->size / 254;
        if (inches % 10 > 4)
            inches += 10;
        fraction = (inches % 100) / 10;
        inches /= 100;

        ConvertIntToDecimalStringN(gStringVar1, inches, STR_CONV_MODE_LEFT_ALIGN, 2);
        ConvertIntToDecimalStringN(gStringVar2, fraction, STR_CONV_MODE_LEFT_ALIGN, 2);
        StringExpandPlaceholders(gStringVar4, sText_Var1DotVar2);
        AddTextPrinterParameterized(WIN_SIZE_FIRM, FONT_NORMAL, gStringVar4, 0x28, 1, 0, NULL);
    }
    else
    {
        AddTextPrinterParameterized(WIN_SIZE_FIRM, FONT_NORMAL, sText_ThreeMarks, 0x28, 1, 0, NULL);
    }
}

static void PrintBerryFirmness(void)
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);
    AddTextPrinterParameterized(WIN_SIZE_FIRM, FONT_NORMAL, sText_FirmSlash, 0, 0x11, TEXT_SKIP_DRAW, NULL);
    if (berry->firmness != BERRY_FIRMNESS_UNKNOWN)
        AddTextPrinterParameterized(WIN_SIZE_FIRM, FONT_NORMAL, sBerryFirmnessStrings[berry->firmness], 0x28, 0x11, 0, NULL);
    else
        AddTextPrinterParameterized(WIN_SIZE_FIRM, FONT_NORMAL, sText_ThreeMarks, 0x28, 0x11, 0, NULL);
}

static void PrintBerryDescription1(void)
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);
    AddTextPrinterParameterized(WIN_DESC, FONT_NORMAL, berry->description1, 0, 1, 0, NULL);
}

static void PrintBerryDescription2(void)
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);
    AddTextPrinterParameterized(WIN_DESC, FONT_NORMAL, berry->description2, 0, 0x11, 0, NULL);
}

static void PrintBerryFeel(void) //
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);
    u8 HandleString;
    if(berry->smoothness < 20){ // E-Reader Berries with 5 feel
        HandleString = BERRY_FEEL_SILKY;
    }
    else if(berry->smoothness < 25){ // Berries with 20 feel
        HandleString = BERRY_FEEL_SMOOTH;
    }
    else if(berry->smoothness < 30){ // Berries with 25 feel
        HandleString = BERRY_FEEL_TENDER;
    }
    else if(berry->smoothness < 65){ // Berries with 30 feel
        HandleString = BERRY_FEEL_CHEWY;
    }
    else if(berry->smoothness < 70){ // E-Reader Berries with 65 feel
        HandleString = BERRY_FEEL_GRITTY;
    }
    else if(berry->smoothness < 80){ // Berries with 70 feel
        HandleString = BERRY_FEEL_CRUMBLY;
    }
    else if(berry->smoothness < 85){ // Berries with 80 feel
        HandleString = BERRY_FEEL_DENSE;
    }
    else{ // Out of bounds, special Cases & E-Reader Berries with 85 feel
        HandleString = BERRY_FEEL_UNKNOWN;
    }
    AddTextPrinterParameterized(WIN_BERRY_DATA, FONT_NORMAL, sBerryFeelStrings[HandleString], 5, 10, 0, NULL);
}

static void PrintBerryGrowth(void) //
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);

    u8 HandleString;
    if(berry->growthDuration < 12){ // Berries that grow in 4-8 hours
        HandleString = BERRY_GROWTH_FAST;
    }
    else if(berry->growthDuration < 20){ // Berries that grow in 4-12-16 hours
        HandleString = BERRY_GROWTH_QUICK;
    }
    else if(berry->growthDuration < 32){ // Berries that grow in 20-24 hours
        HandleString = BERRY_GROWTH_EVEN;
    }
    else if(berry->growthDuration < 60){ // Berries that grow in 32-48 hours
        HandleString = BERRY_GROWTH_SLOW;
    }
    else if(berry->growthDuration < 96){ // Berries that grow in 60-72 hours
        HandleString = BERRY_GROWTH_LATE;
    }
    else{ // Out of bounds, special Cases & Berries that grow in 96+ hours
        HandleString = BERRY_GROWTH_UNKNOWN;
    }
    AddTextPrinterParameterized(WIN_BERRY_DATA, FONT_NORMAL, sBerryGrowthStrings[HandleString], 5, 25, 0, NULL);
}

static void PrintBerryYield(void) //
{   
    //Some of these are never seen unless "Galactic Berries" is obtained and enabled (Gen 4 Style berries)

    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);

    u8 HandleString;
    if(berry->maxYield < 3){ // Yields of 2-1 Berries
        HandleString = BERRY_YIELD_A_PAIR;
    }
    else if(berry->maxYield < 6){ // Yields of 3-5 Berries
        HandleString = BERRY_YIELD_A_FEW;
    }
    else if(berry->maxYield < 12){ // Yields of 6-11 Berries
        HandleString = BERRY_YIELD_SOME;
    }
    else if(berry->maxYield < 24){ // Yields of 12-23 Berries
        HandleString = BERRY_YIELD_LOTS;
    }
    else if(berry->maxYield <= 24){ // Yields of 24+ Berries
        HandleString = BERRY_YIELD_PLENTY;
    }
    else{ // Out of bounds & special Cases
        HandleString = BERRY_YIELD_UNKNOWN;
    }
    AddTextPrinterParameterized(WIN_BERRY_DATA, FONT_NORMAL, sBerryYieldStrings[HandleString], 5, 40, 0, NULL);
}

static void CreateBerrySprite(void)
{
    sBerryTag->currentSpriteBerryId = sBerryTag->berryId - 1;
    sBerryTag->berrySpriteId = CreateBerryTagSprite(sBerryTag->currentSpriteBerryId, 56, 64);
}

static void DestroyBerrySprite(void)
{
    DestroyBerryIconSprite(sBerryTag->berrySpriteId, sBerryTag->currentSpriteBerryId, TRUE);
}


static void CreateSeasonsSprite(void) // if the berry tag is on the back, it loads the season grid, if it's on the front, it loads the berry flower sprite. It was the only way that didn't implode the GBA
{
    if(sBerryTag->onBack){
        sBerryTag->seasonSpriteId = CreateBerrySeasonsSprite();
    }
    else{
        sBerryTag->seasonSpriteId = CreateBerryFlowerSprite(sBerryTag->berryId);//
    }
}

static void CreateFlavorGraficSprite(void) //
{
    sBerryTag->flavorGrafSpriteId = CreateFlavorGrafSprite(177, 66);
    gSprites[sBerryTag->flavorGrafSpriteId].invisible = TRUE;
}

static void CreateNewFlavorProfileSprite(void) //
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);

    CalculateAndloadProfilePalette(sBerryTag->berryId);
    LoadFlavorProfileSpriteData(sBerryTag->berryId);

    if(sBerryTag->berryId+1 == ItemIdToBerryType(ITEM_ENIGMA_BERRY))
    {
        sBerryTag->flavorProfileSpriteIds[0] = CreateFlavorProfileSprite1(sBerryTag->berryId, 177, 66);
        sBerryTag->flavorProfileSpriteIds[1] = CreateFlavorProfileSprite2(sBerryTag->berryId, 177, 66);
        sBerryTag->flavorProfileSpriteIds[2] = CreateFlavorProfileSprite3(sBerryTag->berryId, 177, 66);
        sBerryTag->flavorProfileSpriteIds[3] = CreateFlavorProfileSprite4(sBerryTag->berryId, 177, 66);
        gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = FALSE;
        gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = FALSE;
    }
    else if(sBerryTag->berryId+1 >= ItemIdToBerryType(FIRST_BERRY_INDEX) && sBerryTag->berryId < ItemIdToBerryType(ITEM_ENIGMA_BERRY))
    {
        u8 x1 = 176;
        u8 x2 = 176;
        u8 y1 = 63;
        u8 y2 = 63;

        if(berry->tileNumber1 > 4 || (berry->sour > 30 && berry->sweet == 0 && berry->dry == 0)){ // Flip fix possition sprite 1 && sour?
            x1--;
        }
        if(berry->tileNumber2 > 4){ // Flip fix possition sprite 2
            x2--;
        }
        if(berry->spicy > 30){ // Spicy with 40 value fix position
            y1 = y1 - 3;
        }
        if(berry->dry > 30 && berry->spicy == 0){ // Dry with 40 value fix positions
            x1 = x1 + 1;
        }
        else if(berry->dry > 30){
            x2 = x2 + 1;
        }
        if(berry->sour > 30 && berry->spicy < 40 && berry->dry == 0 && berry->sweet == 0 && berry->bitter < 40 ){ // Sour with 40 value fix positions
            x1--;
        }
        else if(berry->sour > 30){
            x2--;
        }

        sBerryTag->flavorProfileSpriteIds[0] = CreateFlavorProfileSprite1(sBerryTag->berryId+1, x1, y1);
        sBerryTag->flavorProfileSpriteIds[1] = CreateFlavorProfileSprite2(sBerryTag->berryId+1, x2, y2);
        sBerryTag->flavorProfileSpriteIds[2] = CreateFlavorProfileSprite3(sBerryTag->berryId+1, 0, 0);
        sBerryTag->flavorProfileSpriteIds[3] = CreateFlavorProfileSprite4(sBerryTag->berryId+1, 0, 66);
        gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = TRUE;
        gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = TRUE;
    }
    else{
        sBerryTag->flavorProfileSpriteIds[0] = CreateFlavorProfileSprite1(0, 177, 66); // Placeholder
        gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = TRUE;
        gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = TRUE;
    }

    gSprites[sBerryTag->flavorProfileSpriteIds[0]].invisible = TRUE;

    if(sBerryTag->flavorProfileSpriteIds[1])
        gSprites[sBerryTag->flavorProfileSpriteIds[1]].invisible = TRUE;
    if(sBerryTag->flavorProfileSpriteIds[2])
        gSprites[sBerryTag->flavorProfileSpriteIds[2]].invisible = TRUE;
    if(sBerryTag->flavorProfileSpriteIds[3])
        gSprites[sBerryTag->flavorProfileSpriteIds[3]].invisible = TRUE;
}

static void DestroyFlavorProfileSprite(void) //
{
    if(sBerryTag->flavorProfileSpriteIds[1])
        DestroySpriteAndFreeResources(&gSprites[sBerryTag->flavorProfileSpriteIds[1]]);
    if(sBerryTag->flavorProfileSpriteIds[2])
        DestroySpriteAndFreeResources(&gSprites[sBerryTag->flavorProfileSpriteIds[2]]);
    if(sBerryTag->flavorProfileSpriteIds[3])
        DestroySpriteAndFreeResources(&gSprites[sBerryTag->flavorProfileSpriteIds[3]]);

    DestroySpriteAndFreeResources(&gSprites[sBerryTag->flavorProfileSpriteIds[0]]);
}

static void DestroyFlavorGraficSprite(void) //
{
    DestroySpriteAndFreeResources(&gSprites[sBerryTag->flavorGrafSpriteId]);
}

static void DestroySeasonsSprite(void)
{ //
        DestroySpriteAndFreeResources(&gSprites[sBerryTag->seasonSpriteId]);
}

static void CreateFlavorCircleSprites(void)
{
    sBerryTag->flavorCircleIds[FLAVOR_SPICY] = CreateBerryFlavorCircleSprite(64);
    sBerryTag->flavorCircleIds[FLAVOR_DRY] = CreateBerryFlavorCircleSprite(104);
    sBerryTag->flavorCircleIds[FLAVOR_SWEET] = CreateBerryFlavorCircleSprite(144);
    sBerryTag->flavorCircleIds[FLAVOR_BITTER] = CreateBerryFlavorCircleSprite(184);
    sBerryTag->flavorCircleIds[FLAVOR_SOUR] = CreateBerryFlavorCircleSprite(224);
}

static void CreateFlavorXSprites(void) //MOD CONTEST this works exactly like the flavor circles, only that they get replaced by the ???? grafic if no season is present.
{
        sBerryTag->flavorXIds[0] = CreateSeasonXSprite(0, 0);
        sBerryTag->flavorXIds[1] = CreateSeasonXSprite(13, 0);
        sBerryTag->flavorXIds[2] = CreateSeasonXSprite(26, 0);
        sBerryTag->flavorXIds[3] = CreateSeasonXSprite(39, 0);
        sBerryTag->flavorXIds[4] = CreateSeasonXSprite(0, 10);
        sBerryTag->flavorXIds[5] = CreateSeasonXSprite(13, 10);
        sBerryTag->flavorXIds[6] = CreateSeasonXSprite(26, 10);
        sBerryTag->flavorXIds[7] = CreateSeasonXSprite(39, 10);
        sBerryTag->seasonQuestionSpriteId = CreateSeasonQuestionMSprite();
}

static void SetFlavorCirclesVisiblity(void)
{
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);

    if (berry->spicy && !sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_SPICY]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_SPICY]].invisible = TRUE;

    if (berry->dry && !sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_DRY]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_DRY]].invisible = TRUE;

    if (berry->sweet && !sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_SWEET]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_SWEET]].invisible = TRUE;

    if (berry->bitter && !sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_BITTER]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_BITTER]].invisible = TRUE;

    if (berry->sour && !sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_SOUR]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorCircleIds[FLAVOR_SOUR]].invisible = TRUE;
}

static void SetFlavorXVisiblity(void) //MOD CONTEST this one sets the X sprites visibility based off of each berry season type.
{
    /* This is how each season is displayed on the table:
        (Each cell is numbered from the left to the right, in row order)

            |Spring|Summer| Fall |Winter|
        ----+------+------+------+------+
   Planting | ABEI | BCFI | CDGI | ADHI |
        ----+------+------+------+------+
 Harvesting |ADGHI |ABEHI |BCEFI |CDFGI |
        ----+------+------+------+------+
    
        This is all flavor text BTW lol.
    */
    const struct Berry *berry = GetBerryInfo(sBerryTag->berryId);

    if ((berry->growthSeasons == SEASON_A || berry->growthSeasons == SEASON_B || 
          berry->growthSeasons == SEASON_E || berry->growthSeasons == SEASON_I) 
          && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[0]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[0]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_B || berry->growthSeasons == SEASON_C ||
          berry->growthSeasons == SEASON_F || berry->growthSeasons == SEASON_I ) 
          && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[1]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[1]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_C || berry->growthSeasons == SEASON_D ||
          berry->growthSeasons == SEASON_G || berry->growthSeasons == SEASON_I ) 
          && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[2]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[2]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_A || berry->growthSeasons == SEASON_D ||
          berry->growthSeasons == SEASON_H || berry->growthSeasons == SEASON_I ) 
          && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[3]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[3]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_A || berry->growthSeasons == SEASON_D ||
          berry->growthSeasons == SEASON_G || berry->growthSeasons == SEASON_H ||
            berry->growthSeasons == SEASON_I ) && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[4]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[4]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_A || berry->growthSeasons == SEASON_B ||
          berry->growthSeasons == SEASON_E || berry->growthSeasons == SEASON_H ||
            berry->growthSeasons == SEASON_I ) && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[5]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[5]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_B || berry->growthSeasons == SEASON_C ||
          berry->growthSeasons == SEASON_E || berry->growthSeasons == SEASON_F ||
            berry->growthSeasons == SEASON_I ) && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[6]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[6]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_C || berry->growthSeasons == SEASON_D ||
          berry->growthSeasons == SEASON_F || berry->growthSeasons == SEASON_G ||
            berry->growthSeasons == SEASON_I ) && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->flavorXIds[7]].invisible = FALSE;
    else
        gSprites[sBerryTag->flavorXIds[7]].invisible = TRUE;

    if ((berry->growthSeasons == SEASON_J) && sBerryTag->onBack && !sBerryTag->flipping)
        gSprites[sBerryTag->seasonQuestionSpriteId].invisible = FALSE;
    else
        gSprites[sBerryTag->seasonQuestionSpriteId].invisible = TRUE;
}

static void DestroyFlavorCircleSprites(void)
{
    u16 i;

    for (i = 0; i < FLAVOR_COUNT; i++)
        DestroySpriteAndFreeResources(&gSprites[sBerryTag->flavorCircleIds[i]]);
}

static void DestroyFlavorXSprites(void)
{
    u16 i;

    for (i = 0; i < 8; i++)
        DestroySpriteAndFreeResources(&gSprites[sBerryTag->flavorXIds[i]]);
    
    DestroySpriteAndFreeResources(&gSprites[sBerryTag->seasonQuestionSpriteId]);
}

static void PrepareToCloseBerryTagScreen(u8 taskId)
{
    PlaySE(SE_SELECT);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_CloseBerryTagScreen;
}

static void Task_CloseBerryTagScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyBerrySprite();
        DestroyFlavorXSprites();
        DestroyFlavorCircleSprites();
        DestroyFlavorGraficSprite();
        DestroyFlavorProfileSprite();
        Free(sBerryTag);
        FreeAllWindowBuffers();
        SetMainCallback2(CB2_ReturnToBagMenuPocket);
        DestroyTask(taskId);
    }
}

static void Task_HandleInput(u8 taskId) //MOD CONTEST Done, IT works
{
    if (!gPaletteFade.active && !sBerryTag->flipping)
    {
        u16 arrowKeys = JOY_REPEAT(DPAD_ANY);
        if (arrowKeys == DPAD_UP)
                TryChangeDisplayedBerry(taskId, -1); //MOD CONTEST this one has to be changed to work with the berry season table like below, an If statement will suffice.
        else if (arrowKeys == DPAD_DOWN)
                TryChangeDisplayedBerry(taskId, 1);//This one too.
        else if (JOY_NEW(A_BUTTON)){
            if (sBerryTag->onBack)
                PrepareToCloseBerryTagScreen(taskId);
            else{ 
            sBerryTag->flipping = TRUE;
            FlipBerryTag();
            }
        }
        else if (JOY_NEW(B_BUTTON)){
            if (sBerryTag->onBack){
            sBerryTag->flipping = TRUE;
            FlipBerryTag();
            }
            else{ 
                PrepareToCloseBerryTagScreen(taskId);
            }
        }
    }
}


#define tBerryY data[0]
#define tBgOp   data[1]

static void TryChangeDisplayedBerry(u8 taskId, s8 toMove) //MOD CONTEST original change of berry, copied it for season tables.
{
    s16 *data = gTasks[taskId].data;
    s16 currPocketPosition = gBagPosition.scrollPosition[POCKET_BERRIES] + gBagPosition.cursorPosition[POCKET_BERRIES];
    u32 newPocketPosition = currPocketPosition + toMove;
    if (newPocketPosition < ITEM_TO_BERRY(LAST_BERRY_INDEX) && GetBagItemId(POCKET_BERRIES, newPocketPosition) != ITEM_NONE)
    {
        if (toMove < 0)
            tBgOp = BG_COORD_SUB;
        else
            tBgOp = BG_COORD_ADD;

        tBerryY = 0;
        PlaySE(SE_SELECT);
        HandleBagCursorPositionChange(toMove);
        gTasks[taskId].func = Task_DisplayAnotherBerry;
    }
}

static void HandleBagCursorPositionChange(s8 toMove)
{
    u16 *scrollPos = &gBagPosition.scrollPosition[POCKET_BERRIES];
    u16 *cursorPos = &gBagPosition.cursorPosition[POCKET_BERRIES];
    if (toMove > 0)
    {
        if (*cursorPos < 4 || GetBagItemId(POCKET_BERRIES, *scrollPos + 8) == 0)
            *cursorPos += toMove;
        else
            *scrollPos += toMove;
    }
    else
    {
        if (*cursorPos > 3 || *scrollPos == 0)
            *cursorPos += toMove;
        else
            *scrollPos += toMove;
    }

    sBerryTag->berryId = ItemIdToBerryType(GetBagItemId(POCKET_BERRIES, *scrollPos + *cursorPos));
}

#define DISPLAY_SPEED 16

static void Task_DisplayAnotherBerry(u8 taskId) //MOD CONTEST had to dupe this on for season sprites, testing it.
{
    u16 i;
    s16 y;
    s16 *data = gTasks[taskId].data;
    tBerryY += DISPLAY_SPEED;
    tBerryY &= 0xFF;

    if (tBgOp == BG_COORD_ADD)
    {
        switch (tBerryY)
        {
        case 3 * DISPLAY_SPEED:
            FillWindowPixelBuffer(WIN_BERRY_NAME, PIXEL_FILL(0));
            break;
        case 4 * DISPLAY_SPEED:
            PrintBerryNumberAndName();
            break;
        case 5 * DISPLAY_SPEED:
            DestroySeasonsSprite();
            CreateSeasonsSprite();
            DestroyBerrySprite();
            CreateBerrySprite();
            DestroyFlavorGraficSprite();
            CreateFlavorGraficSprite();
            DestroyFlavorProfileSprite();
            CreateNewFlavorProfileSprite();
            SetBerrySpriteVisibility();
            break;
        case 6 * DISPLAY_SPEED:
            FillWindowPixelBuffer(WIN_SIZE_FIRM, PIXEL_FILL(0));
            break;
        case 7 * DISPLAY_SPEED:
            PrintBerrySize();
            PrintBerryFeel();
            break;
        case 8 * DISPLAY_SPEED:
            PrintBerryFirmness();
            PrintBerryGrowth();
            break;
        case 9 * DISPLAY_SPEED:
            SetFlavorCirclesVisiblity();
            DestroyFlavorXSprites();
            CreateFlavorXSprites();
            SetFlavorXVisiblity();
            PrintBerryYield();
            break;
        case 10 * DISPLAY_SPEED:
            FillWindowPixelBuffer(WIN_DESC, PIXEL_FILL(0));
            FillWindowPixelBuffer(WIN_BERRY_DATA, PIXEL_FILL(0));
            break;
        case 11 * DISPLAY_SPEED:
            PrintBerryDescription1();
            break;
        case 12 * DISPLAY_SPEED:
            PrintBerryDescription2();
            break;
        }
    }
    else // BG_COORD_SUB
    {
        switch (tBerryY)
        {
        case 3 * DISPLAY_SPEED:
            FillWindowPixelBuffer(WIN_DESC, PIXEL_FILL(0));
            FillWindowPixelBuffer(WIN_BERRY_DATA, PIXEL_FILL(0));
            break;
        case 4 * DISPLAY_SPEED:
            PrintBerryDescription2();
            break;
        case 5 * DISPLAY_SPEED:
            PrintBerryDescription1();
            break;
        case 6 * DISPLAY_SPEED:
            SetFlavorCirclesVisiblity();
            DestroyFlavorXSprites();
            CreateFlavorXSprites();
            SetFlavorXVisiblity();
            break;
        case 7 * DISPLAY_SPEED:
            PrintBerryYield();
            FillWindowPixelBuffer(WIN_SIZE_FIRM, PIXEL_FILL(0));
            break;
        case 8 * DISPLAY_SPEED:
            PrintBerryFirmness();
            PrintBerryGrowth();
            break;
        case 9 * DISPLAY_SPEED:
            PrintBerrySize();
            PrintBerryFeel();
            break;
        case 10 * DISPLAY_SPEED:
            DestroySeasonsSprite();
            CreateSeasonsSprite();
            DestroyBerrySprite();
            CreateBerrySprite();
            DestroyFlavorGraficSprite();
            CreateFlavorGraficSprite();
            DestroyFlavorProfileSprite();
            CreateNewFlavorProfileSprite();
            SetBerrySpriteVisibility();
            break;
        case 11 * DISPLAY_SPEED:
            FillWindowPixelBuffer(WIN_BERRY_NAME, PIXEL_FILL(0));
            break;
        case 12 * DISPLAY_SPEED:
            PrintBerryNumberAndName();
            break;
        }
    }

    if (tBgOp == BG_COORD_ADD)
        y = -tBerryY;
    else
        y = tBerryY;

    gSprites[sBerryTag->berrySpriteId].y2 = y;
    gSprites[sBerryTag->flavorGrafSpriteId].y2 = y; //MOD CONTEST these move the sprites along with the Berry Tag.
    gSprites[sBerryTag->seasonSpriteId].y2 = y; //
    gSprites[sBerryTag->seasonQuestionSpriteId].y2 = y;//

    for (i = 0; i < FLAVOR_COUNT; i++)//
        gSprites[sBerryTag->flavorCircleIds[i]].y2 = y;
    
    for (i = 0; i < 8; i++)//
        gSprites[sBerryTag->flavorXIds[i]].y2 = y;

    for (i = 0; i < 4; i++)//
        gSprites[sBerryTag->flavorProfileSpriteIds[i]].y2 = y;

    ChangeBgY(1, 0x1000, tBgOp);
    ChangeBgY(2, 0x1000, tBgOp);

    if (tBerryY == 0)
        gTasks[taskId].func = Task_HandleInput;
}