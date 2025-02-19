#pragma once

#include "Crafter.h"

class BruteCrafter : public Crafter {
public:
	BruteCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~BruteCrafter();

	void Solve();
	void RecursiveBruteSolve();

private:
	void BruteSolveConditions();
};