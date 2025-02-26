#include "stdafx.h"

#include "Player.h"

Player::Player(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred) :
	maxPlayerState(maxPlayerState),
	progressPerOneEfficiency(progressPerHundred/100.0f), qualityPerOneEfficiency(qualityPerHundred/100.0f),
	currentPlayerState(maxPlayerState) {}

Player::~Player() {}

/* Player applies the buffs to the current skill and themselves */
const Skills::SkillInformation& Player::CastSkill() {
	SkillEffect();
	UpdatePlayerState();
	return currentSkill;
}

/* Checks for any special conditions for the skill being used and applies them */
bool Player::IsSkillCastable(const Skills::SkillInformation& skill, const int16_t itemDurability) {
	/* 18 is the CP cost skill which can affect combos */
	if (skill.costCP >= 18 && currentPlayerState.cP < 18) {
		return false;
	}
	currentSkill = skill;
	CheckSpecialConditions(itemDurability); 
	return CanCastSkill();
}

bool Player::CanCastSkill() {
	if (currentSkill.costCP > currentPlayerState.cP) {
		return false;
	}

	switch (currentSkill.name) {
	case Skills::SkillName::PRUDENTSYNTHESIS:
	case Skills::SkillName::PRUDENTTOUCH:
		return currentPlayerState.buffs[Buffs::WASTENOT] == 0;
	case Skills::SkillName::MUSCLEMEMORY:
	case Skills::SkillName::REFLECT:
		return currentPlayerState.turn == 1;
	case Skills:: SkillName::BYREGOTSBLESSING:
		return currentPlayerState.innerQuiet > 0;
	case Skills::SkillName::TRAINEDFINESSE:
		return currentPlayerState.innerQuiet == 10;
	case Skills::SkillName::TRAINEDPERFECTION:
		return !currentPlayerState.trainedPerfectionUsed;
	default:
		return true;	
	}

	return true;
}

void Player::SynthesisBuffs() {
	currentSkill.synthesisEfficiency *= progressPerOneEfficiency;
	const uint16_t efficiency = currentSkill.synthesisEfficiency;
	if (currentPlayerState.buffs[MUSCLEMEMORY] > 0) {
		currentPlayerState.buffs[MUSCLEMEMORY] = 0;
		currentSkill.synthesisEfficiency += efficiency;
	}
	if (currentPlayerState.buffs[VENERATION] > 0) {
		currentSkill.synthesisEfficiency += efficiency/2;
	}

	currentPlayerState.buffs[TRAINEDPERFECTION] = 0;
}

void Player::TouchBuffs(const uint8_t innerQuietStacks) {
	ApplyInnerQuiet();
	currentSkill.touchEfficiency *= qualityPerOneEfficiency;
	const uint16_t efficiency = currentSkill.touchEfficiency;
	if (currentPlayerState.buffs[GREATSTRIDES] > 0) {
		currentPlayerState.buffs[GREATSTRIDES] = 0;
		currentSkill.touchEfficiency += efficiency;
	}
	if (currentPlayerState.buffs[INNOVATION] > 0) {
		currentSkill.touchEfficiency += efficiency/2;
	}

	AddInnerQuiet(innerQuietStacks);

	currentPlayerState.buffs[TRAINEDPERFECTION] = 0;
}