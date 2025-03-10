#pragma once

#include "Crafter.h"

class BruteCrafter : public Crafter {
public:
	BruteCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~BruteCrafter();

	void Solve();
	

private:
	void CraftingSolution(CraftingSession& craftingManager, const Skills::SkillInformation& skill);
	void ThreadedSolution(CraftingSession& craftingManager) override;
	void RecursiveBruteSolve(CraftingSession& craftingManager);
	void BruteSolveConditions(CraftingSession& craftingManager);

	uint8_t skillSelectionCounter = 0;
	std::mutex skillSelectionMutex;
};