#include "LineSeries.h"

LineSeries::LineSeries( QObject* a_pParent, const QString& a_strType )
	: QtCharts::QLineSeries( a_pParent )
	, m_strType( a_strType )
{
	// Nothing
}

QString LineSeries::GetType() const
{
	return m_strType;
}
