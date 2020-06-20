#include "MainWindow.h"
#include "ui_mainwindow.h"

//导入命名空间(文本包)
using namespace TextPackage;


//const int MainWindow::maxCharNum=150;//行编辑框中最大字符数
//const int MainWindow::frameFixedHeight=50;
//const int MainWindow::buttonFixedHeight=45;
//const int MainWindow::buttonMinWidth=90;

//全局变量定义
NS_Calculator::Calculator calculator;
int crtView;//当前视图
int crtRecordIndex=-1;
QList<Record> history;//历史记录


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    ,font_main("Consolas",18)
{
    //以下这条语句不会起作用,因为不在布局管理中
//    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    this->resize(this->sizeHint());
    this->setWindowTitle(text_MainTitle);
    //初始化

    menuBar_main=this->menuBar();//创建一个菜单栏,并且获取指针
    this->setMenuBar(menuBar_main);
    statusBar_main=this->statusBar();//创建一个状态栏,并且获取指针
    statusBar_main->setFixedHeight(20);
    this->setStatusBar(statusBar_main);
    widget_main=new QWidget(this);//创建中央组件
    this->setCentralWidget(widget_main);//设置中央组件

    initWidgets();//初始化组件

    //菜单添加action
    menu_start->addAction(action_Clear);
    menu_start->addSeparator();//-----
    menu_start->addAction(action_Quit);

    //动作组添加互斥动作
    actionGroup_view->addAction(action_EmptyView);
    actionGroup_view->addAction(action_SimpleView);
    actionGroup_view->addAction(action_StandardView);
    actionGroup_view->addAction(action_ScienceView);

    menu_view->addAction(action_EmptyView);
    menu_view->addAction(action_SimpleView);
    menu_view->addAction(action_StandardView);
    menu_view->addAction(action_ScienceView);

    menu_option->addAction(action_ClearHistory);
    menu_option->addSeparator();//-----
    menu_option->addAction(action_Settings);


    menu_help->addAction(action_About);
    menu_help->addSeparator();//-----
    menu_help->addAction(action_Help);

    //菜单栏添加菜单
    menuBar_main->addMenu(menu_start);
    menuBar_main->addMenu(menu_view);
    menuBar_main->addMenu(menu_option);
    menuBar_main->addMenu(menu_help);

    initLayouts();//初始化布局
    initSingalSlots();//初始化信号槽

    timer.start();//开启线程

    window_Settings->loadConfigFile();//读取配置文件
    window_Settings->updateCalculatorSettings();//根据配置进行设置
}

MainWindow::~MainWindow()
{
    timer.status=Timer_StatusBar::Over;
    timer.wait();
}

//初始化函数
void MainWindow::initWidgets()
{
    //动作组
    actionGroup_view=new QActionGroup(this);

    //状态栏显示label
    label_StatusBar=new QLabel(this->statusBar_main);
    label_StatusBar->setFixedSize(1000,20);
    QFont tempFont("Consolas",14);
    tempFont.setBold(true);//加粗
    label_StatusBar->setFont(tempFont);

    //初始化action
    action_Quit=new QAction("Quit",this);
    action_Quit->setIcon(QIcon(":/icons/Resource/Icon_Out_16.png"));

    action_StandardView=new QAction("Standard",this);
    action_StandardView->setCheckable(true);
    action_ScienceView=new QAction("Science",this);
    action_ScienceView->setCheckable(true);
    action_SimpleView=new QAction("Simple",this);
    action_SimpleView->setCheckable(true);
    action_EmptyView=new QAction("Empty",this);
    action_EmptyView->setCheckable(true);

    action_Settings=new QAction("Settings",this);
    action_Settings->setIcon(QIcon(":/icons/Resource/Icon_Gear_16.png"));
    action_ClearHistory=new QAction("ClearHistory",this);
    action_ClearHistory->setIcon(QIcon(":/icons/Resource/Icon_TrashBin_16.png"));
    action_About=new QAction("About",this);
    action_Help=new QAction("HowToUse",this);
    action_Clear=new QAction("Clear",this);
    action_Clear->setIcon(QIcon(":/icons/Resource/Icon_Reset_16.png"));

    //初始化菜单栏中的四个菜单
    menu_start=new QMenu("Start",this);
    menu_view=new QMenu("View",this);
    menu_option=new QMenu("Option",this);
    menu_help=new QMenu("Help",this);


    //行编辑框
    lineEdit=new QLineEdit();
    lineEdit->setFont(font_main);
    lineEdit->setMinimumWidth(300);
    lineEdit->setMaxLength(Settings::maxCharNum);//设置最大允许输入的字符数
    lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    lineEdit->setFixedHeight(Settings::frameFixedHeight);//这个是便利函数,内部实现相当于以下两条
    lineEdit->setClearButtonEnabled(true);//开启自带的清除按钮

    //显示框
    textBrowser=new TextBrowser();
    textBrowser->setFont(font_main);
    textBrowser->setAlignment(Qt::AlignRight|Qt::AlignVCenter);//设置对齐方式(右对齐且垂直方向居中)
    textBrowser->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    textBrowser->setFixedHeight(Settings::frameFixedHeight);
    textBrowser->setToolTip(TextPackage::toolTip_resultTextBrowser);//设置提示信息

    //框
    groupBox_Main=new QGroupBox("Calculator");
    groupBox_Main->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    groupBox_Buttons=new QGroupBox("");
    groupBox_Buttons->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
//    groupBox_Main->setContentsMargins(0,0,10,10);

//    lineEdit->setMinimumHeight(30);
//    lineEdit->setMaximumHeight(30);

    //基本按钮
    pushButton_N0=new QPushButton("0");
    pushButton_N1=new QPushButton("1");
    pushButton_N2=new QPushButton("2");
    pushButton_N3=new QPushButton("3");
    pushButton_N4=new QPushButton("4");
    pushButton_N5=new QPushButton("5");
    pushButton_N6=new QPushButton("6");
    pushButton_N7=new QPushButton("7");
    pushButton_N8=new QPushButton("8");
    pushButton_N9=new QPushButton("9");
    pushButton_Point=new QPushButton(".");

    pushButton_Plus=new QPushButton("+");
    pushButton_Minu=new QPushButton("-");
    pushButton_Mult=new QPushButton("*");
    pushButton_Divi=new QPushButton("/");
    pushButton_Pow=new QPushButton("^");
    pushButton_Fact=new QPushButton("!");
    pushButton_Mod=new QPushButton("Mod");
    pushButton_Abs=new QPushButton("Abs");
    pushButton_Log=new QPushButton("log");
    pushButton_Sin=new QPushButton("sin");
    pushButton_Cos=new QPushButton("cos");
    pushButton_Tan=new QPushButton("tan");
    pushButton_ASin=new QPushButton("arcsin");
    pushButton_ACos=new QPushButton("arccos");
    pushButton_ATan=new QPushButton("arctan");
    pushButton_Cot=new QPushButton("cot");
    pushButton_Arra=new QPushButton("ARR");
    pushButton_Comb=new QPushButton("COM");
    pushButton_BraL=new QPushButton("(");
    pushButton_BraR=new QPushButton(")");

    pushButton_Calculate=new QPushButton("=");
    pushButton_Backspace=new QPushButton("DEL");
    pushButton_Clear=new QPushButton("C");

    pushButton_Sqrt=new QPushButton("Sqrt");
    pushButton_Reciprocal=new QPushButton("x^-1");
    pushButton_Square=new QPushButton("x^2");
    pushButton_10log=new QPushButton("10log");
    pushButton_elog=new QPushButton("elog");
    pushButton_10pow=new QPushButton("10^x");


    //按钮指针放入列表中
    buttonList
    <<pushButton_N0
    <<pushButton_N1
    <<pushButton_N2
    <<pushButton_N3
    <<pushButton_N4
    <<pushButton_N5
    <<pushButton_N6
    <<pushButton_N7
    <<pushButton_N8
    <<pushButton_N9
    <<pushButton_Point

    <<pushButton_Plus
    <<pushButton_Minu
    <<pushButton_Mult
    <<pushButton_Divi
    <<pushButton_Pow
    <<pushButton_Fact
    <<pushButton_Mod
    <<pushButton_Abs
    <<pushButton_Log
    <<pushButton_Sin
    <<pushButton_Cos
    <<pushButton_Tan
    <<pushButton_ASin
    <<pushButton_ACos
    <<pushButton_ATan
    <<pushButton_Cot
    <<pushButton_Arra
    <<pushButton_Comb
    <<pushButton_BraL
    <<pushButton_BraR

    <<pushButton_Calculate
    <<pushButton_Backspace
    <<pushButton_Clear

    <<pushButton_Sqrt
    <<pushButton_Reciprocal
    <<pushButton_Square
    <<pushButton_10log
    <<pushButton_elog
    <<pushButton_10pow
    ;

    int size=buttonList.size();
    for(int i=0;i<size;i++)//遍历列表进行设置
    {
        buttonList[i]->setFixedHeight(Settings::buttonFixedHeight);
        buttonList[i]->setFont(font_main);
        buttonList[i]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
        buttonList[i]->setMinimumWidth(Settings::buttonMinWidth);
        buttonList[i]->setFocusPolicy(Qt::NoFocus);//无焦点
    }

    //创建其他窗口(帮助等等)
    window_Settings=new Window_Settings(this);

    window_About=new Window_Info(Settings::filePath_about_MD,this,"about");//关于
    window_Help=new Window_Info(Settings::filePath_read_me_MD,this,"help");//帮助

    window_result_detail=new Window_Info("",this,"result",false,QSize(400,300));//结果显示

}

//初始化布局
void MainWindow::initLayout_EmptyView()
{
//    gridLayout_groupBoxButtons=new QGridLayout();
}

void MainWindow::initLayout_SimpleView()
{
//    gridLayout_groupBoxButtons=new QGridLayout();
    gridLayout_groupBoxButtons->addWidget(pushButton_Clear,0,0,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Backspace,0,1,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Calculate,0,2,1,2);
}

void MainWindow::initLayout_StandardView()
{
//    gridLayout_groupBoxButtons=new QGridLayout();
    gridLayout_groupBoxButtons->addWidget(pushButton_Plus,3,0);
    gridLayout_groupBoxButtons->addWidget(pushButton_Minu,4,0);
    gridLayout_groupBoxButtons->addWidget(pushButton_Mult,5,0);
    gridLayout_groupBoxButtons->addWidget(pushButton_Divi,6,0);

    gridLayout_groupBoxButtons->addWidget(pushButton_N7,3,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N8,3,2);
    gridLayout_groupBoxButtons->addWidget(pushButton_N9,3,3);

    gridLayout_groupBoxButtons->addWidget(pushButton_N4,4,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N5,4,2);
    gridLayout_groupBoxButtons->addWidget(pushButton_N6,4,3);

    gridLayout_groupBoxButtons->addWidget(pushButton_N1,5,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N2,5,2);
    gridLayout_groupBoxButtons->addWidget(pushButton_N3,5,3);

    gridLayout_groupBoxButtons->addWidget(pushButton_Point,6,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N0,6,2);
    gridLayout_groupBoxButtons->addWidget(pushButton_Calculate,6,3);

//    gridLayout_StandardView->addWidget(pushButton_BraL,2,0);
    gridLayout_groupBoxButtons->addWidget(pushButton_Backspace,2,3);
    gridLayout_groupBoxButtons->addWidget(pushButton_BraL,2,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_BraR,2,2);
    gridLayout_groupBoxButtons->addWidget(pushButton_Clear,2,0);
}

void MainWindow::initLayout_ScienceView()
{
//    gridLayout_groupBoxButtons=new QGridLayout();
    //第一行
    gridLayout_groupBoxButtons->addWidget(pushButton_Sqrt,0,0,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Pow,0,1,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Reciprocal,0,2,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Square,0,3,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Clear,0,4,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_BraL,0,5,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_BraR,0,6,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Backspace,0,7,1,1);

    //第二行
    gridLayout_groupBoxButtons->addWidget(pushButton_Sin,1,0,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Cos,1,1,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Tan,1,2,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Cot,1,3,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Plus,1,4,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N7,1,5,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N8,1,6,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N9,1,7,1,1);

    //第三行
    gridLayout_groupBoxButtons->addWidget(pushButton_ASin,2,0,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_ACos,2,1,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_ATan,2,2,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Abs,2,3,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Minu,2,4,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N4,2,5,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N5,2,6,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N6,2,7,1,1);

    //第四行
    gridLayout_groupBoxButtons->addWidget(pushButton_Log,3,0,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_10log,3,1,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_elog,3,2,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Fact,3,3,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Mult,3,4,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N1,3,5,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N2,3,6,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N3,3,7,1,1);

    //第五行
    gridLayout_groupBoxButtons->addWidget(pushButton_Arra,4,0,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Comb,4,1,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Mod,4,2,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_10pow,4,3,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Divi,4,4,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Point,4,5,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_N0,4,6,1,1);
    gridLayout_groupBoxButtons->addWidget(pushButton_Calculate,4,7,1,1);


}

void MainWindow::initLayouts()
{
    vBoxLayout_main=new QVBoxLayout();//主布局
    vBoxLayout_main->setSizeConstraint(QLayout::SetMinimumSize);

    vBoxLayout_groupBoxMain=new QVBoxLayout();
    vBoxLayout_groupBoxMain->setSizeConstraint(QLayout::SetMinimumSize);
    vBoxLayout_groupBoxMain->addWidget(lineEdit);//上布局加入编辑框
    vBoxLayout_groupBoxMain->addWidget(textBrowser);//上布局加入显示框
    groupBox_Main->setLayout(vBoxLayout_groupBoxMain);

    vBoxLayout_main->addWidget(groupBox_Main);

    spacer_main=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    gridLayout_groupBoxButtons=new QGridLayout();//创建按钮布局
    gridLayout_groupBoxButtons->setSizeConstraint(QLayout::SetMinimumSize);

    initLayout_StandardView();
    this->action_StandardView->setChecked(true);
//    initLayout_EmptyView();
//    initLayout_SimpleView();
//    initLayout_ScienceView();

    //创建并在布局中添加弹簧
    spacer_main=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);//弹簧
    vBoxLayout_main->addItem(spacer_main);//添加弹簧

    groupBox_Buttons->setLayout(gridLayout_groupBoxButtons);//按钮组设置按钮布局

    vBoxLayout_main->addWidget(groupBox_Buttons);//主布局添加按钮组
    this->widget_main->setLayout(vBoxLayout_main);//中央组件设置布局

}

//初始化信号槽
void MainWindow::initSingalSlots()
{
    //连接action信号
    connect(action_Clear,&QAction::triggered,this,&MainWindow::on_pushButton_Clear_clicked);//clear按钮
    connect(action_Quit,&QAction::triggered,this,&MainWindow::on_action_Quit_triggered);//quit按钮
    connect(action_Settings,&QAction::triggered,this,&MainWindow::on_action_Settings_triggered);//settings按钮
    connect(action_About,&QAction::triggered,this,&MainWindow::on_action_About_triggered);//关于
    connect(action_Help,&QAction::triggered,this,&MainWindow::on_action_Help_triggered);//帮助
    connect(action_ClearHistory,&QAction::triggered,this,&MainWindow::on_action_ClearHistory_triggered);//帮助

    //连接actionGroup信号
    connect(actionGroup_view,&QActionGroup::triggered,this,&MainWindow::on_actionGroup_view_triggered);

    //连接按钮信号
    connect(pushButton_N0,&QPushButton::clicked,this,&MainWindow::on_pushButton_N0_clicked);
    connect(pushButton_N1,&QPushButton::clicked,this,&MainWindow::on_pushButton_N1_clicked);
    connect(pushButton_N2,&QPushButton::clicked,this,&MainWindow::on_pushButton_N2_clicked);
    connect(pushButton_N3,&QPushButton::clicked,this,&MainWindow::on_pushButton_N3_clicked);
    connect(pushButton_N4,&QPushButton::clicked,this,&MainWindow::on_pushButton_N4_clicked);
    connect(pushButton_N5,&QPushButton::clicked,this,&MainWindow::on_pushButton_N5_clicked);
    connect(pushButton_N6,&QPushButton::clicked,this,&MainWindow::on_pushButton_N6_clicked);
    connect(pushButton_N7,&QPushButton::clicked,this,&MainWindow::on_pushButton_N7_clicked);
    connect(pushButton_N8,&QPushButton::clicked,this,&MainWindow::on_pushButton_N8_clicked);
    connect(pushButton_N9,&QPushButton::clicked,this,&MainWindow::on_pushButton_N9_clicked);
    connect(pushButton_Point,&QPushButton::clicked,this,&MainWindow::on_pushButton_Point_clicked);

    connect(pushButton_Plus,&QPushButton::clicked,this,&MainWindow::on_pushButton_Plus_clicked);
    connect(pushButton_Minu,&QPushButton::clicked,this,&MainWindow::on_pushButton_Minu_clicked);
    connect(pushButton_Mult,&QPushButton::clicked,this,&MainWindow::on_pushButton_Mult_clicked);
    connect(pushButton_Divi,&QPushButton::clicked,this,&MainWindow::on_pushButton_Divi_clicked);
    connect(pushButton_Pow,&QPushButton::clicked,this,&MainWindow::on_pushButton_Pow_clicked);
    connect(pushButton_Fact,&QPushButton::clicked,this,&MainWindow::on_pushButton_Fact_clicked);
    connect(pushButton_Mod,&QPushButton::clicked,this,&MainWindow::on_pushButton_Mod_clicked);
    connect(pushButton_Abs,&QPushButton::clicked,this,&MainWindow::on_pushButton_Abs_clicked);
    connect(pushButton_Log,&QPushButton::clicked,this,&MainWindow::on_pushButton_Log_clicked);
    connect(pushButton_Sin,&QPushButton::clicked,this,&MainWindow::on_pushButton_Sin_clicked);
    connect(pushButton_Cos,&QPushButton::clicked,this,&MainWindow::on_pushButton_Cos_clicked);
    connect(pushButton_Tan,&QPushButton::clicked,this,&MainWindow::on_pushButton_Tan_clicked);
    connect(pushButton_ASin,&QPushButton::clicked,this,&MainWindow::on_pushButton_ASin_clicked);
    connect(pushButton_ACos,&QPushButton::clicked,this,&MainWindow::on_pushButton_ACos_clicked);
    connect(pushButton_ATan,&QPushButton::clicked,this,&MainWindow::on_pushButton_ATan_clicked);
    connect(pushButton_Cot,&QPushButton::clicked,this,&MainWindow::on_pushButton_Cot_clicked);
    connect(pushButton_Arra,&QPushButton::clicked,this,&MainWindow::on_pushButton_Arra_clicked);
    connect(pushButton_Comb,&QPushButton::clicked,this,&MainWindow::on_pushButton_Comb_clicked);
    connect(pushButton_BraL,&QPushButton::clicked,this,&MainWindow::on_pushButton_BraL_clicked);
    connect(pushButton_BraR,&QPushButton::clicked,this,&MainWindow::on_pushButton_BraR_clicked);

    connect(pushButton_Clear,&QPushButton::clicked,this,&MainWindow::on_pushButton_Clear_clicked);
    connect(pushButton_Backspace,&QPushButton::clicked,this,&MainWindow::on_pushButton_Backspace_clicked);

    connect(pushButton_Sqrt,&QPushButton::clicked,this,&MainWindow::on_pushButton_Sqrt_clicked);
    connect(pushButton_Reciprocal,&QPushButton::clicked,this,&MainWindow::on_pushButton_Reciprocal_clicked);
    connect(pushButton_Square,&QPushButton::clicked,this,&MainWindow::on_pushButton_Square_clicked);
    connect(pushButton_10log,&QPushButton::clicked,this,&MainWindow::on_pushButton_10log_clicked);
    connect(pushButton_elog,&QPushButton::clicked,this,&MainWindow::on_pushButton_elog_clicked);
    connect(pushButton_10pow,&QPushButton::clicked,this,&MainWindow::on_pushButton_10pow_clicked);
    connect(pushButton_Calculate,&QPushButton::clicked,this,&MainWindow::calculate);//连接计算信号
    connect(lineEdit,&QLineEdit::returnPressed,this,&MainWindow::calculate);//连接计算信号

    connect(textBrowser,&TextBrowser::mouseDoubleClicked,this,&MainWindow::on_textBrowser_doubleClicked);

    connect(&timer,&Timer_StatusBar::finish,this,&MainWindow::on_timer_done);
//    connect(label_StatusBar,&Label_custom::textChanged,this,&MainWindow::label_Clear);

}

//键盘事件
void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    qDebug()<<crtRecordIndex;
    if(keyEvent->key()==Qt::Key_Up)
    {
        if(crtRecordIndex>0)
        {
            crtRecordIndex--;//指向上一条记录
            Record crt=history[crtRecordIndex];
            lineEdit->setText(crt.expression);
            textBrowser->setText(crt.result);
        }
//        label_StatusBar->setText(QString::asprintf("Record %d",crtRecordIndex));
        pushNewMessage(QString::asprintf("Record %d",crtRecordIndex));
    }
    if(keyEvent->key()==Qt::Key_Down)
    {
        if(crtRecordIndex<(history.size()-1)&&crtRecordIndex>=0)
        {
            crtRecordIndex++;//指向下一条记录
            Record crt=history[crtRecordIndex];
            lineEdit->setText(crt.expression);
            textBrowser->setText(crt.result);
        }
//        label_StatusBar->setText(QString::asprintf("Record %d",crtRecordIndex));
        pushNewMessage(QString::asprintf("Record %d",crtRecordIndex));
    }
}

//槽函数
void MainWindow::on_pushButton_N0_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'0');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N1_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'1');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N2_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'2');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N3_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'3');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N4_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'4');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N5_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'5');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N6_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'6');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N7_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'7');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N8_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'8');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_N9_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'9');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();lineEdit->setText(lineEdit->text()+"9");
}

void MainWindow::on_pushButton_Point_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'.');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();

}

void MainWindow::on_pushButton_Plus_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'+');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Minu_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'-');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Mult_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'*');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Divi_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'/');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Pow_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'^');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Fact_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'!');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Mod_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'%');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Abs_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"Abs()");
    cursorPos+=4;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Log_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"log");
    cursorPos+=3;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Sin_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"sin()");
    cursorPos+=4;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Cos_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"cos()");
    cursorPos+=4;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Tan_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"tan()");
    cursorPos+=4;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_ASin_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"arcsin()");
    cursorPos+=7;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_ACos_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"arccos()");
    cursorPos+=7;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_ATan_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"arctan()");
    cursorPos+=7;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Cot_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"cot()");
    cursorPos+=4;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Arra_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"ARR");
    cursorPos+=3;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Comb_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"COM");
    cursorPos+=3;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_BraL_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,'(');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_BraR_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos++,')');
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

//backspace
void MainWindow::on_pushButton_Backspace_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    if(str.isEmpty())
    {
        return;
    }

    //模拟按键(有效)
    QKeyEvent tabKey(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    QCoreApplication::sendEvent(this->lineEdit, &tabKey);

//    cursorPos=lineEdit->cursorPosition()-1;
//    str.remove(cursorPos,1);
//    lineEdit->setText(str);
//    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

//clear
void MainWindow::on_pushButton_Clear_clicked()
{
    lineEdit->clear();
    textBrowser->clear();
//    label_StatusBar->setText("Cleared");
    pushNewMessage("Cleared");
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Sqrt_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"^(1/2)");
    cursorPos+=6;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Reciprocal_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"^(-1)");
    cursorPos+=5;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_Square_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"^(2)");
    cursorPos+=4;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_10log_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"10log()");
    cursorPos+=6;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_elog_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"elog()");
    cursorPos+=5;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

void MainWindow::on_pushButton_10pow_clicked()
{
    int cursorPos;
    QString str=lineEdit->text();
    cursorPos=lineEdit->cursorPosition();
    str.insert(cursorPos,"*10^()");
    cursorPos+=5;
    lineEdit->setText(str);
    lineEdit->setCursorPosition(cursorPos);
//    lineEdit->setFocus();
}

///action槽函数

void MainWindow::on_action_Quit_triggered()
{
    qApp->exit(0);
}



void MainWindow::on_actionGroup_view_triggered(QAction * sender)
{
    delete gridLayout_groupBoxButtons;//删除布局
    gridLayout_groupBoxButtons=new QGridLayout(groupBox_Buttons);
    int size=buttonList.size();
    for(int i=0;i<size;i++)//遍历列表进行设置
    {
        buttonList[i]->setParent(nullptr);//移除
    }
    if(sender==action_EmptyView)
    {
        initLayout_EmptyView();
//        label_StatusBar->setText("Changed View to EmptyView");
        pushNewMessage("Changed View to EmptyView");
    }
    else if(sender==action_SimpleView)
    {
        initLayout_SimpleView();
//        label_StatusBar->setText("Changed View to SimpleView");
        pushNewMessage("Changed View to SimpleView");
    }
    else if(sender==action_StandardView)
    {
        initLayout_StandardView();
//        label_StatusBar->setText("Changed View to StandardView");
        pushNewMessage("Changed View to StandardView");
    }
    else if(sender==action_ScienceView)
    {
        initLayout_ScienceView();
//        label_StatusBar->setText("Changed View to ScienceView");
        pushNewMessage("Changed View to ScienceView");
    }

    this->groupBox_Buttons->updateGeometry();
    this->repaint();
    //    this->resize(this->sizeHint());
}

void MainWindow::on_timer_done()
{
    label_StatusBar->clear();
    label_StatusBar->setStyleSheet(".QLabel{color:#000000;}");
}

void MainWindow::pushNewMessage(const QString &message, const bool critical)
{
    if(critical)
    {
        qDebug()<<"<critical>";
        label_StatusBar->setStyleSheet(".QLabel{color:#ff0000;}");
    }
    else
        label_StatusBar->setStyleSheet(".QLabel{color:#000000;}");
    label_StatusBar->setText(message);
    timer.startCountdown();//开始倒计时
}

void MainWindow::calculate()
{
    textBrowser->clear();
    QString expression;
    if(lineEdit->hasSelectedText())//选中了部分表达式
        expression=(lineEdit->selectedText());
    else//没有选中
        expression=lineEdit->text();

    calculator.setExpression(expression.toStdString());
    QString formattedResult;
    try
    {
        calculator.calculate();//计算
        formattedResult=QString::fromStdString(calculator.getFormattedResult());
    }
    catch(NS_Calculator::Exception &e)
    {
        textBrowser->setTextColor(Qt::red);
        textBrowser->setText(e.what());
        textBrowser->setAlignment(Qt::AlignRight|Qt::AlignBottom);//设置对齐方式
        textBrowser->setTextColor(Qt::black);
        pushNewMessage("Exception Occured",true);
        return;
    }

    pushNewMessage("Done");

//    if(formattedResult.size()>50)
//    {
//        this->textBrowser->setFixedHeight(Settings::frameFixedHeight*3);
//    }
//    else
//    {
//        this->textBrowser->setFixedHeight(Settings::frameFixedHeight);
//    }
    textBrowser->setText(formattedResult);
//    textBrowser->setToolTip(formattedResult);
    window_result_detail->setConetent(formattedResult);

    textBrowser->setAlignment(Qt::AlignRight|Qt::AlignBottom);//设置对齐方式
    Record crtRecord;
    crtRecord.expression=expression;
    crtRecord.result=formattedResult;

    history<<crtRecord;//将当前记录添加到历史记录
    crtRecordIndex++;//当前索引++

//    this->repaint();
//    qDebug()<<this->sizeHint().width()<<this->sizeHint().height();

}

void MainWindow::label_Clear()
{
    qDebug()<<"clearCalled";
    Sleep(1000);
    this->label_StatusBar->clear();
}

//设置
void MainWindow::on_action_Settings_triggered()
{
//    this->action_Settings->setEnabled(false);//禁用设置动作

    try
    {
        window_Settings->loadConfigFile();//加载配置文件
        window_Settings->show();//显示设置窗口
    }
    catch(QString &str)
    {
        qDebug()<<str;

//        label_StatusBar->setText(str);
        pushNewMessage(str,true);

    }

}

//帮助
void MainWindow::on_action_Help_triggered()
{
    window_Help->reloadContent();//重载内容
    this->window_Help->show();//显示
}


void MainWindow::on_action_About_triggered()
{
    window_About->reloadContent();//重载内容
    this->window_About->show();//显示关于窗口
}


//清空历史
void MainWindow::on_action_ClearHistory_triggered()
{
    int size=history.size();
//    this->label_StatusBar->setText(QString::asprintf("%d Records Cleared",size));
    pushNewMessage(QString::asprintf("%d Records Cleared",size));

    history.clear();//清空
    crtRecordIndex=-1;
    //清空显示框
//    lineEdit->clear();
    //    textBrowser->clear();
}

void MainWindow::on_textBrowser_doubleClicked()
{
    window_result_detail->show();
}







