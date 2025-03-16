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
	if (PreCraftSmartLogic(craftingManager, skill.name) && craftingManager.CraftingTurn(skill)) {
		PostCraftSmartLogic(craftingManager, skill.name);
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
bool SmartCrafter::PreCraftSmartLogic(const CraftingSession& craftingManager, const Skills::SkillName skillName) const {
	const PlayerState& player = craftingManager.GetPlayer().GetCurrentPlayerState();
	const Item& item = craftingManager.GetItem();

	/* No point in adding quality when the item is maximum quality */	// TODO Remove by just using skill information type
	/*if (item.IsItemMaxQuality() && IsTouchAction(skillName) && skillName != Skills::SkillName::DELICATESYNTHESIS) {
		return false;
	}*/
	
	/* Great Strides is about to run out without having been used */
	if (player.buffs[Buffs::GREATSTRIDES] == 1 && !IsTouchAction(skillName)) {
		return false;
	}
	/* Innovation buff is about to run out and wasn't used at all */
	if (player.buffs[Buffs::INNOVATION] == 1 && !craftingManager.GetTouchUsedDuringInnovation() && !IsTouchAction(skillName)) {
		return false;
	}
	/* Veneration buff is about to run out and wasn't used at all */
	else if (player.buffs[Buffs::VENERATION] == 1 && !craftingManager.GetSynthesisUsedDuringVeneration() && !IsSynthesisAction(skillName)) {	// else as Innovation and Veneration cannot be on the same buff timer
		return false;
	}
	/* Final Appraisal buff is about to run out and the item hasn't reached appraisal or the move isn't a synthesis move */
	if (player.buffs[Buffs::FINALAPPRAISAL] == 1 && (!IsSynthesisAction(skillName) || !item.IsItemAppraised())) {
		return false;
	}

	switch (skillName) {
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
		return !(player.buffs[Buffs::INNOVATION] > 0 && !craftingManager.GetTouchUsedDuringInnovation());		// False if a touch skill hasn't been used during the current innovation time
	case Skills::SkillName::VENERATION:
		return !(player.buffs[Buffs::VENERATION] > 0 && !craftingManager.GetSynthesisUsedDuringVeneration());	// False if a synthesis skill hasn't been used during the current veneration time
	case Skills::SkillName::FINALAPPRAISAL:
		return !craftingManager.GetFinalAppraisalUsed();					// False if Final Appraisal has been used

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

/* Applies logic and updates logic states if there is a reason to even attempt a craft */
void SmartCrafter::PostCraftSmartLogic(CraftingSession& craftingManager, const Skills::SkillName skillName) {
	InnovationLogicControl(craftingManager, skillName);
	VenerationLogicControl(craftingManager, skillName);
	if (skillName == Skills::SkillName::FINALAPPRAISAL) {
		craftingManager.SetFinalAppraisalUsed();
	}
}

bool SmartCrafter::IsSynthesisAction(const Skills::SkillName skill) const {
	switch (skill) {
	case Skills::SkillName::BASICSYNTHESIS:
	case Skills::SkillName::CAREFULSYNTHESIS:
	case Skills::SkillName::PRUDENTSYNTHESIS:
	case Skills::SkillName::GROUNDWORK:
	case Skills::SkillName::MUSCLEMEMORY:
	case Skills::SkillName::DELICATESYNTHESIS:			// Unique case that requires this function
		return true;
	default:
		return false;
	}
}

bool SmartCrafter::IsTouchAction(const Skills::SkillName skill) const {
	switch (skill) {
	case Skills::SkillName::BASICTOUCH:
	case Skills::SkillName::STANDARDTOUCH:
	case Skills::SkillName::ADVANCEDTOUCH:
	case Skills::SkillName::BYREGOTSBLESSING:
	case Skills::SkillName::PRUDENTTOUCH:
	case Skills::SkillName::PREPARATORYTOUCH:
	case Skills::SkillName::REFLECT:
	case Skills::SkillName::TRAINEDFINESSE:
	case Skills::SkillName::REFINEDTOUCH:
	case Skills::SkillName::DELICATESYNTHESIS:			// Unique case that requires this function
		return true;
	default:
		return false;
	}
}

void SmartCrafter::InnovationLogicControl(CraftingSession& craftingManager, const Skills::SkillName skillName) {
	if (skillName == Skills::SkillName::INNOVATION) {
		craftingManager.SetTouchUsedDuringInnovation(false);
		return;
	}

	if (craftingManager.GetPlayer().GetCurrentPlayerState().buffs[Buffs::INNOVATION] > 0) {
		if (!craftingManager.GetTouchUsedDuringInnovation()) {
			craftingManager.SetTouchUsedDuringInnovation(IsTouchAction(skillName));
		}
	}
	else {
		craftingManager.SetTouchUsedDuringInnovation(false);
	}
}

void SmartCrafter::VenerationLogicControl(CraftingSession& craftingManager, const Skills::SkillName skillName) {
	if (skillName == Skills::SkillName::VENERATION) {
		craftingManager.SetSynthesisUsedDuringVeneration(false);
		return;
	}

	if (craftingManager.GetPlayer().GetCurrentPlayerState().buffs[Buffs::VENERATION] > 0) {
		if (!craftingManager.GetSynthesisUsedDuringVeneration()) {
			craftingManager.SetSynthesisUsedDuringVeneration(IsSynthesisAction(skillName));
		}
	}
	else {
		craftingManager.SetSynthesisUsedDuringVeneration(false);
	}
}