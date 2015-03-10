#include "querywindow.h"
#include "ui_querywindow.h"

queryWindow::queryWindow(QString dsn, QString db, QString tableName, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::queryWindow)
{
  ui->setupUi(this);

  //qDebug() << "Query instance created.";

  //connect signals and slots
  connect(ui->updateButton, SIGNAL(pressed()), this, SLOT(updateQuery()));
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(closeQuery()));
  connect(ui->actionSave_Query, SIGNAL(triggered()), this, SLOT(saveQuery()));
  connect(ui->actionSave_Query_As, SIGNAL(triggered()), this, SLOT(saveQueryAs()));
  connect(ui->actionExport_to_CSV, SIGNAL(triggered()), this, SLOT(exportQuery()));
  connect(ui->editQuery, SIGNAL(pressedEnter()), this, SLOT(updateQuery()));
  connect(ui->actionOpen_Query, SIGNAL(triggered()), this, SLOT(openQuery()));

  //display default query
  model = new QSqlQueryModel(this);
  qry = tr("SELECT * FROM %1").arg(tableName);
  ui->editQuery->setPlainText(qry);
  model->setQuery(qry);
  ui->tableViewQuery->setModel(model);

  //init file path
  queryFilePath = "";

  //store dsn and db name for this query
  currentDSN = dsn;
  currentDBname = db;
}

queryWindow::~queryWindow()
{
  delete ui;
}

void queryWindow::updateQuery()
{
  qry = ui->editQuery->toPlainText();
  model->setQuery(qry);
  ui->tableViewQuery->setModel(model);
}

void queryWindow::saveQuery()
{
  if(queryFilePath.isEmpty())
    queryFilePath = QFileDialog::getSaveFileName(this, tr("Save Query"), "./query", tr("QRY Files (*.qry)"));

  QFile queryFile(queryFilePath);

  if(queryFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      QTextStream in(&queryFile);

      in << currentDSN << "," << currentDBname << '\n';
      in << qry;

      queryFile.flush();
      queryFile.close();
    }
  else
    int ret = QMessageBox::warning(this, tr("Save Query"), tr("Failure: Cannot Save File"), QMessageBox::Cancel);

}

void queryWindow::saveQueryAs()
{
  queryFilePath = QFileDialog::getSaveFileName(this, tr("Save Query"), "./query", tr("QRY Files (*.qry)"));

  QFile queryFile(queryFilePath);

  if(queryFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      QTextStream in(&queryFile);

      in << currentDSN << "," << currentDBname << '\n';
      in << qry;

      queryFile.flush();
      queryFile.close();
    }
  else
    int ret = QMessageBox::warning(this, tr("Save Query"), tr("Failure: Cannot Save File"), QMessageBox::Cancel);

}

void queryWindow::closeQuery()
{
  this->close();
}

void queryWindow::exportQuery()
{
  QStringList names = getFieldNames();

  mapWindow *map = new mapWindow(currentDSN, currentDBname, names, model, this);
  map->show();
}

void queryWindow::openQuery()
{
  queryFilePath = QFileDialog::getOpenFileName(this, tr("Open Query"), "./query", tr("QRY Files (*.qry)"));

  QFile queryFile(queryFilePath);

  if(queryFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream out(&queryFile);

      QString line = out.readLine();
      QStringList dbInfo = line.split(",");

      if(dbInfo.at(0).contains(currentDSN) && dbInfo.at(1).contains(currentDBname))
        {
          qry = out.readLine();

          queryFile.close();

          //update data model
          ui->editQuery->setPlainText(qry);
          updateQuery();
        }
      else
        int ret = QMessageBox::warning(this, tr("Open Query"), tr("Failure: Incorrect DSN/Database Name"), QMessageBox::Cancel);

      queryFile.close();
    }
  else
    {
      int ret = QMessageBox::warning(this, tr("Open Query"), tr("Failure: Cannot Open File"), QMessageBox::Cancel);
    }

}

QStringList queryWindow::getFieldNames()
{
  //get number of columns
  int colCount = model->columnCount();

  //collect field names and return
  QStringList fNames;
  for(int i = 0; i < colCount; i++)
  {
      fNames.append(model->headerData(i,Qt::Horizontal).toString());
  }
  return fNames;
}
