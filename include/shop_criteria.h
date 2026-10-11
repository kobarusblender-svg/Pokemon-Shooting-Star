#ifndef GUARD_SHOP_CRITERIA_H
#define GUARD_SHOP_CRITERIA_H

void TryBuildDynamicShopItemList(const u16 **ogItemList, u16 *resultingTotal);
void TryFreeDynamicShopItemList(const u16 **ogItemList);

// Add new Criterias below!
bool32 ShopCriteriaBerry(enum Item item);
bool32 ShopCriteriaFlower(enum Item item);

#endif // GUARD_SHOP_CRITERIA_H
