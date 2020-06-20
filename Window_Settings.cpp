#include "Window_Settings.h"
#include "MainWindow.h"

const int Window_Settings::fixedGroupBoxHeight=50;
const int Window_Settings::minWidth=100;

//读取配置文件(不存在则创建)
void Window_Settings::loadConfigFile()
{
    QString lineContent;
    QString path=Settings::filePath_conf;//获取配置文件路径
    QFile file_conf(path);//设置文件
    int i;
    QTextStream ts;

    if(file_conf.exists())//文件存在
    {
        if(file_conf.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            //读取格式模式
            lineContent=file_conf.readLine();//抛弃一行内容
            lineContent=file_conf.readLine();//读取一行内容
            if(lineContent.isEmpty())
                throw QString("<Exception> ReadingFileError");

            ts.setString(&lineContent,QIODevice::ReadOnly);//设置字符串(只读)

            ts>>i;//读取一个int类型值
            this->format=i;


            //读取精度值
            lineContent=file_conf.readLine();//抛弃一行内容
            lineContent=file_conf.readLine();//读取一行内容
            if(lineContent.isEmpty())
                throw QString("<Exception> ReadingFileError");
            ts.setString(&lineContent,QIODevice::ReadOnly);
            ts>>i;//读取一个int类型值
            this->precision=i;

            //读取角单位
            lineContent=file_conf.readLine();//抛弃一行内容
            lineContent=file_conf.readLine();//读取一行内容
            if(lineContent.isEmpty())
                throw QString("<Exception> ReadingFileError");
            ts.setString(&lineContent,QIODevice::ReadOnly);
            ts>>i;//读取一个int类型值
            this->angleUnit=i;

            //读取是否显示分隔符
            lineContent=file_conf.readLine();//抛弃一行内容
            lineContent=file_conf.readLine();//读取一行内容
            if(lineContent.isEmpty())
                throw QString("<Exception> ReadingFileError");
            ts.setString(&lineContent,QIODevice::ReadOnly);
            ts>>i;//读取一个int类型值
            this->displayDigitSeparator=i;


            //检查从文件读取的内容
            if(!(format>=0&&format<=2))
                throw QString("<Exception> ConfigFileContentIllegal");
            if(!(angleUnit>=0&&angleUnit<=1))
                throw QString("<Exception> ConfigFileContentIllegal");
            if(!(displayDigitSeparator==false||displayDigitSeparator==true))
                throw QString("<Exception> ConfigFileContentIllegal");
            if(precision<0)
                throw QString("<Exception> ConfigFileContentIllegal");

        }
        else
            throw QString("<Exception> FailToOpenFile");
    }
    else//文件不存在
    {
        if(file_conf.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            ts.setDevice(&file_conf);//设置设备
            ts<<"#format:\n"<<this->format<<endl;
            ts<<"#precision:\n"<<this->precision<<endl;
            ts<<"#angleUnit:\n"<<this->angleUnit<<endl;
            ts<<"#displayDigitSeparator:\n"<<this->displayDigitSeparator<<endl;
            ts.flush();
            file_conf.close();//关闭文件
        }
        else
            throw QString("<Exception> FailToCreateFile");
    }

    setWidgets();//设置组件状态
    return;
}

//保存配置到文件
void Window_Settings::saveConfigToFile()
{
    QString path=Settings::filePath_conf;//获取配置文件路径
    QFile file_conf(path);//设置文件
    QTextStream ts;

    //根据用户选择更改设置
    int i;
    for(i=0;i<3;i++)
    {
        if(btnList_FormatSelect[i]->isChecked())
        {
            format=i;
            break;
        }
    }
    precision=this->spinBox_Precision->value();
    for(i=0;i<2;i++)
    {
        if(btnList_AngleUnitSelect[i]->isChecked())
        {
            angleUnit=i;
            break;
        }
    }
    displayDigitSeparator=checkBox_DigitSeparatorDisplay->isChecked();//


    //写入到文件
    if(file_conf.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
    {
        ts.setDevice(&file_conf);//设置设备
        ts<<"#format:\n"<<this->format<<endl;
        ts<<"#precision:\n"<<this->precision<<endl;
        ts<<"#angleUnit:\n"<<this->angleUnit<<endl;
        ts<<"#displayDigitSeparator:\n"<<this->displayDigitSeparator<<endl;
        ts.flush();
        file_conf.close();//关闭文件
        qDebug()<<"FileSaved";
//        static_cast<MainWindow*>(this->parent())->label_StatusBar->setText("FileSaved");
        static_cast<MainWindow*>(this->parent())->pushNewMessage("FilesSaved");
    }
    else
    {
        throw QString("<Exception> OpenFile");
    }
}

void Window_Settings::setWidgets()
{
    //设置初始选中的按钮
    btnList_FormatSelect[this->format]->setChecked(true);

    spinBox_Precision->setValue(precision);

    btnList_AngleUnitSelect[this->angleUnit]->setChecked(true);

    checkBox_DigitSeparatorDisplay->setChecked(displayDigitSeparator);

}

void Window_Settings::updateCalculatorSettings()
{
    //根据设置设定计算器
    calculator.setFormat
    (
        static_cast<NS_Calculator::Calculator::FormatMode>(this->format)
    );
    calculator.setPrecision(this->precision);
    calculator.setAngleUnit
    (
        static_cast<NS_Calculator::Calculator::AngleUnit>(this->angleUnit)
    );
    calculator.setDisplayDigitSeparator(this->displayDigitSeparator);
}

void Window_Settings::initSingalSlots()
{
    //保存按钮
    connect(pushBtn_Apply,&QPushButton::clicked,this,&Window_Settings::saveConfigToFile);
    connect(pushBtn_Apply,&QPushButton::clicked,this,&Window_Settings::updateCalculatorSettings);

    //取消按钮
    connect(pushBtn_Cancel,&QPushButton::clicked,this,&QWidget::hide);

    //OK按钮
    connect(pushBtn_ApplyAndHide,&QPushButton::clicked,this,&Window_Settings::saveConfigToFile);
    connect(pushBtn_ApplyAndHide,&QPushButton::clicked,this,&Window_Settings::updateCalculatorSettings);
    connect(pushBtn_ApplyAndHide,&QPushButton::clicked,this,&QWidget::hide);

}

Window_Settings::Window_Settings(QWidget *parent)
    :QWidget (parent)
    ,format(2)
    ,precision(2)
    ,angleUnit(1)
    ,displayDigitSeparator(false)
{
    this->setWindowTitle("Settings");//改变标题
    this->setWindowFlag(Qt::Window,true);//设置成窗口
//    this->setWindowFlag(Qt::Dialog,true);//设置成对话框
    this->setWindowModality(Qt::WindowModal);//窗口模态
    this->setWindowFlag(Qt::CustomizeWindowHint,true);//使用自定义的标题栏
    this->setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    this->setWindowFlag(Qt::WindowMinimizeButtonHint,false);
//    this->setWindowFlag(Qt::WindowCloseButtonHint,true);



    //创建主区域
    groupBox_main=new QGroupBox("Settings",this);

    //创建groupBox
    this->groupBox_FormatSelect=new QGroupBox("Format",this);
    groupBox_FormatSelect->setFixedHeight(fixedGroupBoxHeight);
    this->groupBox_Precision=new QGroupBox("Percision",this);
    groupBox_Precision->setFixedHeight(fixedGroupBoxHeight);
    this->groupBox_AngelUnitSelect=new QGroupBox("AngleUnit",this);
    groupBox_AngelUnitSelect->setFixedHeight(fixedGroupBoxHeight);
    this->groupBox_DigitSeparatorDisplaySwitch=new QGroupBox("DisplayDigitSeparator",this);
    groupBox_DigitSeparatorDisplaySwitch->setFixedHeight(fixedGroupBoxHeight);

    //创建组件
    this->radioBtn_Fix=new QRadioButton("Fix",groupBox_FormatSelect);
    radioBtn_Fix->setMinimumWidth(minWidth);
    this->radioBtn_Sci=new QRadioButton("Sci",groupBox_FormatSelect);
    radioBtn_Sci->setMinimumWidth(minWidth);
    this->radioBtn_Norm=new QRadioButton("Norm",groupBox_FormatSelect);
    radioBtn_Norm->setMinimumWidth(minWidth);

    //互斥按钮指针入列表
    btnList_FormatSelect
            <<radioBtn_Fix
            <<radioBtn_Sci
            <<radioBtn_Norm
    ;

    this->spinBox_Precision=new QSpinBox(groupBox_Precision);
    spinBox_Precision->setMinimumWidth(minWidth);
    spinBox_Precision->setMinimum(0);//设置最小值
    spinBox_Precision->setMaximum(20);//设置最大值

    this->radioBtn_Deg=new QRadioButton("Deg",groupBox_AngelUnitSelect);
    radioBtn_Deg->setMinimumWidth(minWidth);
    this->radioBtn_Rad=new QRadioButton("Rad",groupBox_AngelUnitSelect);
    radioBtn_Rad->setMinimumWidth(minWidth);

    btnList_AngleUnitSelect
            <<radioBtn_Deg
            <<radioBtn_Rad
    ;

    this->checkBox_DigitSeparatorDisplay=new QCheckBox("SeparatorDisplay",this);
    checkBox_DigitSeparatorDisplay->setMinimumWidth(minWidth);

    this->pushBtn_Apply=new QPushButton("Apply",this);//保存按钮
    this->pushBtn_Cancel=new QPushButton("Cancel",this);//取消按钮
    this->pushBtn_ApplyAndHide=new QPushButton("OK",this);//OK按钮

    //创建布局
    this->gridLayout_main=new QGridLayout(this);//主布局
    this->vBoxLayout_ScrollArea=new QVBoxLayout(groupBox_main);//滚动区域的布局
    this->hBoxLayout_groupBox_FormatSelect=new QHBoxLayout(groupBox_FormatSelect);//第一个组的布局
    this->hBoxLayout_groupBox_Precision=new QHBoxLayout(groupBox_Precision);//第二个组的布局
    this->hBoxLayout_groupBox_AngleUnitSelect=new QHBoxLayout(groupBox_AngelUnitSelect);//第三个组的布局
    this->hBoxLayout_groupBox_DigitSeparatorDisplaySwitch=new QHBoxLayout(groupBox_DigitSeparatorDisplaySwitch);//第四个组的布局
    this->hBoxLayout_buttons=new QHBoxLayout();//底部按钮不江湖


    //布局添加组件
    vBoxLayout_ScrollArea->addWidget(groupBox_FormatSelect);
    vBoxLayout_ScrollArea->addWidget(groupBox_Precision);
    vBoxLayout_ScrollArea->addWidget(groupBox_AngelUnitSelect);
    vBoxLayout_ScrollArea->addWidget(groupBox_DigitSeparatorDisplaySwitch);
    vBoxLayout_ScrollArea->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Minimum,QSizePolicy::Expanding));
    vBoxLayout_ScrollArea->setSpacing(20);

    gridLayout_main->addWidget(groupBox_main,0,0);
    gridLayout_main->setRowStretch(1,1);
    gridLayout_main->addLayout(hBoxLayout_buttons,2,0);

//    hBoxLayout_buttons->insertSpacerItem(0,new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding));
    hBoxLayout_buttons->addStretch(1);

    pushBtn_ApplyAndHide->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pushBtn_Cancel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    pushBtn_Apply->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    hBoxLayout_buttons->addWidget(pushBtn_ApplyAndHide,1);//主布局添加OK按钮
    hBoxLayout_buttons->addWidget(pushBtn_Cancel,2);//主布局添加取消按钮
    hBoxLayout_buttons->addWidget(pushBtn_Apply,3);//主布局添加应用按钮

    //groupBox布局添加组件
    hBoxLayout_groupBox_FormatSelect->addWidget(radioBtn_Fix);
    hBoxLayout_groupBox_FormatSelect->addWidget(radioBtn_Sci);
    hBoxLayout_groupBox_FormatSelect->addWidget(radioBtn_Norm);
    hBoxLayout_groupBox_FormatSelect->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum));

    hBoxLayout_groupBox_Precision->addWidget(spinBox_Precision);
    hBoxLayout_groupBox_Precision->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum));

    hBoxLayout_groupBox_AngleUnitSelect->addWidget(radioBtn_Deg);
    hBoxLayout_groupBox_AngleUnitSelect->addWidget(radioBtn_Rad);
    hBoxLayout_groupBox_AngleUnitSelect->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum));

    hBoxLayout_groupBox_DigitSeparatorDisplaySwitch->addWidget(checkBox_DigitSeparatorDisplay);
    hBoxLayout_groupBox_DigitSeparatorDisplaySwitch->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum));

    initSingalSlots();//初始化信号槽
}



