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

PixelMatrix FormatJson::JsonToPixelMatrix()
{
	QJsonObject body = QJsonDocument::fromJson(m_textEdit->toPlainText().toUtf8()).object();
	QStringList keys = body.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        qDebug() << "key" << i << " is:" << keys.at(i);
    }

	int width = body["w"].toString().toInt();
	int height = body["h"].toString().toInt();
	qDebug() << width << height;
	
	auto dataarray = body["color"].toArray();
	QVector<QVector<QColor>> data;
	for (int i = 0; i < height; i++)
	{
		QVector<QColor> row;
		for (int j = 0; j < width; j++)
		{
			// TODO: json读取数组有问题，初步方案，修改存储时的数组部分的存储方式
			QColor color;
			color.setNamedColor(dataarray[i * width + j].toString());
			qDebug() << "key" << dataarray[i * width + j].toString();
			row.push_back(color);
		}
		data.push_back(row);
	}
	//PixelMatrix matrix = PixelMatrix{ width.toInt(), QVector<QColor>(height.toInt(), Qt::black) };
	return data;
}

void FormatJson::PixelMatrixToJson(PixelMatrix pixelMatrix)
{
	m_height = pixelMatrix.length();
	m_width = pixelMatrix[0].length();
	QString data;
	QString colo;
	data += "[";
	colo += "[";
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
		colo += "";
		data += "";

	}
	colo += "]";
	data += "]";
	QJsonObject jsonbody;
	jsonbody.insert("w", QString::number(m_width));
	jsonbody.insert("h", QString::number(m_height));
	jsonbody.insert("data", data);
	jsonbody.insert("color", colo);


	m_textEdit->setText(QString(QJsonDocument(jsonbody).toJson()));
}


void FormatJson::GetDrawFinish(PixelMatrix e) {
	PixelMatrixToJson(e);
}