#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <QTextBrowser>

class TextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    TextBrowser()=default;

    void mouseDoubleClickEvent(QMouseEvent* event)override;

signals:
    //鼠标(左键)双击信号
    void mouseDoubleClicked();

};

#endif // TEXTBROWSER_H
