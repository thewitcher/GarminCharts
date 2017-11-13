#ifndef CSVREADER_H
#define CSVREADER_H

#include <QString>
#include <QVector>
#include <QHash>

#include "functional"

class CSVReader
{
public:
	CSVReader();

	bool Load( const QString& a_rFileName );

	QVector<qreal> GetData( const QString& a_rDataType ) const;
	QVector<QString> GetStringData( const QString& a_rDataType ) const;
	const QVector<QString>& GetTypes() const;

private:
	QVector<qreal> GetDoubleData( const QVector<QString>& a_rData , std::function<double(double)> a_converter ) const;
	QVector<qreal> GetDoubleDataFromDescription( const QVector<QString>& a_rData , std::function<double(double)> a_converter ) const;

	QVector<QString> m_aColumnNames;
	QHash<int, QVector<QString>> m_aCSVRepresentation;
};

#endif // CSVREADER_H
