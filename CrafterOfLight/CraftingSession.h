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

	void CraftingTurn(const Skills::SkillInformation& skill);

	bool SaveCraftingTurn(uint8_t turn);
	bool LoadCraftingTurn(uint8_t turn);

	inline Player GetPlayer() const;
	inline Item GetItem() const;	

private:
	inline void ApplyPlayerItemBuffs();

protected:
	struct CraftState {
		PlayerState playerState;
		ItemState	itemState;
	};

	Player player;
	Item item;
	uint8_t currentTurn = 0;
	std::array<CraftState, 60> craftState;


};

inline Player CraftingSession::GetPlayer() const {
	return player;
}

inline Item CraftingSession::GetItem() const {
	return item;
}

inline void CraftingSession::ApplyPlayerItemBuffs() {
	if (craftState[currentTurn].playerState.buffs[Buffs::FINALAPPRAISAL] > 0 && item.IsItemCrafted()) {
		item.AppraiseItem();
	}

	if (craftState[currentTurn].playerState.buffs[Buffs::MANIPULATION] > 0 && item.IsItemBroken()) {
		item.ManipulateItem();
	}
}