#ifndef QUERYEDITOR_H
#define QUERYEDITOR_H

#include <QPlainTextEdit>

class queryEditor : public QPlainTextEdit
{
  Q_OBJECT
public:
  explicit queryEditor(QWidget *parent = 0);

private:
  void keyPressEvent(QKeyEvent *e);

signals:
  void pressedEnter();

public slots:

};

#endif // QUERYEDITOR_H
