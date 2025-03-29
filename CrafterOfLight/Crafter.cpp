#include "stdafx.h"

#include "Crafter.h"

Crafter::Crafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState)
: craftingOptions(craftingOptions), skillSelection(userSkills), maxPlayerState(maxPlayerState),
progressPerHundred(progressPerHundred), qualityPerHundred(qualityPerHundred), maxItemState(maxItemState) {
	bestCraftTime = craftingOptions.maxTime;
	totalNumberOfCasts.emplace_back(0);
	for (uint8_t i{ 1 }; i <= craftingOptions.maxTurnLimit; ++i) {
		remainingCasts += std::pow(userSkills.size(), i);
		totalNumberOfCasts.insert(totalNumberOfCasts.begin(),  remainingCasts);
	}
	totalCasts = remainingCasts;
	progressUpdateTimer = new QTimer(this);
	connect(progressUpdateTimer, &QTimer::timeout, this, &Crafter::EmitRemainingCrafts);
	progressUpdateTimer->start(1000);
}

Crafter::~Crafter() {
	delete progressUpdateTimer;
}

/* Finds solutions and cleans up */
void Crafter::Solve() {
	ThreadedSolving(2);

	if (!forceQuit) {
		emit ResultReady(GetSolution(), bestCraftTime);
	}
	remainingCasts = 0;
	emit EmitRemainingCrafts();
	emit Finished();
}

/* Creates a comma-separated string of skills */
const std::string Crafter::GetSkillSelection() const {
	std::string output = "";
	for (const auto& skill : skillSelection) {
		output.append(Skills::GetSkillName(skill.name));
		output.append(", ");
	}

	return output;
}

/* Returns a vector of all the solutions of the fastest time */
std::vector<std::vector<Skills::SkillName>> Crafter::GetSolution() const {
	std::vector<std::vector<Skills::SkillName>> vectorOutput{};
	if (solutions.empty()) {
		return vectorOutput;
	}
	for (const auto& skillVector : solutions.at(bestCraftTime)) {
		vectorOutput.emplace_back(skillVector);
	}
	return vectorOutput;
}

/* Debug case to ensure that the crafting is working as expected */
void Crafter::Debug_VerifyCrafts() {
	PlayerState debugPlayer;
	debugPlayer.cP = 78;
	debugPlayer.innerQuiet = 0;
	debugPlayer.trainedPerfectionUsed = false;
	debugPlayer.lastSkillUsed = Skills::SkillName::NONE;
	debugPlayer.combo = false;
	debugPlayer.turn = 1;

	/* Check that the combo between basic touch, standard touch, and advanced touch is working */
	{
		CraftingSession craftingDebug = CraftingSession(debugPlayer, 100, 100, { 1000, 1000, 100 });

		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::BASICTOUCH]);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - 18);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::STANDARDTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100 + 137);							// inner quiet had worked and rounded down
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (18 + 18));		// combo has worked
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::ADVANCEDTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100 + 137 + 180);						// inner quiet has worked and rounded down
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (18 + 18 + 18));		// combo has worked
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::ADVANCEDTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100 + 137 + 180);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (18 + 18 + 18));
	}

	/* Verify that combo doesn't pass between the different states*/
	{
		CraftingSession craftingDebug = CraftingSession(debugPlayer, 100, 100, { 1000, 1000, 100 });

		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::BASICTOUCH]);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - 18);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::ADVANCEDTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100 + 165);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (18 + 46));		// combo has worked
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::ADVANCEDTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100 + 165);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (18 + 46));
	}

	/* Verify the refined touch combo works */
	{
		CraftingSession craftingDebug = CraftingSession(debugPlayer, 100, 100, { 1000, 1000, 100 });

		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::BASICTOUCH]);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - 18);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::REFINEDTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100 + 110);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (18 + 24));
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::BASICTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 100 + 110 + 130);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (18 + 24 + 18));
	}

	/* Verify the refined touch combo works */
	{
		CraftingSession craftingDebug = CraftingSession(debugPlayer, 100, 100, { 1000, 1000, 100 });

		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::STANDARDTOUCH]);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - 32);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 125);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::ADVANCEDTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 125 + 165);
		assert(craftingDebug.GetPlayer().GetCurrentPlayerState().cP == debugPlayer.cP - (32 + 46));
	}

	debugPlayer.cP = 1000;

	/* Verify inner quiet and repair works */
	{
		CraftingSession craftingDebug = CraftingSession(debugPlayer, 100, 100, { 1000, 1000, 100 });

		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::MASTERSMEND]);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
		craftingDebug.CraftingTurn(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
		assert(craftingDebug.GetItem().GetCurrentQuality() == 200 + 240 + 280 + 320 + 360 + 400 + 400);
		assert(craftingDebug.GetItem().GetCurrentDurability() == 100 - 20 - 20 - 20 - 20 + 30 - 20 - 20 - 20);
	}

}

/* Slot to emit a signal to update the UI on the remaining number of crafts */
void Crafter::EmitRemainingCrafts() {
	emit RemainingCrafts(remainingCasts);
}

/* Enacts the application of skill effects */
void Crafter::RecursiveSolve(CraftingSession& craftingManager) {
	for (const auto& skill : skillSelection) {
		if (forceQuit) {
			return;
		}

		CraftingSolution(craftingManager, skill);
	}
}

/* Creates and manages threads to find a solution */
void Crafter::ThreadedSolving(int threadCount) {
	std::vector<std::thread> threads;
	threads.reserve(threadCount);
	std::vector<CraftingSession> craftingManagers;
	craftingManagers.reserve(threadCount);		// Necessary to stop the address change with each new object added to the vector
	for (uint8_t i{ 0 }; i < threadCount; ++i) {
		craftingManagers.emplace_back(CraftingSession(maxPlayerState, progressPerHundred, qualityPerHundred, maxItemState));
		threads.emplace_back(std::thread(&Crafter::ThreadedSolution, this, std::ref(craftingManagers[i])));
	}

	while (!forceQuit && threadsFinished != threadCount) {
		uint64_t craftingManagerCasts{ 0 };
		for (const CraftingSession& manager : craftingManagers) {
			craftingManagerCasts += manager.totalCasts;
		}
		remainingCasts = totalCasts - craftingManagerCasts;
		qApp->processEvents();
		if (QThread::currentThread()->isInterruptionRequested()) {
			forceQuit = true;
		}
		QThread::currentThread()->msleep(250);
	}

	for (std::thread& thread : threads) {
		thread.join();
	}
}

/* Allows threads to "bunny hop" to the next available skill and follow that chain */
void Crafter::ThreadedSolution(CraftingSession& craftingManager) {
	for (; skillSelectionCounter < skillSelection.size();) {
		skillSelectionMutex.lock();
		uint8_t skill = skillSelectionCounter;
		++skillSelectionCounter;
		skillSelectionMutex.unlock();
		if (craftingOptions.forceTurnOneSkills && !(skillSelection[skill].name == Skills::SkillName::MUSCLEMEMORY || skillSelection[skill].name == Skills::SkillName::REFLECT)) {
			totalCasts -= totalNumberOfCasts[1];
			remainingCasts = totalCasts;
			continue;
		}
		if (forceQuit) {
			return;
		}

		CraftingSolution(craftingManager, skillSelection[skill]);
	}

	++threadsFinished;
}