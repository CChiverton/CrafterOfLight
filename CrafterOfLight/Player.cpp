#include "stdafx.h"

#include "Player.h"

Player::Player(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred) :
	maxPlayerState(maxPlayerState),
	progressPerOneEfficiency(progressPerHundred/100.0f), qualityPerOneEfficiency(qualityPerHundred/100.0f),
	currentPlayerState(maxPlayerState) {}

Player::~Player() {}