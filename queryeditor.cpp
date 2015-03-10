#include "queryeditor.h"

queryEditor::queryEditor(QWidget *parent) :
  QPlainTextEdit(parent)
{
}

void queryEditor::keyPressEvent(QKeyEvent *e)
{
  if(e->key()==Qt::Key_Return)
    {
      emit pressedEnter();
    }
  else
    {
      QPlainTextEdit::keyPressEvent( e );
    }
}
