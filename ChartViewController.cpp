#include "ChartViewController.h"

#include "Constants.h"

#include <QtCharts/QBarSet>
#include <QDebug>
#include <QDate>

ChartViewController::ChartViewController()
{
	// Nothing
}

void ChartViewController::ConfigureXAxis()
{
	m_pAxisX->setFormat( Constants::DATA_FORMAT );
	m_pChart->addAxis( m_pAxisX, Qt::AlignBottom );
}

void ChartViewController::Draw( QVector<QPointF> a_SeriesData , const QString& a_strLegend )
{
	ConfigureXAxis();

	QtCharts::QLineSeries* pSeries = new QtCharts::QLineSeries;

	for ( auto pPoint : a_SeriesData )
	{
		*pSeries << pPoint;
	}

	pSeries->setName( a_strLegend );
	m_pChart->addSeries( pSeries );

	QtCharts::QValueAxis* pAxisY = GetYAxis();
	pAxisY->setLinePenColor( pSeries->pen().color() );
	m_pChart->addAxis( pAxisY, GetYAxisAlignment() );

	pSeries->attachAxis( m_pAxisX );
	pSeries->attachAxis( pAxisY );

	KeepOriginaleXRange();
	FilterXAxisByDate();

	m_iChartCount++;
}

Qt::AlignmentFlag ChartViewController::GetYAxisAlignment() const
{
	return ( m_iChartCount % 2 == 0 ? Qt::AlignLeft : Qt::AlignRight );
}

QtCharts::QValueAxis* ChartViewController::GetYAxis()
{
	if ( !m_aYAxises.contains( m_iChartCount ) )
	{
		m_aYAxises.insert( m_iChartCount, new QtCharts::QValueAxis );
	}

	return m_aYAxises.value( m_iChartCount );
}

void ChartViewController::SetChartView( QtCharts::QChartView* a_pChartView )
{
	m_pChartView = a_pChartView;
	ConfigureView();
}

void ChartViewController::SetXAxisRange( const QDateTime& a_minDate, const QDateTime& a_maxDate )
{
	m_minDate = a_minDate;
	m_maxDate = a_maxDate;
	m_pAxisX->setMin( m_minDate );
	m_pAxisX->setMax( m_maxDate );
}

void ChartViewController::ResetDateFilter()
{
	m_minDate = m_originalMinDate;
	m_maxDate = m_originalMaxDate;
	FilterXAxisByDate();
}

void ChartViewController::slotToolTip( const QPointF& a_rPoint, bool a_bState )
{
	if ( m_pChartTip == nullptr )
	{
		m_pChartTip = QSharedPointer<ChartTip>::create( m_pChart );
	}

	if ( a_bState )
	{
		m_pChartTip->SetText( QString( "X: %1 \nY: %2 " ).arg( QDateTime::fromMSecsSinceEpoch( a_rPoint.x() ).toString( Constants::DATA_FORMAT ) ).arg( a_rPoint.y() ) );
		m_pChartTip->SetAnchor( a_rPoint );
		m_pChartTip->setZValue( 11 );
		m_pChartTip->UpdateGeometry();
		m_pChartTip->show();
	}
	else
	{
		m_pChartTip->hide();
	}
}

void ChartViewController::slotKeepChartTip()
{
	m_aChartTips.append( m_pChartTip );
	m_pChartTip = QSharedPointer<ChartTip>::create( m_pChart );
}

void ChartViewController::ClearChartTips()
{
	m_pChartTip.clear();
	m_aChartTips.clear();
}

void ChartViewController::ClearChart()
{
	for ( auto pYAxis : m_aYAxises )
	{
		m_pChart->removeAxis( pYAxis );
	}

	m_aYAxises.clear();
	m_pChart->removeAllSeries();

	ClearChartTips();

	m_iChartCount = 0;

	ResetDateFilter();
}

void ChartViewController::ConfigureView()
{
	m_pChartView->setRenderHint( QPainter::Antialiasing );
	m_pChartView->setChart( m_pChart );
	m_pChart->legend()->setAlignment( Qt::AlignBottom );
	m_pChart->setTitle( "Analiza treningów" );
	m_pChartView->setRubberBand( QtCharts::QChartView::HorizontalRubberBand );
}

void ChartViewController::KeepOriginaleXRange()
{
	m_originalMinDate = m_pAxisX->min();
	m_originalMaxDate = m_pAxisX->max();
}

void ChartViewController::FilterXAxisByDate()
{
	if ( m_maxDate.isValid() )
	{
		m_pAxisX->setMax( m_maxDate );
	}
	if ( m_minDate.isValid() )
	{
		m_pAxisX->setMin( m_minDate );
	}
}

void ChartViewController::slotOnClearChartTipsClicked()
{
	ClearChartTips();
}
