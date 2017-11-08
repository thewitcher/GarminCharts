#include "AccountDialog.h"
#include "ui_AccountDialog.h"
#include "GarminAccount.h"
#include "GarminConnector.h"

#include <QDebug>

AccountDialog::AccountDialog( QWidget* a_pParent )
	: QDialog( a_pParent ),
	 m_pUI( new Ui::AccountDialog )
{
	m_pUI->setupUi( this );

	m_pUI->m_pNoSyncRadioButton->setChecked( true );
	m_pUI->m_pSyncCountSpinBox->setEnabled( false );

	connect( m_pUI->m_pLastRadioButton, &QRadioButton::toggled, m_pUI->m_pSyncCountSpinBox, &QSpinBox::setEnabled, Qt::UniqueConnection );
	connect( m_pUI->m_pOkButton, &QRadioButton::clicked, this, &AccountDialog::slotOnOkClicked, Qt::UniqueConnection );
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

void AccountDialog::slotOnOkClicked( bool /*a_bChecked*/ )
{
	m_pUI->m_pOkButton->setEnabled( false );

	GarminAccount::GetInstance()->SetName( GetAccoutName() );
	if ( GetAccoutName() == "Marcin" )
	{
		GarminAccount::GetInstance()->SetLogin( "thewitcher87@gmail.com" );
		GarminAccount::GetInstance()->SetPasswd( "Yennefer1987" );
	}
	else if ( GetAccoutName() == "Agnieszka" )
	{
		GarminAccount::GetInstance()->SetLogin( "agnieszkahaber87@gmail.com" );
		GarminAccount::GetInstance()->SetPasswd( "Powstanie44" );
	}
	else
	{
		qWarning() << "No such account - exit";
		reject();
		return;
	}

	connect( GarminConnector::GetInstance(), &GarminConnector::signalDownloadDataFinished, this, &AccountDialog::slotOnDownloadDataFinished, Qt::UniqueConnection );
	connect( GarminConnector::GetInstance(), &GarminConnector::signalDownloadNotNeeded, this, &AccountDialog::accept, Qt::UniqueConnection );
	connect( GarminConnector::GetInstance(), &GarminConnector::signalDownloadDataStarted, this, &AccountDialog::slotOnDownloadDataStarted, Qt::UniqueConnection );
	connect( GarminConnector::GetInstance(), &GarminConnector::signalReadyReadStandardOutput, this, &AccountDialog::slotOnReadyReadStandardOutput, Qt::UniqueConnection );

	GarminConnector::GetInstance()->SetDownloadCount( GetSyncCount() );
	GarminConnector::GetInstance()->DownloadDataFromGarmin();
}

void AccountDialog::slotOnDownloadDataFinished( int /*a_iExitCode*/, QProcess::ExitStatus /*a_eExitStatus*/ )
{
	m_pUI->m_pOutputLabel->setText( "All files downloaded:)\n" );

	disconnect( m_pUI->m_pOkButton, &QRadioButton::clicked, this, &AccountDialog::slotOnOkClicked );
	connect( m_pUI->m_pOkButton, &QRadioButton::clicked, this, &AccountDialog::accept, Qt::UniqueConnection );

	m_pUI->m_pOkButton->setEnabled( true );
}

void AccountDialog::slotOnDownloadDataStarted()
{
	m_pUI->m_pAllRadioButton->setEnabled( false );
	m_pUI->m_pLastRadioButton->setEnabled( false );
	m_pUI->m_pNoSyncRadioButton->setEnabled( false );
	m_pUI->m_pAccountComboBox->setEnabled( false );

	m_pUI->m_pOutputLabel->setText( "Downloading files from Garmin Connect...\n" );
}

void AccountDialog::slotOnReadyReadStandardOutput( const QString& a_rOutput )
{
	m_pUI->m_pOutputLabel->setText( m_pUI->m_pOutputLabel->text() + a_rOutput );
}
