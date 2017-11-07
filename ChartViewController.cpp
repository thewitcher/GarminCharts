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

void ChartViewController::Draw( QVector<QPointF> a_SeriesData )
{
	ConfigureXAxis();

	QtCharts::QLineSeries* pSeries = new QtCharts::QLineSeries;

	for ( auto pPoint : a_SeriesData )
	{
		*pSeries << pPoint;
	}

	pSeries->setName( "Kalorie" );
	m_pChart->addSeries( pSeries );

	QtCharts::QValueAxis* pAxisY = GetYAxis();
	pAxisY->setLinePenColor( pSeries->pen().color() );
	m_pChart->addAxis( pAxisY, GetYAxisAlignment() );

	pSeries->attachAxis( m_pAxisX );
	pSeries->attachAxis( pAxisY );

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
	m_pChart->removeAxis( m_pAxisX );

	for ( auto pXAxis : m_aYAxises )
	{
		m_pChart->removeAxis( pXAxis );
	}

	m_pChart->removeAllSeries();

	ClearChartTips();

	m_iChartCount = 0;
}

void ChartViewController::ConfigureView()
{
	m_pChartView->setRenderHint( QPainter::Antialiasing );
	m_pChartView->setChart( m_pChart );
	m_pChart->legend()->setAlignment( Qt::AlignBottom );
	m_pChart->setTitle( "Analiza treningów" );
}

void ChartViewController::slotOnClearChartTipsClicked()
{
	ClearChartTips();
}
