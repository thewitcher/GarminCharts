#ifndef CHARTVIEWCONTROLLER_H
#define CHARTVIEWCONTROLLER_H

#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#include <QObject>

#include "ChartTip.h"

class ChartViewController : public QObject
{

	Q_OBJECT

public:
	ChartViewController();

	void Draw( QVector<QPointF> a_SeriesData );
	void SetChartView( QtCharts::QChartView* a_pChartView );

public slots:
	void slotOnClearChartTipsClicked();

protected slots:
	void slotToolTip( const QPointF& a_rPoint, bool a_bState );
	void slotKeepChartTip();

private:
	void ClearChart();
	void ClearChartTips();

	void ConfigureXAxis();
	void ConfigureView();

	Qt::AlignmentFlag GetYAxisAlignment() const;
	QtCharts::QValueAxis* GetYAxis();

	QtCharts::QChartView* m_pChartView = nullptr;
	QtCharts::QChart* m_pChart = new QtCharts::QChart;

	QtCharts::QDateTimeAxis* m_pAxisX = new QtCharts::QDateTimeAxis;
	QHash<int, QtCharts::QValueAxis*> m_aYAxises;

	QSharedPointer<ChartTip> m_pChartTip;
	QList<QSharedPointer<ChartTip>> m_aChartTips;

	int m_iChartCount = 0;
};

#endif // CHARTVIEWCONTROLLER_H
