#include "mapwindow.h"
#include "ui_mapwindow.h"

mapWindow::mapWindow(QString dsn, QString db, const QStringList &fields, const QSqlQueryModel *mod, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::mapWindow)
{
  ui->setupUi(this);

  //set this window to modal
  this->setWindowModality(Qt::ApplicationModal);
  //connect signals and slots
  connect(ui->exportButton, SIGNAL(pressed()), this, SLOT(exportCSV()));
  connect(ui->cancelButton, SIGNAL(pressed()), this, SLOT(cancel()));
  connect(ui->actionCancel, SIGNAL(triggered()), this, SLOT(cancel()));
  connect(ui->actionSave_Map, SIGNAL(triggered()), this, SLOT(saveMap()));
  connect(ui->actionSave_Map_As, SIGNAL(triggered()), this, SLOT(saveMapAS()));
  connect(ui->actionOpen_Map, SIGNAL(triggered()), this, SLOT(openMap()));
  connect(ui->listWidget_3, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(toggleMapBool(QListWidgetItem*)));

  //init
  mapFilePath = "";

  //copy in original field list and data model
  ogFields = fields;
  model = mod;

  //render orig fields
  setOGfields();

  //render new fields to og names as default
  setNewFields(ogFields);

  //render default true map bools
  QStringList mapBools;
  setMapBools(mapBools);

  //store validators
  currentDSN = dsn;
  currentDBname = db;
}

mapWindow::~mapWindow()
{
  delete ui;
}

void mapWindow::cancel()
{
  this->close();
}

void mapWindow::exportCSV()
{
  //get path to save CSV
  QString csvFilePath = QFileDialog::getSaveFileName(this, tr("Save Data CSV"), "./report", tr("CSV Files (*.csv)"));

  //store column and row counts
  int rowCount = model->rowCount();
  int colCount = model->columnCount();

  //open path to write
  QFile csvFile(csvFilePath);

  //write over any existing data
  if(csvFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      QTextStream in(&csvFile);

      QString field = "";
      QStringList newNames = getNewFields();
      QStringList mapBools = getMapBools();

      //determine qty fields for export
      int qtyBools = 0;
      for(int k = 0; k < mapBools.size(); k++)
        {
          if(mapBools.at(k).contains("yes"))
            qtyBools++;
        }

      //counter for exported fields
      int countBools = 0;

      //write with newly mapped field names
      for(int j = 0; j < newNames.size(); j++)
        {
          if(mapBools.at(j).contains("yes"))
            {
              if(countBools == (qtyBools - 1))
                in << "\"" << newNames.at(j) << "\"" << '\n';
              else
                in << "\"" << newNames.at(j) << "\"" << ",";
              countBools++;
            }
        }

      //for each record
      for(int i = 0; i < rowCount; i++)
        {
          //reset bool counter
          countBools = 0;
          //write each field, comma separated
          for(int j = 0; j < colCount; j++)
            {
              if(mapBools.at(j).contains("yes"))
                {
                  QModelIndex index = model->index(i,j);
                  field = model->data(index).toString();
                  if(countBools == (qtyBools - 1))
                    in << "\"" << field << "\"" << '\n';
                  else
                    in << "\"" << field << "\"" << ",";
                  countBools++;
                }
            }
        }
      csvFile.flush();
      csvFile.close();
    }
  else
    int ret = QMessageBox::warning(this, tr("Export Data"), tr("Failure: Cannot Save File"), QMessageBox::Cancel);
}

void mapWindow::saveMap()
{
  if(mapFilePath.isEmpty())
    mapFilePath = QFileDialog::getSaveFileName(this, tr("Save Field Map"), "./fieldMap", tr("CSV Files (*.csv)"));
  exportFieldMap();
}

void mapWindow::saveMapAS()
{
  mapFilePath = QFileDialog::getSaveFileName(this, tr("Save Field Map"), "./fieldMap", tr("CSV Files (*.csv)"));
  exportFieldMap();
}

void mapWindow::openMap()
{
  mapFilePath = QFileDialog::getOpenFileName(this, tr("Open Field Map"), "./fieldMap", tr("CSV Files (*.csv)"));

  QFile mapFile(mapFilePath);

  QStringList newNames, mapBools, validators;

  if(mapFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QTextStream out(&mapFile);

      QString line = out.readLine();
      validators = line.split(",");

      if(validators.at(0).contains(currentDSN) && validators.at(1).contains(currentDBname))
        {

          line = out.readLine();
          mapBools = line.split(",");
          setMapBools(mapBools);

          line = out.readLine();
          ogFields = line.split(",");
          setOGfields();

          line = out.readLine();
          newNames = line.split(",");
          setNewFields(newNames);
        }
      else
        int ret = QMessageBox::warning(this, tr("Open Field Map"), tr("Failure: Incorrect DSN/Database Name"), QMessageBox::Cancel);

      mapFile.close();
    }
  else
    {
      int ret = QMessageBox::warning(this, tr("Open Field Map"), tr("Failure: Cannot Open File"), QMessageBox::Cancel);
    }
}

void mapWindow::exportFieldMap()
{
  QFile mapFile(mapFilePath);

  QStringList newNames = getNewFields();
  QStringList mapBools = getMapBools();

  if(mapFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
      QTextStream in(&mapFile);

      in << currentDSN << "," << currentDBname << '\n';

      for(int h = 0; h < mapBools.size(); h++)
        {
          if(h == (mapBools.size() - 1))
            {
              if(mapBools.at(h).contains("yes"))
                {
                  in << "yes" << '\n';
                }
              else
                {
                  in << "no" << '\n';
                }
            }
          else
            {
              if(mapBools.at(h).contains("yes"))
                {
                  in << "yes" << ",";
                }
              else
                {
                  in << "no" << ",";
                }
            }
        }

      for(int i = 0; i < ogFields.size(); i++)
        {
          if(i == (ogFields.size() - 1))
            in << ogFields.at(i) << '\n';
          else
            in << ogFields.at(i) << ",";
        }

      for(int j = 0; j < newNames.size(); j++)
        {
          if(j == (newNames.size() - 1))
            in << newNames.at(j) << '\n';
          else
            in << newNames.at(j) << ",";
        }

      mapFile.flush();
      mapFile.close();
    }
  else
    {
      int ret = QMessageBox::warning(this, tr("Save Field Map"), tr("Failure: Cannot Save File"), QMessageBox::Cancel);
    }
}

QStringList mapWindow::getNewFields()
{
  QStringList newFields;
  for(int i = 0; i < ui->listWidgetCSV->count(); i++)
    {
      newFields.append(ui->listWidgetCSV->item(i)->text());
    }

  return newFields;
}

void mapWindow::setOGfields()
{
  //clear data
  ui->listWidgetSQL->clear();

  //render orig fields
  ui->listWidgetSQL->addItems(ogFields);

  //set og fields alignment to right justification
  for(int i = 0; i < ui->listWidgetSQL->count(); i++)
    {
      ui->listWidgetSQL->item(i)->setTextAlignment(Qt::AlignRight);
    }
}

void mapWindow::setNewFields(QStringList l)
{
  //clear data
  ui->listWidgetCSV->clear();

  //render new fields to og names as default
  ui->listWidgetCSV->addItems(l);

  //set new fields alignment to right justification
  for(int i = 0; i < ui->listWidgetCSV->count(); i++)
    {
      ui->listWidgetCSV->item(i)->setTextAlignment(Qt::AlignLeft);
      ui->listWidgetCSV->item(i)->setFlags(ui->listWidgetCSV->item(i)->flags() | Qt::ItemIsEditable);
    }
}

void mapWindow::setMapBools(QStringList l)
{
  //clear data
  ui->listWidget_3->clear();

  //set and render each field bool
  if(!l.isEmpty())
    {
      for(int x = 0; x < l.size(); x++)
        {
          if(l.at(x).contains("yes"))
            {
              ui->listWidget_3->addItem("-->");
            }
          else
            ui->listWidget_3->addItem(" X ");
          ui->listWidget_3->item(x)->setTextAlignment(Qt::AlignCenter);
        }
    }
  else
    {
      for(int i = 0; i < ogFields.size(); i++)
        {
          ui->listWidget_3->addItem("-->");
          ui->listWidget_3->item(i)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

void mapWindow::toggleMapBool(QListWidgetItem *item)
{
  QString currentState = item->text();
  if(currentState.contains("-->"))
    {
      item->setText(" X ");
    }
  else
    {
      item->setText("-->");
    }
}

QStringList mapWindow::getMapBools()
{
  QStringList mapBools;
  for(int i = 0; i < ui->listWidget_3->count(); i++)
    {
      if(ui->listWidget_3->item(i)->text().contains("X"))
        mapBools.append("no");
      else
        mapBools.append("yes");
    }

  return mapBools;

}
