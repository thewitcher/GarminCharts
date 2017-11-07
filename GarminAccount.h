#ifndef GARMINACCOUNT_H
#define GARMINACCOUNT_H

#include <QString>

#include "Macros.h"

class GarminAccount
{
public:
	static GarminAccount* GetInstance();

	ADD_MEMBER_WITH_GETER_AND_SETTER( Passwd, str, QString, "" )
	ADD_MEMBER_WITH_GETER_AND_SETTER( Login, str, QString, "" )
	ADD_MEMBER_WITH_GETER_AND_SETTER( Name, str, QString, "" )

private:
	GarminAccount();

	static GarminAccount* m_pInstance;
};

#endif // GARMINACCOUNT_H
