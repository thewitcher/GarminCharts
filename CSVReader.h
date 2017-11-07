#ifndef CSVREADER_H
#define CSVREADER_H

#include <QString>
#include <QVector>
#include <QHash>

class CSVReader
{
public:
	CSVReader();

	void Load( const QString& a_rFileName );

	QVector<qreal> GetData( const QString& a_rDataType ) const;
	const QVector<QString>& GetTypes() const;

private:
	QVector<qreal> GetDoubleData( const QVector<QString>& a_rData ) const;

	QVector<QString> m_aColumnNames;
	QHash<int, QVector<QString>> m_aCSVRepresentation;
};

#endif // CSVREADER_H
