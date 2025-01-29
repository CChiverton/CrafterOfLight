#pragma once
#include <cstdint>
#include "Item.h"
#include "Skills.h"

enum Buffs : int {
	MUSCLEMEMORY = 0,
	WASTENOT,
	GREATSTRIDES,
	INNOVATION,
	VENERATION,
	FINALAPPRAISAL,
	MANIPULATION,
	TRAINEDPERFECTION,
	NUMBUFF
};

struct PlayerState {
	uint16_t cP = 0;
	uint8_t innerQuiet = 0;
	bool trainedPerfectionUsed = false;
	std::array<uint8_t, Buffs::NUMBUFF> buffs{};
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

	bool CastSkill(const Skills::SkillName skillName);
	bool IsSkillCastable(const Skills::SkillInformation& skill, const int16_t itemDurability);

private:
	void CheckSpecialConditions(const int16_t itemDurability);
	bool CanCastSkill();
	void SkillEffect();

protected:
	PlayerState maxPlayerState, currentPlayerState;
	const float progressPerOneEfficiency, qualityPerOneEfficiency;

private:
	Skills::SkillInformation currentSkill = { Skills::SkillName::NONE,	Skills::SkillType::NONE, 0,	0, 0, 0, 0 };
	
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