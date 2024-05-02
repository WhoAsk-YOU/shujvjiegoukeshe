#include "diary_write.h"
Diary_Write::Diary_Write(QString accountNumber, QString place)
{
    this->accountNumber = accountNumber;
    this->place = place;
    initWidget();
    connect(buttonChooseback, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回游学日记管理界面的信号
    });
    connect(buttonSubmit, &QPushButton::clicked, [=]() {  //若点击提交按钮
        clickSubmit();
    });
}

Diary_Write::~Diary_Write(){
    delete lineEditDiaryTitle;
    lineEditDiaryTitle = NULL;
    delete diaryWriteEdit;
    diaryWriteEdit = NULL;
    delete buttonSubmit;
    buttonSubmit = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
}

void Diary_Write::clickSubmit(){
    QString diaryTitle = lineEditDiaryTitle->text();
    QString diaryContent = diaryWriteEdit->toPlainText();
    int userId,placeId;
    QSqlQuery query;
    if (diaryTitle == "") {
        QMessageBox::information(this, "提交失败", "提交失败，日记名不能为空");
        return;
    }
    if (diaryContent == "") {
        QMessageBox::information(this, "提交失败", "提交失败，日记内容不能为空");
        return;
    }
    //向数据库中添加日记信息
    query.prepare("select user_id from t_user where account_number = :accountNumber");
    query.bindValue(":accountNumber",accountNumber);
    query.exec();
    query.next();
    userId = query.value(0).toInt();
    query.clear();
    query.prepare("select place_id from t_place_ranking where name = :place");
    query.bindValue(":place",place);
    query.exec();
    query.next();
    placeId = query.value(0).toInt();
    query.clear();
    query.prepare("insert into t_diary(title,content,writer,place,heat_value,good_comments) values(:title,:content,:writer,:place,:heat_value,:good_comments)");
    query.bindValue(":title", diaryTitle);
    query.bindValue(":content", diaryContent);
    query.bindValue(":writer", userId);
    query.bindValue(":place", placeId);
    query.bindValue(":heat_value", 0);
    query.bindValue(":good_comments", 0);
    query.exec();
    QMessageBox::information(this, "提交成功", "日记提交成功");
    emit this->chooseback();
}

void Diary_Write::initWidget(){
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);

    lineEditDiaryTitle = new QLineEdit(this);
    lineEditDiaryTitle->setGeometry(440, WIDTH * 2 / 19  , 520, WIDTH / 15 - 10);
    lineEditDiaryTitle->setFont(QFont("黑体", 18));
    lineEditDiaryTitle->setPlaceholderText("请输入日记名称");
    lineEditDiaryTitle->setClearButtonEnabled(true);
    lineEditDiaryTitle->setMaxLength(70);
    diaryWriteEdit = new QTextEdit(this);
    diaryWriteEdit->setGeometry(275, WIDTH * 4 / 19  , 850, 550);
    diaryWriteEdit->setFont(QFont("黑体", 18));
    buttonSubmit = new QPushButton("提交", this);
    buttonSubmit->move(LENGTH*5/11, WIDTH * 8 / 9-30);
    buttonSubmit->resize(LENGTH / 11, WIDTH / 11);
    buttonSubmit->setFont(QFont("黑体", 22));
    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, WIDTH * 8 / 9);
    buttonChooseback->resize(LENGTH / 9, WIDTH / 9);
    buttonChooseback->setFont(QFont("黑体", 25));
}

