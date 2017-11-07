#include "GarminConnector.h"
#include "GarminAccount.h"

#include <QString>
#include <QDebug>

GarminConnector* GarminConnector::m_pInstance = nullptr;

GarminConnector* GarminConnector::GetInstance()
{
	if ( m_pInstance == nullptr )
	{
		m_pInstance = new GarminConnector;
	}

	return m_pInstance;
}

void GarminConnector::DownloadDataFromGarmin()
{
	auto pGA = GarminAccount::GetInstance();

	QString strGarminQuery;

	if ( GetDownloadCount() == 0 )
	{
		return;
	}
	else if ( GetDownloadCount() > 0 )
	{
		strGarminQuery = QString( "python gcexport.py -d %1 -c %2 -f original -u --user \"%3\" --password \"%4\"" )
			.arg( GetDownloadPath() )
			.arg( GetDownloadCount() )
			.arg( pGA->GetLogin() )
			.arg( pGA->GetPasswd() );
	}
	else if ( GetDownloadCount() == -1 ) // all
	{
		strGarminQuery = QString( "python gcexport.py -d %1 -c all -f original -u --user \"%2\" --password \"%3\"" )
			.arg( GetDownloadPath() )
			.arg( pGA->GetLogin() )
			.arg( pGA->GetPasswd() );
	}

	m_pProcess->start( strGarminQuery );

	connect( m_pProcess.data(), SIGNAL( started() ), this, SIGNAL( signalDownloadDataStarted() ), Qt::UniqueConnection );
	connect( m_pProcess.data(), SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SIGNAL( signalDownloadDataFinished( int,QProcess::ExitStatus ) ), Qt::UniqueConnection );
	connect( m_pProcess.data(), SIGNAL( readyReadStandardOutput() ), this, SLOT( slotOnReadyReadStandardOutput() ), Qt::UniqueConnection );
}

QString GarminConnector::GetDownloadPath() const
{
	return "./" + GarminAccount::GetInstance()->GetName() + "_account";
}

void GarminConnector::slotOnReadyReadStandardOutput()
{
	emit signalReadyReadStandardOutput( QString( m_pProcess->readAllStandardOutput() )/*.split( '\n' )*/ );
}
