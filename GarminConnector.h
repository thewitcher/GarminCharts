#ifndef GARMINCONNECTOR_H
#define GARMINCONNECTOR_H

#include "Macros.h"

#include <QString>
#include <QProcess>
#include <QSharedPointer>

class GarminConnector : public QObject
{

	Q_OBJECT

public:
	static GarminConnector* GetInstance();

	void DownloadDataFromGarmin();
	QString GetDownloadPath() const;

	ADD_MEMBER_WITH_GETER_AND_SETTER( DownloadCount, i, int, 0 )

signals:
	void signalDownloadDataFinished( int a_iExitCode, QProcess::ExitStatus a_eExitStatus );
	void signalReadyReadStandardOutput( const QString& a_rOutput );
	void signalDownloadDataStarted();

protected slots:
	void slotOnReadyReadStandardOutput();

private:
	GarminConnector() = default;

	static GarminConnector* m_pInstance;
	QSharedPointer<QProcess> m_pProcess = QSharedPointer<QProcess>::create();
};

#endif // GARMINCONNECTOR_H
