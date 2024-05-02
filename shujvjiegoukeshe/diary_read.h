#ifndef DIARY_READ_H
#define DIARY_READ_H

#include "Structure.h"

class Diary_Read : public QWidget
{
    Q_OBJECT
public:
    Diary_Read(int diaryId);
    ~Diary_Read();

signals:
    void chooseback();  //返回信号

private:
    int diaryId;  //日记编号
    QTextEdit* diaryReadEdit = NULL;  //查看日记文本框
    QPushButton* buttonChooseback = NULL;  //返回按钮

    void initWidget();  //界面初始化函数
};

#endif // DIARY_READ_H
