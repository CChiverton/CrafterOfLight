#include "stdafx.h"

#include "SmartCrafter.h"

SmartCrafter::SmartCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState)
	: Crafter(craftingOptions, userSkills, maxPlayerState, progressPerHundred, qualityPerHundred, maxItemState)
{
};

SmartCrafter::~SmartCrafter() {};

/* Attempts to cast skill and updates the cast count */
void SmartCrafter::CraftingSolution(CraftingSession& craftingManager, const Skills::SkillInformation& skill) {
	--remainingCasts;
	if (craftingManager.CraftingTurn(skill)) {
		SmartSolveConditions(craftingManager);
	}
	else {
		craftingManager.totalCasts += totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
	}
}

/* Determines the state of the item and handles the save state of the crafting chain */
void SmartCrafter::SmartSolveConditions(CraftingSession& craftingManager) {
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
		RecursiveSolve(craftingManager);
		/* Revert to previous step */
		craftingManager.LoadLastCraftingTurn();
	}
}