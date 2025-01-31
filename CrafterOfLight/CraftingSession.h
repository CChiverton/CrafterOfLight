#pragma once

#include <cstdint>
#include "Item.h"
#include "Player.h"

/* Keeps records of the entire crafting session and acts as mediator between Player and Item */
class CraftingSession {
public:
	CraftingSession(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred,
		ItemState baseItemState);
	~CraftingSession();

	bool CraftingTurn(const Skills::SkillInformation& skill);

	bool SaveCraftingTurn(uint8_t turn, uint8_t time);
	bool SaveCurrentCraftingTurn();
	bool LoadCraftingTurn(uint8_t turn);
	bool LoadLastCraftingTurn();
	bool ReloadCraftingTurn();

	inline Player GetPlayer() const;
	inline Item GetItem() const;
	inline uint8_t GetCraftingSessionTurn() const;
	inline uint8_t GetCraftingSessionDuration() const;

private:
	inline void ApplyPlayerItemBuffs();
	inline void LoadCraftState(uint8_t turn);

protected:
	struct CraftState {
		PlayerState playerState;
		ItemState	itemState;
		uint8_t turn = 0;
		uint8_t duration = 0;
	};

	Player player;
	Item item;
	CraftState currentState;
	uint8_t currentSkillDuration = 0;
	std::array<CraftState, 60> craftState;


};

inline Player CraftingSession::GetPlayer() const {
	return player;
}

inline Item CraftingSession::GetItem() const {
	return item;
}

inline uint8_t CraftingSession::GetCraftingSessionTurn() const {
	return currentState.turn;
}

inline uint8_t CraftingSession::GetCraftingSessionDuration() const {
	return currentState.duration + currentSkillDuration;
}

inline void CraftingSession::ApplyPlayerItemBuffs() {
	if (craftState[currentState.turn].playerState.buffs[Buffs::FINALAPPRAISAL] > 0 && item.IsItemCrafted()) {
		item.AppraiseItem();
	}

	if (craftState[currentState.turn].playerState.buffs[Buffs::MANIPULATION] > 0 && item.IsItemBroken()) {
		item.ManipulateItem();
	}
}

inline void CraftingSession::LoadCraftState(uint8_t turn) {
	currentState = craftState[turn];
	player.LoadPlayerState(currentState.playerState);
	item.LoadItemState(currentState.itemState);
	currentSkillDuration = 0;
}