#include "ChartViewController.h"

#include "Constants.h"
#include "LineSeries.h"
#include "DateHelper.h"

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

void ChartViewController::DrawChartTipsForSeries( const QString& a_rLabelsMode )
{
	ClearChartTips();

	if ( !a_rLabelsMode.isEmpty() )
	{
		m_strLabelsMode = a_rLabelsMode;
	}

	int iFactor = 0;

	if ( m_strLabelsMode == "Bez etykiet" )
	{
		return;
	}
	else if ( m_strLabelsMode == "Etykieta na każdym punkcie" )
	{
		iFactor = 1;
	}
	else if ( m_strLabelsMode == "Etykieta na co drugim punkcie" )
	{
		iFactor = 2;
	}
	else if ( m_strLabelsMode == "Etykieta na co trzecim punkcie" )
	{
		iFactor = 3;
	}

	for ( auto pSerie : m_pChart->series() )
	{
		QtCharts::QLineSeries* pLineSeries = dynamic_cast<QtCharts::QLineSeries*>( pSerie );
		for ( int iIndex = 0 ; iIndex < pLineSeries->count() ; ++iIndex )
		{
			if ( iIndex % iFactor == 0 )
			{
				QPointF point = pLineSeries->at( iIndex );
				DrawChartTip( point, pLineSeries );
			}
		}
	}
}

void ChartViewController::Draw( QVector<QPointF> a_SeriesData , const QString& a_strType )
{
	ConfigureXAxis();

	LineSeries* pSeries = nullptr;
	if ( a_strType == Constants::DATA_TYPE_AVG_PACE || a_strType == Constants::DATA_TYPE_MAX_PACE )
	{
		pSeries = new LineSeries( m_pChart, []( const qreal a_fValue ) { return DateHelper::GetStringFromSeconds( DateHelper::GetSecondsFromDecimalValue( a_fValue ), "mm:ss" ); } );
	}
	else
	{
		pSeries = new LineSeries( m_pChart, []( const qreal a_fValue ) { return QString::number( a_fValue ); } );
	}

	for ( auto pPoint : a_SeriesData )
	{
		*pSeries << pPoint;
	}

	connect( pSeries, &LineSeries::hovered, this, &ChartViewController::signalDataHovered, Qt::UniqueConnection );

	pSeries->setName( a_strType );
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

void ChartViewController::SetChartView( ChartView* a_pChartView )
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

	DrawChartTipsForSeries( "" );
}

void ChartViewController::ResetDateFilter()
{
	m_minDate = m_originalMinDate;
	m_maxDate = m_originalMaxDate;
	FilterXAxisByDate();
}

void ChartViewController::DrawChartTip( const QPointF& a_rPoint, QtCharts::QLineSeries* a_pSerie )
{
	QSharedPointer<ChartTip> pChartTip = QSharedPointer<ChartTip>::create( m_pChart );
	pChartTip->SetText( QString( "X: %1 \nY: %2 " )
						.arg( QDateTime::fromMSecsSinceEpoch( a_rPoint.x() ).toString( Constants::DATA_FORMAT ) )
						.arg( a_rPoint.y() ) );
	pChartTip->SetAnchor( m_pChart->mapToPosition( a_rPoint, a_pSerie ) );
	pChartTip->setZValue( 11 );
	pChartTip->UpdateGeometry();
	pChartTip->show();

	m_aChartTips.append( pChartTip );
}

void ChartViewController::ClearChartTips()
{
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
	ResetDateFilter();

	m_iChartCount = 0;
}

void ChartViewController::ConfigureView()
{
	m_pChartView->setRenderHint( QPainter::Antialiasing );
	m_pChartView->setChart( m_pChart );
	m_pChartView->setLineWidth( 10 );

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
