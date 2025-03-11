#include "stdafx.h"
#include "CraftingSession.h"

CraftingSession::CraftingSession(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred,
	ItemState maxItemState)
	: player(Player(maxPlayerState, progressPerHundred, qualityPerHundred)), item(Item(maxItemState)) {
	SaveCurrentCraftingTurn();
}

CraftingSession::~CraftingSession() {}

/* Assumes the item is in a workable state */
bool CraftingSession::CraftingTurn(const Skills::SkillInformation& skill) {
	if (!player.IsSkillCastable(skill, item.GetCurrentDurability())) {
		return false;
	}

	item.CraftItem(player.CastSkill());
	ApplyPlayerItemBuffs(skill.name == Skills::SkillName::FINALAPPRAISAL);

	currentState.skillName = skill.name;
	currentSkillDuration = skill.castTime;

	return true;
}

bool CraftingSession::SaveCraftingTurn(uint8_t turn, uint8_t time) {
	craftState[turn] = { player.GetCurrentPlayerState(), item.GetCurrentItemState(), currentState.turn, uint8_t(currentState.duration + currentSkillDuration), currentState.skillName,
		currentState.finalAppraisalUsed, currentState.synthesisUsedDuringVeneration, currentState.touchUsedDuringInnovation };
	currentState = craftState[turn];
	currentSkillDuration = 0;
	return true;
}

/* Saves the current turn with the added skill time cast before moving on to the next turn */
bool CraftingSession::SaveCurrentCraftingTurn() {
	craftState[currentState.turn] = { player.GetCurrentPlayerState(), item.GetCurrentItemState(), currentState.turn, uint8_t(currentState.duration + currentSkillDuration), currentState.skillName,
		currentState.finalAppraisalUsed, currentState.synthesisUsedDuringVeneration, currentState.touchUsedDuringInnovation};
	currentState = craftState[currentState.turn];
	++currentState.turn;
	currentSkillDuration = 0;
	return true;
}

/* Loads specified turn */
bool CraftingSession::LoadCraftingTurn(uint8_t turn) {
	LoadCraftState(turn);
	return true;
}

/* Loads previous saved turn */
bool CraftingSession::LoadLastCraftingTurn() {
	if (currentState.turn < 2) {
		return false;
	}
	LoadCraftState(currentState.turn-2);	// As state turn is currently tracking an unsaved turn, -1 is current saved turn.
	return true;
}

/* Resets current state to last saved turn */
bool CraftingSession::ReloadCraftingTurn() {
	if (currentState.turn < 1) {
		return false;
	}
	LoadCraftingTurn(currentState.turn - 1);
	return true;
}