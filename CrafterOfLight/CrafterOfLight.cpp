#include "stdafx.h"
#include "CrafterOfLight.h"
#include "BruteCrafter.h"
#include <format>

CrafterOfLight::CrafterOfLight(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_bruteCraft, &QPushButton::clicked, this, &CrafterOfLight::BruteCraft);
    connect(ui.pushButton_smartCraft, &QPushButton::clicked, this, &CrafterOfLight::SmartCraft);
    connect(ui.checkBox_maxQuality, &QCheckBox::checkStateChanged, this, &CrafterOfLight::ToggleCraftingSkills);
    ToggleCraftingSkills();
}

CrafterOfLight::~CrafterOfLight()
{}

void CrafterOfLight::BruteCraft() {
    DeleteMacros();
    PlayerState state = { ui.spinBox_maxCP->value() };
    BruteCrafter bruteCrafter = BruteCrafter(UserCraftingOptions(), UserSkillSelection(), state, ui.spinBox_progress->value(), ui.spinBox_quality->value(), UserMaxItemState());
    bruteCrafter.RecursiveBruteSolve();
    ui.label_info->setText(QString::number(bruteCrafter.GetBestCraftTime()));
}

void CrafterOfLight::SmartCraft() {
    DeleteMacros();
    ui.gridLayout_macroOutput->addWidget(new QPushButton("1"), 0, 0);
    QLineEdit* option = new QLineEdit("This is an example");                
    option->setReadOnly(true);
    ui.gridLayout_macroOutput->addWidget(option, 0, 1);
    PlayerState state = { ui.spinBox_maxCP->value()};
    Crafter crafter = Crafter(UserCraftingOptions(), UserSkillSelection(), state, ui.spinBox_progress->value(), ui.spinBox_quality->value(), UserMaxItemState());
   /* Item item = crafter.GetItem();
    Player player = crafter.GetPlayer();
    PlayerState playerState = player.GetCurrentPlayerState();
    std::string output = std::format("Item Max Progress: {}, Quality: {}, Durability: {}\nPlayer CP: {}, ProgEff: {:3.2f}, QualEff: {:3.2f}\n Maximum Turns: {}",
        item.GetMaxProgress(), item.GetMaxQuality(), item.GetMaxDurability(), playerState.cP, player.GetProgressEfficiency(), player.GetQualityEfficiency(), crafter.GetMaximumTurns());
    ui.label_info->setText(QString::fromStdString(output));*/
    ui.label_info->setText(QString::fromStdString(crafter.GetSkillSelection()));
    crafter.Debug_VerifyCrafts();
}

void CrafterOfLight::ToggleCraftingSkills() {
    if (ui.checkBox_maxQuality->isChecked()) {
        SetQualitySkills(false);
    }
    else {
        SetQualitySkills(true);
    }
}

std::vector<Skills::SkillInformation> CrafterOfLight::UserSkillSelection() const {
    std::vector<Skills::SkillInformation> skills;
    if (ui.pushButton_basicSynthesis->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::BASICSYNTHESIS]);
    }
    if (ui.pushButton_carefulSynthesis->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::CAREFULSYNTHESIS]);
    }
    if (ui.pushButton_prudentSynthesis->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::PRUDENTSYNTHESIS]);
    }
    if (ui.pushButton_groundwork->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::GROUNDWORK]);
    }
    if (ui.pushButton_muscleMemory->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::MUSCLEMEMORY]);
    }
    if (ui.pushButton_basicTouch->isChecked() && ui.pushButton_basicTouch->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::BASICTOUCH]);
    }
    if (ui.pushButton_standardTouch->isChecked() && ui.pushButton_standardTouch->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::STANDARDTOUCH]);
    }
    if (ui.pushButton_advancedTouch->isChecked() && ui.pushButton_advancedTouch->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::ADVANCEDTOUCH]);
    }
    if (ui.pushButton_byregotsBlessing->isChecked() && ui.pushButton_byregotsBlessing->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::BYREGOTSBLESSING]);
    }
    if (ui.pushButton_prudentTouch->isChecked() && ui.pushButton_prudentTouch->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::PRUDENTTOUCH]);
    }
    if (ui.pushButton_preparatoryTouch->isChecked() && ui.pushButton_preparatoryTouch->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::PREPARATORYTOUCH]);
    }
    if (ui.pushButton_reflect->isChecked() && ui.pushButton_reflect->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::REFLECT]);
    }
    if (ui.pushButton_trainedFinesse->isChecked() && ui.pushButton_trainedFinesse->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::TRAINEDFINESSE]);
    }
    if (ui.pushButton_refinedTouch->isChecked() && ui.pushButton_refinedTouch->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::REFINEDTOUCH]);
    }
    if (ui.pushButton_wasteNotI->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::WASTENOTI]);
    }
    if (ui.pushButton_wasteNotII->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::WASTENOTII]);
    }
    if (ui.pushButton_greatStrides->isChecked() && ui.pushButton_greatStrides->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::GREATSTRIDES]);
    }
    if (ui.pushButton_innovation->isChecked() && ui.pushButton_innovation->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::INNOVATION]);
    }
    if (ui.pushButton_veneration->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::VENERATION]);
    }
    if (ui.pushButton_finalAppraisal->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::FINALAPPRAISAL]);
    }
    if (ui.pushButton_mastersMend->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::MASTERSMEND]);
    }
    if (ui.pushButton_manipulation->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::MANIPULATION]);
    }
    if (ui.pushButton_immaculateMend->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::IMMACULATEMEND]);
    }
    if (ui.pushButton_trainedPerfection->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::TRAINEDPERFECTION]);
    }
    if (ui.pushButton_observe->isChecked()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::OBSERVE]);
    }
    if (ui.pushButton_delicateSynthesis->isChecked() && ui.pushButton_delicateSynthesis->isEnabled()) {
        skills.emplace_back(Skills::SkillArray[(int)Skills::SkillName::DELICATESYNTHESIS]);
    }

    return skills;
}

CraftingOptions CrafterOfLight::UserCraftingOptions() const {
    return { (uint8_t)ui.spinBox_maximumTurns->value(), ui.checkBox_maxQuality->isChecked() };
}

ItemState CrafterOfLight::UserMaxItemState() const {
    return { (uint16_t)ui.spinBox_itemProgress->value(), (uint16_t)ui.spinBox_itemQuality->value(), (int16_t)ui.spinBox_itemDurability->value() };
}

void CrafterOfLight::DeleteMacros() {
    int row = 0;
    while (ui.gridLayout_macroOutput->count()) {
        QWidget* widgetButton = ui.gridLayout_macroOutput->itemAtPosition(row,0)->widget();
        QWidget* widgetText = ui.gridLayout_macroOutput->itemAtPosition(row, 1)->widget();
        if (widgetButton) {
            ui.gridLayout_macroOutput->removeWidget(widgetButton);
            delete widgetButton;
        }
        if (widgetText) {
            ui.gridLayout_macroOutput->removeWidget(widgetText);
            delete widgetText;
        }
        ++row;
    }
}

void CrafterOfLight::SetQualitySkills(bool state) {
    ui.pushButton_basicTouch->setDisabled(state);
    ui.pushButton_standardTouch->setDisabled(state);
    ui.pushButton_advancedTouch->setDisabled(state);
    ui.pushButton_byregotsBlessing->setDisabled(state);
    ui.pushButton_prudentTouch->setDisabled(state);
    ui.pushButton_preparatoryTouch->setDisabled(state);
    ui.pushButton_reflect->setDisabled(state);
    ui.pushButton_trainedFinesse->setDisabled(state);
    ui.pushButton_refinedTouch->setDisabled(state);
    ui.pushButton_greatStrides->setDisabled(state);
    ui.pushButton_innovation->setDisabled(state);
    ui.pushButton_delicateSynthesis->setDisabled(state);
}