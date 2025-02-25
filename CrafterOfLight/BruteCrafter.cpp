#include "stdafx.h"

#include "BruteCrafter.h"
#include <thread>

BruteCrafter::BruteCrafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState)
	: Crafter(craftingOptions, userSkills, maxPlayerState, progressPerHundred, qualityPerHundred, maxItemState) 
{};

BruteCrafter::~BruteCrafter() {};

void BruteCrafter::Solve() {
	std::thread threadOne(&BruteCrafter::ThreadedSolution, this, std::ref(craftingManagerOne));
	std::thread threadTwo(&BruteCrafter::ThreadedSolution, this, std::ref(craftingManagerTwo));

	while (!forceQuit && threadsFinished != 2) {
		qApp->processEvents();
		if (QThread::currentThread()->isInterruptionRequested()) {
			forceQuit = true;
		}
		QThread::currentThread()->msleep(250);
	}
	threadOne.join();
	threadTwo.join();

	Crafter::Solve();
}

/* Allows threads to "bunny hop" to the next available skill and follow that chain */
void BruteCrafter::ThreadedSolution(CraftingSession& craftingManager) {
	for (; skillSelectionCounter < skillSelection.size();) {
		skillSelectionMutex.lock();
		++skillSelectionCounter;
		uint8_t skill = skillSelectionCounter;
		skillSelectionMutex.unlock();
		if (forceQuit) {
			return;
		}
		
		--remainingCasts;
		if (craftingManager.CraftingTurn(skillSelection[skillSelectionCounter-1])) {
			BruteSolveConditions(craftingManager);
		}
		else {
			remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		}
	}

	++threadsFinished;
}

/* Enacts the application of skill effects */
void BruteCrafter::RecursiveBruteSolve(CraftingSession& craftingManager) {
	for (const auto& skill : skillSelection) {
		if (forceQuit) {
			return;
		}
		--remainingCasts;
		if (craftingManager.CraftingTurn(skill)) {
			BruteSolveConditions(craftingManager);
		}
		else {
			remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		}
	}
}

/* Determines the state of the item and handles the save state of the crafting chain */
void BruteCrafter::BruteSolveConditions(CraftingSession& craftingManager) {
	Item item = craftingManager.GetItem();
	
	if (item.IsItemCrafted()) {
		remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
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
		remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
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