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

Diary_Write::~Diary_Write() {
    delete lineEditDiaryTitle;
    lineEditDiaryTitle = NULL;
    delete diaryWriteEdit;
    diaryWriteEdit = NULL;
    delete buttonSubmit;
    buttonSubmit = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
}

void Diary_Write::clickSubmit() {
    QString diaryTitle = lineEditDiaryTitle->text();
    QString diaryContent = diaryWriteEdit->toPlainText();
    int userId, placeId;
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
    query.bindValue(":accountNumber", accountNumber);
    query.exec();
    query.next();
    userId = query.value(0).toInt();
    query.clear();
    query.prepare("select place_id from t_place_ranking where name = :place");
    query.bindValue(":place", place);
    query.exec();
    query.next();
    placeId = query.value(0).toInt();
    query.clear();
    query.prepare("insert into t_diary(title,content,writer,place,heat_value,total_score,rating_frequency) "
                  "values(:title,:content,:writer,:place,:heat_value,:total_score,:rating_frequency)");
    query.bindValue(":title", diaryTitle);
    query.bindValue(":content", huffmanCompression(diaryContent));
    query.bindValue(":writer", userId);
    query.bindValue(":place", placeId);
    query.bindValue(":heat_value", 0);
    query.bindValue(":total_score", 0);
    query.bindValue(":rating_frequency", 0);
    query.exec();
    QMessageBox::information(this, "提交成功", "日记提交成功");
    emit this->chooseback();
}

void Diary_Write::initWidget() {
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);

    lineEditDiaryTitle = new QLineEdit(this);
    lineEditDiaryTitle->setGeometry(490, 170, 520, WIDTH / 15 - 10);
    lineEditDiaryTitle->setPlaceholderText("请输入日记名称");
    lineEditDiaryTitle->setClearButtonEnabled(true);
    lineEditDiaryTitle->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff]+"), this));
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
    diaryWriteEdit = new QTextEdit(this);
    diaryWriteEdit->setGeometry(490, 230, 520, 550);
    //diaryWriteEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff]+"), this));
    diaryWriteEdit->setStyleSheet(""
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
    buttonSubmit = new QPushButton("提交", this);
    buttonSubmit->move(700, WIDTH * 8 / 9 - 30);
    buttonSubmit->resize(91, 57);
    buttonSubmit->setStyleSheet("QPushButton {"
                                "    background-color: #3399FF; /* 浅蓝色背景 */"
                                "    border-style: outset;"
                                "    border-width: 2px;"
                                "    border-radius: 10px; /* 圆角 */"
                                "    border-color: #1C5FAF; /* 稍深一点的蓝色边框 */"
                                "    font: bold 21px 黑体;"
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

void Diary_Write::generateCodes(Node* root, const QString& str, QMap<QChar, QString>& huffmanCode) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

Node* Diary_Write::buildHuffmanTree(const QString& text) {
    QMap<QChar, int> freq;
    for (QChar ch : text) {
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, compare> pq;
    for (auto it = freq.begin(); it != freq.end(); ++it) {
        pq.push(new Node(it.key(), it.value()));
    }

    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        Node* node = new Node(QChar(), sum);
        node->left = left;
        node->right = right;
        pq.push(node);
    }

    return pq.top();
}

QString Diary_Write::huffmanCompression(const QString& text) {
    Node* root = buildHuffmanTree(text);
    QMap<QChar, QString> huffmanCode;
    generateCodes(root, "", huffmanCode);

    QString compressedText = "";
    for (QChar ch : text) {
        compressedText += huffmanCode[ch];
    }

    QString huffmanCodeStr = "";
    for (auto it = huffmanCode.begin(); it != huffmanCode.end(); ++it) {
        huffmanCodeStr += it.key();
        huffmanCodeStr += it.value();
        huffmanCodeStr += '|';
    }

    return compressedText + '|' + huffmanCodeStr;
}

void Diary_Write::paintEvent(QPaintEvent*) {
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
    painter.drawText((LENGTH - textRect.width()) / 2, 100, "当前所在景区/学校:" + place);
}
