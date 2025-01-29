#include "stdafx.h"

#include "Crafter.h"

Crafter::Crafter(uint8_t maxTurns, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState)
: maxTurnLimit(maxTurns), craftingManager(maxPlayerState, progressPerHundred, qualityPerHundred, maxItemState) {}

Crafter::~Crafter() {}