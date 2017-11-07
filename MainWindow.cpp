#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Constants.h"

#include <QDebug>

MainWindow::MainWindow( QWidget* a_pParent )
	: QMainWindow( a_pParent )
	, m_pMainWindowUI( new Ui::MainWindow )
{
	m_pMainWindowUI->setupUi( this );

	m_chartViewController.SetChartView( m_pMainWindowUI->m_pChartView );

	connect( m_pMainWindowUI->m_pActionQuit, &QAction::triggered, this, &MainWindow::slotOnQuit, Qt::UniqueConnection );
	connect( m_pMainWindowUI->m_pAddPushButton, &QPushButton::clicked, this, &MainWindow::slotOnAddButtonClicked, Qt::UniqueConnection );

	connect( m_pMainWindowUI->pushButton, &QPushButton::clicked, this, &MainWindow::slotOnClicked, Qt::UniqueConnection );

	m_reader.Load( GarminConnector::GetInstance()->GetDownloadPath() + "/activities.csv" );
	CreateListOfAvailableDataTypes();
}

MainWindow::~MainWindow()
{
	delete m_pMainWindowUI;
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
	m_chartViewController.Draw( aDataToDraw );
}

void MainWindow::slotOnClicked()
{
	m_chartViewController.m_pChart->scroll( 10, 0 );
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
	}
}
