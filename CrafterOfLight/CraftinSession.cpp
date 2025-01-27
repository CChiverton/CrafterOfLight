#include "stdafx.h"
#include "CraftingSession.h"

CraftingSession::CraftingSession(uint8_t maxTurns,
	PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred,
	ItemState maxItemState)
	: maxCraftingTurns(maxTurns),
	player(Player(maxPlayerState, progressPerHundred, qualityPerHundred)), item(Item(maxItemState)) {
	SaveCraftingTurn(currentTurn);
}

CraftingSession::~CraftingSession() {}

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