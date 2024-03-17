//游学路线规划、场所查询、游学日记管理选择界面
#ifndef CHOOSE_WIDGET_H
#define CHOOSE_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include "route_strategy.h"
#include "search_architect.h"
#include "diary_management.h"

class Choose_Widget : public QWidget
{
    Q_OBJECT
public:
    Choose_Widget(QString accountNumber, QString place);
    ~Choose_Widget();

signals:
    void chooseback();  //返回信号

private slots:
    void clickSearchArchitect();
    void clickDiary();
    void clickRoute();

private:
    int length;  //窗口的长
    int width;  //窗口的宽
    QString accountNumber;
    QString place;
    Route_Strategy* routeStrategy = NULL;
    Search_Architect* searchArchitect = NULL;
    Diary_Management* diaryManagement = NULL;
    QPushButton* buttonSearchArchitect = NULL;  //场所查询按钮
    QPushButton* buttonDiary = NULL;  //游学日记管理选择按钮
    QPushButton* buttonRoute = NULL;  //游学路线规划按钮
    QPushButton* buttonChooseback = NULL;  //返回按钮

    void initWidget();  //界面初始化函数
};

#endif // CHOOSE_WIDGET_H
