#pragma once

#include <cstdint>

struct ItemState {
	uint16_t progress = 0;
	uint16_t quality = 0;
	int16_t durability = 0;
};

class Item {
public:
	Item(ItemState maxItemState);
	~Item();

	/* Progress */
	inline const uint16_t GetCurrentProgress() const;
	inline const uint16_t GetMaxProgress() const;

	/* Quality */
	inline const uint16_t GetCurrentQuality() const;
	inline const uint16_t GetMaxQuality() const;

	/* Durability */
	inline const int16_t GetCurrentDurability() const;
	inline const int16_t GetMaxDurability() const;

private:
	ItemState currentItemState, maxItemState;
};

inline const uint16_t Item::GetCurrentProgress() const {
	return currentItemState.progress;
}

inline const uint16_t Item::GetMaxProgress() const {
	return maxItemState.progress;
}

inline const uint16_t Item::GetCurrentQuality() const {
	return currentItemState.quality;
}

inline const uint16_t Item::GetMaxQuality() const {
	return maxItemState.quality;
}

inline const int16_t Item::GetCurrentDurability() const {
	return currentItemState.durability;
}

inline const int16_t Item::GetMaxDurability() const {
	return maxItemState.durability;
}