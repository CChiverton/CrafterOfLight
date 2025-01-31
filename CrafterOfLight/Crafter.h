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
	inline const uint8_t GetBestCraftTime() const;

	void Debug_VerifyCrafts();
protected:
	struct CraftTurn {
		Skills::SkillName name = Skills::SkillName::NONE;
		uint8_t time = 0;
	};

	inline bool IsSolved() const;

	CraftingSession craftingManager;
	CraftingOptions craftingOptions;
	std::vector<Skills::SkillInformation> skillSelection;
	CraftTurn currentCraftingTurn;
	uint8_t bestCraftTime = 255;
	
};

inline const uint8_t Crafter::GetMaximumTurns() const {
	return craftingOptions.maxTurnLimit;
}

inline bool Crafter::IsSolved() const {
	return craftingManager.GetItem().IsItemCrafted() && (craftingOptions.maxQualityRequired && craftingManager.GetItem().IsItemMaxQuality());
}

inline const uint8_t Crafter::GetBestCraftTime() const {
	return bestCraftTime;
}