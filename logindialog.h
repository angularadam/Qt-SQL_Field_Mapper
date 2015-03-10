#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>

/*!
 *Makes class LoginDialog a child to its parent, QDialog
 */
class LoginDialog : public QDialog
{
  /*!
 *Turns Login Dialog into a QObject
 */
  Q_OBJECT

private:  
  /*!
      * A label for the username component.
      */
  QLabel* labelDatabaseName;

  /*!
      * A label for the password.
      */
  //QLabel* labelPassword;

  /*!
      * A label for the DSN.
      */
  QLabel* labelDSN;

  /*!
      * An editable combo box for allowing the user
      * to enter his username or select it from a list.
      */
  QLineEdit* editDatabaseName;

  /*!
      * A field to let the user enters his password.
      */
  //QLineEdit* editPassword;

  /*!
      * A field to let the user enter the DSN.
      */
  QLineEdit* editDSN;

  /*!
      * The standard dialog button box.
      */
  QDialogButtonBox* buttons;

  /*!
      * A method to set up all dialog components and
      * initialize them.
      */
  void setUpGUI();

public:
  explicit LoginDialog(QWidget *parent = 0);

  /*!
      * Sets the proposed username, that can come for instance
      * from a shared setting.
      *\param username the string that represents the current username
      * to display
      */
  void setDatabaseName( QString& databaseName );

  /*!
      * Sets the current password to propose to the user for the login.
      * \param password the password to fill into the dialog form
      */
  //void setPassword( QString& password );

  /*!
      * Sets the current DSN to propose to the user for the login.
      *\param DSN
      */
  void setDSN( QString& DSN );



signals:

  /*!
      * A signal emitted when the login is performed.
      * \param username the username entered in the dialog
      * \param password the password entered in the dialog
      * \param index the number of the username selected in the combobox
      */
  void acceptLogin( QString&, QString&);

  /*!
      * Signal emitted when user aborts login.
      */
  void aborted();

public slots:
  /*!
      * A slot to adjust the emitting of the signal.
      */
  void slotAcceptLogin();

  /*!
      * Closes dialog or program.
      */
  void abort();

};

#endif // DIALOG_H
