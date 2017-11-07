#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui
{
	class AccountDialog;
}

class AccountDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AccountDialog( QWidget* a_pParent = 0 );
	~AccountDialog();

	QString GetAccoutName() const;
	int GetSyncCount() const;

public slots:
	void slotOnOkClicked( bool a_bChecked );
	void slotOnDownloadDataFinished( int a_iExitCode, QProcess::ExitStatus a_eExitStatus );
	void slotOnDownloadDataStarted();
	void slotOnReadyReadStandardOutput( const QString& a_rOutput );

private:
	Ui::AccountDialog* m_pUI;
};

#endif // ACCOUNTDIALOG_H
