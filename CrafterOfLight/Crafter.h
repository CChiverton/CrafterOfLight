#pragma once

#include "CraftingSession.h"
#include <vector>
#include <map>

struct CraftingOptions {
	uint8_t maxTurnLimit = 1;
	bool maxQualityRequired = false;
};

class Crafter {
public:
	Crafter(CraftingOptions craftingOptions, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState);
	~Crafter();

	inline const uint8_t GetMaximumTurns() const;
	const std::string GetSkillSelection() const;
	inline const uint8_t GetBestCraftTime() const;
	std::string GetSolution() const;

	void Debug_VerifyCrafts();
protected:
	inline bool IsSolved() const;
	inline void AddSolution();

	CraftingSession craftingManager;
	CraftingOptions craftingOptions;
	std::vector<Skills::SkillInformation> skillSelection;
	std::map<uint8_t, std::vector<std::vector<Skills::SkillName>>> solutions;
	uint8_t bestCraftTime = 255;
	
};

inline const uint8_t Crafter::GetMaximumTurns() const {
	return craftingOptions.maxTurnLimit;
}

inline bool Crafter::IsSolved() const {
	return craftingManager.GetItem().IsItemCrafted() && (craftingOptions.maxQualityRequired && craftingManager.GetItem().IsItemMaxQuality());
}

/* Note: Assumes that the final turn has been added to the crafting history */
inline void Crafter::AddSolution() {
	bestCraftTime = craftingManager.GetCraftingSessionDuration();
	std::array<CraftingSession::CraftState, 60> history = craftingManager.GetCurrentCraftingHistory();
	std::vector<Skills::SkillName> solution{};

	for (uint8_t i{ 1 }; i < craftingManager.GetCraftingSessionTurn(); ++i) {
		solution.emplace_back(history[i].skillName);
	}
	solutions[bestCraftTime].emplace_back(solution);
}

inline const uint8_t Crafter::GetBestCraftTime() const {
	return bestCraftTime;
}