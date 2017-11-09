#include "LineSeries.h"

LineSeries::LineSeries( QObject* a_pParent , std::function<QString(qreal)> a_converter )
	: QtCharts::QLineSeries( a_pParent )
	, m_converter( a_converter )
{
	// Nothing
}

QString LineSeries::DisplayValue( const qreal a_fValue ) const
{
	return m_converter( a_fValue );
}
