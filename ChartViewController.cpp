#include "ChartViewController.h"

#include "Constants.h"
#include "LineSeries.h"
#include "DateHelper.h"

#include <QtCharts/QBarSet>
#include <QDebug>
#include <QDate>

ChartViewController::ChartViewController()
{
	connect( m_pChartView, &ChartView::signalScrollingFinished, this, &ChartViewController::slotOnScrollingFinished, Qt::UniqueConnection );
}

void ChartViewController::ConfigureXAxis()
{
	m_pAxisX->setFormat( Constants::DATA_FORMAT );
	m_pAxisX->setTickCount( 20 );
	m_pChart->addAxis( m_pAxisX, Qt::AlignBottom );
}

void ChartViewController::DrawChartTips( const QString& a_rLabelsMode, const QString& a_rSeriesType )
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
		LineSeries* pLineSeries = dynamic_cast<LineSeries*>( pSerie );
		if ( pLineSeries->GetType() == a_rSeriesType )
		{
			for ( int iIndex = 0 ; iIndex < pLineSeries->count() ; ++iIndex )
			{
				if ( iIndex % iFactor == 0 )
				{
					QPointF point = pLineSeries->at( iIndex );
					DrawChartTip( point, pLineSeries );
				}
			}
			break;
		}
	}
}

void ChartViewController::DrawTrendLines( const QString& a_rSeriesType )
{
	DrawTrendLines( m_pAxisX->min(), m_pAxisX->max(), a_rSeriesType );
}

/** <!--------------------------------------------------------------------------------------------->
 *
 * y = ax+b
 *
 * a = [(1/N)SUM_t(ty) -t'y'] / [(1/N)SUM_t(t - t')^2]
 * N - data count
 * t - index of data: 1, 2, 3, ...
 * y - data value
 * ' - average of data
 *
 * b = y'-at'
 <!---------------------------------------------------------------------------------------------> */
void ChartViewController::DrawTrendLines( const QDateTime& a_rMinDate, const QDateTime& a_rMaxDate, const QString& a_rSeriesType )
{
	for ( auto pSerie : m_pChart->series() )
	{
		LineSeries* pLineSeries = dynamic_cast<LineSeries*>( pSerie );
		if ( pLineSeries->GetType() == a_rSeriesType )
		{
			qreal ty = 0.0;
			qreal avgT = 0.0;
			qreal avgY = 0.0;
			int iCount = 0;
			for ( int iIndex = 0 ; iIndex < pLineSeries->count() ; ++iIndex )
			{
				QPointF point = pLineSeries->at( iIndex );

				if ( a_rMinDate.toMSecsSinceEpoch() <= point.x() && point.x() <= a_rMaxDate.toMSecsSinceEpoch() )
				{
					avgT += point.x();
					avgY += point.y();

					ty += point.x() * point.y();

					iCount++;
				}
			}

			qreal tyDividedByCount = ty / qreal( iCount );
			avgT = avgT / qreal( iCount );
			avgY = avgY / qreal( iCount );

			qreal TtimesY = avgT * avgY;

			qreal numeratorValue = tyDividedByCount - TtimesY;

			qreal denominatorValue = 0.0;
			for ( int iIndex = 0 ; iIndex < pLineSeries->count() ; ++iIndex )
			{
				QPointF point = pLineSeries->at( iIndex );

				if ( a_rMinDate.toMSecsSinceEpoch() <= point.x() && point.x() <= a_rMaxDate.toMSecsSinceEpoch() )
				{
					denominatorValue += ( point.x() - avgT ) * ( point.x() - avgT );
				}
			}
			denominatorValue = denominatorValue / qreal( iCount );

			qreal a1 = numeratorValue / denominatorValue;
			qreal a0 = avgY - a1 * avgT;

			QVector<QPointF> aData;
			aData << QPointF( m_pAxisX->min().toMSecsSinceEpoch(), m_pAxisX->min().toMSecsSinceEpoch() * a1 + a0 )
				  << QPointF( m_pAxisX->max().toMSecsSinceEpoch(), m_pAxisX->max().toMSecsSinceEpoch() * a1 + a0 );

			Draw( aData, a_rSeriesType, true );

			break;
		}
	}
}

void ChartViewController::slotOnScrollingFinished()
{
	SetXAxisRange( m_pAxisX->min(), m_pAxisX->max() );
}

void ChartViewController::Draw( QVector<QPointF> a_SeriesData , const QString& a_rType, bool a_bTrendLine )
{
	ConfigureXAxis();

	LineSeries* pSeries = new LineSeries( m_pChart, a_rType );

	for ( auto pPoint : a_SeriesData )
	{
		*pSeries << pPoint;
	}

	connect( pSeries, &LineSeries::hovered, this, &ChartViewController::signalDataHovered, Qt::UniqueConnection );

	pSeries->setName( a_rType );
	m_pChart->addSeries( pSeries );

	pSeries->attachAxis( m_pAxisX );

	if ( a_bTrendLine )
	{
		pSeries->setName( a_rType + QString( " - " + Constants::DATA_TYPE_TREND_LINE ) );
		for ( auto pSerie : m_pChart->series() )
		{
			LineSeries* pLineSeries = dynamic_cast<LineSeries*>( pSerie );
			if ( pLineSeries->GetType() == a_rType )
			{
				for ( auto pAxis : pLineSeries->attachedAxes() )
				{
					if ( pAxis->orientation() == Qt::Vertical )
					{
						pSeries->attachAxis( pAxis );
					}
				}
				break;
			}
		}
	}
	else
	{
		QtCharts::QValueAxis* pAxisY = GetYAxis();
		pAxisY->setLinePenColor( pSeries->pen().color() );
		m_pChart->addAxis( pAxisY, GetYAxisAlignment() );
		pSeries->attachAxis( pAxisY );
	}

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

	ClearChartTips();
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
