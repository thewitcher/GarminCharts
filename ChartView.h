#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>

class ChartView : public QtCharts::QChartView
{

	Q_OBJECT

public:
	ChartView( QWidget* a_pParent = Q_NULLPTR );
	virtual ~ChartView() {}

signals:
	void signalScrollingFinished();

protected:
	void keyPressEvent( QKeyEvent* a_pEvent ) override;
	void keyReleaseEvent( QKeyEvent* a_pEvent ) override;
};

#endif // CHARTVIEW_H
