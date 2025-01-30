#pragma once

#include "CraftingSession.h"
#include <vector>
struct CraftingOptions {
	uint8_t maxTurnLimit = 1;
	bool maxQualityRequired = false;
};

class Crafter {
public:
	

	Crafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~Crafter();

	inline const uint8_t GetMaximumTurns() const;
	const std::string GetSkillSelection() const;


protected:
	CraftingSession craftingManager;
	CraftingOptions craftingOptions;
	std::vector<Skills::SkillInformation> skillSelection;
};

inline const uint8_t Crafter::GetMaximumTurns() const {
	return craftingOptions.maxTurnLimit;
}