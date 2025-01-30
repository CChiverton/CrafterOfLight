#pragma once

#include <QtWidgets/QWidget>
#include "ui_CrafterOfLight.h"
#include <vector>
#include "Skills.h"

class CrafterOfLight : public QWidget
{
    Q_OBJECT

public:
    CrafterOfLight(QWidget *parent = nullptr);
    ~CrafterOfLight();

private slots:
    void BruteCraft();
    void SmartCraft();
    void ToggleCraftingSkills();

private:
    Ui::CrafterOfLightClass ui;

    std::vector<Skills::SkillInformation> UserSkillSelection();
    void SetQualitySkills(bool state);
    void DeleteMacros();
};
