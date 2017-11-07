#include "CSVReader.h"

#include "Constants.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

CSVReader::CSVReader()
{
	// Nothing
}

void CSVReader::Load( const QString& a_rFileName )
{
	QFile csvFile( a_rFileName );
	if ( !csvFile.open( QIODevice::ReadOnly ) )
	{
		qWarning() << "File cannot be opened: " << a_rFileName;
		return;
	}

	QTextStream reader( &csvFile );

	QString strLine = reader.readLine();
	int iCount = 0;
	int iLine = 0;
	while ( !strLine.isEmpty() )
	{
		QStringList aData = strLine.split( ";" );

		if ( iCount == 0 ) // First line
		{
			iCount = aData.size();
			m_aColumnNames = aData.toVector();
		}
		else
		{
			if ( iCount != aData.size() )
			{
				qDebug() << aData;
				qWarning() << QString( "Invalid CSV file: was: %1, current line: %2, line: %3" ).arg( iCount ).arg( aData.size() ).arg( iLine );
				throw 1;
			}

			for ( int iColumn = 0 ; iColumn < aData.size() ; ++iColumn )
			{
				m_aCSVRepresentation[ iColumn ].append( aData.at( iColumn ) );
			}
		}

		++iLine;
		strLine = reader.readLine();
	}
}

QVector<qreal> CSVReader::GetDoubleData( const QVector<QString>& a_rData ) const
{
	QVector<qreal> aRealData;
	for ( QString strData : a_rData )
	{
		strData.replace( "\"", "" );

		bool bOk = true;
		double fData = strData.toDouble( &bOk );

		if ( !bOk )
		{
			fData = 0.0;
		}
		aRealData.append( fData );
	}
	return aRealData;
}

QVector<qreal> CSVReader::GetData( const QString& a_rDataType ) const
{
	const int iDataIndex = m_aColumnNames.indexOf( a_rDataType );
	const QVector<QString>& aData = m_aCSVRepresentation[ iDataIndex ];

	return GetDoubleData( aData );
}

const QVector<QString>& CSVReader::GetTypes() const
{
	return m_aColumnNames;
}
