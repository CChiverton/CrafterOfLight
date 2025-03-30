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
		/* Smart tracking features */
		bool finalAppraisalUsed = false;
		bool synthesisUsedDuringVeneration = false;
		bool touchUsedDuringInnovation = false;
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
	inline const CraftState& GetCurrentState() const;
	inline uint8_t GetCraftingSessionTurn() const;
	inline uint8_t GetCraftingSessionDuration() const;
	inline const std::array<CraftState, 30>& GetCurrentCraftingHistory() const;

private:
	inline void ApplyPlayerItemBuffs(bool finalAppraisal);
	inline void LoadCraftState(uint8_t turn);

public:
	/* "Smart" features */
	inline void SetFinalAppraisalUsed();
	inline bool GetFinalAppraisalUsed() const;
	inline void SetSynthesisUsedDuringVeneration(bool usedState);
	inline bool GetSynthesisUsedDuringVeneration() const;
	inline void SetTouchUsedDuringInnovation(bool usedState);
	inline bool GetTouchUsedDuringInnovation() const;
	inline void FullRepairItem();

	uint64_t totalCasts = 0;

protected:
	

	Player player;
	Item item;
	CraftState currentState;
	uint8_t currentSkillDuration = 0;
	std::array<CraftState, 30> craftState;


};

inline const Player& CraftingSession::GetPlayer() const {
	return player;
}

inline const Item& CraftingSession::GetItem() const {
	return item;
}

inline const CraftingSession::CraftState& CraftingSession::GetCurrentState() const {
	return currentState;
}

inline uint8_t CraftingSession::GetCraftingSessionTurn() const {
	return currentState.turn;
}

inline uint8_t CraftingSession::GetCraftingSessionDuration() const {
	return currentState.duration + currentSkillDuration;
}

inline const std::array<CraftingSession::CraftState, 30>& CraftingSession::GetCurrentCraftingHistory() const {
	return craftState;
}

/* Looks at the previous turn in chain as the current turn has not been saved, meaning a different craft chain of buffs may be there */
inline void CraftingSession::ApplyPlayerItemBuffs(bool finalAppraisal) {
	uint8_t turn = currentState.turn - 1;
	if (craftState[turn].playerState.buffs[Buffs::FINALAPPRAISAL] > 0 && item.IsItemCrafted()) {
		item.AppraiseItem();
	}

	if (!finalAppraisal && craftState[turn].playerState.buffs[Buffs::MANIPULATION] > 0 && !item.IsItemBroken()) {
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


/* "Smart" features */
inline void CraftingSession::SetFinalAppraisalUsed() {
	currentState.finalAppraisalUsed = true;
}

inline bool CraftingSession::GetFinalAppraisalUsed() const {
	return currentState.finalAppraisalUsed;
}

inline void CraftingSession::SetSynthesisUsedDuringVeneration(bool usedState) {
	currentState.synthesisUsedDuringVeneration = usedState;
}
inline bool CraftingSession::GetSynthesisUsedDuringVeneration() const {
	return currentState.synthesisUsedDuringVeneration;
}

inline void CraftingSession::SetTouchUsedDuringInnovation(bool usedState) {
	currentState.touchUsedDuringInnovation = usedState;
}
inline bool CraftingSession::GetTouchUsedDuringInnovation() const {
	return currentState.touchUsedDuringInnovation;
}

inline void CraftingSession::FullRepairItem() {
	item.CraftItem({ Skills::SkillName::NONE,		Skills::SkillType::NONE,			0,		-127,		0,	0,		0 });
}