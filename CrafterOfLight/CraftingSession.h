#pragma once

#include <cstdint>
#include "Item.h"
#include "Player.h"

/* Keeps records of the entire crafting session and acts as mediator between Player and Item */
class CraftingSession {
public:
	CraftingSession(uint8_t maxTurns,
		PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred,
		ItemState baseItemState);
	~CraftingSession();

	bool SaveCraftingTurn(uint8_t turn);
	bool LoadCraftingTurn(uint8_t turn);

	inline Player GetPlayer() const;
	inline Item GetItem() const;
	inline uint8_t GetMaximumTurns() const;

protected:
	struct CraftState {
		PlayerState playerState;
		ItemState	itemState;
	};

	Player player;
	Item item;
	const uint8_t maxCraftingTurns;
	uint8_t currentTurn = 0;
	std::array<CraftState, 60> craftState;
};

inline Player CraftingSession::GetPlayer() const {
	return player;
}

inline Item CraftingSession::GetItem() const {
	return item;
}

inline uint8_t CraftingSession::GetMaximumTurns() const {
	return maxCraftingTurns;
}