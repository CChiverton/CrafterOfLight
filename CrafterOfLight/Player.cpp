#include "stdafx.h"

#include "Player.h"

Player::Player(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred) :
	maxPlayerState(maxPlayerState),
	progressPerOneEfficiency(progressPerHundred/100.0f), qualityPerOneEfficiency(qualityPerHundred/100.0f),
	currentPlayerState(maxPlayerState) {}

Player::~Player() {}

bool Player::CastSkill(const Skills::SkillName skillName) {
	if (currentSkill.name != skillName) {			// sanity check to test that the player is casting the right skill
		return false;
	}
	SkillEffect();
	// Update player state 
	// Return calculated efficiency, durability cost
}

bool Player::IsSkillCastable(const Skills::SkillInformation& skill, const int16_t itemDurability) {
	currentSkill = skill;
	CheckSpecialConditions(itemDurability); 
	return CanCastSkill();
}

// Maybe change touch skills to be default 18CP and increase if not combo? Will stop the CP check being late
void Player::CheckSpecialConditions(const int16_t itemDurability) {
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
		return currentPlayerState.turn = 1;
	case Skills:: SkillName::BYREGOTSBLESSING:
		return currentPlayerState.innerQuiet > 0;
	case Skills::SkillName::TRAINEDFINESSE:
		return currentPlayerState.innerQuiet = 10;
	case Skills::SkillName::TRAINEDPERFECTION:
		return !currentPlayerState.trainedPerfectionUsed;
	default:
		return true;	
	}

	return true;
}

void Player::SkillEffect() {
	if (currentPlayerState.buffs[TRAINEDPERFECTION] > 0) {
		currentSkill.costDurability = 0;
	} else if (currentPlayerState.buffs[WASTENOT] > 0) {
		currentSkill.costDurability /= 2;
	}
	switch (currentSkill.name) {
	/* Synthesis skills */
	case Skills::SkillName::BASICSYNTHESIS:
	case Skills::SkillName::CAREFULSYNTHESIS:
	case Skills::SkillName::PRUDENTSYNTHESIS:
	case Skills::SkillName::GROUNDWORK:
		// synthesis skill cast
		break;
	case Skills::SkillName::MUSCLEMEMORY:
		// synthesis skill cast
		currentPlayerState.buffs[Buffs::MUSCLEMEMORY] = 6;
		break;

	/* Touch skills */
	case Skills::SkillName::BASICTOUCH:
	case Skills::SkillName::STANDARDTOUCH:
	case Skills::SkillName::ADVANCEDTOUCH:
		// touch skills cast
		// add inner quiet
		break;
	case Skills::SkillName::BYREGOTSBLESSING:
		currentSkill.touchEfficiency += currentPlayerState.innerQuiet * 20;
		// touch skills cast
		break;
	case Skills::SkillName::PRUDENTTOUCH:
		// touch skills cast
		// add inner quiet
		break;
	case Skills::SkillName::PREPARATORYTOUCH:
		// touch skills cast
		// add 2 inner quiet
		break;
	case Skills::SkillName::REFLECT:
		// touch skills cast
		// add 2 inner quiet
		break;
	case Skills::SkillName::TRAINEDFINESSE:
		// touch skills cast
		break;

	case Skills::SkillName::REFINEDTOUCH:
		// touch skills cast
		// add inner quiet (2 if combo)
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
		// synthesis skill
		// touch skill
		// add inner quiet
	case Skills::SkillName::NONE:
		break;
	default:
		break;
	}
}