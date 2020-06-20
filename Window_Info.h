#ifndef WINDOW_ABOUT_H
#define WINDOW_ABOUT_H

#include <QDialog>
#include <QTextBrowser>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QFont>
#include <QSplitter>

#include "Text.h"

//关于窗口类
class Window_Info:public QDialog
{

public:
    //构造函数
    explicit Window_Info
    (QString filePath,QWidget *parent=nullptr,const QString &title="...",const bool &is_file_path=true,const QSize& size={600,600});

    //加载内容(文件)
    void reloadContent();
    //设置内容
    void setConetent(const QString & content);


private:
    QString filePath;

private:
    QTextBrowser *textBrowser;//文本显示框

    QGridLayout *gridLayout_main;//主布局

    QPushButton *pushBtn_Close;//关闭按钮

    QFrame *line;//分割线




};











#endif // WINDOW_ABOUT_H
