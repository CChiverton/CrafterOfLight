#pragma once
#include <cstdint>
#include "Item.h"
#include "Skills.h"

struct PlayerState {
	uint16_t cP = 0;
	uint8_t innerQuiet = 0;
	bool trainedPerfectionUsed = false;
	Skills::SkillName lastSkillUsed = Skills::SkillName::NONE;
	bool combo = false;
	uint8_t turn = 1;
};

class Player {
public:
	Player(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred);
	~Player();

	inline const PlayerState GetCurrentPlayerState() const;
	inline void LoadPlayerState(PlayerState playerState);
	inline const float GetProgressEfficiency() const;
	inline const float GetQualityEfficiency() const;

private:
	void CheckSpecialConditions(const Skills::SkillName skillName, uint16_t& skillCost, uint8_t& skillEfficiency, int16_t& skillDurabilityCost, const int16_t itemDurability);
	bool CanCastSkill(const Skills::SkillName skillName, const uint16_t skillCost);

protected:
	PlayerState maxPlayerState, currentPlayerState;
	const float progressPerOneEfficiency, qualityPerOneEfficiency;
};

inline const PlayerState Player::GetCurrentPlayerState() const {
	return currentPlayerState;
}

inline void Player::LoadPlayerState(PlayerState playerState) {
	currentPlayerState = playerState;
}

inline const float Player::GetProgressEfficiency() const {
	return progressPerOneEfficiency;
}

inline const float Player::GetQualityEfficiency() const {
	return qualityPerOneEfficiency;
}