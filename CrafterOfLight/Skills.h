#pragma once
#include <string>
#include <array>
/*
Skills added here are usable regardless of item condition or specialisation
*/
namespace Skills {

	const enum class SkillName {
		/* Synthesis skills */
		BASICSYNTHESIS = 0,
		CAREFULSYNTHESIS,
		PRUDENTSYNTHESIS,
		GROUNDWORK,
		MUSCLEMEMORY,

		/* Touch skills */
		BASICTOUCH,
		STANDARDTOUCH,
		ADVANCEDTOUCH,
		BYREGOTSBLESSING,
		PRUDENTTOUCH,
		PREPARATORYTOUCH,
		REFLECT,
		TRAINEDFINESSE,
		REFINEDTOUCH,

		/* Buff skills */
		WASTENOTI,
		WASTENOTII,
		GREATSTRIDES,
		INNOVATION,
		VENERATION,
		FINALAPPRAISAL,

		/* Repair skills */
		MASTERSMEND,
		MANIPULATION,
		IMMACULATEMEND,

		/* Other skills */
		TRAINEDPERFECTION,
		OBSERVE,
		DELICATESYNTHESIS,

		NONE
	};

	const enum class SkillType {
		SYNTHESIS = 0,
		TOUCH,
		BUFF,
		REPAIR,
		OTHER,
		NONE
	};

	std::string GetSkillName(SkillName skillName);
	
	const struct SkillInformation {
		SkillName	name;
		SkillType	type;
		uint8_t		costCP;
		int16_t		costDurability;
		uint16_t		synthesisEfficiency;
		uint16_t		touchEfficiency;
		uint8_t		castTime;
	};

	extern const std::array<SkillInformation, 26> SkillArray;
}
