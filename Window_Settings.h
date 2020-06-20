#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QtWidgets>

#include "Settings.h"




class Window_Settings:public QWidget
{
    Q_OBJECT

private:
    static const int fixedGroupBoxHeight;//固定组高度
    static const int minWidth;//最小宽度

public:
    //构造函数
    explicit Window_Settings(QWidget *parent=nullptr);

    void loadConfigFile();//读取配置文件
    void saveConfigToFile();//保存配置到文件
    void setWidgets();//根据文件读取的内容设置组件

    int format;//模式
    int precision;//精度
    int angleUnit;//角度单位
    bool displayDigitSeparator;//位分隔符

private:
    QGroupBox *groupBox_main;//主区域

    //布局
//    QVBoxLayout *vBoxLayout_main;//主布局
    QGridLayout *gridLayout_main;//主布局

    QVBoxLayout *vBoxLayout_ScrollArea;//滚动区域布局

    QHBoxLayout *hBoxLayout_groupBox_FormatSelect;
    QHBoxLayout *hBoxLayout_groupBox_Precision;
    QHBoxLayout *hBoxLayout_groupBox_AngleUnitSelect;
    QHBoxLayout *hBoxLayout_groupBox_DigitSeparatorDisplaySwitch;
    QHBoxLayout *hBoxLayout_buttons;//底部按钮布局

    QGroupBox *groupBox_FormatSelect;
    //三个模式选择互斥按钮
    QRadioButton *radioBtn_Sci;
    QRadioButton *radioBtn_Fix;
    QRadioButton *radioBtn_Norm;
    QList<QRadioButton*> btnList_FormatSelect;

    QGroupBox *groupBox_Precision;
    QSpinBox *spinBox_Precision;

    QGroupBox *groupBox_AngelUnitSelect;
    //两个角度单位选择互斥按钮
    QRadioButton *radioBtn_Deg;
    QRadioButton *radioBtn_Rad;
    QList<QRadioButton*> btnList_AngleUnitSelect;

    QPushButton *pushBtn_Apply;//保存按钮
    QPushButton *pushBtn_Cancel;//取消按钮
    QPushButton *pushBtn_ApplyAndHide;//OK按钮


    QGroupBox *groupBox_DigitSeparatorDisplaySwitch;
    //
    QCheckBox *checkBox_DigitSeparatorDisplay;//检查盒

public slots:
    void updateCalculatorSettings();

private:
    void initSingalSlots();//初始化信号槽


};









#endif // SETTINGWINDOW_H
