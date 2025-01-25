#include "stdafx.h"
#include "Item.h"

Item::Item(uint16_t maxItemProgress, uint16_t maxItemQuality, int16_t maxItemDurability) :
	maxItemState{maxItemProgress, maxItemQuality, maxItemDurability},
	currentItemState{ 0, 0, maxItemDurability } {}

Item::~Item() {}