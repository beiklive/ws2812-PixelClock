#include "PixelPaper.h"

PixelPaper::PixelPaper(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	PaperInit();
}

void PixelPaper::PaperInit()
{
	// ³õÊ¼»¯»­²¼
	resizeWidget();
	SetSize();
	DrawMap();
}

PixelPaper::~PixelPaper()
{
}

void PixelPaper::resizeWidget()
{
	this->setFixedSize(QSize(GetWidth()*(GetStep()), GetHeight()*GetStep()));
}

void PixelPaper::SetStep(int step)
{
	m_step = step;
}

void PixelPaper::SetWidth(int width)
{
	m_width = width;
	SetSize();
}

void PixelPaper::SetHeight(int heigth)
{
	m_height = heigth;
	SetSize();
}

void PixelPaper::SetSize()
{
	PixelMatrix temp{ std::move(map) };
	map = PixelMatrix(GetWidth(), QVector<QColor>(GetHeight()));

	for (int i = 0; i < temp.length() && i < GetWidth(); ++i)
	{
		for (int j = 0; j < temp[i].length() && j < GetHeight(); ++j)
		{
			map[i][j] = temp[i][j];
		}
	}
	emit DrawFinish(map);
}

void PixelPaper::DrawMap()
{
	update();
	resizeWidget();
}

void PixelPaper::ResizeMap()
{
	DrawMap();
}

int PixelPaper::GetWidth() const
{
	return m_width;
}

int PixelPaper::GetHeight() const
{
	return m_height;
}

int PixelPaper::GetStep() const
{
	return m_step;
}

int PixelPaper::NumToPixel(int num)
{

	return num * GetStep();
}

QPoint PixelPaper::PixelToNum(QPoint mouse)
{
	auto x = mouse.x() / GetStep();
	auto y = mouse.y() / GetStep();
	return QPoint(x, y);
}

void PixelPaper::SetLeftColor(QColor c)
{
	LeftColor = c;
}

void PixelPaper::SetRightColor(QColor c)
{
	RightColor = c;
}

void PixelPaper::ClearAllColor()
{
	for (int i = 0; i < map.length() && i < GetWidth(); ++i)
	{
		for (int j = 0; j < map[i].length(); ++j)
		{
			map[i][j] = BackColor;
		}
	}
	update();
}

void PixelPaper::LoadColorMap(PixelMatrix e)
{
	for (int i = 0; i < e.length() && i < GetWidth(); ++i)
	{
		for (int j = 0; j < e[i].length(); ++j)
		{
			map[i][j] = e[i][j];
		}
	}
	update();
}

PixelMatrix PixelPaper::GetMap() const
{
	return map;
}


void PixelPaper::mousePressEvent(QMouseEvent* event)
{
	m_isMouseDown = true;
	m_isLButtonDown = event->button() == Qt::LeftButton;
	auto mousepos = PixelToNum(event->pos());
	if (mousepos.x() >= 0 && mousepos.x() < GetWidth() && mousepos.y() >= 0 && mousepos.y() < GetHeight())
	{
		if (m_isLButtonDown)
		{
			map[mousepos.x()][mousepos.y()] = LeftColor;
		}
		else
		{
			map[mousepos.x()][mousepos.y()] = RightColor;
		}
		update();
	}

}

void PixelPaper::mouseMoveEvent(QMouseEvent* event)
{
	auto mousepos = PixelToNum(event->pos());
	if (mousepos.x() >= 0 && mousepos.x() < GetWidth() && mousepos.y() >= 0 && mousepos.y() < GetHeight())
	{
		if (m_isMouseDown)
		{
			if (m_isLButtonDown)
			{
				map[mousepos.x()][mousepos.y()] = LeftColor;
			}
			else
			{
				map[mousepos.x()][mousepos.y()] = RightColor;
			}
			update();
		}
		else
		{
			return QWidget::mouseMoveEvent(event);
		}
	}

}

void PixelPaper::mouseReleaseEvent(QMouseEvent* event)
{
	m_isMouseDown = false;
	m_isLButtonDown = false;
	emit DrawFinish(map);
}


void PixelPaper::paintEvent(QPaintEvent*)
{
	QPainter painter{ this };

	painter.setPen(QPen(Qt::transparent));
	for (int i = 0; i < GetWidth(); i++)
	{
		for (int j = 0; j < GetHeight(); j++)
		{
			painter.setBrush(QBrush(map[i][j], Qt::SolidPattern));
			painter.drawRect(i * GetStep(), j * GetStep(), GetStep(), GetStep());
		}
	}

	painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine));
	for (int i = 0; i <= GetWidth(); i += 1)
	{
		painter.drawLine(QPoint(NumToPixel(i), 0), QPoint(NumToPixel(i), NumToPixel(GetHeight())));
	}
	for (int i = 0; i <= GetHeight(); i += 1)
	{
		painter.drawLine(QPoint(0, NumToPixel(i)), QPoint(NumToPixel(GetWidth()), NumToPixel(i)));
	}

}

void PixelPaper::ChangeWidth(int num)
{
	SetWidth(num);
	DrawMap();
}

void PixelPaper::ChangeHeight(int num)
{
	SetHeight(num);
	DrawMap();
}

void PixelPaper::ChangeStep(int num)
{
	SetStep(num);
	DrawMap();
}

void PixelPaper::ClearPaper() {
	ClearAllColor();
	emit DrawFinish(map);
}