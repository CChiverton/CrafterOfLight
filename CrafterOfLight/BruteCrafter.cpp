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
		if (forceQuit) {
			skillSelectionMutex.unlock();
			return;
		}
		--remainingCasts;
		if (craftingManager.CraftingTurn(skillSelection[skillSelectionCounter-1])) {
			skillSelectionMutex.unlock();
			BruteSolveConditions(craftingManager);
		}
		else {
			skillSelectionMutex.unlock();
			remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		}
	}

	++threadsFinished;
}

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
	craftingManager.LoadLastCraftingTurn();
}

void BruteCrafter::BruteSolveConditions(CraftingSession& craftingManager) {
	Item item = craftingManager.GetItem();
	if (item.IsItemCrafted()) {
		remainingCasts -= totalNumberOfCasts[craftingManager.GetCraftingSessionTurn()];
		if (!craftingOptions.maxQualityRequired || (craftingOptions.maxQualityRequired && item.IsItemMaxQuality())) {
			craftingManager.SaveCurrentCraftingTurn();		// Add the current turn to the history
			AddSolution(craftingManager);
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
		RecursiveBruteSolve(craftingManager);
	}
	
}