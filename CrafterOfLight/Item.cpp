#include "stdafx.h"
#include "Item.h"

Item::Item(ItemState maxItemState) :
	maxItemState(maxItemState) ,
	currentItemState{ 0, 0, maxItemState.durability} {}

Item::~Item() {}