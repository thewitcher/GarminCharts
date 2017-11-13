#ifndef LINESERIES_H
#define LINESERIES_H

#include <QtCharts/QLineSeries>

#include <functional>

class LineSeries : public QtCharts::QLineSeries
{
public:
	LineSeries( QObject* a_pParent, const QString& a_strType );
	virtual ~LineSeries() {}

	QString GetType() const;

private:
	const QString m_strType = "";
};

#endif // LINESERIES_H
