#include "diary_read.h"

Diary_Read::Diary_Read(int diaryId, QString accountNumber, QString place)
{
    this->diaryId = diaryId;
    this->accountNumber = accountNumber;
    this->place = place;
    QSqlQuery query;
    query.prepare("UPDATE t_diary SET heat_value = heat_value + 1 WHERE diary_id = :diaryId");
    query.bindValue(":diaryId", diaryId);
    query.exec();
    query.clear();
    initWidget();
    connect(buttonChooseback, &QPushButton::clicked, [=]() {  //若点击返回按钮
        mark();
    });
    connect(buttonSubmit, &QPushButton::clicked, [=]() {  //若点击修改按钮
        submitAlterDiary();
    });
    connect(buttonDelete, &QPushButton::clicked, [=]() {  //若点击删除按钮
        deleteDiary();
    });
}

Diary_Read::~Diary_Read(){
    delete diaryReadEdit;
    diaryReadEdit = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
    delete lineEditDiaryTitle;
    lineEditDiaryTitle = NULL;
    delete buttonSubmit;
    buttonSubmit = NULL;
    delete buttonDelete;
    buttonDelete = NULL;
}

void Diary_Read::mark(){
    QSqlQuery query;
    query.prepare("SELECT account_number FROM t_user WHERE user_id = (SELECT writer FROM t_diary WHERE diary_id = :diaryId)");
    query.bindValue(":diaryId", diaryId);
    query.exec();
    query.next();
    if(query.value(0).toString() == accountNumber){  //若是本人查看日记，则不必评分
        emit this->chooseback();  // 发出返回游学日记管理界面的信号
        return;
    }
    bool ok;
    int score = QInputDialog::getInt(this, "评分", "请输入日记评分(1-100)：", 0, 1, 100, 1, &ok);
    if(ok){  //若点击ok按钮
        query.clear();
        query.prepare("UPDATE t_diary SET rating_frequency = rating_frequency + 1, "
                      "total_score = total_score + :score WHERE diary_id = :diaryId");
        query.bindValue(":score", score);
        query.bindValue(":diaryId", diaryId);
        query.exec();
    }
    emit this->chooseback();  // 发出返回游学日记管理界面的信号
}

void Diary_Read::submitAlterDiary(){
    QSqlQuery query;
    QString writerAccountNumber;  //日记作者
    query.prepare("SELECT account_number FROM t_user WHERE user_id = (SELECT writer FROM t_diary WHERE diary_id = :diaryId)");
    query.bindValue(":diaryId", diaryId);
    query.exec();
    query.next();
    writerAccountNumber = query.value(0).toString();
    if(writerAccountNumber == accountNumber){
        query.clear();
        if(lineEditDiaryTitle->text().isEmpty()){
            query.prepare("UPDATE t_diary SET content = :content WHERE diary_id = :diaryId");
            query.bindValue(":content", diaryReadEdit->toPlainText());
            query.bindValue(":diaryId", diaryId);
        }
        else{
            query.prepare("UPDATE t_diary SET title = :title, content = :content WHERE diary_id = :diaryId");
            query.bindValue(":title", lineEditDiaryTitle->text());
            query.bindValue(":content", diaryReadEdit->toPlainText());
            query.bindValue(":diaryId", diaryId);
        }
        query.exec();
        QMessageBox::information(this, "修改成功", "修改成功");
        emit this->chooseback();
    }
    else{
        QMessageBox::information(this, "修改失败", "修改失败，仅日记作者可修改本人日记");
    }
    return;
}

void Diary_Read::deleteDiary(){
    QSqlQuery query;
    QString writerAccountNumber;  //日记作者
    query.prepare("SELECT account_number FROM t_user WHERE user_id = (SELECT writer FROM t_diary WHERE diary_id = :diaryId)");
    query.bindValue(":diaryId", diaryId);
    query.exec();
    query.next();
    writerAccountNumber = query.value(0).toString();
    if(writerAccountNumber == accountNumber){
        query.clear();
        query.prepare("DELETE FROM t_diary WHERE diary_id = :diaryId");
        query.bindValue(":diaryId", diaryId);
        query.exec();
        QMessageBox::information(this, "删除成功", "删除成功");
        emit this->chooseback();
    }
    else{
        QMessageBox::information(this, "删除失败", "删除失败，仅日记作者可删除本人日记");
    }
    return;
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
    diaryReadEdit->setPlainText(query.value(0).toString());
    diaryReadEdit->setGeometry(490, 230, 520, 550);
    diaryReadEdit->setStyleSheet(""
                                  "QTextEdit {"
                                  "    background-color: #e0f0ff;" // 非常浅的蓝色背景
                                  "    border-style: solid;"
                                  "    border-width: 1px;"
                                  "    border-color: #b0c0d0;" // 深一点的蓝色边框
                                  "    border-radius: 8px;" // 圆角边框
                                  "    padding: 8px;" // 内边距
                                  "    font-family: 黑体;"
                                  "    font-size: 12pt;"
                                  "}"
                                  "");
    query.clear();
    query.prepare("SELECT account_number FROM t_user WHERE user_id = (SELECT writer FROM t_diary WHERE diary_id = :diaryId)");
    query.bindValue(":diaryId", diaryId);
    query.exec();
    query.next();
    diaryReadEdit->setReadOnly(!(query.value(0).toString() == accountNumber));
    lineEditDiaryTitle = new QLineEdit(this);
    lineEditDiaryTitle->setGeometry(490, 170, 520, WIDTH / 15 - 10);
    lineEditDiaryTitle->setPlaceholderText("请输入修改后的日记名称(如有必要)");
    lineEditDiaryTitle->setClearButtonEnabled(true);
    lineEditDiaryTitle->setMaxLength(70);
    lineEditDiaryTitle->setStyleSheet("QLineEdit {"
                                      "border: 2px solid #A6C1FF;" // 浅蓝色边框
                                      "border-radius: 8px;"         // 圆角
                                      "padding: 5px 10px;"          // 内边距
                                      "font-size: 15px 黑体;"            // 字体大小
                                      "background-color: #FFFFFF;"  // 背景颜色为白色
                                      "selection-background-color: #A6C1FF;" // 选择时文字背景颜色
                                      "}"
                                      "QLineEdit:hover {"
                                      "border: 2px solid #799BFF;" // 悬浮时边框颜色变深
                                      "}"
                                      "QLineEdit:focus {"
                                      "border: 2px solid #3F7FFF;" // 聚焦时边框颜色更深
                                      "}"
                                      "QLineEdit QAbstractSpinBox::up-button, QLineEdit QAbstractSpinBox::down-button {"
                                      "border-style: none;" // 去掉上下箭头按钮的边框
                                      "background: transparent;" // 设置箭头按钮背景透明
                                      "}"
                                      "QLineEdit::placeholder {"
                                      "color: #A0A0A0;" // 提示文本颜色
                                      "font-style: italic;" // 提示文本斜体
                                      "}"
                                      );
    buttonSubmit = new QPushButton("提交修改", this);
    buttonSubmit->move(600, WIDTH * 8 / 9 - 30);
    buttonSubmit->resize(91, 57);
    buttonSubmit->setStyleSheet("QPushButton {"
                                "    background-color: #3399FF; /* 浅蓝色背景 */"
                                "    border-style: outset;"
                                "    border-width: 2px;"
                                "    border-radius: 10px; /* 圆角 */"
                                "    border-color: #1C5FAF; /* 稍深一点的蓝色边框 */"
                                "    font: bold 18px 黑体;"
                                "    min-width: 3em;"
                                "    padding: 6px;"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: #1C5FAF; /* 鼠标悬停时的背景颜色 */"
                                "}"
                                "QPushButton:pressed {"
                                "    background-color: #082F5A; /* 按钮按下时的背景颜色 */"
                                "    border-style: inset;"
                                "}"
                                );
    buttonDelete = new QPushButton("删除日记", this);
    buttonDelete->move(800, WIDTH * 8 / 9 - 30);
    buttonDelete->resize(91, 57);
    buttonDelete->setStyleSheet("QPushButton {"
                                "    background-color: #3399FF; /* 浅蓝色背景 */"
                                "    border-style: outset;"
                                "    border-width: 2px;"
                                "    border-radius: 10px; /* 圆角 */"
                                "    border-color: #1C5FAF; /* 稍深一点的蓝色边框 */"
                                "    font: bold 18px 黑体;"
                                "    min-width: 3em;"
                                "    padding: 6px;"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: #1C5FAF; /* 鼠标悬停时的背景颜色 */"
                                "}"
                                "QPushButton:pressed {"
                                "    background-color: #082F5A; /* 按钮按下时的背景颜色 */"
                                "    border-style: inset;"
                                "}"
                                );
    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, WIDTH * 17 / 18);
    buttonChooseback->resize(50, WIDTH / 18);
    buttonChooseback->setStyleSheet(
        "QPushButton {"
        "    background-color: #ff9444;" // 设置背景颜色为较浅的偏橙色
        "    border-style: outset;"       // 边框样式
        "    border-width: 2px;"          // 边框宽度
        "    border-radius: 10px;"        // 边框圆角
        "    border-color: #555555;"      // 边框颜色
        "    font: bold 25px;"             // 字体大小和粗细
        "    min-width: 3em;"             // 调整按钮的最小宽度
        "    padding: 4px;"               // 调整内边距
        "    color: white;"                // 字体颜色
        "}"
        "QPushButton:hover {"
        "    background-color: #ffa944;"  // 悬停时的背景颜色调整为更浅的偏橙色
        "}"
        "QPushButton:pressed {"
        "    background-color: #ff8c44;"  // 鼠标按下时的背景颜色调整为较深的偏橙色
        "    border-style: inset;"
        "}"
        );

}

void Diary_Read::paintEvent(QPaintEvent*) {
    QPen pen(Qt::white);  //画笔颜色为白色
    pen.setWidth(0);  //画笔宽度
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/3.jpg");
    int windowWidth = this->width();
    int windowHeight = this->height();
    QPixmap scaledPix = pix.scaled(windowWidth, windowHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, scaledPix);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setBrush(Qt::white);
    painter.setOpacity(0.5);
    painter.drawRect(400, 0, 700, 950);
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setOpacity(1);
    painter.setFont(QFont("黑体", 25));
    QRect textRect = painter.boundingRect(QRect(), Qt::TextSingleLine, "当前所在景区/学校:" + place);
    painter.drawText((LENGTH - textRect.width())/2, 100, "当前所在景区/学校:" + place);
}
