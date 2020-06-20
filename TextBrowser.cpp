#include "TextBrowser.h"


void TextBrowser::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit mouseDoubleClicked();//发射信号
}
