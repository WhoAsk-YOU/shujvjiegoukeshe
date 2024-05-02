#include "diary_read.h"

Diary_Read::Diary_Read(int diaryId)
{
    this->diaryId = diaryId;
    initWidget();
    connect(buttonChooseback, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回游学日记管理界面的信号
    });
}

Diary_Read::~Diary_Read(){
    delete diaryReadEdit;
    diaryReadEdit = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
}

void Diary_Read::initWidget(){
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);
    QSqlQuery query;
    query.prepare("select content from t_diary where diary_id = :diaryId");  //从数据库中查询日记内容
    query.bindValue(":diaryId",diaryId);
    query.exec();
    query.next();
    diaryReadEdit = new QTextEdit(this);
    diaryReadEdit->setReadOnly(true);  //只可读，不可更改
    diaryReadEdit->setGeometry(275, WIDTH * 4 / 19  , 850, 550);
    diaryReadEdit->setFont(QFont("黑体", 18));
    diaryReadEdit->setPlainText(query.value(0).toString());
    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, WIDTH * 8 / 9);
    buttonChooseback->resize(LENGTH / 9, WIDTH / 9);
    buttonChooseback->setFont(QFont("黑体", 25));
}
