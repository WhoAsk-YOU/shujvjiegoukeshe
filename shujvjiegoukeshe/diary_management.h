//游学日记管理界面
#ifndef DIARY_MANAGEMENT_H
#define DIARY_MANAGEMENT_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QSqlQuery>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QPainter>
#include "Structure.h"

class Diary_Management : public QWidget
{
    Q_OBJECT
public:
    Diary_Management(QString accountNumber, QString place);
    ~Diary_Management();

signals:
    void chooseback();  //返回信号

private slots:
    void showResult();  //展示查询结果

private:
    int mode;  //若点击了目的地查询按钮,mode=1;若点击了日记名查询按钮,mode=2
    QString accountNumber;  //当前账户名
    QString place;  //当前所在景区/学校
    QLabel* labelDestinationDM = NULL;  //游学目的地
    QPushButton* buttonChoosebackDM = NULL;  //返回按钮
    QPushButton* buttonWriteDM = NULL;  //写日记按钮
    QPushButton* buttonQueryDestinationDM = NULL;  //查询按钮（目的地日记）
    QPushButton* buttonQueryDiaryNameDM = NULL;  //查询按钮（日记名）
    QRadioButton* buttonHeatValueDM = NULL;  //按热度排序
    QRadioButton* buttonGoodCommentsDM = NULL;  //按评价排序
    QComboBox* boxDestinationDM = NULL;  //目的地
    QLineEdit* lineEditDiaryNameDM = NULL;  //日记名输入框
    QTableWidget* tableDiaryInfoDM = NULL;  //日记信息表格

    void initWidget();  //界面初始化函数
    void paintEvent(QPaintEvent*);  //重写paintEvent函数，用于在屏幕上打印出当前账户名及当前所在景区/学校
};

#endif // DIARY_MANAGEMENT_H
