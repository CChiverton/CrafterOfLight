#include "stdafx.h"

#include "Crafter.h"

Crafter::Crafter(uint8_t maxTurns, std::vector<Skills::SkillInformation> userSkills, PlayerState maxPlayerState, uint16_t progressPerHundred, uint16_t qualityPerHundred, ItemState maxItemState)
: maxTurnLimit(maxTurns), skillSelection(userSkills),
craftingManager(maxPlayerState, progressPerHundred, qualityPerHundred, maxItemState) {}

Crafter::~Crafter() {}

const std::string Crafter::GetSkillSelection() const {
	std::string output = "";
	for (const auto& skill : skillSelection) {
		output.append(Skills::GetSkillName(skill.name));
		output.append(", ");
	}

	return output;
}