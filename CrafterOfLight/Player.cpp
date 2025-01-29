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
	// Calculate efficiency, durability cost
	// Change player state 
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
			currentSkill.efficiency /= 2;
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
		/* Check buff status here */
		break;
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