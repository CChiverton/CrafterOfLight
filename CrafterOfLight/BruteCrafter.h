#pragma once

#include "Crafter.h"

class BruteCrafter : public Crafter {
public:
	BruteCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~BruteCrafter();

	void Solve();
	

private:
	void ThreadedSolution(CraftingSession& craftingManager);
	void RecursiveBruteSolve(CraftingSession& craftingManager);
	void BruteSolveConditions(CraftingSession& craftingManager);

	uint8_t skillSelectionCounter = 0;
	std::mutex skillSelectionMutex;
	uint8_t threadsFinished = 0;
};