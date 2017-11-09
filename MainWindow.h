#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ChartViewController.h"
#include "GarminConnector.h"
#include "CSVReader.h"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow( QWidget* a_pParent = 0 );
	~MainWindow();

	bool LoadCSVFile();

private slots:
	void slotOnQuit( bool a_bChecked );
	void slotOnAddButtonClicked();
	void slotOnDateFilterButtonClicked();
	void slotOnResetRangeButtonClicked();
	void slotOnClearButtonClicked();
	void slotOnDeleteLabelsButtonClicked();

private:
	QVector<QPointF> CreateSeries( const QString& a_rDataType );
	void CreateConnections();
	void CreateListOfAvailableDataTypes();
	void SetupDates();

	Ui::MainWindow* m_pMainWindowUI;
	ChartViewController m_chartViewController;
	CSVReader m_reader;
};

#endif // MAINWINDOW_H
