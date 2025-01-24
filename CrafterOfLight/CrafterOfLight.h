#pragma once

#include <QtWidgets/QWidget>
#include "ui_CrafterOfLight.h"

class CrafterOfLight : public QWidget
{
    Q_OBJECT

public:
    CrafterOfLight(QWidget *parent = nullptr);
    ~CrafterOfLight();

private slots:
    void BruteCraft();
    void SmartCraft();

private:
    Ui::CrafterOfLightClass ui;

    void DeleteMacros();
};
