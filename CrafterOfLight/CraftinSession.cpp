#include "stdafx.h"
#include "CraftingSession.h"

CraftingSession::CraftingSession(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred,
	ItemState maxItemState)
	: player(Player(maxPlayerState, progressPerHundred, qualityPerHundred)), item(Item(maxItemState)) {
	SaveCraftingTurn(currentTurn);
}

CraftingSession::~CraftingSession() {}

void CraftingSession::CraftingTurn(const Skills::SkillInformation& skill) {
	if (!player.IsSkillCastable(skill, item.GetCurrentDurability()) || item.GetCurrentDurability() <= 0) {
		return;
	}

	item.CraftItem(player.CastSkill(skill.name));
	ApplyPlayerItemBuffs();

	SaveCraftingTurn(currentTurn);
}

bool CraftingSession::SaveCraftingTurn(uint8_t turn) {
	craftState[turn] = { player.GetCurrentPlayerState(), item.GetCurrentItemState() };
	++currentTurn;
	return true;
}

bool CraftingSession::LoadCraftingTurn(uint8_t turn) {
	player.LoadPlayerState(craftState[turn].playerState);
	item.LoadItemState(craftState[turn].itemState);
	currentTurn = turn;
	return true;
}