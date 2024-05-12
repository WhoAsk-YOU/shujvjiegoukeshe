#ifndef DIARY_WRITE_H
#define DIARY_WRITE_H

#include "Structure.h"

class Diary_Write : public QWidget
{
    Q_OBJECT
public:
    Diary_Write(QString accountNumber, QString place);
    ~Diary_Write();

signals:
    void chooseback();  //返回信号

private slots:
    void clickSubmit();

private:
    QString accountNumber;  //当前账户名
    QString place;  //当前所在景区/学校
    QLineEdit* lineEditDiaryTitle = NULL;  //日记名输入框
    QTextEdit* diaryWriteEdit = NULL;  //日记编辑文本框
    QPushButton* buttonSubmit = NULL;  //提交按钮
    QPushButton* buttonChooseback = NULL;  //返回按钮
    void initWidget();  //界面初始化函数
    void paintEvent(QPaintEvent*);
};

#endif // DIARY_WRITE_H
