#include "AccountDialog.h"
#include "ui_AccountDialog.h"

#include <QDebug>

AccountDialog::AccountDialog( QWidget* a_pParent )
	: QDialog( a_pParent ),
	 m_pUI( new Ui::AccountDialog )
{
	m_pUI->setupUi( this );

	m_pUI->m_pNoSyncRadioButton->setChecked( true );
	m_pUI->m_pSyncCountSpinBox->setEnabled( false );

	connect( m_pUI->m_pLastRadioButton, &QRadioButton::toggled, m_pUI->m_pSyncCountSpinBox, &QSpinBox::setEnabled, Qt::UniqueConnection );
	connect( m_pUI->m_pOkButton, &QRadioButton::clicked, this, &AccountDialog::accept, Qt::UniqueConnection );
}

AccountDialog::~AccountDialog()
{
	delete m_pUI;
}

QString AccountDialog::GetAccoutName() const
{
	return m_pUI->m_pAccountComboBox->currentText();
}

int AccountDialog::GetSyncCount() const
{
	if ( m_pUI->m_pAllRadioButton->isChecked() )
	{
		return -1;
	}
	else if ( m_pUI->m_pNoSyncRadioButton->isChecked() )
	{
		return 0;
	}
	else if ( m_pUI->m_pLastRadioButton->isChecked() )
	{
		return m_pUI->m_pSyncCountSpinBox->value();
	}

	qWarning() << "GetSyncCount - no radio button";
	throw 1;
}
