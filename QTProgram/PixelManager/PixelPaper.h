#pragma once

#include <QWidget>
#include "ui_PixelPaper.h"
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>

using PixelMatrix = QVector<QVector<QColor>>;
using PixelMatrixQueue = QVector<PixelMatrix>;


class PixelPaper : public QWidget
{
	Q_OBJECT

public:
	PixelPaper(QWidget *parent = Q_NULLPTR);
	void PaperInit();
	~PixelPaper();

	//修改控件大小
	void resizeWidget();
	
	//刷新画布
	void DrawMap();
	void ResizeMap();
	//获取画布基本属性
	int GetWidth() const;
	int GetHeight() const;
	int GetStep() const;
	//数值转换
	int NumToPixel(int num);
	QPoint PixelToNum(QPoint mouse);
	//参数设置
	void SetStep(int step);
	void SetWidth(int width);
	void SetHeight(int heigth);
	void SetLeftColor(QColor);
	void SetRightColor(QColor);

	//容器操作
	void ClearAllColor();
	void LoadColorMap(PixelMatrix);
	PixelMatrix GetMap() const;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent*) override;

	bool m_isMouseDown{ false };
	bool m_isLButtonDown{ false };
private:
	Ui::PixelPaper ui;

	//画布初始属性
	int m_width = 32;
	int m_height = 8;
	int m_step = 20;

	//初始颜色
	QColor GridColor = Qt::gray;
	QColor BackColor = Qt::black;
	QColor LeftColor = Qt::red;
	QColor RightColor = BackColor;

	//容器
	PixelMatrix map{ m_width, QVector<QColor>(m_height, BackColor) };

public slots:

	void SetSize();
	void ChangeWidth(int num);
	void ChangeHeight(int num);
	void ChangeStep(int num);
signals:
	void DrawFinish(PixelMatrix e);
};
