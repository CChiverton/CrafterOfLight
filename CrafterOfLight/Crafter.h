#pragma once

#include "CraftingSession.h"
#include <vector>

class Crafter {
public:
	Crafter(uint8_t maxTurns, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~Crafter();

	inline const uint8_t GetMaximumTurns() const;
	const std::string GetSkillSelection() const;


protected:
	CraftingSession craftingManager;
	const uint8_t maxTurnLimit;
	std::vector<Skills::SkillInformation> skillSelection;
};

inline const uint8_t Crafter::GetMaximumTurns() const {
	return maxTurnLimit;
}