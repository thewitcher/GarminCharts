#include "Settings.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

Settings* Settings::m_pSettings = nullptr;

Settings* Settings::GetInstance()
{
	if ( !m_pSettings )
	{
		m_pSettings = new Settings;
	}
	return m_pSettings;
}

void Settings::Load()
{
	QFile settings( "./settings.txt" );
	if ( settings.open( QFile::ReadOnly ) )
	{
		QTextStream out( &settings );

		QString strKey;
		QString strValue;

		while ( !out.atEnd() )
		{
			out >> strKey;
			out >> strValue;

			if ( !strKey.isEmpty() && !strValue.isEmpty() )
			{
				m_aSettings.insert( strKey, strValue );
			}
		}
	}
}

void Settings::Save( const QString& a_rSettingName, const QString& a_rSettingValue )
{
	m_aSettings.insert( a_rSettingName, a_rSettingValue );

	QFile settings( "./settings.txt" );
	if ( settings.open( QFile::WriteOnly ) )
	{
		QTextStream out( &settings );

		QHashIterator<QString, QString> iter( m_aSettings );
		while ( iter.hasNext() )
		{
			iter.next();
			out << iter.key() << " " << iter.value() << "\n";
		}

	}
}

QString Settings::GetSetting( const QString& a_rSettingName ) const
{
	if ( !m_aSettings.contains( a_rSettingName ) )
	{
		qWarning() << "No such setting: " << a_rSettingName;
		return "";
	}
	return m_aSettings.value( a_rSettingName );
}

Settings::Settings()
{
	// Nothing
}
