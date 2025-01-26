#pragma once
#include <cstdint>
#include "Item.h"

struct PlayerState {
	uint16_t cP = 0;
};

class Player {
public:
	Player(PlayerState maxPlayerState, Item maxItemState);
	~Player();

	inline const Item GetItemState() const;

protected:
	PlayerState maxPlayerState, currentPlayerState;
	Item item;

	
};

inline const Item Player::GetItemState() const {
	return item;
}