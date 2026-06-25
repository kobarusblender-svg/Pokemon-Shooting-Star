#ifndef GUARD_ITEM_MENU_ICONS_H
#define GUARD_ITEM_MENU_ICONS_H

extern const struct CompressedSpriteSheet gBagMaleSpriteSheet;
extern const struct CompressedSpriteSheet gBagFemaleSpriteSheet;
extern const struct SpritePalette gBagPaletteTable;
extern const struct CompressedSpriteSheet gBerryCheckCircleSpriteSheet;
extern const struct SpritePalette gBerryCheckCirclePaletteTable;
extern const struct CompressedSpriteSheet gBerrySeasonSpriteSheet; //MOD CONTEST the following graphics are for the back side of the tag.
extern const struct CompressedSpriteSheet gBerrySeasonXSpriteSheet; //
extern const struct CompressedSpriteSheet gBerrySeasonsQuestionSpriteSheet; //
extern const struct CompressedSpriteSheet gBerryFlavorGrafSpriteSheet; //
extern const struct SpritePalette gBerrySeasonsPaletteTable; //
extern const struct SpritePalette gBerryFlavorGrafPaletteTable; //
/*
static const union AffineAnimCmd sAffineAnims_SqueezeBerryTagDown[3]; //
static const union AffineAnimCmd sAffineAnims_StretchBerryTagUp[3]; //*/

void RemoveBagSprite(u8 id);
void AddBagVisualSprite(u8 bagPocketId);
void SetBagVisualPocketId(u8 bagPocketId, bool8 isSwitchingPockets);
void ShakeBagSprite(void);
void AddSwitchPocketRotatingBallSprite(s16 rotationDirection);
void AddBagItemIconSprite(enum Item itemId, u8 id);
void RemoveBagItemIconSprite(u8 id);
void CreateItemMenuSwapLine(void);
void SetItemMenuSwapLineInvisibility(bool8 invisible);
void UpdateItemMenuSwapLinePos(u8 y);

// Note: Because of removing gDecompressionBuffer and allowing to create more than one berry sprite at a time, the memory for gfx is allocated dynamically.
// For CreateBerryTagSprite and CreateSpinningBerrySprite, the allocated ptr is stored in two last data fields(data[6], data[7]), so make sure to NOT put anything in there!
// The corresponding code has already been edited in berry_tag_screen.c and berry_blender.c
#define BERRY_ICON_GFX_PTR_DATA_ID 6
u32 CreateBerryTagSprite(u32 id, s32 x, s32 y);
u32 CreateSpinningBerrySprite(u32 berryId, s32 x, s32 y, bool32 startAffine);
// Needs to always call either of these after creating a Berry Icon sprite, because it frees allocated memory!
void DestroyBerryIconSprite(u32 spriteId, u32 berryId, bool32 freePal);
void DestroyBerryIconSpritePtr(struct Sprite *sprite, u32 berryId, bool32 freePal);

void FreeBerryIconSpritePalette(u32 berryId); // Unused atm, because it's also handled by DestroyBerryIconSprite. Leaving it as it is, because it may still be useful in some custom cases.

u8 CreateBerryFlavorCircleSprite(s16 x);
u8 CreateBerrySeasonsSprite(void);// MOD CONTEST
u8 CreateSeasonXSprite(s16 x, s16 y); //
u8 CreateSeasonQuestionMSprite(); //
u8 CreateFlavorGrafSprite(s16 x, s16 y); //
void LoadFlavorProfileSpriteData(s16 BerryNumber); //
u8 CreateFlavorProfileSprite1(s16 BerryNumber, s16 x, s16 y); //
u8 CreateFlavorProfileSprite2(s16 BerryNumber, s16 x, s16 y); //
u8 CreateFlavorProfileSprite3(s16 BerryNumber, s16 x, s16 y); //
u8 CreateFlavorProfileSprite4(s16 BerryNumber, s16 x, s16 y); //
u8 CreateBerryFlowerSprite(u32 BerryNumber);//
void CalculateAndloadProfilePalette(s16 BerryNumber); //

#endif // GUARD_ITEM_MENU_ICONS_H
