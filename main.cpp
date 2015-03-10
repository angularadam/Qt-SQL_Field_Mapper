#include "vwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  vWindow v;
  //w.show();

  return a.exec();
}
