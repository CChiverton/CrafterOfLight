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

	inline bool IsQualitySkill(Skills::SkillName skillName) const;

	uint8_t FindMinimumQualitySteps(CraftingSession& craftingManager, const std::vector<Skills::SkillInformation>& qualitySkills, uint8_t maxSteps);
	uint8_t FindNumberOfQualitySkillsInSession(const CraftingSession& craftingManager) const;

	uint8_t minQualitySteps{ 0 };
};

inline bool SmartCrafter::IsQualitySkill(Skills::SkillName skillName) const {
	return IsTouchAction(skillName) || skillName == Skills::SkillName::INNOVATION || skillName == Skills::SkillName::GREATSTRIDES;
}