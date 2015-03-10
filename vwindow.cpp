#include "vwindow.h"
#include "ui_vwindow.h"

vWindow::vWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::vWindow)
{
  ui->setupUi(this);

  //connect signals/slots
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(closeDB()));
  connect(ui->tableView, SIGNAL(pressed(QModelIndex)), this, SLOT(chooseTable(QModelIndex)));
  connect(ui->actionConnect_via_ODBC, SIGNAL(triggered()), this, SLOT(login()));
  connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(ui->actionExport_to_CSV, SIGNAL(triggered()), this, SLOT(exportCSV()));
  connect(ui->queryButton, SIGNAL(pressed()), this, SLOT(queryDatabase()));

  //model with field info
  fieldModel = new QSqlQueryModel(this);

  //model with table info
  model = new QSqlQueryModel(this);

  //attempt to open DB
  db = QSqlDatabase::addDatabase("QODBC");

  //attempt to login database
  login();

  /*
  //only for when testing w/o ODBC connection
  this->show();
  */
}

vWindow::~vWindow()
{
  db.close();
  delete ui;
  //delete model;
  //delete fieldModel;
}

void vWindow::slotAcceptUserLogin(QString &databaseName, QString &DSN)
{
  //qDebug() << databaseName << DSN;
  currentDSN = DSN;
  currentDBname = databaseName;
  db.setDatabaseName(DSN);

  QString testQry = tr("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_SCHEMA='%1'").arg(databaseName);
  //qDebug() << testQry;

  if(db.open())
    {
      ui->label_DSN->setText(DSN);
      ui->label_database->setText(databaseName);
      //"SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE' AND TABLE_SCHEMA='netflix'"
      model->setQuery(testQry);
      ui->tableView->setModel(model);
      this->show();
      chooseTable(model->index(0,0));
    }
  else
    {
      int ret = QMessageBox::warning(this, tr("Connect to ODBC"), tr("Failure: %1").arg(db.lastError().text()), QMessageBox::Cancel);
      db.close();
      login();
    }
}

void vWindow::closeDB()
{
  db.close();
}

void vWindow::chooseTable(QModelIndex item)
{
  currentTableName = model->data(item).toString();
  //qDebug() << data << item;
  ui->label_table->setText(currentTableName);
  QString qry = tr("SELECT * FROM %1").arg(currentTableName);
  fieldModel->setQuery(qry);
  ui->tableView_2->setModel(fieldModel);
  //qDebug() << qry;

}

void vWindow::login()
{
  //login
  LoginDialog* loginDialog = new LoginDialog( this );
  connect( loginDialog,
                   SIGNAL(acceptLogin(QString&,QString&)),
                   this,
                   SLOT(slotAcceptUserLogin(QString&,QString&)));
  connect(loginDialog, SIGNAL(aborted()), this, SLOT(loginAborted()));
  loginDialog->exec();
}

void vWindow::reLogin()
{
  //clear old data
  model->clear();
  fieldModel->clear();
  db.removeDatabase(currentDBname);

  //login
  login();

}

void vWindow::exportCSV()
{
    //get field names from SQL table
    QStringList fieldNames = getFieldNames();

    /*
    //test field names (for w/o ODBC connection)
    QStringList testList;
    testList.append("First");
    testList.append("Second");
    testList.append("Third");
    testList.append("Fourth");
    testList.append("Fifth");
    testList.append("Sixth");
    */

    //open field mapping dialog
    mapWindow *mapper = new mapWindow(currentDSN, currentDBname, fieldNames, fieldModel, this);
    mapper->show();
}

QStringList vWindow::getFieldNames()
{
    //get number of columns
    int colCount = fieldModel->columnCount();

    //collect field names and return
    QStringList fNames;
    for(int i = 0; i < colCount; i++)
    {
        fNames.append(fieldModel->headerData(i,Qt::Horizontal).toString());
    }
    return fNames;
}

void vWindow::loginAborted()
{
  if(!this->isVisible())
    this->show();
}

void vWindow::queryDatabase()
{
  if(!currentDBname.isEmpty() && !currentDSN.isEmpty())
    {
      queryWindow *query = new queryWindow(currentDSN, currentDBname, currentTableName, this);
      query->show();
    }
  else
    QMessageBox::warning(this, tr("Query Database"), tr("Failure: No ODBC Connection"), QMessageBox::Cancel);

}
