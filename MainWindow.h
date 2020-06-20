#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//标准头文件
#include <QMainWindow>
#include <QtWidgets>//组件
#include <QLayout>//布局

//自定义头文件
#include "Calculator.h"//核心数学计算
#include "Window_Settings.h"//设置窗口
#include "Window_Info.h"//信息窗口
#include "Text.h"
#include "Timer_StatusBar.h"
#include "Settings.h"
#include "TextBrowser.h"


#pragma comment(lib,"../Calculator.lib")//引入动态库

//namespace Ui
//{
//class MainWindow;
//}
//不使用可视化创建界面

//记录
struct Record
{
    QString expression;//表达式
    QString result;//结果
};


//全局变量声明
extern NS_Calculator::Calculator calculator;//全局计算器对象
extern int crtView;//当前视图
extern int crtRecordIndex;//当前记录索引
extern QList<Record> history;//历史记录





class MainWindow:public QMainWindow
{
    Q_OBJECT
friend class Window_Settings;
friend class Window_Info;
public:
    static int precision;//精度

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initWidgets();//初始化各种组件


public:
//    Mathematics::Calculator calculator;
    QFont font_main;//程序主要字体

//    int crtView;//当前视图

    QList<QPushButton *> buttonList;
    QList<QAction *> actionList;

    //默认组件
    QMenuBar *menuBar_main;//菜单条
    QStatusBar *statusBar_main;//状态栏
    QToolBar *toolBar_main;//主工具栏
    QWidget *widget_main;//中心组件

    //菜单
    QMenu *menu_start;//开始
    QMenu *menu_view;//视图
    QMenu *menu_option;//选项
    QMenu *menu_help;//帮助

    //组
    QGroupBox *groupBox_Main;
    QGroupBox *groupBox_Buttons;

    //QLabel
    QLabel *label_StatusBar;//在状态栏上显示信息的标签

    //行编辑框
    QLineEdit *lineEdit;

    //文本显示框
    TextBrowser *textBrowser;

    //actions
    QAction *action_Quit;//退出


    QActionGroup *actionGroup_view;//动作组
    QAction *action_SimpleView;//简易视图
    QAction *action_StandardView;//标准视图
    QAction *action_ScienceView;//科学视图
    QAction *action_EmptyView;//空视图

    QAction *action_Settings;//设置
    QAction *action_ClearHistory;//清除历史记录
    QAction *action_Help;//帮助
    QAction *action_About;//关于

    QAction *action_Clear;//清除结果框和输入框

    //布局
    QGridLayout *gridLayout_groupBoxButtons;//四个标准布局

    QVBoxLayout *vBoxLayout_groupBoxMain;//包含两个框的单独布局
    QVBoxLayout *vBoxLayout_main;//主要布局

    //弹簧
    QSpacerItem *spacer_main;

    ///按钮

    //基本按钮
    QPushButton *pushButton_N0;
    QPushButton *pushButton_N1;
    QPushButton *pushButton_N2;
    QPushButton *pushButton_N3;
    QPushButton *pushButton_N4;
    QPushButton *pushButton_N5;
    QPushButton *pushButton_N6;
    QPushButton *pushButton_N7;
    QPushButton *pushButton_N8;
    QPushButton *pushButton_N9;
    QPushButton *pushButton_Point;

    QPushButton *pushButton_Plus;
    QPushButton *pushButton_Minu;
    QPushButton *pushButton_Mult;
    QPushButton *pushButton_Divi;
    QPushButton *pushButton_Pow;
    QPushButton *pushButton_Fact;
    QPushButton *pushButton_Mod;
    QPushButton *pushButton_Abs;
    QPushButton *pushButton_Log;
    QPushButton *pushButton_Sin;
    QPushButton *pushButton_Cos;
    QPushButton *pushButton_Tan;
    QPushButton *pushButton_ASin;
    QPushButton *pushButton_ACos;
    QPushButton *pushButton_ATan;
    QPushButton *pushButton_Cot;
    QPushButton *pushButton_Arra;
    QPushButton *pushButton_Comb;
    QPushButton *pushButton_BraL;
    QPushButton *pushButton_BraR;

    //附加按钮

    QPushButton *pushButton_Calculate;
    QPushButton *pushButton_Backspace;
    QPushButton *pushButton_Clear;

    QPushButton *pushButton_Sqrt;//根号
    QPushButton *pushButton_Reciprocal;//倒数
    QPushButton *pushButton_Square;//平方
    QPushButton *pushButton_10log;//以10为底的log
    QPushButton *pushButton_elog;//以e为底的log
    QPushButton *pushButton_10pow;//10^x

    //多窗口
    Window_Settings *window_Settings;//设置窗口

    Window_Info *window_About;//关于窗口
    Window_Info *window_Help;//帮助
    Window_Info *window_result_detail;//结果详细


    Timer_StatusBar timer;


private slots:
    //按钮槽
    void on_pushButton_N0_clicked();
    void on_pushButton_N1_clicked();
    void on_pushButton_N2_clicked();
    void on_pushButton_N3_clicked();
    void on_pushButton_N4_clicked();
    void on_pushButton_N5_clicked();
    void on_pushButton_N6_clicked();
    void on_pushButton_N7_clicked();
    void on_pushButton_N8_clicked();
    void on_pushButton_N9_clicked();
    void on_pushButton_Point_clicked();

    void on_pushButton_Plus_clicked();//+
    void on_pushButton_Minu_clicked();//-
    void on_pushButton_Mult_clicked();//*
    void on_pushButton_Divi_clicked();// /
    void on_pushButton_Pow_clicked();//^
    void on_pushButton_Fact_clicked();// !
    void on_pushButton_Mod_clicked();//%
    void on_pushButton_Abs_clicked();//绝对值
    void on_pushButton_Log_clicked();//log
    void on_pushButton_Sin_clicked();//sin
    void on_pushButton_Cos_clicked();//cos
    void on_pushButton_Tan_clicked();//tan
    void on_pushButton_ASin_clicked();//arcsin
    void on_pushButton_ACos_clicked();//arccos
    void on_pushButton_ATan_clicked();//arctan
    void on_pushButton_Cot_clicked();//cot
    void on_pushButton_Arra_clicked();//arr
    void on_pushButton_Comb_clicked();//com
    void on_pushButton_BraL_clicked();//(
    void on_pushButton_BraR_clicked();//)

    void on_pushButton_Backspace_clicked();//退格
    void on_pushButton_Clear_clicked();//清除

    void on_pushButton_Sqrt_clicked();
    void on_pushButton_Reciprocal_clicked();
    void on_pushButton_Square_clicked();
    void on_pushButton_10log_clicked();
    void on_pushButton_elog_clicked();
    void on_pushButton_10pow_clicked();

    //action槽
    void on_action_Quit_triggered();//退出
    void on_action_About_triggered();//关于
    void on_action_Settings_triggered();//设置
    void on_action_Help_triggered();//帮助
    void on_action_ClearHistory_triggered();//清空历史

    //QTextBrowser槽
    void on_textBrowser_doubleClicked();//双击


    //actionGroup槽
    void on_actionGroup_view_triggered(QAction *);

    //计时器结束
    void on_timer_done();

    //推送新消息
    void pushNewMessage(const QString&message,const bool critical=false);


    //其他
    void calculate();//计算
    void label_Clear();//清空状态栏信息


private:

    void initLayout_EmptyView();//四个布局的初始化
    void initLayout_SimpleView();
    void initLayout_StandardView();
    void initLayout_ScienceView();

    void initLayouts();//初始化各种布局
    void initSingalSlots();//初始化信号槽

protected:
    virtual void keyPressEvent(QKeyEvent *keyEvent);


};

#endif // MAINWINDOW_H
