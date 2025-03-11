#pragma once

#include "Crafter.h"

class SmartCrafter : public Crafter {
public:
	SmartCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~SmartCrafter();

private:
	void CraftingSolution(CraftingSession& craftingManager, const Skills::SkillInformation& skill) override;
	void SmartSolveConditions(CraftingSession& craftingManager);
	bool SmartLogic(const CraftingSession& craftingManager, const Skills::SkillName skill);
};