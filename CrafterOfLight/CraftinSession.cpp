#include "stdafx.h"
#include "CraftingSession.h"

CraftingSession::CraftingSession(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred,
	ItemState maxItemState)
	: player(Player(maxPlayerState, progressPerHundred, qualityPerHundred)), item(Item(maxItemState)) {
	SaveCurrentCraftingTurn();
}

CraftingSession::~CraftingSession() {}

bool CraftingSession::CraftingTurn(const Skills::SkillInformation& skill) {
	if (!player.IsSkillCastable(skill, item.GetCurrentDurability()) || item.GetCurrentDurability() <= 0) {
		return false;
	}

	item.CraftItem(player.CastSkill(skill.name));
	ApplyPlayerItemBuffs();

	currentSkillDuration = skill.castTime;

	return true;
}

bool CraftingSession::SaveCraftingTurn(uint8_t turn, uint8_t time) {
	craftState[turn] = { player.GetCurrentPlayerState(), item.GetCurrentItemState(), turn, time };
	currentState = craftState[turn];
	currentSkillDuration = 0;
	return true;
}

/* Saves the current turn with the added skill time cast before moving on to the next turn */
bool CraftingSession::SaveCurrentCraftingTurn() {
	currentState.duration += currentSkillDuration;
	craftState[currentState.turn] = { player.GetCurrentPlayerState(), item.GetCurrentItemState(), currentState.turn, currentState.duration};
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

/* Reloads current saved turn */
bool CraftingSession::ReloadCraftingTurn() {
	player.LoadPlayerState(currentState.playerState);
	item.LoadItemState(currentState.itemState);
	currentSkillDuration = 0;
	return true;
}