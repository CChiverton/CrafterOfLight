#pragma once

#include "Crafter.h"

class BruteCrafter : public Crafter {
public:
	BruteCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~BruteCrafter();	

private:
	void CraftingSolution(CraftingSession& craftingManager, const Skills::SkillInformation& skill) override;
	void BruteSolveConditions(CraftingSession& craftingManager);
};