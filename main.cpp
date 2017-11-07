#include "MainWindow.h"
#include "AccountDialog.h"

#include <QApplication>
#include <QDebug>

#include "GarminAccount.h"
#include "GarminConnector.h"

int main( int a_iArgc, char* a_pArgv[] )
{
	QApplication a( a_iArgc, a_pArgv );

	AccountDialog dialog;
	dialog.setModal( true );
	dialog.exec();

	GarminAccount::GetInstance()->SetName( dialog.GetAccoutName() );
	if ( dialog.GetAccoutName() == "Marcin" )
	{
		GarminAccount::GetInstance()->SetLogin( "thewitcher87@gmail.com" );
		GarminAccount::GetInstance()->SetPasswd( "Yennefer1987" );
	}
	else if ( dialog.GetAccoutName() == "Agnieszka" )
	{
		GarminAccount::GetInstance()->SetLogin( "agnieszkahaber87@gmail.com" );
		GarminAccount::GetInstance()->SetPasswd( "Powstanie44" );
	}
	else
	{
		qWarning() << "No such account - exit";
		return 1;
	}

	GarminConnector::GetInstance()->SetDownloadCount( dialog.GetSyncCount() );

	MainWindow w;
	w.showMaximized();

	return a.exec();
}
