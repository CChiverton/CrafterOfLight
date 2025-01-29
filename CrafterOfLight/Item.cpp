#include "stdafx.h"
#include "Item.h"

Item::Item(ItemState maxItemState) :
	maxItemState(maxItemState) ,
	currentItemState{ 0, 0, maxItemState.durability} {}

Item::~Item() {}

void Item::CraftItem(const Skills::SkillInformation& skill) {
	RemoveDurability(skill.costDurability);
	currentItemState.progress += skill.synthesisEfficiency;
	currentItemState.quality += skill.touchEfficiency;
}