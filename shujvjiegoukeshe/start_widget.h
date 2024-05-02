//开始界面
#ifndef START_WIDGET_H
#define START_WIDGET_H

#include "destination_recommendation.h"  //目的地推荐界面，用于创建对象并跳转

QT_BEGIN_NAMESPACE
namespace Ui {
class Start_widget;
}
QT_END_NAMESPACE

class Start_widget : public QWidget {
    Q_OBJECT

public:
    Start_widget(QWidget* parent = nullptr);
    ~Start_widget();

private slots:
    void logOn();  //登录槽函数
    void signIn();  //注册槽函数

private:
    Ui::Start_widget* ui;
    bool havingSignIn = false;  //是否注册过，即数据库里是否有输入的账号
    Destination_Recommendation* destinationRecommendation = NULL;  //目的地推荐界面
    QLabel* labelTitle = NULL;  //学生游学系统标签
    QPushButton* buttonLogOn = NULL;  //登录按钮
    QPushButton* buttonSignIn = NULL;  //注册按钮
    QLineEdit* lineEditAccountNumber = NULL;  //账号输入框
    QLineEdit* lineEditPassword = NULL;  //密码输入框

    void initWidget();  //界面初始化函数

};
#endif // START_WIDGET_H
