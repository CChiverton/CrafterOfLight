#include "stdafx.h"

#include "BruteCrafter.h"

BruteCrafter::BruteCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState)
	: Crafter(craftingOptions, userSkills, maxPlayerState, progressPerHundred, qualityPerHundred, maxItemState) 
{};

BruteCrafter::~BruteCrafter() {};

void BruteCrafter::RecursiveBruteSolve() {
	for (const auto& skill : skillSelection) {
		--remainingCasts;
		if (craftingManager.CraftingTurn(skill)) {
			BruteSolveConditions();
		}
		else {
			remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		}
	}
	craftingManager.LoadLastCraftingTurn();
}

void BruteCrafter::BruteSolveConditions() {
	Item item = craftingManager.GetItem();
	if (item.IsItemCrafted()) {
		remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		if (!craftingOptions.maxQualityRequired || (craftingOptions.maxQualityRequired && item.IsItemMaxQuality())) {
			craftingManager.SaveCurrentCraftingTurn();		// Add the current turn to the history
			AddSolution();
			craftingManager.LoadLastCraftingTurn();
		}
		else {
			craftingManager.ReloadCraftingTurn();
		}
	}
	else if (item.IsItemBroken() || craftingManager.GetCraftingSessionDuration() >= bestCraftTime - 2 || craftingManager.GetCraftingSessionTurn() >= craftingOptions.maxTurnLimit) {
		remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		craftingManager.ReloadCraftingTurn();
	}
	else {
		craftingManager.SaveCurrentCraftingTurn();
		RecursiveBruteSolve();
	}
	
}