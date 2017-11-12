#ifndef CHARTVIEWCONTROLLER_H
#define CHARTVIEWCONTROLLER_H

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#include <QObject>
#include <QDateTime>

#include "ChartTip.h"
#include "ChartView.h"

class ChartViewController : public QObject
{

	Q_OBJECT

public:
	ChartViewController();

	void Draw( QVector<QPointF> a_SeriesData, const QString& a_strType );
	void SetChartView( ChartView* a_pChartView );
	void SetXAxisRange( const QDateTime& a_minDate, const QDateTime& a_maxDate );
	void ResetDateFilter();
	void ClearChart();
	void ClearChartTips();
	void DrawChartTipsForSeries( const QString& a_rLabelsMode );

signals:
	void signalDataHovered( const QPointF& a_rPoint, bool a_bState );

protected slots:
	void DrawChartTip( const QPointF& a_rPoint , QtCharts::QLineSeries* a_pSerie );

private:
	void ConfigureXAxis();
	void ConfigureView();

	void KeepOriginaleXRange();
	void FilterXAxisByDate();

	Qt::AlignmentFlag GetYAxisAlignment() const;
	QtCharts::QValueAxis* GetYAxis();

	ChartView* m_pChartView = nullptr;
	QtCharts::QChart* m_pChart = new QtCharts::QChart;

	QtCharts::QDateTimeAxis* m_pAxisX = new QtCharts::QDateTimeAxis;
	QHash<int, QtCharts::QValueAxis*> m_aYAxises;

	QList<QSharedPointer<ChartTip>> m_aChartTips;

	int m_iChartCount = 0;

	QString m_strLabelsMode = "Bez etykiet";

	QDateTime m_minDate;
	QDateTime m_maxDate;

	QDateTime m_originalMinDate;
	QDateTime m_originalMaxDate;
};

#endif // CHARTVIEWCONTROLLER_H
