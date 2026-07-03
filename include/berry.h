#ifndef GUARD_BERRY_H
#define GUARD_BERRY_H

void SetEnigmaBerry(u8 *src);
bool32 IsEnigmaBerryValid(void);
const struct BerryInfo *GetBerryInfo(enum BerryId berry);
struct BerryTree *GetBerryTreeInfo(u8 id);
bool32 ObjectEventInteractionWaterBerryTree(void);
bool8 IsPlayerFacingEmptyBerryTreePatch(void);
bool8 TryToWaterBerryTree(void);
void ClearBerryTrees(void);
void BerryTreeTimeUpdate(s32 minutes);
void PlantBerryTree(u8 id, enum BerryId berry, u8 stage, bool8 allowGrowth);
void RemoveBerryTree(u8 id);
u8 GetBerryTypeByBerryTreeId(u8 id);
u8 GetStageByBerryTreeId(u8 id);
void GetBerryNameByBerryType(u8 berry, u8 *string);
void Bag_ChooseBerry(void);
void Bag_ChooseMulch(void);
void ObjectEventInteractionGetBerryTreeData(void);
void ObjectEventInteractionPlantBerryTree(void);
void ObjectEventInteractionPickBerryTree(void);
void ObjectEventInteractionRemoveBerryTree(void);
void ObjectEventInteractionApplyMulch(void);
bool8 PlayerHasBerries(void);
void SetBerryTreesSeen(void);
bool32 BerryTreeGrow(struct BerryTree *tree);

extern const struct SpriteFrameImage gPicTable_CheriBerryTree[];
extern const struct SpriteFrameImage gPicTable_ChestoBerryTree[];
extern const struct SpriteFrameImage gPicTable_PechaBerryTree[];
extern const struct SpriteFrameImage gPicTable_RawstBerryTree[];
extern const struct SpriteFrameImage gPicTable_AspearBerryTree[];
extern const struct SpriteFrameImage gPicTable_LeppaBerryTree[];
extern const struct SpriteFrameImage gPicTable_OranBerryTree[];
extern const struct SpriteFrameImage gPicTable_PersimBerryTree[];
extern const struct SpriteFrameImage gPicTable_LumBerryTree[];
extern const struct SpriteFrameImage gPicTable_SitrusBerryTree[];
extern const struct SpriteFrameImage gPicTable_FigyBerryTree[];
extern const struct SpriteFrameImage gPicTable_WikiBerryTree[];
extern const struct SpriteFrameImage gPicTable_MagoBerryTree[];
extern const struct SpriteFrameImage gPicTable_AguavBerryTree[];
extern const struct SpriteFrameImage gPicTable_IapapaBerryTree[];
extern const struct SpriteFrameImage gPicTable_RazzBerryTree[];
extern const struct SpriteFrameImage gPicTable_BlukBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_NanabBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_WepearBerryTree[];
extern const struct SpriteFrameImage gPicTable_PinapBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_PomegBerryTree[];
extern const struct SpriteFrameImage gPicTable_KelpsyBerryTree[];
extern const struct SpriteFrameImage gPicTable_QualotBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_HondewBerryTree[];
extern const struct SpriteFrameImage gPicTable_GrepaBerryTree[];
extern const struct SpriteFrameImage gPicTable_TamatoBerryTree[];
extern const struct SpriteFrameImage gPicTable_CornnBerryTree[];
extern const struct SpriteFrameImage gPicTable_MagostBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_RabutaBerryTree[];
extern const struct SpriteFrameImage gPicTable_NomelBerryTree[];
extern const struct SpriteFrameImage gPicTable_SpelonBerryTree[];
extern const struct SpriteFrameImage gPicTable_PamtreBerryTree[];
extern const struct SpriteFrameImage gPicTable_WatmelBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_DurinBerryTree[];
extern const struct SpriteFrameImage gPicTable_BelueBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_PaprikBerryTree[];//E-Reader Berries:
extern const struct SpriteFrameImage gPicTable_EggantBerryTree[];//
extern const struct SpriteFrameImage gPicTable_DrashBerryTree[];//
extern const struct SpriteFrameImage gPicTable_SkuashBerryTree[];//
extern const struct SpriteFrameImage gPicTable_PumkinBerryTree[];//
extern const struct SpriteFrameImage gPicTable_ChilanERBerryTree[];//
extern const struct SpriteFrameImage gPicTable_MaliumBerryTree[];//
extern const struct SpriteFrameImage gPicTable_TopoBerryTree[];//
extern const struct SpriteFrameImage gPicTable_StribBerryTree[];//
extern const struct SpriteFrameImage gPicTable_NoionBerryTree[];//
extern const struct SpriteFrameImage gPicTable_NutpeaBerryTree[];//
extern const struct SpriteFrameImage gPicTable_RokaBerryTree[];//
/*extern const struct SpriteFrameImage gPicTable_OccaBerryTree[];
extern const struct SpriteFrameImage gPicTable_YacheBerryTree[];
extern const struct SpriteFrameImage gPicTable_ChopleBerryTree[];
extern const struct SpriteFrameImage gPicTable_KebiaBerryTree[];
extern const struct SpriteFrameImage gPicTable_ShucaBerryTree[];
extern const struct SpriteFrameImage gPicTable_PayapaBerryTree[];
extern const struct SpriteFrameImage gPicTable_TangaBerryTree[];
extern const struct SpriteFrameImage gPicTable_KasibBerryTree[];
extern const struct SpriteFrameImage gPicTable_HabanBerryTree[];
extern const struct SpriteFrameImage gPicTable_ColburBerryTree[];
extern const struct SpriteFrameImage gPicTable_RoseliBerryTree[];*/
extern const struct SpriteFrameImage gPicTable_LiechiBerryTree[];
extern const struct SpriteFrameImage gPicTable_GanlonBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_SalacBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_PetayaBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_ApicotBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_LansatBerryTree[];
extern const struct SpriteFrameImage gPicTable_StarfBerryTree[];//MOD CONTEST
extern const struct SpriteFrameImage gPicTable_EnigmaBerryTree[];//MOD CONTEST
/*extern const struct SpriteFrameImage gPicTable_MicleBerryTree[];
extern const struct SpriteFrameImage gPicTable_CustapBerryTree[];
extern const struct SpriteFrameImage gPicTable_JabocaBerryTree[];
extern const struct SpriteFrameImage gPicTable_RowapBerryTree[];*/

extern const u8 gBerryTreePaletteSlotTable_Cheri[];
extern const u8 gBerryTreePaletteSlotTable_Chesto[];
extern const u8 gBerryTreePaletteSlotTable_Pecha[];
extern const u8 gBerryTreePaletteSlotTable_Rawst[];
extern const u8 gBerryTreePaletteSlotTable_Aspear[];
extern const u8 gBerryTreePaletteSlotTable_Leppa[];
extern const u8 gBerryTreePaletteSlotTable_Oran[];
extern const u8 gBerryTreePaletteSlotTable_Persim[];
extern const u8 gBerryTreePaletteSlotTable_Lum[];
extern const u8 gBerryTreePaletteSlotTable_Sitrus[];
extern const u8 gBerryTreePaletteSlotTable_Figy[];
extern const u8 gBerryTreePaletteSlotTable_Wiki[];
extern const u8 gBerryTreePaletteSlotTable_Mago[];
extern const u8 gBerryTreePaletteSlotTable_Aguav[];
extern const u8 gBerryTreePaletteSlotTable_Iapapa[];
extern const u8 gBerryTreePaletteSlotTable_Razz[];
extern const u8 gBerryTreePaletteSlotTable_Bluk[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Nanab[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Wepear[];
extern const u8 gBerryTreePaletteSlotTable_Pinap[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Pomeg[];
extern const u8 gBerryTreePaletteSlotTable_Kelpsy[];
extern const u8 gBerryTreePaletteSlotTable_Qualot[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Hondew[];
extern const u8 gBerryTreePaletteSlotTable_Grepa[];
extern const u8 gBerryTreePaletteSlotTable_Tamato[];
extern const u8 gBerryTreePaletteSlotTable_Cornn[];
extern const u8 gBerryTreePaletteSlotTable_Magost[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Rabuta[];
extern const u8 gBerryTreePaletteSlotTable_Nomel[];
extern const u8 gBerryTreePaletteSlotTable_Spelon[];
extern const u8 gBerryTreePaletteSlotTable_Pamtre[];
extern const u8 gBerryTreePaletteSlotTable_Watmel[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Durin[];
extern const u8 gBerryTreePaletteSlotTable_Belue[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Paprik[];//E reader Berries:
extern const u8 gBerryTreePaletteSlotTable_Eggant[];//
extern const u8 gBerryTreePaletteSlotTable_Drash[];//
extern const u8 gBerryTreePaletteSlotTable_Skuash[];//
extern const u8 gBerryTreePaletteSlotTable_Pumkin[];//
extern const u8 gBerryTreePaletteSlotTable_ChilanER[];//
extern const u8 gBerryTreePaletteSlotTable_Malium[];//
extern const u8 gBerryTreePaletteSlotTable_Topo[];//
extern const u8 gBerryTreePaletteSlotTable_Strib[];//
extern const u8 gBerryTreePaletteSlotTable_Noion[];//
extern const u8 gBerryTreePaletteSlotTable_Nutpea[];//
extern const u8 gBerryTreePaletteSlotTable_Roka[];//
//extern const u8 gBerryTreePaletteSlotTable_Occa[];
//extern const u8 gBerryTreePaletteSlotTable_Yache[];
//extern const u8 gBerryTreePaletteSlotTable_Chople[];
//extern const u8 gBerryTreePaletteSlotTable_Kebia[];
//extern const u8 gBerryTreePaletteSlotTable_Shuca[];
//extern const u8 gBerryTreePaletteSlotTable_Payapa[];
//extern const u8 gBerryTreePaletteSlotTable_Tanga[];
//extern const u8 gBerryTreePaletteSlotTable_Kasib[];
//extern const u8 gBerryTreePaletteSlotTable_Haban[];
//extern const u8 gBerryTreePaletteSlotTable_Colbur[];
//extern const u8 gBerryTreePaletteSlotTable_Roseli[];
extern const u8 gBerryTreePaletteSlotTable_Liechi[];
extern const u8 gBerryTreePaletteSlotTable_Ganlon[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Salac[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Petaya[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Apicot[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Lansat[];
extern const u8 gBerryTreePaletteSlotTable_Starf[];//MOD CONTEST
extern const u8 gBerryTreePaletteSlotTable_Enigma[];//MOD CONTEST
//extern const u8 gBerryTreePaletteSlotTable_Micle[];
//extern const u8 gBerryTreePaletteSlotTable_Custap[];
//extern const u8 gBerryTreePaletteSlotTable_Jaboca[];
//extern const u8 gBerryTreePaletteSlotTable_Rowap[];

extern const u32 gBerryPic_Cheri[];
extern const u32 gBerryPic_Chesto[];
extern const u32 gBerryPic_Pecha[];
extern const u32 gBerryPic_Rawst[];
extern const u32 gBerryPic_Aspear[];
extern const u32 gBerryPic_Leppa[];
extern const u32 gBerryPic_Oran[];
extern const u32 gBerryPic_Persim[];
extern const u32 gBerryPic_Lum[];
extern const u32 gBerryPic_Sitrus[];
extern const u32 gBerryPic_Figy[];
extern const u32 gBerryPic_Wiki[];
extern const u32 gBerryPic_Mago[];
extern const u32 gBerryPic_Aguav[];
extern const u32 gBerryPic_Iapapa[];
extern const u32 gBerryPic_Razz[];
extern const u32 gBerryPic_Bluk[];
extern const u32 gBerryPic_Nanab[];
extern const u32 gBerryPic_Wepear[];
extern const u32 gBerryPic_Pinap[];
extern const u32 gBerryPic_Pomeg[];
extern const u32 gBerryPic_Kelpsy[];
extern const u32 gBerryPic_Qualot[];
extern const u32 gBerryPic_Hondew[];
extern const u32 gBerryPic_Grepa[];
extern const u32 gBerryPic_Tamato[];
extern const u32 gBerryPic_Cornn[];
extern const u32 gBerryPic_Magost[];
extern const u32 gBerryPic_Rabuta[];
extern const u32 gBerryPic_Nomel[];
extern const u32 gBerryPic_Spelon[];
extern const u32 gBerryPic_Pamtre[];
extern const u32 gBerryPic_Watmel[];
extern const u32 gBerryPic_Durin[];
extern const u32 gBerryPic_Belue[];
/*extern const u32 gBerryPic_Chilan[];
extern const u32 gBerryPic_Occa[];
extern const u32 gBerryPic_Passho[];
extern const u32 gBerryPic_Wacan[];
extern const u32 gBerryPic_Rindo[];
extern const u32 gBerryPic_Yache[];
extern const u32 gBerryPic_Chople[];
extern const u32 gBerryPic_Kebia[];
extern const u32 gBerryPic_Shuca[];
extern const u32 gBerryPic_Coba[];
extern const u32 gBerryPic_Payapa[];
extern const u32 gBerryPic_Tanga[];
extern const u32 gBerryPic_Charti[];
extern const u32 gBerryPic_Kasib[];
extern const u32 gBerryPic_Haban[];
extern const u32 gBerryPic_Colbur[];
extern const u32 gBerryPic_Babiri[];
extern const u32 gBerryPic_Roseli[];*/
extern const u32 gBerryPic_Paprik[];
extern const u32 gBerryPic_Eggant[];
extern const u32 gBerryPic_Drash[];
extern const u32 gBerryPic_Skuash[];
extern const u32 gBerryPic_Pumkin[];
extern const u32 gBerryPic_ChilanER[];
extern const u32 gBerryPic_Malium[];
extern const u32 gBerryPic_Topo[];
extern const u32 gBerryPic_Strib[];
extern const u32 gBerryPic_Noion[];
extern const u32 gBerryPic_Nutpea[];
extern const u32 gBerryPic_Roka[];
extern const u32 gBerryPic_Liechi[];
extern const u32 gBerryPic_Ganlon[];
extern const u32 gBerryPic_Salac[];
extern const u32 gBerryPic_Petaya[];
extern const u32 gBerryPic_Apicot[];
extern const u32 gBerryPic_Lansat[];
extern const u32 gBerryPic_Starf[];
extern const u32 gBerryPic_Enigma[];/*
extern const u32 gBerryPic_Micle[];
extern const u32 gBerryPic_Custap[];
extern const u32 gBerryPic_Jaboca[];
extern const u32 gBerryPic_Rowap[];
extern const u32 gBerryPic_Kee[];
extern const u32 gBerryPic_Maranga[];*/

extern const u16 gBerryPalette_Cheri[];
extern const u16 gBerryPalette_Chesto[];
extern const u16 gBerryPalette_Pecha[];
extern const u16 gBerryPalette_Rawst[];
extern const u16 gBerryPalette_Aspear[];
extern const u16 gBerryPalette_Leppa[];
extern const u16 gBerryPalette_Oran[];
extern const u16 gBerryPalette_Persim[];
extern const u16 gBerryPalette_Lum[];
extern const u16 gBerryPalette_Sitrus[];
extern const u16 gBerryPalette_Figy[];
extern const u16 gBerryPalette_Wiki[];
extern const u16 gBerryPalette_Mago[];
extern const u16 gBerryPalette_Aguav[];
extern const u16 gBerryPalette_Iapapa[];
extern const u16 gBerryPalette_Razz[];
extern const u16 gBerryPalette_Bluk[];
extern const u16 gBerryPalette_Nanab[];
extern const u16 gBerryPalette_Wepear[];
extern const u16 gBerryPalette_Pinap[];
extern const u16 gBerryPalette_Pomeg[];
extern const u16 gBerryPalette_Kelpsy[];
extern const u16 gBerryPalette_Qualot[];
extern const u16 gBerryPalette_Hondew[];
extern const u16 gBerryPalette_Grepa[];
extern const u16 gBerryPalette_Tamato[];
extern const u16 gBerryPalette_Cornn[];
extern const u16 gBerryPalette_Magost[];
extern const u16 gBerryPalette_Rabuta[];
extern const u16 gBerryPalette_Nomel[];
extern const u16 gBerryPalette_Spelon[];
extern const u16 gBerryPalette_Pamtre[];
extern const u16 gBerryPalette_Watmel[];
extern const u16 gBerryPalette_Durin[];
extern const u16 gBerryPalette_Belue[];
/*extern const u16 gBerryPalette_Chilan[];
extern const u16 gBerryPalette_Occa[];
extern const u16 gBerryPalette_Passho[];
extern const u16 gBerryPalette_Wacan[];
extern const u16 gBerryPalette_Rindo[];
extern const u16 gBerryPalette_Yache[];
extern const u16 gBerryPalette_Chople[];
extern const u16 gBerryPalette_Kebia[];
extern const u16 gBerryPalette_Shuca[];
extern const u16 gBerryPalette_Coba[];
extern const u16 gBerryPalette_Payapa[];
extern const u16 gBerryPalette_Tanga[];
extern const u16 gBerryPalette_Charti[];
extern const u16 gBerryPalette_Kasib[];
extern const u16 gBerryPalette_Haban[];
extern const u16 gBerryPalette_Colbur[];
extern const u16 gBerryPalette_Babiri[];
extern const u16 gBerryPalette_Roseli[];*/
extern const u16 gBerryPalette_Paprik[];
extern const u16 gBerryPalette_Eggant[];
extern const u16 gBerryPalette_Drash[];
extern const u16 gBerryPalette_Skuash[];
extern const u16 gBerryPalette_Pumkin[];
extern const u16 gBerryPalette_ChilanER[];
extern const u16 gBerryPalette_Malium[];
extern const u16 gBerryPalette_Topo[];
extern const u16 gBerryPalette_Strib[];
extern const u16 gBerryPalette_Noion[];
extern const u16 gBerryPalette_Nutpea[];
extern const u16 gBerryPalette_Roka[];
extern const u16 gBerryPalette_Liechi[];
extern const u16 gBerryPalette_Ganlon[];
extern const u16 gBerryPalette_Salac[];
extern const u16 gBerryPalette_Petaya[];
extern const u16 gBerryPalette_Apicot[];
extern const u16 gBerryPalette_Lansat[];
extern const u16 gBerryPalette_Starf[];
extern const u16 gBerryPalette_Enigma[];
/*extern const u16 gBerryPalette_Micle[];
extern const u16 gBerryPalette_Custap[];
extern const u16 gBerryPalette_Jaboca[];
extern const u16 gBerryPalette_Rowap[];
extern const u16 gBerryPalette_Kee[];
extern const u16 gBerryPalette_Maranga[];*/

// berry flower pics
extern const u32 gFlowerPic_Cheri[];
extern const u32 gFlowerPic_Chesto[];
extern const u32 gFlowerPic_Pecha[];
extern const u32 gFlowerPic_Rawst[];
extern const u32 gFlowerPic_Aspear[];
extern const u32 gFlowerPic_Leppa[];
extern const u32 gFlowerPic_Oran[];
extern const u32 gFlowerPic_Persim[];
extern const u32 gFlowerPic_Lum[];
extern const u32 gFlowerPic_Sitrus[];
extern const u32 gFlowerPic_Figy[];
extern const u32 gFlowerPic_Wiki[];
extern const u32 gFlowerPic_Mago[];
extern const u32 gFlowerPic_Aguav[];
extern const u32 gFlowerPic_Iapapa[];
extern const u32 gFlowerPic_Razz[];
extern const u32 gFlowerPic_Bluk[];
extern const u32 gFlowerPic_Nanab[];
extern const u32 gFlowerPic_Wepear[];
extern const u32 gFlowerPic_Pinap[];
extern const u32 gFlowerPic_Pomeg[];
extern const u32 gFlowerPic_Kelpsy[];
extern const u32 gFlowerPic_Qualot[];
extern const u32 gFlowerPic_Hondew[];
extern const u32 gFlowerPic_Grepa[];
extern const u32 gFlowerPic_Tamato[];
extern const u32 gFlowerPic_Cornn[];
extern const u32 gFlowerPic_Magost[];
extern const u32 gFlowerPic_Rabuta[];
extern const u32 gFlowerPic_Nomel[];
extern const u32 gFlowerPic_Spelon[];
extern const u32 gFlowerPic_Pamtre[];
extern const u32 gFlowerPic_Watmel[];
extern const u32 gFlowerPic_Durin[];
extern const u32 gFlowerPic_Belue[];/*
extern const u32 gFlowerPic_Chilan[];
extern const u32 gFlowerPic_Occa[];
extern const u32 gFlowerPic_Passho[];
extern const u32 gFlowerPic_Wacan[];
extern const u32 gFlowerPic_Rindo[];
extern const u32 gFlowerPic_Yache[];
extern const u32 gFlowerPic_Chople[];
extern const u32 gFlowerPic_Kebia[];
extern const u32 gFlowerPic_Shuca[];
extern const u32 gFlowerPic_Coba[];
extern const u32 gFlowerPic_Payapa[];
extern const u32 gFlowerPic_Tanga[];*/
extern const u32 gFlowerPic_Pumkin[];// MOD CONTEST E-reader berries:
extern const u32 gFlowerPic_Drash[];
extern const u32 gFlowerPic_Eggant[];
extern const u32 gFlowerPic_Strib[];
extern const u32 gFlowerPic_ChilanER[];
extern const u32 gFlowerPic_Nutpea[];
extern const u32 gFlowerPic_Noion[];//(Japanese exclusive Ginema berry)
extern const u32 gFlowerPic_Roka[];//(Japanese exclusive Kuo berry)
extern const u32 gFlowerPic_Skuash[];//(Japanese exclusive Yago berry)
extern const u32 gFlowerPic_Paprik[];//(Japanese exclusive Touga berry)
extern const u32 gFlowerPic_Malium[];//(Japanese exclusive Niniku berry)
extern const u32 gFlowerPic_Topo[];//(Japanese exclusive Topo berry)
//extern const u32 gFlowerPic_Charti[];
//extern const u32 gFlowerPic_Kasib[];
//extern const u32 gFlowerPic_Haban[];
//extern const u32 gFlowerPic_Colbur[];
//extern const u32 gFlowerPic_Babiri[];
//extern const u32 gFlowerPic_Roseli[];
extern const u32 gFlowerPic_Liechi[];
extern const u32 gFlowerPic_Ganlon[];
extern const u32 gFlowerPic_Salac[];
extern const u32 gFlowerPic_Petaya[];
extern const u32 gFlowerPic_Apicot[];
extern const u32 gFlowerPic_Lansat[];
extern const u32 gFlowerPic_Starf[];
extern const u32 gFlowerPic_Enigma[];
//extern const u32 gFlowerPic_Micle[];
//extern const u32 gFlowerPic_Custap[];
//extern const u32 gFlowerPic_Jaboca[];
//extern const u32 gFlowerPic_Rowap[];
//extern const u32 gFlowerPic_Kee[];
//extern const u32 gFlowerPic_Maranga[];

//Berry Flower Palettes
extern const u16 gFlowerPalette_Cheri[];
extern const u16 gFlowerPalette_Chesto[];
extern const u16 gFlowerPalette_Pecha[];
extern const u16 gFlowerPalette_Rawst[];
extern const u16 gFlowerPalette_Aspear[];
extern const u16 gFlowerPalette_Leppa[];
extern const u16 gFlowerPalette_Oran[];
extern const u16 gFlowerPalette_Persim[];
extern const u16 gFlowerPalette_Lum[];
extern const u16 gFlowerPalette_Sitrus[];
extern const u16 gFlowerPalette_Figy[];
extern const u16 gFlowerPalette_Wiki[];
extern const u16 gFlowerPalette_Mago[];
extern const u16 gFlowerPalette_Aguav[];
extern const u16 gFlowerPalette_Iapapa[];
extern const u16 gFlowerPalette_Razz[];
extern const u16 gFlowerPalette_Bluk[];
extern const u16 gFlowerPalette_Nanab[];
extern const u16 gFlowerPalette_Wepear[];
extern const u16 gFlowerPalette_Pinap[];
extern const u16 gFlowerPalette_Pomeg[];
extern const u16 gFlowerPalette_Kelpsy[];
extern const u16 gFlowerPalette_Qualot[];
extern const u16 gFlowerPalette_Hondew[];
extern const u16 gFlowerPalette_Grepa[];
extern const u16 gFlowerPalette_Tamato[];
extern const u16 gFlowerPalette_Cornn[];
extern const u16 gFlowerPalette_Magost[];
extern const u16 gFlowerPalette_Rabuta[];
extern const u16 gFlowerPalette_Nomel[];
extern const u16 gFlowerPalette_Spelon[];
extern const u16 gFlowerPalette_Pamtre[];
extern const u16 gFlowerPalette_Watmel[];
extern const u16 gFlowerPalette_Durin[];
extern const u16 gFlowerPalette_Belue[];
//extern const u16 gFlowerPalette_Chilan[];
//extern const u16 gFlowerPalette_Occa[];
//extern const u16 gFlowerPalette_Passho[];
//extern const u16 gFlowerPalette_Wacan[];
//extern const u16 gFlowerPalette_Rindo[];
//extern const u16 gFlowerPalette_Yache[];
//extern const u16 gFlowerPalette_Chople[];
//extern const u16 gFlowerPalette_Kebia[];
//extern const u16 gFlowerPalette_Shuca[];
//extern const u16 gFlowerPalette_Coba[];
//extern const u16 gFlowerPalette_Payapa[];
//extern const u16 gFlowerPalette_Tanga[];
extern const u16 gFlowerPalette_Pumkin[];//MOD CONTEST E-Reader berries:
extern const u16 gFlowerPalette_Drash[];
extern const u16 gFlowerPalette_Eggant[];
extern const u16 gFlowerPalette_Strib[]; 
extern const u16 gFlowerPalette_ChilanER[];
extern const u16 gFlowerPalette_Nutpea[];
extern const u16 gFlowerPalette_Noion[];//(Japanese exclusive Ginema berry)
extern const u16 gFlowerPalette_Roka[];//(Japanese exclusive Kuo berry)
extern const u16 gFlowerPalette_Skuash[];//(Japanese exclusive Yago berry)
extern const u16 gFlowerPalette_Paprik[];//(Japanese exclusive Touga berry)
extern const u16 gFlowerPalette_Malium[];//(Japanese exclusive Niniku berry)
extern const u16 gFlowerPalette_Topo[];// (Japanese exclusive Topo berry)
//extern const u16 gFlowerPalette_Charti[];
//extern const u16 gFlowerPalette_Kasib[];
//extern const u16 gFlowerPalette_Haban[];
//extern const u16 gFlowerPalette_Colbur[];
//extern const u16 gFlowerPalette_Babiri[];
//extern const u16 gFlowerPalette_Roseli[];
extern const u16 gFlowerPalette_Liechi[];
extern const u16 gFlowerPalette_Ganlon[];
extern const u16 gFlowerPalette_Salac[];
extern const u16 gFlowerPalette_Petaya[];
extern const u16 gFlowerPalette_Apicot[];
extern const u16 gFlowerPalette_Lansat[];
extern const u16 gFlowerPalette_Starf[];
extern const u16 gFlowerPalette_Enigma[];
//extern const u16 gFlowerPalette_Micle[];
//extern const u16 gFlowerPalette_Custap[];
//extern const u16 gFlowerPalette_Jaboca[];
//extern const u16 gFlowerPalette_Rowap[];
//extern const u16 gFlowerPalette_Kee[];
//extern const u16 gFlowerPalette_Maranga[];

extern const struct Berry gBerries[];

#endif // GUARD_BERRY_H
