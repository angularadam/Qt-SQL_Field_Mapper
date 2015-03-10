#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    setUpGUI();
    setWindowTitle( tr("Connect to ODBC") );
    setModal( true );
}

void LoginDialog::setUpGUI(){
    // set up the layout
    QGridLayout* formGridLayout = new QGridLayout( this );

    // initialize the DSN line edit so that it is editable
    editDSN = new QLineEdit( this );
    //editDSN->setEditable( true );
    // initialize the database name line edit so that it is editable
    editDatabaseName = new QLineEdit( this );
    //editUsername->setEditable( true );
    // initialize the password field so that it does not echo
    // characters
    //editPassword = new QLineEdit( this );
    //editPassword->setEchoMode( QLineEdit::Password );

    // initialize the labels
    labelDatabaseName = new QLabel( this );
    //labelPassword = new QLabel( this );
    labelDSN = new QLabel( this );
    labelDatabaseName->setText( tr( "Database Name" ) );
    labelDatabaseName->setBuddy( editDatabaseName );
    //labelPassword->setText( tr( "Password" ) );
    //labelPassword->setBuddy( editPassword );
    labelDSN->setText( tr( "DSN" ) );
    labelDSN->setBuddy( editDSN );



    // initialize buttons
    buttons = new QDialogButtonBox( this );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Cancel );
    buttons->button( QDialogButtonBox::Ok )->setText( tr("Login") );
    buttons->button( QDialogButtonBox::Cancel )->setText( tr("Abort") );

    // connects slots
    connect( buttons->button( QDialogButtonBox::Cancel ),
             SIGNAL(clicked()),
             this,
             SLOT(abort())
             );

    connect( buttons->button( QDialogButtonBox::Ok ),
             SIGNAL(clicked()),
             this,
             SLOT(slotAcceptLogin()) );

    // place components into the dialog
    formGridLayout->addWidget( labelDSN, 0, 0 );
    formGridLayout->addWidget( editDSN, 0, 1 );
    formGridLayout->addWidget( labelDatabaseName, 1, 0 );
    formGridLayout->addWidget( editDatabaseName, 1, 1 );
    //formGridLayout->addWidget( labelPassword, 2, 0 );
    //formGridLayout->addWidget( editPassword, 2, 1 );
    formGridLayout->addWidget( buttons, 3, 0, 1, 2 );

    setLayout( formGridLayout );


}


void LoginDialog::setDatabaseName(QString &databaseName){
  editDatabaseName->setText( databaseName );
}


/*void LoginDialog::setPassword(QString &password){
    editPassword->setText( password );
}*/

void LoginDialog::slotAcceptLogin(){
    QString databaseName = editDatabaseName->text();
    //QString password = editPassword->text();
    QString DSN    = editDSN->text();

    emit acceptLogin( databaseName,  // current username
                      DSN        // current DSN
                      );

    // close this dialog
    close();
}


void LoginDialog::setDSN(QString &DSN){
    editDSN->setText( DSN );
}

void LoginDialog::abort()
{
  emit aborted();
  close();
}

