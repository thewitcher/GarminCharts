#include "GarminConnector.h"
#include "GarminAccount.h"

#include <QString>

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
	system( strGarminQuery.toStdString().data() );
}

QString GarminConnector::GetDownloadPath() const
{
	return "./" + GarminAccount::GetInstance()->GetName() + "_account";
}
