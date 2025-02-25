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

	inline const PlayerState& GetCurrentPlayerState() const;
	inline void LoadPlayerState(PlayerState& playerState);
	inline const float GetProgressEfficiency() const;
	inline const float GetQualityEfficiency() const;

	const Skills::SkillInformation& CastSkill();
	bool IsSkillCastable(const Skills::SkillInformation& skill, const int16_t itemDurability);

private:
	inline void CheckSpecialConditions(const int16_t itemDurability);
	bool CanCastSkill();
	inline void SkillEffect();
	inline void UpdatePlayerState();
	void SynthesisBuffs();
	void TouchBuffs(const uint8_t innerQuietStacks);
	inline void ApplyInnerQuiet();
	inline void AddInnerQuiet(const uint8_t stacks);

protected:
	PlayerState maxPlayerState, currentPlayerState;
	const float progressPerOneEfficiency, qualityPerOneEfficiency;

private:
	Skills::SkillInformation currentSkill = { Skills::SkillName::NONE,	Skills::SkillType::NONE, 0,	0, 0, 0, 0 };
	
};

inline const PlayerState& Player::GetCurrentPlayerState() const {
	return currentPlayerState;
}

inline void Player::LoadPlayerState(PlayerState& playerState) {
	currentPlayerState = playerState;
}

inline const float Player::GetProgressEfficiency() const {
	return progressPerOneEfficiency;
}

inline const float Player::GetQualityEfficiency() const {
	return qualityPerOneEfficiency;
}


// Maybe change touch skills to be default 18CP and increase if not combo? Will stop the CP check being late
inline void Player::CheckSpecialConditions(const int16_t itemDurability) {
	if (currentPlayerState.buffs[TRAINEDPERFECTION] > 0 && currentSkill.costDurability != 0) {
		currentSkill.costDurability = 0;
	}
	switch (currentSkill.name) {
	case Skills::SkillName::GROUNDWORK:
		if (itemDurability < currentSkill.costDurability) {
			currentSkill.synthesisEfficiency /= 2;
		}
		currentPlayerState.combo = false;
		break;
	case Skills::SkillName::STANDARDTOUCH:
		if (currentPlayerState.lastSkillUsed == Skills::SkillName::BASICTOUCH) {
			currentSkill.costCP = 18;
			currentPlayerState.combo = true;
		}
		break;
	case Skills::SkillName::ADVANCEDTOUCH:
		if ((currentPlayerState.lastSkillUsed == Skills::SkillName::STANDARDTOUCH && currentPlayerState.combo == true) || currentPlayerState.lastSkillUsed == Skills::SkillName::OBSERVE) {
			currentSkill.costCP = 18;
		}
		currentPlayerState.combo = false;
		break;
	case Skills::SkillName::REFINEDTOUCH:
		if (currentPlayerState.lastSkillUsed == Skills::SkillName::BASICTOUCH) {
			currentPlayerState.combo = true;
		}
		break;
	default:
		currentPlayerState.combo = false;
		break;
	}
}

/*
Player applies immediate effect before acting on the item
*/
void Player::SkillEffect() {
	if (currentPlayerState.buffs[WASTENOT] > 0) {
		currentSkill.costDurability /= 2;
	}
	switch (currentSkill.name) {
		/* Synthesis skills */
	case Skills::SkillName::BASICSYNTHESIS:
	case Skills::SkillName::CAREFULSYNTHESIS:
	case Skills::SkillName::PRUDENTSYNTHESIS:
	case Skills::SkillName::GROUNDWORK:
		SynthesisBuffs();
		break;
	case Skills::SkillName::MUSCLEMEMORY:
		SynthesisBuffs();
		currentPlayerState.buffs[Buffs::MUSCLEMEMORY] = 6;
		break;

		/* Touch skills */
	case Skills::SkillName::BASICTOUCH:
	case Skills::SkillName::STANDARDTOUCH:
	case Skills::SkillName::ADVANCEDTOUCH:
		TouchBuffs(1);
		break;
	case Skills::SkillName::BYREGOTSBLESSING:
		currentSkill.touchEfficiency += currentPlayerState.innerQuiet * 20;
		TouchBuffs(0);
		currentPlayerState.innerQuiet = 0;
		break;
	case Skills::SkillName::PRUDENTTOUCH:
		TouchBuffs(1);
		break;
	case Skills::SkillName::PREPARATORYTOUCH:
	case Skills::SkillName::REFLECT:
		TouchBuffs(2);
		break;
	case Skills::SkillName::TRAINEDFINESSE:
		TouchBuffs(0);
		break;
	case Skills::SkillName::REFINEDTOUCH:
		if (currentPlayerState.combo) {
			TouchBuffs(2);
		}
		else {
			TouchBuffs(1);
		}
		break;

		/* Buff skills */
	case Skills::SkillName::WASTENOTI:
		currentPlayerState.buffs[WASTENOT] = 5;
		break;
	case Skills::SkillName::WASTENOTII:
		currentPlayerState.buffs[WASTENOT] = 9;
		break;
	case Skills::SkillName::GREATSTRIDES:
		currentPlayerState.buffs[GREATSTRIDES] = 4;
		break;
	case Skills::SkillName::INNOVATION:
		currentPlayerState.buffs[INNOVATION] = 5;
		break;
	case Skills::SkillName::VENERATION:
		currentPlayerState.buffs[VENERATION] = 5;
		break;
	case Skills::SkillName::FINALAPPRAISAL:
		currentPlayerState.buffs[FINALAPPRAISAL] = 5;
		break;

		/* Repair skills */
	case Skills::SkillName::MASTERSMEND:
		break;
	case Skills::SkillName::MANIPULATION:
		currentPlayerState.buffs[MANIPULATION] = 9;
		break;
	case Skills::SkillName::IMMACULATEMEND:
		break;

		/* Other skills */
	case Skills::SkillName::TRAINEDPERFECTION:
		currentPlayerState.buffs[TRAINEDPERFECTION] = 255;
		currentPlayerState.trainedPerfectionUsed = true;
		break;
	case Skills::SkillName::OBSERVE:
		break;
	case Skills::SkillName::DELICATESYNTHESIS:
		SynthesisBuffs();
		TouchBuffs(1);
		break;
	case Skills::SkillName::NONE:
		break;
	default:
		break;
	}

	currentPlayerState.lastSkillUsed = currentSkill.name;
}

inline void Player::UpdatePlayerState() {
	currentPlayerState.cP -= currentSkill.costCP;
	++currentPlayerState.turn;

	if (currentSkill.name == Skills::SkillName::FINALAPPRAISAL) {
		return;
	}

	for (uint8_t& buff : currentPlayerState.buffs) {
		if (buff > 0) {
			--buff;
		}
	}
}

inline void Player::ApplyInnerQuiet() {
	currentSkill.touchEfficiency += (currentSkill.touchEfficiency / 10.0f) * currentPlayerState.innerQuiet;
}

inline void Player::AddInnerQuiet(const uint8_t stacks) {
	currentPlayerState.innerQuiet += stacks;
	if (currentPlayerState.innerQuiet > 10) {
		currentPlayerState.innerQuiet = 10;
	}
}