#pragma once
#include <cstdint>
#include "Item.h"

struct PlayerState {
	uint16_t cP = 0;
};

class Player {
public:
	Player(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred);
	~Player();

	inline const PlayerState GetCurrentPlayerState() const;
	inline void LoadPlayerState(PlayerState playerState);
	inline const float GetProgressEfficiency() const;
	inline const float GetQualityEfficiency() const;

protected:
	PlayerState maxPlayerState, currentPlayerState;
	const float progressPerOneEfficiency, qualityPerOneEfficiency;
};

inline const PlayerState Player::GetCurrentPlayerState() const {
	return currentPlayerState;
}

inline void Player::LoadPlayerState(PlayerState playerState) {
	currentPlayerState = playerState;
}

inline const float Player::GetProgressEfficiency() const {
	return progressPerOneEfficiency;
}

inline const float Player::GetQualityEfficiency() const {
	return qualityPerOneEfficiency;
}