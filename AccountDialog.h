#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <QDialog>

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

private:
	Ui::AccountDialog* m_pUI;
};

#endif // ACCOUNTDIALOG_H
