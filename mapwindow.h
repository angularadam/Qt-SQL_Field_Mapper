#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
  class mapWindow;
}

class mapWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit mapWindow(QString dsn, QString db, const QStringList &fields, const QSqlQueryModel *mod, QWidget *parent = 0);
  ~mapWindow();

public slots:

  //closes this dialog
  void cancel();

  //save field map
  void saveMap();

  //save field map as
  void saveMapAS();

  //open a field map file
  void openMap();

  //writes data model to CSV using the field map
  void exportCSV();

  //toggles field mapping bool
  void toggleMapBool(QListWidgetItem *item);

private:
  Ui::mapWindow *ui;

  //path and filename of current map file
  QString mapFilePath;

  //db validators
  QString currentDSN, currentDBname;

  //original SQL field names
  QStringList ogFields;

  //SQL data
  const QSqlQueryModel *model;

  //writes field map to disk
  void exportFieldMap();

  //get new field names
  QStringList getNewFields();

  //get current mapping bools
  QStringList getMapBools();

  //set original field names
  void setOGfields();

  //set new field names
  void setNewFields(QStringList l);

  //set mapping bools
  void setMapBools(QStringList l);

};

#endif // MAPWINDOW_H
