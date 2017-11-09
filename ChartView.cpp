#include "ChartView.h"

#include <QDebug>

ChartView::ChartView( QWidget* a_pParent )
	: QtCharts::QChartView( a_pParent )
{
	// Nothing
}

void ChartView::keyPressEvent( QKeyEvent* a_pEvent )
{
	if ( a_pEvent->key() == Qt::Key_Left )
	{
		chart()->scroll( -10, 0 );
	}
	else if ( a_pEvent->key() == Qt::Key_Right )
	{
		chart()->scroll( 10, 0 );
	}
	QtCharts::QChartView::keyPressEvent( a_pEvent );
}

void ChartView::keyReleaseEvent( QKeyEvent* a_pEvent )
{
	QtCharts::QChartView::keyReleaseEvent( a_pEvent );
}
