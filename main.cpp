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
	if( dialog.exec() != QDialog::Accepted )
	{
		return 1;
	}

	MainWindow w;
	w.showMaximized();

	return a.exec();
}
