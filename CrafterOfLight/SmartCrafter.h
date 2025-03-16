#pragma once

#include "Crafter.h"

class SmartCrafter : public Crafter {
public:
	SmartCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~SmartCrafter();

private:
	void CraftingSolution(CraftingSession& craftingManager, const Skills::SkillInformation& skill) override;
	void SmartSolveConditions(CraftingSession& craftingManager);
	bool PreCraftSmartLogic(const CraftingSession& craftingManager, const Skills::SkillName skillName) const;
	void PostCraftSmartLogic(CraftingSession& craftingManager, const Skills::SkillName skillName);

	bool IsSynthesisAction(const Skills::SkillName skill) const;
	bool IsTouchAction(const Skills::SkillName skill) const;

	void InnovationLogicControl(CraftingSession& craftingManager, const Skills::SkillName skillName);
	void VenerationLogicControl(CraftingSession& craftingManager, const Skills::SkillName skillName);
};