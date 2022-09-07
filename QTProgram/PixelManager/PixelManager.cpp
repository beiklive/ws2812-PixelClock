#include "PixelManager.h"
PixelManager::PixelManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	
	ui.Color1->setStyleSheet("background-color:rgb(255,0,0);");
	ui.Color2->setStyleSheet("background-color:rgb(0,0,0);");


	ConnectionsInit();
}


void PixelManager::ConnectionsInit() {
    connect(ui.Color1, SIGNAL(clicked()), this, SLOT(SetLeftColor()));
    connect(ui.Color2, SIGNAL(clicked()), this, SLOT(SetRightColor()));
	
	connect(ui.SaveFileBtn, SIGNAL(clicked()), this, SLOT(SaveAsFile()));
	connect(ui.LoadFileBtn, SIGNAL(clicked()), this, SLOT(OpenFile()));
	
}
void PixelManager::SaveAsFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.json)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
			return;
		}
		
		QTextStream out(&file);
		out << ui.textEdit->toPlainText().toUtf8();
	}
}
void PixelManager::OpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.json)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName);
		qDebug() << "fileName" << fileName;
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
			return;
		}
		QTextStream in(&file);
		ui.textEdit->setText(in.readAll());
		qDebug() << "out----------------------";
		ui.widget->LoadColorMap(ui.textEdit->JsonToPixelMatrix());
	}
}
void PixelManager::SetLeftColor() {
	QColor color = QColorDialog::getColor(Qt::red, this);
	ui.Color1->setStyleSheet("background-color: " + color.name());
	ui.widget->SetLeftColor(color);	
}

void PixelManager::SetRightColor() {
	QColor color = QColorDialog::getColor(Qt::red, this);
	ui.Color2->setStyleSheet("background-color: " + color.name());
	ui.widget->SetRightColor(color);
}