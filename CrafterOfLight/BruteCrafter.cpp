#include "stdafx.h"

#include "BruteCrafter.h"

BruteCrafter::BruteCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState)
	: Crafter(craftingOptions, userSkills, maxPlayerState, progressPerHundred, qualityPerHundred, maxItemState) 
{};

BruteCrafter::~BruteCrafter() {};

/* Attempts to cast skill and updates the cast count */
void BruteCrafter::CraftingSolution(CraftingSession& craftingManager, const Skills::SkillInformation& skill) {
	--remainingCasts;
	if (craftingManager.CraftingTurn(skill)) {
		BruteSolveConditions(craftingManager);
	}
	else {
		craftingManager.totalCasts += totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
	}
}

/* Enacts the application of skill effects */
void BruteCrafter::RecursiveBruteSolve(CraftingSession& craftingManager) {
	for (const auto& skill : skillSelection) {
		if (forceQuit) {
			return;
		}

		CraftingSolution(craftingManager, skill);
	}
}

/* Determines the state of the item and handles the save state of the crafting chain */
void BruteCrafter::BruteSolveConditions(CraftingSession& craftingManager) {
	const Item& item = craftingManager.GetItem();
	
	if (item.IsItemCrafted()) {
		craftingManager.totalCasts += totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		if (!craftingOptions.maxQualityRequired || item.IsItemMaxQuality()) {
			/* Save and record solution */
			craftingManager.SaveCurrentCraftingTurn();
			AddSolution(craftingManager);
			/* Revert save and load state before getting in here */
			craftingManager.LoadLastCraftingTurn();
		}
		else {
			/* Undo the changes caused by this step*/
			craftingManager.ReloadCraftingTurn();
		}
	}
		/*		Item unworkable				Not enough time for a synth step, which is 3 seconds								This was the last turn		*/
	else if (item.IsItemBroken() || craftingManager.GetCraftingSessionDuration() > bestCraftTime - 3 || craftingManager.GetCraftingSessionTurn() >= craftingOptions.maxTurnLimit) {
		craftingManager.totalCasts += totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		/* Undo the changes caused by this step*/
		craftingManager.ReloadCraftingTurn();
	}
	else {
		/* Save and proceed to next step */
		craftingManager.SaveCurrentCraftingTurn();
		RecursiveBruteSolve(craftingManager);
		/* Revert to previous step */
		craftingManager.LoadLastCraftingTurn();
	}
}