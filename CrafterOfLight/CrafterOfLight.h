#pragma once

#include <QtWidgets/QWidget>
#include "ui_CrafterOfLight.h"
#include <vector>
#include "Skills.h"
#include "Crafter.h"

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

    std::vector<Skills::SkillInformation> UserSkillSelection() const;
    CraftingOptions UserCraftingOptions() const;
    ItemState UserMaxItemState() const;
    void SetQualitySkills(bool state);
    void DeleteMacros();
};
