#include "GarminAccount.h"

GarminAccount* GarminAccount::m_pInstance = nullptr;

GarminAccount::GarminAccount()
{
	// Nothing
}

GarminAccount* GarminAccount::GetInstance()
{
	if ( m_pInstance == nullptr )
	{
		m_pInstance = new GarminAccount;
	}

	return m_pInstance;
}

