#pragma once

#include <QtWidgets/QWidget>
#include "ui_CrafterOfLight.h"
#include <vector>
#include "Skills.h"
#include "Crafter.h"

class CrafterOfLight : public QWidget
{
    Q_OBJECT
    QThread crafterThread;

public:
    CrafterOfLight(QWidget *parent = nullptr);
    ~CrafterOfLight();

public slots:
    void HandleResults(const std::vector<std::vector<Skills::SkillName>> &results, uint8_t bestCraftTime);
    void UpdateProgressBar(uint64_t remainingCasts);

signals:
    void FindSolution();

private slots:
    void BruteCraft();
    void SmartCraft();
    void ToggleCraftingSkills();
    


private:
    Ui::CrafterOfLightClass ui;

    std::vector<Skills::SkillInformation> UserSkillSelection() const;
    std::vector<std::string> solutions{};
    CraftingOptions UserCraftingOptions() const;
    ItemState UserMaxItemState() const;
    void SetQualitySkills(bool state);
    void DeleteMacros();
    std::string CreateMacro(std::vector<Skills::SkillName>);
    uint64_t progressBarCasts = 0;
};
