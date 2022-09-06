#include "FormatJson.h"

FormatJson::FormatJson(QWidget* parent)
	: QTextEdit(parent)
{
	ui.setupUi(this);
	m_textEdit = this;
}

void FormatJson::InfoFormat()
{
	
}

void FormatJson::SetTextEditPtr(FormatJson* ptr)
{
	if (ptr != nullptr)
	{
		m_textEdit = ptr;
	}
}

//PixelMatrix FormatJson::JsonToPixelMatrix()
//{
//	QJsonObject body = QJsonDocument::fromJson(m_textEdit->toPlainText().toUtf8()).object();
//	return ;
//}

void FormatJson::PixelMatrixToJson(PixelMatrix pixelMatrix)
{
	m_height = pixelMatrix.length();
	m_width = pixelMatrix[0].length();
	QString data;
	QString colo;
	data += "[\n";
	colo += "[\n";
	for (int j = 0; j < m_width; ++j)
	{
		for (int i = 0; i < m_height; ++i)
		{
			auto r = pixelMatrix[i][j].red();
			auto g = pixelMatrix[i][j].green();
			auto b = pixelMatrix[i][j].blue();
			colo += QString("[%1,%2,%3]").arg(QString::number(r))
										.arg(QString::number(g))
										.arg(QString::number(b));
			if (r || g || b) {
				data += "1";
			}
			else
			{
				data += "0";
			}
			
			if (j == m_width - 1 && i == m_height - 1)
			{
				
			}
			else {
				data += ",";
				colo += ",";
			}

		}
		colo += "\n";
		data += "\n";
			
	}
	colo += "]";
	data += "]";
	QString body = QString("\
{\n\
'w' : %1,\n\
'h' : %2,\n\
'data' : %3,\n\
'color' : %4\n\
}\
	").arg(QString::number(m_width))
		.arg(QString::number(m_height))
		.arg(data)
		.arg(colo);
	
	m_textEdit->setText(body);
}


void FormatJson::GetDrawFinish(PixelMatrix e) {
	PixelMatrixToJson(e);
}