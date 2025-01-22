#pragma once

#include <QtWidgets/QWidget>
#include "ui_CrafterOfLight.h"

class CrafterOfLight : public QWidget
{
    Q_OBJECT

public:
    CrafterOfLight(QWidget *parent = nullptr);
    ~CrafterOfLight();

private:
    Ui::CrafterOfLightClass ui;
};
