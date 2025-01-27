#pragma once
#include <cstdint>
#include "Item.h"

struct PlayerState {
	uint16_t cP = 0;
};

class Player {
public:
	Player(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, Item maxItemState);
	~Player();

	inline const Item GetItemState() const;
	inline const PlayerState GetCurrentPlayerState() const;
	inline const float GetProgressEfficiency() const;
	inline const float GetQualityEfficiency() const;

protected:
	PlayerState maxPlayerState, currentPlayerState;
	const float progressPerOneEfficiency, qualityPerOneEfficiency;
	Item item;

	
};

inline const Item Player::GetItemState() const {
	return item;
}

inline const PlayerState Player::GetCurrentPlayerState() const {
	return currentPlayerState;
}

inline const float Player::GetProgressEfficiency() const {
	return progressPerOneEfficiency;
}

inline const float Player::GetQualityEfficiency() const {
	return qualityPerOneEfficiency;
}