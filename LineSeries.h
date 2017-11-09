#ifndef LINESERIES_H
#define LINESERIES_H

#include <QtCharts/QLineSeries>

#include <functional>

class LineSeries : public QtCharts::QLineSeries
{
public:
	LineSeries( QObject* a_pParent, std::function<QString(qreal)> a_converter );
	virtual ~LineSeries() {}

	QString DisplayValue( const qreal a_fValue ) const;

private:
	std::function<QString(qreal)> m_converter = []( const qreal a_fValue ) { return QString::number( a_fValue ); };
};

#endif // LINESERIES_H
