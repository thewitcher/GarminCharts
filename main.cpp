#include "MainWindow.h"
#include "AccountDialog.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#include "GarminAccount.h"
#include "GarminConnector.h"

#include "Settings.h"

int main( int a_iArgc, char* a_pArgv[] )
{
	QApplication a( a_iArgc, a_pArgv );

	Settings::GetInstance()->Load();

	AccountDialog dialog;
	dialog.setModal( true );
	if( dialog.exec() != QDialog::Accepted )
	{
		return 1;
	}

	MainWindow w;

	if ( !w.LoadCSVFile() )
	{
		QMessageBox msgBox;
		msgBox.setText( "Cannot load csv file - download data from Garmin connect first" );
		msgBox.exec();
		return 1;
	}

	w.showMaximized();

	return a.exec();
}
