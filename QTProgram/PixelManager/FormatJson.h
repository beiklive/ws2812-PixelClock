#pragma once

#include <QWidget>
#include <QTextEdit>
#include "ui_FormatJson.h"
#include <QJsonObject>
#include <QJsonDocument>

//#include "PixelPaper.h"

#include <QVector>

using PixelMatrix = QVector<QVector<QColor>>;
using PixelMatrixQueue = QVector<PixelMatrix>;

class FormatJson : public QTextEdit
{
	Q_OBJECT

public:
	FormatJson(QWidget* parent = Q_NULLPTR);
	~FormatJson() = default;

	void InfoFormat();
	void SetTextEditPtr(FormatJson* ptr);
	//PixelMatrix JsonToPixelMatrix();
	void PixelMatrixToJson(PixelMatrix pixelMatrix);
private:
	Ui::FormatJson ui;
	FormatJson *m_textEdit;

	int m_width = 32;
	int m_height = 8;
	//int m_data[32*8];
	//int m_color[32*8][3];

public slots:
	void GetDrawFinish(PixelMatrix e);
};
