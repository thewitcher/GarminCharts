#ifndef GARMINCONNECTOR_H
#define GARMINCONNECTOR_H

#include "Macros.h"

#include <QString>

class GarminConnector
{
public:
	static GarminConnector* GetInstance();

	void DownloadDataFromGarmin();
	QString GetDownloadPath() const;

	ADD_MEMBER_WITH_GETER_AND_SETTER( DownloadCount, i, int, 0 )

private:
	GarminConnector() = default;

	static GarminConnector* m_pInstance;
};

#endif // GARMINCONNECTOR_H
