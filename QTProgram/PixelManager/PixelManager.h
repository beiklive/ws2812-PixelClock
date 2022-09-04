#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PixelManager.h"

class PixelManager : public QMainWindow
{
    Q_OBJECT

public:
    PixelManager(QWidget *parent = Q_NULLPTR);

private:
    Ui::PixelManagerClass ui;
};
