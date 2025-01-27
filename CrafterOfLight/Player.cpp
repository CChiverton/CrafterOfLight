#include "stdafx.h"

#include "Player.h"

Player::Player(PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred) :
	maxPlayerState(maxPlayerState),
	progressPerOneEfficiency(progressPerHundred/100.0f), qualityPerOneEfficiency(qualityPerHundred/100.0f),
	currentPlayerState(maxPlayerState) {}

Player::~Player() {}

void Player::CheckSpecialConditions(const Skills::SkillName skillName, uint16_t& skillCost, uint8_t& skillEfficiency, int16_t& skillDurabilityCost, const int16_t itemDurability) {
	switch (skillName) {
	case Skills::SkillName::GROUNDWORK:
		if (itemDurability < skillDurabilityCost) {
			skillEfficiency /= 2;
		}
		currentPlayerState.combo = false;
		break;
	case Skills::SkillName::STANDARDTOUCH:
		if (currentPlayerState.lastSkillUsed == Skills::SkillName::BASICTOUCH) {
			skillCost = 18;
			currentPlayerState.combo = true;
		}
		break;
	case Skills::SkillName::ADVANCEDTOUCH:
		if ((currentPlayerState.lastSkillUsed == Skills::SkillName::STANDARDTOUCH && currentPlayerState.combo == true) || currentPlayerState.lastSkillUsed == Skills::SkillName::OBSERVE) {
			skillCost = 18;
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

bool Player::CanCastSkill(const Skills::SkillName skillName, const uint16_t skillCost) {
	if (skillCost > currentPlayerState.cP) {
		return false;
	}

	switch (skillName) {
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