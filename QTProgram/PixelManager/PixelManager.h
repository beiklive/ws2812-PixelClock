#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PixelManager.h"
#include<QColorDialog>
#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include "FormatJson.h"
class PixelManager : public QMainWindow
{
    Q_OBJECT

public:
    PixelManager(QWidget *parent = Q_NULLPTR);

private:
    Ui::PixelManagerClass ui;

private:
    void ConnectionsInit();
	
    

private:
    QColor background = Qt::black;
    QColor RightColor = background;
    QColor LeftColor = Qt::red;


public slots:
    void SetRightColor();
	void SetLeftColor();
    void SaveAsFile();
    void OpenFile();

};
