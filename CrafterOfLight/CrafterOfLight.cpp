#include "stdafx.h"
#include "CrafterOfLight.h"
#include "BruteCrafter.h"
#include "SmartCrafter.h"
#include <format>
#include <thread>

CrafterOfLight::CrafterOfLight(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_bruteCraft, &QPushButton::clicked, this, &CrafterOfLight::BruteCraft);
    connect(ui.pushButton_smartCraft, &QPushButton::clicked, this, &CrafterOfLight::SmartCraft);
    connect(ui.checkBox_maxQuality, &QCheckBox::checkStateChanged, this, &CrafterOfLight::ToggleCraftingSkills);
    LoadJsonSettings();
    ToggleCraftingSkills();
}

/* Cleanup */
CrafterOfLight::~CrafterOfLight()
{
    crafterThread.requestInterruption();
    crafterThread.quit();
    crafterThread.wait();

    SaveJsonSettings();
}

/* "Dumb" crafting option with no tricks to optimisation */
void CrafterOfLight::BruteCraft() {
    DeleteMacros();
    if (!SessionSetup()) {
        return;
    }
    PlayerState state = { ui.spinBox_maxCP->value() };
    BruteCrafter* bruteCrafter = new BruteCrafter(UserCraftingOptions(), UserSkillSelection(), state, ui.spinBox_progress->value(), ui.spinBox_quality->value(), UserMaxItemState());
    progressBarCasts = bruteCrafter->GetRemainingCasts();
    ui.progressBar->setMaximum(100);
    ui.progressBar->setValue(0);
    bruteCrafter->moveToThread(&crafterThread);
    connect(&crafterThread, &QThread::finished, bruteCrafter, &QObject::deleteLater);
    connect(bruteCrafter, &BruteCrafter::Finished, bruteCrafter, &QObject::deleteLater);
    connect(this, &CrafterOfLight::FindSolution, bruteCrafter, &BruteCrafter::Solve);
    connect(bruteCrafter, &BruteCrafter::RemainingCrafts, this, &CrafterOfLight::UpdateProgressBar);
    connect(bruteCrafter, &BruteCrafter::ResultReady, this, &CrafterOfLight::HandleResults);

    crafterThread.start();
    emit FindSolution();
}

/* "Intelligent" crafting option that uses tricks and logic to attempt to reduce the total solving time */
void CrafterOfLight::SmartCraft() {
    DeleteMacros();
    if (!SessionSetup()) {
        return;
    }
    PlayerState state = { ui.spinBox_maxCP->value() };
    SmartCrafter* smartCrafter = new SmartCrafter(UserCraftingOptions(), UserSkillSelection(), state, ui.spinBox_progress->value(), ui.spinBox_quality->value(), UserMaxItemState());
    progressBarCasts = smartCrafter->GetRemainingCasts();
    ui.progressBar->setMaximum(100);
    ui.progressBar->setValue(0);
    smartCrafter->moveToThread(&crafterThread);
    connect(&crafterThread, &QThread::finished, smartCrafter, &QObject::deleteLater);
    connect(smartCrafter, &SmartCrafter::Finished, smartCrafter, &QObject::deleteLater);
    connect(this, &CrafterOfLight::FindSolution, smartCrafter, &SmartCrafter::Solve);
    connect(smartCrafter, &SmartCrafter::RemainingCrafts, this, &CrafterOfLight::UpdateProgressBar);
    connect(smartCrafter, &SmartCrafter::ResultReady, this, &CrafterOfLight::HandleResults);

    crafterThread.start();
    emit FindSolution();
}

/* Toggles the user selectable skills based on whether quality is something the user wishes to find */
void CrafterOfLight::ToggleCraftingSkills() {
    if (ui.checkBox_maxQuality->isChecked()) {
        SetQualitySkills(false);
    }
    else {
        SetQualitySkills(true);
    }
}

/* Copies the macro to the clipboard that corresponds to the vector of the found solutions */
void CrafterOfLight::SolutionButtonClicked() {
    QPushButton* widgetButton = qobject_cast<QPushButton*>(sender());
    if (widgetButton) {
        ui.label_info->setText("Copied to clipboard");
        QClipboard* clipboard = QGuiApplication::clipboard();
        clipboard->setText(QString::fromStdString(solutions[widgetButton->text().toInt() - 1]));
    }
}

/* Receives all the solutions from the crafter and makes changes to the UI to represent this */
void CrafterOfLight::HandleResults(const std::vector<std::vector<Skills::SkillName>> &results, uint8_t bestCraftTime) {
    if (results.size() == 0) {
        ui.label_info->setText(QString("No solutions found"));
        return;
    }
    solutions.clear();
    for (uint8_t i{ 0 }; i < results.size(); ++i) {
        QPushButton* button = new QPushButton(QString::number(i + 1));
        ui.gridLayout_macroOutput->addWidget(button, i, 0);
        solutions.emplace_back(CreateMacro(results[i]));
        connect(button, &QPushButton::clicked, this, &CrafterOfLight::SolutionButtonClicked);
    }
    ui.label_info->setText(QString("Best time: ") + QString::number(bestCraftTime) + QString(" seconds\n"));
    crafterThread.quit();
    crafterThread.wait();
}

/* Slot that changes the progress bar based on signal received from the crafter */
void CrafterOfLight::UpdateProgressBar(uint64_t remainingCasts) {
    ui.progressBar->setValue(float(progressBarCasts - remainingCasts)/progressBarCasts * 100);
    ui.progressBar->update();
    ++sessionTime;
    ui.label_sessionTime->setText(QString::number(sessionTime) + QString(" seconds"));
}

/* Adds skills to a vector to be passed along to the crafter. To be called only on creation of a crafter instance */
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

/* Returns the user crafting options from the UI */
CraftingOptions CrafterOfLight::UserCraftingOptions() const {
    return { (uint8_t)ui.spinBox_maximumTurns->value(), ui.checkBox_maxQuality->isChecked(), ui.checkBox_forceTurnOne->isChecked()};
}

/* Returns the item stats from the UI */
ItemState CrafterOfLight::UserMaxItemState() const {
    return { (uint16_t)ui.spinBox_itemProgress->value(), (uint16_t)ui.spinBox_itemQuality->value(), (int16_t)ui.spinBox_itemDurability->value() };
}

/* Deletes all the buttons and previously found solutions to make way for a new craft */
void CrafterOfLight::DeleteMacros() {
    int row = 0;
    while (ui.gridLayout_macroOutput->count()) {
        QWidget* widgetButton = ui.gridLayout_macroOutput->itemAtPosition(row,0)->widget();
        //QWidget* widgetText = ui.gridLayout_macroOutput->itemAtPosition(row, 1)->widget();
        if (widgetButton) {
            ui.gridLayout_macroOutput->removeWidget(widgetButton);
            delete widgetButton;
        }
       /* if (widgetText) {
            ui.gridLayout_macroOutput->removeWidget(widgetText);
            delete widgetText;
        }*/
        ++row;
    }
}

/* Sets up the UI and checks frees the resources required for solving */
bool CrafterOfLight::SessionSetup() {
    if (UserSkillSelection().size() == 0) {
        ui.label_info->setText(QString("Please select at least one skill"));
        return false;
    }
    if (crafterThread.isRunning()) {
        crafterThread.requestInterruption();
        crafterThread.quit();
        crafterThread.wait();
        ui.label_info->setText(QString("Restarting"));
    }
    sessionTime = 0;
    ui.label_sessionTime->setText(QString::number(sessionTime) + QString(" seconds"));
    return true;
}

/* Creates the text macro able to be pasted and used */
std::string CrafterOfLight::CreateMacro(std::vector<Skills::SkillName> skillList) {
    std::string output = "";
    for (const auto& skill : skillList) {
        output.append("/ac \"");
        output.append(Skills::GetSkillName(skill));
        output.append("\" <wait.");
        output.append(std::to_string(Skills::SkillArray[int(skill)].castTime));
        output.append(">\n");
    }
    return output;
}

/* Changes the clickable state of the quality skills */
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

void CrafterOfLight::SaveJsonSettings() {
    QJsonObject json;
    json["maxProgress"] = ui.spinBox_itemProgress->value();
    json["maxQuality"] = ui.spinBox_itemQuality->value();
    json["maxDurability"] = ui.spinBox_itemDurability->value();

    json["maxCP"] = ui.spinBox_maxCP->value();
    json["progress"] = ui.spinBox_progress->value();
    json["quality"] = ui.spinBox_quality->value();

    json["maxTurns"] = ui.spinBox_maximumTurns->value();
    json["findQuality"] = ui.checkBox_maxQuality->isChecked();
    json["turnOne"] = ui.checkBox_forceTurnOne->isChecked();

    json["basicSynthesis"] = ui.pushButton_basicSynthesis->isChecked();
    json["carefulSynthesis"] = ui.pushButton_carefulSynthesis->isChecked();
    json["prudentSynthesis"] = ui.pushButton_prudentSynthesis->isChecked();
    json["groundwork"] = ui.pushButton_groundwork->isChecked();
    json["muscleMemory"] = ui.pushButton_muscleMemory->isChecked();
    json["basicTouch"] = ui.pushButton_basicTouch->isChecked();
    json["standardTouch"] = ui.pushButton_standardTouch->isChecked();
    json["advancedTouch"] = ui.pushButton_advancedTouch->isChecked();
    json["byregot"] = ui.pushButton_byregotsBlessing->isChecked();
    json["prudentTouch"] = ui.pushButton_prudentTouch->isChecked();
    json["preparatoryTouch"] = ui.pushButton_preparatoryTouch->isChecked();
    json["reflect"] = ui.pushButton_reflect->isChecked();
    json["trainedFinesse"] = ui.pushButton_trainedFinesse->isChecked();
    json["refinedTouch"] = ui.pushButton_refinedTouch->isChecked();
    json["wasteNotI"] = ui.pushButton_wasteNotI->isChecked();
    json["wasteNotII"] = ui.pushButton_wasteNotII->isChecked();
    json["greatStrides"] = ui.pushButton_greatStrides->isChecked();
    json["innovation"] = ui.pushButton_innovation->isChecked();
    json["veneration"] = ui.pushButton_veneration->isChecked();
    json["finalAppraisal"] = ui.pushButton_finalAppraisal->isChecked();
    json["mastersMend"] = ui.pushButton_mastersMend->isChecked();
    json["manipulation"] = ui.pushButton_manipulation->isChecked();
    json["immaculateMend"] = ui.pushButton_immaculateMend->isChecked();
    json["trainedPerfection"] = ui.pushButton_trainedPerfection->isChecked();
    json["observe"] = ui.pushButton_observe->isChecked();
    json["delicateSynthesis"] = ui.pushButton_delicateSynthesis->isChecked();

    QFile settingsFile("settings.json");
    settingsFile.open(QIODevice::WriteOnly);
    settingsFile.write(QJsonDocument(json).toJson());
    settingsFile.close();
}

void CrafterOfLight::LoadJsonSettings() {
    QFileInfo checkFile("settings.json");
    if (checkFile.exists()) {
        QFile settingsFile("settings.json");
        settingsFile.open(QIODevice::ReadOnly);
        QJsonDocument json(QJsonDocument::fromJson(settingsFile.readAll()));

        ui.spinBox_itemProgress->setValue(json["maxProgress"].toInt());
        ui.spinBox_itemQuality->setValue(json["maxQuality"].toInt());
        ui.spinBox_itemDurability->setValue(json["maxDurability"].toInt());

        ui.spinBox_maxCP->setValue(json["maxCP"].toInt());
        ui.spinBox_progress->setValue(json["progress"].toInt());
         ui.spinBox_quality->setValue(json["quality"].toInt());

        ui.spinBox_maximumTurns->setValue(json["maxTurns"].toInt());
        ui.checkBox_maxQuality->setChecked(json["findQuality"].toBool());
        ui.checkBox_forceTurnOne->setChecked(json["turnOne"].toBool());

        ui.pushButton_basicSynthesis->setChecked(json["basicSynthesis"].toBool());
        ui.pushButton_carefulSynthesis->setChecked(json["carefulSynthesis"].toBool());
        ui.pushButton_prudentSynthesis->setChecked(json["prudentSynthesis"].toBool());
        ui.pushButton_groundwork->setChecked(json["groundwork"].toBool());
        ui.pushButton_muscleMemory->setChecked(json["muscleMemory"].toBool());
        ui.pushButton_basicTouch->setChecked(json["basicTouch"].toBool());
        ui.pushButton_standardTouch->setChecked(json["standardTouch"].toBool());
        ui.pushButton_advancedTouch->setChecked(json["advancedTouch"].toBool());
        ui.pushButton_byregotsBlessing->setChecked(json["byregot"].toBool());
        ui.pushButton_prudentTouch->setChecked(json["prudentTouch"].toBool());
        ui.pushButton_preparatoryTouch->setChecked(json["preparatoryTouch"].toBool());
        ui.pushButton_reflect->setChecked(json["reflect"].toBool());
        ui.pushButton_trainedFinesse->setChecked(json["trainedFinesse"].toBool());
        ui.pushButton_refinedTouch->setChecked(json["refinedTouch"].toBool());
        ui.pushButton_wasteNotI->setChecked(json["wasteNotI"].toBool());
        ui.pushButton_wasteNotII->setChecked(json["wasteNotII"].toBool());
        ui.pushButton_greatStrides->setChecked(json["greatStrides"].toBool());
        ui.pushButton_innovation->setChecked(json["innovation"].toBool());
        ui.pushButton_veneration->setChecked(json["veneration"].toBool());
        ui.pushButton_finalAppraisal->setChecked(json["finalAppraisal"].toBool());
        ui.pushButton_mastersMend->setChecked(json["mastersMend"].toBool());
        ui.pushButton_manipulation->setChecked(json["manipulation"].toBool());
        ui.pushButton_immaculateMend->setChecked(json["immaculateMend"].toBool());
        ui.pushButton_trainedPerfection->setChecked(json["trainedPerfection"].toBool());
        ui.pushButton_observe->setChecked(json["observe"].toBool());
        ui.pushButton_delicateSynthesis->setChecked(json["delicateSynthesis"].toBool());

        settingsFile.close();
    }
}