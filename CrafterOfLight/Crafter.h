#pragma once

#include "CraftingSession.h"
#include <vector>
#include <map>

struct CraftingOptions {
	uint8_t maxTurnLimit = 1;
	bool maxQualityRequired = false;
};

class Crafter : public QObject {

	Q_OBJECT

public:
	Crafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~Crafter();

	virtual void Solve();
	inline const uint8_t GetMaximumTurns() const;
	const std::string GetSkillSelection() const;
	inline const uint8_t GetBestCraftTime() const;
	std::vector<std::vector<Skills::SkillName>> GetSolution() const;
	inline uint64_t GetRemainingCasts() const;

	void Debug_VerifyCrafts();

signals:
	void ResultReady(const std::vector<std::vector<Skills::SkillName>> &result, uint8_t bestCraftTime);
	void RemainingCrafts(uint64_t remainingCrafts);
	void Finished();

public slots:
	void EmitRemainingCrafts();

protected:
	void ThreadedSolving(int threadCount);
	virtual void ThreadedSolution(CraftingSession& craftingManager) = 0;
	inline bool IsSolved(const CraftingSession& craftingManager) const;
	inline void AddSolution(const CraftingSession& craftingManager);

	const uint16_t progressPerHundred, qualityPerHundred;
	const PlayerState maxPlayerState;
	const ItemState maxItemState;
	const CraftingOptions craftingOptions;
	const std::vector<Skills::SkillInformation> skillSelection;
	std::map<uint8_t, std::vector<std::vector<Skills::SkillName>>> solutions;
	uint8_t bestCraftTime = 255;
	std::vector<uint64_t> totalNumberOfCasts{};
	uint64_t remainingCasts = 0, totalCasts = 0;
	bool forceQuit = false;
	uint8_t threadsFinished = 0;
	std::mutex solutionMutex;

private:
	QTimer* progressUpdateTimer;


};

inline const uint8_t Crafter::GetMaximumTurns() const {
	return craftingOptions.maxTurnLimit;
}

inline bool Crafter::IsSolved(const CraftingSession& craftingManager) const {
	return craftingManager.GetItem().IsItemCrafted() && (craftingOptions.maxQualityRequired && craftingManager.GetItem().IsItemMaxQuality());
}

/* Note: Assumes that the final turn has been added to the crafting history */
inline void Crafter::AddSolution(const CraftingSession& craftingManager) {
	solutionMutex.lock();
	if (craftingManager.GetCraftingSessionDuration() > bestCraftTime) {		// Added safety in case of a multi-threaded conflict
		solutionMutex.unlock();
		return;
	}
	
	bestCraftTime = craftingManager.GetCraftingSessionDuration();
	const std::array<CraftingSession::CraftState, 30>& history = craftingManager.GetCurrentCraftingHistory();
	std::vector<Skills::SkillName> solution{};
	solution.reserve(craftingManager.GetCraftingSessionTurn());

	for (uint8_t i{ 1 }; i < craftingManager.GetCraftingSessionTurn(); ++i) {
		solution.emplace_back(history[i].skillName);
	}
	solutions[bestCraftTime].emplace_back(solution);
	solutionMutex.unlock();
}

inline const uint8_t Crafter::GetBestCraftTime() const {
	return bestCraftTime;
}

inline uint64_t Crafter::GetRemainingCasts() const {
	return remainingCasts;
}