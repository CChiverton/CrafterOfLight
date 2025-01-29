#include "stdafx.h"
#include "CrafterOfLight.h"
#include "Crafter.h"
#include <format>

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
    PlayerState state = { ui.spinBox_maxCP->value()};
    ItemState itemState = { ui.spinBox_itemProgress->value(), ui.spinBox_itemQuality->value(), ui.spinBox_itemDurability->value()};
    Crafter crafter = Crafter(ui.spinBox_maximumTurns->value(), state, ui.spinBox_progress->value(), ui.spinBox_quality->value(), itemState);
   /* Item item = crafter.GetItem();
    Player player = crafter.GetPlayer();
    PlayerState playerState = player.GetCurrentPlayerState();
    std::string output = std::format("Item Max Progress: {}, Quality: {}, Durability: {}\nPlayer CP: {}, ProgEff: {:3.2f}, QualEff: {:3.2f}\n Maximum Turns: {}",
        item.GetMaxProgress(), item.GetMaxQuality(), item.GetMaxDurability(), playerState.cP, player.GetProgressEfficiency(), player.GetQualityEfficiency(), crafter.GetMaximumTurns());
    ui.label_info->setText(QString::fromStdString(output));*/
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