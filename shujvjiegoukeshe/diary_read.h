﻿#ifndef DIARY_READ_H
#define DIARY_READ_H

#include "Structure.h"

class Diary_Read : public QWidget
{
    Q_OBJECT
public:
    Diary_Read(int diaryId, QString accountNumber, QString place);
    ~Diary_Read();

signals:
    void chooseback();  //返回信号

private slots:
    void mark();
    void submitAlterDiary();
    void deleteDiary();

private:
    int diaryId;  //日记编号
    QString accountNumber;
    QString place;
    QLineEdit* lineEditDiaryTitle = NULL;  //日记名输入框
    QTextEdit* diaryReadEdit = NULL;  //查看日记文本框
    QPushButton* buttonChooseback = NULL;  //返回按钮
    QPushButton* buttonSubmit = NULL;  //提交修改按钮
    QPushButton* buttonDelete = NULL;  //删除日记按钮
    void initWidget();  //界面初始化函数
    void paintEvent(QPaintEvent*);
};

#endif // DIARY_READ_H
