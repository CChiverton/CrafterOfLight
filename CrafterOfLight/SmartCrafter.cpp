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
	if (SmartLogic(craftingManager, skill.name) && craftingManager.CraftingTurn(skill)) {
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

/* Applies a logic as to whether there is any point in attempting a craft */
bool SmartCrafter::SmartLogic(const CraftingSession& craftingManager, const Skills::SkillName skill) {
	const PlayerState& player = craftingManager.GetPlayer().GetCurrentPlayerState();
	const Item& item = craftingManager.GetItem();
	switch (skill) {
	/* Synthesis */
	case Skills::SkillName::BASICSYNTHESIS:
	case Skills::SkillName::CAREFULSYNTHESIS:
	case Skills::SkillName::PRUDENTSYNTHESIS:
	case Skills::SkillName::GROUNDWORK:
	case Skills::SkillName::MUSCLEMEMORY:
		return true;

	/* Touch */
	case Skills::SkillName::BASICTOUCH:
	case Skills::SkillName::STANDARDTOUCH:
	case Skills::SkillName::ADVANCEDTOUCH:
	case Skills::SkillName::BYREGOTSBLESSING:
	case Skills::SkillName::PRUDENTTOUCH:
	case Skills::SkillName::PREPARATORYTOUCH:
	case Skills::SkillName::REFLECT:
	case Skills::SkillName::TRAINEDFINESSE:
	case Skills::SkillName::REFINEDTOUCH:
		return true;

	/* Buffs */
	case Skills::SkillName::WASTENOTI:
		return player.buffs[Buffs::WASTENOT] != 4;						// False if cast last turn
	case Skills::SkillName::WASTENOTII:
		return player.buffs[Buffs::WASTENOT] != 8;						// False if cast last turn
	case Skills::SkillName::GREATSTRIDES:
		return player.buffs[Buffs::GREATSTRIDES] == 0;					// False if there is an active Great Strides buff
	case Skills::SkillName::INNOVATION:
		return player.buffs[Buffs::INNOVATION] != 4;					// False if cast last turn
	case Skills::SkillName::VENERATION:
		return player.buffs[Buffs::VENERATION] != 4;					// False if cast last turn
	case Skills::SkillName::FINALAPPRAISAL:
		return player.buffs[Buffs::FINALAPPRAISAL] == 0;				// False if there is an active Final Appraisal buff

	/* Repair */
	case Skills::SkillName::MASTERSMEND:
		return item.GetCurrentDurability() != item.GetMaxDurability();	// False if the item has full durability
	case Skills::SkillName::MANIPULATION:
		return player.buffs[Buffs::MANIPULATION] != 8;					// False if cast last turn
	case Skills::SkillName::IMMACULATEMEND:
		return item.GetCurrentDurability() != item.GetMaxDurability();	// False if the item has full durability

	/* Other */
	case Skills::SkillName::TRAINEDPERFECTION:
	case Skills::SkillName::OBSERVE:
	case Skills::SkillName::DELICATESYNTHESIS:
		return true;
	default:
		return true;
	}
}