#pragma once

#include <cstdint>
#include "Item.h"
#include "Player.h"

/* Keeps records of the entire crafting session and acts as mediator between Player and Item */
/* TODO make it so that there is only one source which controls what turn the system is on */
class CraftingSession {
public:
	struct CraftState {
		PlayerState playerState;
		ItemState	itemState;
		uint8_t turn = 0;
		uint8_t duration = 0;
		Skills::SkillName skillName;
	};

	CraftingSession(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred,
		ItemState baseItemState);
	~CraftingSession();

	bool CraftingTurn(const Skills::SkillInformation& skill);

	bool SaveCraftingTurn(uint8_t turn, uint8_t time);
	bool SaveCurrentCraftingTurn();
	bool LoadCraftingTurn(uint8_t turn);
	bool LoadLastCraftingTurn();
	bool ReloadCraftingTurn();

	inline const Player& GetPlayer() const;
	inline const Item& GetItem() const;
	inline uint8_t GetCraftingSessionTurn() const;
	inline uint8_t GetCraftingSessionDuration() const;
	inline std::array<CraftState, 60> GetCurrentCraftingHistory() const;

private:
	inline void ApplyPlayerItemBuffs(bool finalAppraisal);
	inline void LoadCraftState(uint8_t turn);

protected:
	

	Player player;
	Item item;
	CraftState currentState;
	uint8_t currentSkillDuration = 0;
	std::array<CraftState, 60> craftState;


};

inline const Player& CraftingSession::GetPlayer() const {
	return player;
}

inline const Item& CraftingSession::GetItem() const {
	return item;
}

inline uint8_t CraftingSession::GetCraftingSessionTurn() const {
	return currentState.turn;
}

inline uint8_t CraftingSession::GetCraftingSessionDuration() const {
	return currentState.duration + currentSkillDuration;
}

inline std::array<CraftingSession::CraftState, 60> CraftingSession::GetCurrentCraftingHistory() const {
	return craftState;
}

/* Looks at the previous turn in chain as the current turn has not been saved, meaning a different craft chain of buffs may be there */
inline void CraftingSession::ApplyPlayerItemBuffs(bool finalAppraisal) {
	if (craftState[currentState.turn - 1].playerState.buffs[Buffs::FINALAPPRAISAL] > 0 && item.IsItemCrafted()) {
		item.AppraiseItem();
	}

	if (!finalAppraisal && craftState[currentState.turn - 1].playerState.buffs[Buffs::MANIPULATION] > 0 && !item.IsItemBroken()) {
		item.ManipulateItem();
	}
}

/* Load selected turn */
inline void CraftingSession::LoadCraftState(uint8_t turn) {
	currentState = craftState[turn];
	player.LoadPlayerState(currentState.playerState);
	item.LoadItemState(currentState.itemState);
	++currentState.turn;
	currentSkillDuration = 0;
}