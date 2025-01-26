#include "stdafx.h"
#include "CrafterOfLight.h"
#include "Player.h"

CrafterOfLight::CrafterOfLight(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_bruteCraft, &QPushButton::clicked, this, &CrafterOfLight::BruteCraft);
    connect(ui.pushButton_smartCraft, &QPushButton::clicked, this, &CrafterOfLight::SmartCraft);
}

CrafterOfLight::~CrafterOfLight()
{}

void CrafterOfLight::BruteCraft() {
    DeleteMacros();
    ui.spinBox_maxCP->setValue(15);
}

void CrafterOfLight::SmartCraft() {
    DeleteMacros();
    ui.gridLayout_macroOutput->addWidget(new QPushButton("1"), 0, 0);
    QLineEdit* option = new QLineEdit("This is an example");                
    option->setReadOnly(true);
    ui.gridLayout_macroOutput->addWidget(option, 0, 1);
    PlayerState state = { 10 };
    ItemState itemState = { 3000, 11000, 80 };
    Player player = Player(state, itemState);
    Item item = player.GetItemState();
    ui.spinBox_itemDurability->setValue(item.GetMaxDurability());
    ui.spinBox_itemProgress->setValue(item.GetMaxProgress());
    ui.spinBox_itemQuality->setValue(item.GetMaxQuality());
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