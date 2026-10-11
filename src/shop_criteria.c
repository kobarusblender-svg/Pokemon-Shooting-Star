#include "global.h"
#include "item.h"
#include "script.h"
#include "event_data.h"
#include "malloc.h"
#include "shop_criteria.h"

static EWRAM_DATA const u16 *sDynamicShopItemListRef = NULL;

// Remove the UNUSED if you'll use the functions!
static UNUSED bool32 ShopCriteriaByBadgeCount(u32 count);
static UNUSED bool32 ShopCriteriaByFlag(u32 flagId);
static UNUSED bool32 ShopCriteriaByVar(u32 varId, u32 varValue);
bool32 ShopCriteriaBerry(enum Item item);
bool32 ShopCriteriaFlower(enum Item item);

void TryBuildDynamicShopItemList(const u16 **ogItemList, u16 *resultingTotal)
{
    sDynamicShopItemListRef = *ogItemList;

    u16 *list = AllocZeroed((*resultingTotal + 1) * sizeof(u16));
    u32 overallIdx = 0, idx = 0;

    while (idx < *resultingTotal)
    {
        enum Item item = sDynamicShopItemListRef[idx];

        if (IsItemShopCriteriaFulfilled(item))
        {
            list[overallIdx] = item;
            overallIdx++;
        }

        idx++;
    }

    list[overallIdx] = ITEM_NONE;

    *ogItemList = list;
    *resultingTotal = overallIdx;
}

void TryFreeDynamicShopItemList(const u16 **ogItemList)
{
    Free((u16 *)*ogItemList);
    *ogItemList = sDynamicShopItemListRef;
}

// Add new Criterias below!

static UNUSED bool32 ShopCriteriaByBadgeCount(u32 count) //MOD CONTEST add ribbons to this if ever used
{
    u32 badgeCount = 0;

    for (u32 badgeFlag = FLAG_BADGE01_GET; badgeFlag < FLAG_BADGE01_GET + NUM_BADGES; badgeFlag++)
    {
        if (FlagGet(badgeFlag))
            badgeCount++;
    }

    if (badgeCount >= count)
        return TRUE;

    return FALSE;
}

// These two below are somewhat identical to ShopCriteriaByBadgeCount
// but uses only one specific event var/flag check. Useful if you need
// a specific badge flag instead of just the badge total.

static UNUSED bool32 ShopCriteriaByFlag(u32 flagId)
{
    if (FlagGet(flagId))
        return TRUE;

    return FALSE;
}

static UNUSED bool32 ShopCriteriaByVar(u32 varId, u32 varValue)
{
    if (VarGet(varId) >= varValue)
        return TRUE;

    return FALSE;
}

//MOD CONTEST These criteria handle both the Berry Flower and Berry Master shops.

//Displays 5 first berries even if still unknown, displays next 5 only if known, and only displays any other berries if there are 0 in bag while having their flowers.
bool32 ShopCriteriaBerry(enum Item item)
{
    int i;
    
    if (CountTotalItemQuantityInBag(item) < 1  || item <= ITEM_ASPEAR_BERRY 
        || (item <= ITEM_SITRUS_BERRY && BagPocket_CheckHasItem2(&gBagPockets[POCKET_BERRIES], item)))
    {
        for(i = 0; i < 6; i++)
        {
            if(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_HELD_ITEM) == item) //Checking is not held by any party pokemon
            {
                return FALSE;
            }
        }

        if ((item > ITEM_BELUE_BERRY && FlagGet(FLAG_FLOWERS_LEGEND)) //Checking the flower vars
                || (item <= ITEM_BELUE_BERRY && FlagGet(FLAG_FLOWERS_COMMON))
                || item <= ITEM_ASPEAR_BERRY)
            return TRUE;
        else
            return FlagGet(FLAG_FLOWER_CHERI + item - FIRST_BERRY_INDEX);
    }

    return FALSE;
}

//Displays 10 first flowers, unlocked berries and adjacent berries.
bool32 ShopCriteriaFlower(enum Item item)
{
    u8 FlowerBerryGauge = FIRST_FLOWER_INDEX - FIRST_BERRY_INDEX;

    if ((BagPocket_CheckHasItem2(&gBagPockets[POCKET_BERRIES], item - FlowerBerryGauge)) || item <= ITEM_FLOWER_SITRUS 
        || BagPocket_CheckHasItem2(&gBagPockets[POCKET_BERRIES], item + 1 - FlowerBerryGauge) 
        || BagPocket_CheckHasItem2(&gBagPockets[POCKET_BERRIES], item - 1 - FlowerBerryGauge))
        return TRUE;

    return FALSE;
}
