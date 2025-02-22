#pragma once

#include <cstdint>
#include "Skills.h"

struct ItemState {
	uint16_t progress = 0;
	uint16_t quality = 0;
	int16_t durability = 0;
};

class Item {
public:
	Item(ItemState maxItemState);
	~Item();

	/* State */
	inline ItemState GetCurrentItemState() const;
	inline void LoadItemState(ItemState itemState);
	void CraftItem(const Skills::SkillInformation& skill);

	/* Progress */
	inline const uint16_t GetCurrentProgress() const;
	inline const uint16_t GetMaxProgress() const;
	inline const bool IsItemCrafted() const;
	inline void AppraiseItem();

	/* Quality */
	inline const uint16_t GetCurrentQuality() const;
	inline const uint16_t GetMaxQuality() const;
	inline const bool IsItemMaxQuality() const;

	/* Durability */
	inline const int16_t GetCurrentDurability() const;
	inline const int16_t GetMaxDurability() const;
	inline void RemoveDurability(int16_t durability);
	inline const bool IsItemBroken() const;
	inline void ManipulateItem();

private:
	ItemState currentItemState, maxItemState;
};

inline ItemState Item::GetCurrentItemState() const {
	return currentItemState;
}

inline void Item::LoadItemState(ItemState itemState) {
	currentItemState = itemState;
}

inline const uint16_t Item::GetCurrentProgress() const {
	return currentItemState.progress;
}

inline const uint16_t Item::GetMaxProgress() const {
	return maxItemState.progress;
}

inline const bool Item::IsItemCrafted() const {
	return currentItemState.progress >= maxItemState.progress;
}

inline void Item::AppraiseItem() {
	currentItemState.progress = maxItemState.progress - 1;
}

inline const uint16_t Item::GetCurrentQuality() const {
	return currentItemState.quality;
}

inline const uint16_t Item::GetMaxQuality() const {
	return maxItemState.quality;
}

inline const bool Item::IsItemMaxQuality() const {
	return currentItemState.quality >= maxItemState.quality;
}

inline const int16_t Item::GetCurrentDurability() const {
	return currentItemState.durability;
}

inline const int16_t Item::GetMaxDurability() const {
	return maxItemState.durability;
}

inline const bool Item::IsItemBroken() const {
	return currentItemState.durability <= 0;
}

inline void Item::RemoveDurability(int16_t durability) {
	currentItemState.durability -= durability;
	if (currentItemState.durability > maxItemState.durability) {
		currentItemState.durability = maxItemState.durability;
	}
}

inline void Item::ManipulateItem() {
	RemoveDurability(-5);
}