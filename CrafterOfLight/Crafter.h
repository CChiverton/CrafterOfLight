#pragma once

#include "CraftingSession.h"

class Crafter {
public:
	Crafter(uint8_t maxTurns, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~Crafter();

	inline const uint8_t GetMaximumTurns() const;


protected:
	CraftingSession craftingManager;
	const uint8_t maxTurnLimit;
};

inline const uint8_t Crafter::GetMaximumTurns() const {
	return maxTurnLimit;
}