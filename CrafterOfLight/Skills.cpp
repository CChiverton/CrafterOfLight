#include "stdafx.h"
#include "Skills.h"

namespace Skills {

	std::string GetSkillName(SkillName skillName) {
		switch (skillName) {
		case SkillName::BASICSYNTHESIS:
			return "Basic Synthesis";
		case SkillName::CAREFULSYNTHESIS:
			return "Careful Synthesis";
		case SkillName::PRUDENTSYNTHESIS:
			return "Prudent Synthesis";
		case SkillName::GROUNDWORK:
			return "Groundwork";
		case SkillName::MUSCLEMEMORY:
			return "Muscle Memory";
		case SkillName::BASICTOUCH:
			return "Basic Touch";
		case SkillName::STANDARDTOUCH:
			return "Standard Touch";
		case SkillName::ADVANCEDTOUCH:
			return "Advanced Touch";
		case SkillName::BYREGOTSBLESSING:
			return "Byregot's Blessing";
		case SkillName::PRUDENTTOUCH:
			return "Prudent Touch";
		case SkillName::PREPARATORYTOUCH:
			return "Preparatory Touch";
		case SkillName::REFLECT:
			return "Reflect";
		case SkillName::TRAINEDFINESSE:
			return "Trained Finesse";
		case SkillName::REFINEDTOUCH:
			return "Refined Touch";
		case SkillName::WASTENOTI:
			return "Waste Not I";
		case SkillName::WASTENOTII:
			return "Waste Not II";
		case SkillName::GREATSTRIDES:
			return "Great Strides";
		case SkillName::INNOVATION:
			return "Innovation";
		case SkillName::VENERATION:
			return "Veneration";
		case SkillName::FINALAPPRAISAL:
			return "Final Appraisal";
		case SkillName::MASTERSMEND:
			return "Master's Mend";
		case SkillName::MANIPULATION:
			return "Manipulation";
		case SkillName::IMMACULATEMEND:
			return "Immaculate Mend";
		case SkillName::TRAINEDPERFECTION:
			return "Trained Perfection";
		case SkillName::OBSERVE:
			return "Observe";
		case SkillName::DELICATESYNTHESIS:
			return "Delicate Synthesis";
		case SkillName::NONE:
			return "None";
		default:
			return "Skill not found";
		}
		return "";
	}

	const std::array<SkillInformation, 26> SkillArray = { {			 /* CP	DURABILITY	SYNTH	TOUCH	TIME*/
		{SkillName::BASICSYNTHESIS,		SkillType::SYNTHESIS,			0,		10,		120,	0,		3	},
		{SkillName::CAREFULSYNTHESIS,	SkillType::SYNTHESIS,			7,		10,		180,	0,		3	},
		{SkillName::PRUDENTSYNTHESIS,	SkillType::SYNTHESIS,			18,		5,		180,	0,		3	},
		{SkillName::GROUNDWORK,			SkillType::SYNTHESIS,			18,		20,		360,	0,		3	},
		{SkillName::MUSCLEMEMORY,		SkillType::SYNTHESIS,			6,		10,		300,	0,		3	},
		{SkillName::BASICTOUCH,			SkillType::TOUCH,				18,		10,		0,		100,	3	},
		{SkillName::STANDARDTOUCH,		SkillType::TOUCH,				32,		10,		0,		125,	3	},
		{SkillName::ADVANCEDTOUCH,		SkillType::TOUCH,				46,		10,		0,		150,	3	},
		{SkillName::BYREGOTSBLESSING,	SkillType::TOUCH,				24,		10,		0,		100,	3	},
		{SkillName::PRUDENTTOUCH,		SkillType::TOUCH,				25,		5,		0,		100,	3	},
		{SkillName::PREPARATORYTOUCH,	SkillType::TOUCH,				40,		20,		0,		200,	3	},
		{SkillName::REFLECT,			SkillType::TOUCH,				6,		10,		0,		300,	3	},
		{SkillName::TRAINEDFINESSE,		SkillType::TOUCH,				32,		0,		0,		100,	3	},
		{SkillName::REFINEDTOUCH,		SkillType::TOUCH,				24,		10,		0,		100,	3	},
		{SkillName::WASTENOTI,			SkillType::BUFF,				56,		0,		0,		0,		2	},
		{SkillName::WASTENOTII,			SkillType::BUFF,				98,		0,		0,		0,		2	},
		{SkillName::GREATSTRIDES,		SkillType::BUFF,				32,		0,		0,		0,		2	},
		{SkillName::INNOVATION,			SkillType::BUFF,				18,		0,		0,		0,		2	},
		{SkillName::VENERATION,			SkillType::BUFF,				18,		0,		0,		0,		2	},
		{SkillName::FINALAPPRAISAL,		SkillType::BUFF,				1,		0,		0,		0,		2	},
		{SkillName::MASTERSMEND,		SkillType::REPAIR,				88,		-30,	0,		0,		3	},
		{SkillName::MANIPULATION,		SkillType::REPAIR,				96,		0,		0,		0,		2	},
		{SkillName::IMMACULATEMEND,		SkillType::REPAIR,				112,	-127,	0,		0,		3	},
		{SkillName::TRAINEDPERFECTION,	SkillType::OTHER,				0,	0	,		0,		0,		3	},
		{SkillName::OBSERVE,			SkillType::OTHER,				7,	0	,		0,		0,		3	},
		{SkillName::DELICATESYNTHESIS,	SkillType::OTHER,				32,		10,		150,	100,	3	}
	} };
}