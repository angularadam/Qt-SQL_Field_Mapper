#ifndef VWINDOW_H
#define VWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQueryModel>
#include <QHeaderView>

#include <QDebug>

#include "logindialog.h"
#include "mapwindow.h"
#include "querywindow.h"

namespace Ui {
  class vWindow;
}

class vWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit vWindow(QWidget *parent = 0);
  ~vWindow();

signals:

public slots:
  void slotAcceptUserLogin(QString&, QString&);
  void closeDB();
  void chooseTable(QModelIndex);
  void login();
  void reLogin();
  void exportCSV();
  void loginAborted();
  void queryDatabase();

private:
  Ui::vWindow *ui;
  //model with table info
  QSqlQueryModel *model;
  //model with field info
  QSqlQueryModel *fieldModel;
  //QSqlTableModel *model;
  //QSqlRelationalTableModel *model;
  QSqlDatabase db;
  QString currentDSN, currentDBname, currentTableName;

  QStringList getFieldNames();

};

#endif // VWINDOW_H
