#ifndef QUERYWINDOW_H
#define QUERYWINDOW_H

#include <QMainWindow>

#include <QtSql>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QMessageBox>

#include <QDebug>

#include "mapwindow.h"
#include "queryeditor.h"


namespace Ui {
  class queryWindow;
}

class queryWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit queryWindow(QString dsn, QString db, QString tableName, QWidget *parent = 0);
  ~queryWindow();

public slots:
  void updateQuery();
  void saveQuery();
  void saveQueryAs();
  void closeQuery();
  void exportQuery();
  void openQuery();

private:
  Ui::queryWindow *ui;

  //data model store
  QSqlQueryModel *model;

  //query string
  QString qry;

  //DSN and db name for query
  QString currentDSN, currentDBname;

  //query file path
  QString queryFilePath;

  //gets list of field names
  QStringList getFieldNames();
};

#endif // QUERYWINDOW_H
