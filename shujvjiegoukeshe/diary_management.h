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
    int length;  //窗口的长
    int width;  //窗口的宽
    int mode;  //若点击了目的地查询按钮,mode=1;若点击了日记名查询按钮,mode=2
    QString accountNumber;
    QString place;
    QLabel* labelDestination = NULL;  //游学目的地
    QPushButton* buttonChooseback = NULL;  //返回按钮
    QPushButton* buttonWrite = NULL;  //写日记按钮
    QPushButton* buttonQueryDestination = NULL;  //查询按钮（目的地日记）
    QPushButton* buttonQueryDiaryName = NULL;  //查询按钮（日记名）
    QRadioButton* buttonHeatValue = NULL;  //按热度排序
    QRadioButton* buttonGoodComments = NULL;  //按评价排序
    QComboBox* boxDestination = NULL;  //目的地
    QLineEdit* lineEditDiaryName = NULL;  //日记名输入框
    QTableWidget* tableDiaryInfo = NULL;  //日记信息表格

    void initWidget();  //界面初始化函数

};

#endif // DIARY_MANAGEMENT_H
