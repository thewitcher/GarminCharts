#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Constants.h"
#include "Settings.h"

#include <QDebug>

MainWindow::MainWindow( QWidget* a_pParent )
	: QMainWindow( a_pParent )
	, m_pMainWindowUI( new Ui::MainWindow )
{
	m_pMainWindowUI->setupUi( this );
	m_chartViewController.SetChartView( m_pMainWindowUI->m_pChartView );

	CreateConnections();
	SetupDates();
}

MainWindow::~MainWindow()
{
	delete m_pMainWindowUI;
}

void MainWindow::CreateConnections()
{
	connect( m_pMainWindowUI->m_pActionQuit, &QAction::triggered, this, &MainWindow::slotOnQuit, Qt::UniqueConnection );
	connect( m_pMainWindowUI->m_pAddPushButton, &QPushButton::clicked, this, &MainWindow::slotOnAddButtonClicked, Qt::UniqueConnection );
	connect( m_pMainWindowUI->m_pDateFilterButton, &QPushButton::clicked, this, &MainWindow::slotOnDateFilterButtonClicked, Qt::UniqueConnection );
	connect( m_pMainWindowUI->m_pResetRangeButton, &QPushButton::clicked, this, &MainWindow::slotOnResetRangeButtonClicked, Qt::UniqueConnection );
	connect( m_pMainWindowUI->m_pClearButton, &QPushButton::clicked, this, &MainWindow::slotOnClearButtonClicked, Qt::UniqueConnection );
	connect( m_pMainWindowUI->m_pAddLabels, &QPushButton::clicked, this, &MainWindow::slotOnAddLabelButtonClicked, Qt::UniqueConnection );
	connect( &m_chartViewController, &ChartViewController::signalDataHovered, this, &MainWindow::slotShowValue, Qt::UniqueConnection );
}

void MainWindow::SetupDates()
{
	auto pSettings = Settings::GetInstance();
	const QString strMinDateFromSettings = pSettings->GetSetting( "MinDate" );
	const QString strMaxDateFromSettings = pSettings->GetSetting( "MaxDate" );

	QDate minDate;
	QDate maxDate;

	if ( strMaxDateFromSettings.isEmpty() )
	{
		maxDate = QDate::currentDate();
	}
	else
	{
		maxDate = QDate::fromString( strMaxDateFromSettings, Constants::DATA_FORMAT );
	}

	if ( strMinDateFromSettings.isEmpty() )
	{
		minDate = maxDate.addMonths( -2 );
	}
	else
	{
		minDate = QDate::fromString( strMinDateFromSettings, Constants::DATA_FORMAT );
	}

	m_pMainWindowUI->m_pMinDate->setDate( minDate );
	m_pMainWindowUI->m_pMaxDate->setDate( maxDate );
}

bool MainWindow::LoadCSVFile()
{
	if ( m_reader.Load( GarminConnector::GetInstance()->GetDownloadPath() + "/activities.csv" ) )
	{
		CreateListOfAvailableDataTypes();
		return true;
	}
	return false;
}

QVector<QPointF> MainWindow::CreateSeries( const QString& a_rDataType )
{
	auto aYData = m_reader.GetData( a_rDataType );
	auto aXData = m_reader.GetData( "Begin Timestamp (Raw Milliseconds)" );

	if ( aYData.size() != aXData.size() )
	{
		qWarning() << "Invalid data to draw";
		throw 1;
	}

	QVector<QPointF> aDataToDraw;
	for ( int i = 0 ; i < aYData.size() ; ++i )
	{
		aDataToDraw.append( QPointF( aXData.at( i ), aYData.at( i ) ) );
	}

	std::sort( aDataToDraw.begin(), aDataToDraw.end(), []( const QPointF& a_rPoint1, const QPointF& a_rPoint2 )->bool
	{
		return ( a_rPoint1.x() < a_rPoint2.x() );
	} );

	return aDataToDraw;
}

void MainWindow::slotOnQuit( bool /*a_bChecked*/ )
{
	qApp->quit();
}

void MainWindow::slotOnAddButtonClicked()
{
	QVector<QPointF> aDataToDraw = CreateSeries( m_pMainWindowUI->m_pDataTypeComboBox->currentText() );
	m_chartViewController.Draw( aDataToDraw, m_pMainWindowUI->m_pDataTypeComboBox->currentText() );
}

void MainWindow::slotOnDateFilterButtonClicked()
{
	m_chartViewController.SetXAxisRange( m_pMainWindowUI->m_pMinDate->dateTime(), m_pMainWindowUI->m_pMaxDate->dateTime() );
	Settings::GetInstance()->Save( "MinDate", m_pMainWindowUI->m_pMinDate->dateTime().toString( Constants::DATA_FORMAT ) );
	Settings::GetInstance()->Save( "MaxDate", m_pMainWindowUI->m_pMaxDate->dateTime().toString( Constants::DATA_FORMAT ) );
}

void MainWindow::slotOnResetRangeButtonClicked()
{
	m_chartViewController.ResetDateFilter();
}

void MainWindow::slotOnClearButtonClicked()
{
	m_chartViewController.ClearChart();
}

void MainWindow::slotOnDeleteLabelsButtonClicked()
{
	m_chartViewController.ClearChartTips();
}

void MainWindow::slotOnAddLabelButtonClicked()
{
	m_chartViewController.DrawChartTipsForSeries( m_pMainWindowUI->m_pLabelsComboBox->currentText() );
}

void MainWindow::slotShowValue( const QPointF& a_rPoint, bool a_bState )
{
	if ( a_bState )
	{
		m_pMainWindowUI->m_pValue->setText( "Data: " + QDateTime::fromMSecsSinceEpoch( a_rPoint.x() ).toString( Constants::DATA_FORMAT ) +
											", wartość: " + QString::number( a_rPoint.y() ) );
	}
	else
	{
		m_pMainWindowUI->m_pValue->setText( "" );
	}
}

void MainWindow::CreateListOfAvailableDataTypes()
{
	const QVector<QString>& rTypes = m_reader.GetTypes();
	for ( const QString& strType : rTypes )
	{
		if ( strType == Constants::DATA_TYPE_CALORIES )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_MAX_HR )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_DISTANCE )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_AVG_HR )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_MIN_HR )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_AVG_PACE )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_DURATION )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_MAX_PACE )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_VOMAX )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_TRAINING_EFFECT )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
		if ( strType == Constants::DATA_TYPE_DESCRIPTION_HRREST )
		{
			m_pMainWindowUI->m_pDataTypeComboBox->addItem( strType );
		}
	}
}
