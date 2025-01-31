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

bool CraftingSession::SaveCurrentCraftingTurn() {
	craftState[currentState.turn] = { player.GetCurrentPlayerState(), item.GetCurrentItemState(), currentState.turn, (uint8_t)(currentState.duration + currentSkillDuration)};
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
	if (currentState.turn < 1) {
		return false;
	}
	--currentState.turn;
	LoadCraftState(currentState.turn);
	return true;
}

/* Reloads current saved turn */
bool CraftingSession::ReloadCraftingTurn() {
	LoadCraftState(currentState.turn);
	return true;
}