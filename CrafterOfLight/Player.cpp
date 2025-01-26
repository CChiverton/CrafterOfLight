#include "stdafx.h"

#include "Player.h"

Player::Player(PlayerState maxPlayerState, Item maxItemState) :
	maxPlayerState(maxPlayerState),
	currentPlayerState(maxPlayerState),
	item(maxItemState) {}

Player::~Player() {}