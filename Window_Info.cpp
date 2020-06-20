#include "Window_Info.h"



Window_Info::Window_Info(QString _info,QWidget *parent,const QString &title,const bool &is_file_path,const QSize& size)
    :QDialog(parent)
    ,filePath(_info)
{
    using namespace TextPackage;

    //设置自身属性
//    this->setWindowFlag(Qt::WindowModal,true);
    this->setWindowTitle(title);


    this->setMinimumSize(size);

    this->textBrowser=new QTextBrowser();
    this->line=new QFrame();

    line->setFrameShape(QFrame::HLine);

    this->pushBtn_Close=new QPushButton("Close");

    //创建布局
    this->gridLayout_main=new QGridLayout(this);

    //布局添加组件
    gridLayout_main->addWidget(textBrowser,0,0);//添加主groupBox
    gridLayout_main->addWidget(line,1,0);
    gridLayout_main->addWidget(pushBtn_Close,2,0,Qt::AlignRight);//添加关闭按钮

    pushBtn_Close->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //连接信号
    connect(pushBtn_Close,&QPushButton::clicked,this,&QDialog::hide);//关闭按钮

    //设置字体
    QFont font_tmp("Microsoft YaHei");
    textBrowser->setFont(font_tmp);

    if(is_file_path)
        this->reloadContent();
    else
    {
        textBrowser->setText(_info);
        textBrowser->setFontPointSize(14);
    }

}

void Window_Info::reloadContent()
{
    textBrowser->setSource(filePath);//设置源(基于扩展名识别)
}

void Window_Info::setConetent(const QString &content)
{
    this->textBrowser->setPlainText(content);
}


