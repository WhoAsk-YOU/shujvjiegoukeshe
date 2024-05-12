//游学日记管理界面
#include "diary_management.h"

Diary_Management::Diary_Management(QString accountNumber, QString place)
{
    this->accountNumber = accountNumber;
    this->place = place;
    initWidget();
    connect(buttonChoosebackDM, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回选择界面的信号
    });
    connect(buttonQueryDestinationDM, &QPushButton::clicked, [=]() {  //若点击目的地查询按钮
        mode = 1;
        showResult();
    });
    connect(buttonQueryDiaryNameDM, &QPushButton::clicked, [=]() {  //若点击日记名查询按钮
        mode = 2;
        showResult();
    });
    connect(buttonQueryDiaryContentDM, &QPushButton::clicked, [=]() {  //若点击全文检索查询按钮
        mode = 3;
        showResult();
    });
    connect(buttonWriteDM, &QPushButton::clicked, [=]() {  //若点击写日记按钮
        clickWrite();
    });
    connect(tableDiaryInfoDM, &QTableWidget::cellClicked, [=](int row, int column) {  //若点击表格第二列中的某个单元格
        tableClicked(row, column);  //跳转到读日记界面
    });
}

Diary_Management::~Diary_Management() {
    delete labelDestinationDM;
    labelDestinationDM = NULL;
    delete labelDiaryNameDM;
    labelDiaryNameDM = NULL;
    delete labelDiaryContentDM;
    labelDiaryContentDM = NULL;
    delete buttonChoosebackDM;
    buttonChoosebackDM = NULL;
    delete buttonWriteDM;
    buttonWriteDM = NULL;
    delete buttonHeatValueDM;
    buttonHeatValueDM = NULL;
    delete buttonGoodCommentsDM;
    buttonGoodCommentsDM = NULL;
    delete buttonQueryDestinationDM;
    buttonQueryDestinationDM = NULL;
    delete buttonQueryDiaryNameDM;
    buttonQueryDiaryNameDM = NULL;
    delete boxDestinationDM;
    boxDestinationDM = NULL;
    delete lineEditDiaryNameDM;
    lineEditDiaryNameDM = NULL;
    delete tableDiaryInfoDM;
    tableDiaryInfoDM = NULL;
    delete lineEditDiaryContentDM;
    lineEditDiaryContentDM = NULL;
    delete buttonQueryDiaryContentDM;
    buttonQueryDiaryContentDM = NULL;
}

void Diary_Management::showResult() {
    string destination = boxDestinationDM->currentText().toStdString();
    string diaryName = lineEditDiaryNameDM->text().toStdString();
    vector<DiaryInfo> diaries;
    QSqlQuery query;
    query.exec("select d.diary_id,d.title,u.account_number,p.name,d.heat_value,d.good_comments "
               "from t_diary d join t_user u on d.writer = u.user_id "
               "join t_place_ranking p on d.place = p.place_id");
    while (query.next()){
        DiaryInfo diaryInfo;
        diaryInfo.id = query.value(0).toInt();
        diaryInfo.diaryName = query.value(1).toString().toStdString();
        diaryInfo.authorAccount = query.value(2).toString().toStdString();
        diaryInfo.destination = query.value(3).toString().toStdString();
        diaryInfo.heatValue = query.value(4).toInt();
        diaryInfo.goodComments = query.value(5).toInt();
        diaries.push_back(diaryInfo);
    }
    //调用查找和排序函数
    if(mode == 1)
        diaries = search(destination, diaries);
    else if(mode == 2)
        diaries = search(diaryName, diaries);

    for (int i = 0; i < tableDiaryInfoDM->rowCount(); i++)  //清空表中的内容
        for (int j = 0; j < tableDiaryInfoDM->columnCount(); j++)
            tableDiaryInfoDM->setItem(i, j, new QTableWidgetItem(""));
    for (int i = 0; i < min(tableDiaryInfoDM->rowCount(), (int)diaries.size()); i++) {
        QTableWidgetItem* itemId = new QTableWidgetItem(QString::number(diaries[i].id));
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(diaries[i].diaryName));
        QTableWidgetItem* itemAccountNumber = new QTableWidgetItem(QString::fromStdString(diaries[i].authorAccount));
        QTableWidgetItem* itemDestination = new QTableWidgetItem(QString::fromStdString(diaries[i].destination));
        QTableWidgetItem* itemHeatValue = new QTableWidgetItem(QString::number(diaries[i].heatValue));
        QTableWidgetItem* itemGoodComments = new QTableWidgetItem(QString::number(diaries[i].goodComments));
        itemId->setTextAlignment(Qt::AlignCenter);
        itemName->setTextAlignment(Qt::AlignCenter);
        itemAccountNumber->setTextAlignment(Qt::AlignCenter);
        itemDestination->setTextAlignment(Qt::AlignCenter);
        itemHeatValue->setTextAlignment(Qt::AlignCenter);
        itemGoodComments->setTextAlignment(Qt::AlignCenter);
        tableDiaryInfoDM->setItem(i, 0, itemId);
        tableDiaryInfoDM->setItem(i, 1, itemName);
        tableDiaryInfoDM->setItem(i, 2, itemAccountNumber);
        tableDiaryInfoDM->setItem(i, 3, itemDestination);
        tableDiaryInfoDM->setItem(i, 4, itemHeatValue);
        tableDiaryInfoDM->setItem(i, 5, itemGoodComments);
    }
}

void Diary_Management::tableClicked(int row, int column){
    if (column == 1) {
        if (tableDiaryInfoDM->item(row, column)->text().isEmpty())
            return;
        QTableWidgetItem* item = tableDiaryInfoDM->item(row, 0);
        this->hide();
        diaryRead = new Diary_Read(item->text().toInt(), accountNumber, place);
        diaryRead->setGeometry(this->geometry());
        diaryRead->show();
        connect(diaryRead, &Diary_Read::chooseback, [=]() {  //若在选择界面点击了返回按钮，接受返回信号
            this->setGeometry(diaryRead->geometry());
            this->show();
            delete diaryRead;
            diaryRead = NULL;
        });
    }
}

void Diary_Management::clickWrite(){
    this->hide();
    diaryWrite = new Diary_Write(accountNumber,place);
    diaryWrite->setGeometry(this->geometry());
    diaryWrite->show();
    connect(diaryWrite, &Diary_Write::chooseback, [=]() {
        this->setGeometry(diaryWrite->geometry());
        this->show();
        delete diaryWrite;
        diaryWrite = NULL;
    });
}

void Diary_Management::initWidget() {
    QStringList horizontalHeaderLabels;
    QSqlQuery query;
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);

    labelDestinationDM = new QLabel("目的地查找：", this);
    labelDestinationDM->setGeometry(740, 217, 200, 45);
    labelDestinationDM->setAlignment(Qt::AlignCenter);
    labelDestinationDM->setStyleSheet("QLabel{font:18px;}");
    labelDiaryNameDM = new QLabel("日记名查找：", this);
    labelDiaryNameDM->setGeometry(239, 217, 200, 45);
    labelDiaryNameDM->setAlignment(Qt::AlignCenter);
    labelDiaryNameDM->setStyleSheet("QLabel{font:18px;}");
    labelDiaryContentDM = new QLabel("日记全文检索：", this);
    labelDiaryContentDM->setGeometry(247, 300, 200, 45);
    labelDiaryContentDM->setAlignment(Qt::AlignCenter);
    labelDiaryContentDM->setStyleSheet("QLabel{font:18px;}");
    buttonChoosebackDM = new QPushButton("返回", this);
    buttonChoosebackDM->move(0, WIDTH * 17 / 18);
    buttonChoosebackDM->resize(50, WIDTH / 18);
    buttonChoosebackDM->setStyleSheet(
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
    buttonWriteDM = new QPushButton("写日记", this);
    buttonWriteDM->move(1395, WIDTH * 17 / 18);
    buttonWriteDM->resize(50, WIDTH / 18);
    buttonWriteDM->setStyleSheet("QPushButton {"
                                 "    background-color: #e5f2ff;" // 非常浅的蓝色背景
                                 "    border-style: outset;"     // 边框样式
                                 "    border-width: 2px;"        // 边框宽度
                                 "    border-radius: 10px;"      // 边框圆角
                                 "    border-color: #bfc9ca;"    // 边框颜色
                                 "    font: bold 25px;"      // 字体和大小
                                 "    min-width: 3em;"          // 最小宽度
                                 "    padding: 4px;"             // 内边距
                                 "}"
                                 "QPushButton:hover {"
                                 "    background-color: #dce4f5;" // 鼠标悬停时的背景颜色
                                 "}"
                                 "QPushButton:pressed {"
                                 "    background-color: #c7d5e0;" // 按钮按下时的背景颜色
                                 "    border-style: inset;"       // 按下时的边框样式
                                 "}");
    buttonQueryDiaryContentDM = new QPushButton("查询", this);
    buttonQueryDiaryContentDM->move(1127, 297);
    buttonQueryDiaryContentDM->resize(70, WIDTH / 17);
    buttonQueryDiaryContentDM->setStyleSheet("QPushButton {"
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
    buttonQueryDestinationDM = new QPushButton("查询", this);
    buttonQueryDestinationDM->move(1127, 215);
    buttonQueryDestinationDM->resize(70, WIDTH / 17);
    buttonQueryDestinationDM->setStyleSheet("QPushButton {"
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
    buttonQueryDiaryNameDM = new QPushButton("查询", this);
    buttonQueryDiaryNameDM->move(640, 215);
    buttonQueryDiaryNameDM->resize(70, WIDTH / 17);
    buttonQueryDiaryNameDM->setStyleSheet("QPushButton {"
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

    buttonHeatValueDM = new QRadioButton("按热度排序", this);
    buttonHeatValueDM->setChecked(true);
    buttonHeatValueDM->move(650, 170);
    buttonHeatValueDM->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");
    buttonGoodCommentsDM = new QRadioButton("按评价排序", this);
    buttonGoodCommentsDM->move(800, 170);
    buttonGoodCommentsDM->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");

    lineEditDiaryNameDM = new QLineEdit(this);
    lineEditDiaryNameDM->setGeometry(390, WIDTH * 3 / 19 + 65, 250, WIDTH / 17);
    lineEditDiaryNameDM->setPlaceholderText("请输入日记名称");
    lineEditDiaryNameDM->setClearButtonEnabled(true);
    lineEditDiaryNameDM->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff]+"), this));  //搜索日记名只允许输入中文字符
    lineEditDiaryNameDM->setStyleSheet("QLineEdit {"
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
    lineEditDiaryContentDM = new QLineEdit(this);
    lineEditDiaryContentDM->setGeometry(410, WIDTH * 3 / 19 + 147, 717, WIDTH / 17);
    lineEditDiaryContentDM->setPlaceholderText("请输入日记内容");
    lineEditDiaryContentDM->setClearButtonEnabled(true);
    lineEditDiaryContentDM->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff]+"), this));  //搜索日记名只允许输入中文字符
    lineEditDiaryContentDM->setStyleSheet("QLineEdit {"
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
    tableDiaryInfoDM = new QTableWidget(10, 6, this);  //创建10行6列的表格，用于显示日记信息
    tableDiaryInfoDM->verticalHeader()->setVisible(false);
    tableDiaryInfoDM->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableDiaryInfoDM->move((LENGTH - 952)/2, 370);
    horizontalHeaderLabels << "日记编号" << "日记名称" << "作者账户名" << "目的地" << "热度" << "好评数";
    tableDiaryInfoDM->setHorizontalHeaderLabels(horizontalHeaderLabels);
    tableDiaryInfoDM->setStyleSheet("QTableWidget {"
                                  "    background-color: #e6f2ff; /* 浅蓝色背景 */"
                                  "    border: 1px solid #007bff; /* 深蓝色边框 */"
                                  "    font: 18px 黑体;"
                                  "}"
                                  "QTableWidget::item {"
                                  "    border: none; /* 无单元格边框 */"
                                  "    padding: 5px; /* 单元格内边距 */"
                                  "}"
                                  "QTableWidget::item:selected {"
                                  "    background-color: #007bff; /* 选中单元格的背景颜色 */"
                                  "    color: white;"
                                  "}"
                                  "QTableWidget::horizontalHeader {"
                                  "    background-color: #007bff; /* 水平表头背景颜色 */"
                                  "    color: white; /* 水平表头文字颜色 */"
                                  "    font-weight: bold; /* 水平表头文字加粗 */"
                                  "    border: none; /* 无水平表头边框 */"
                                  "    height: 40px; /* 水平表头高度 */"
                                  "}"
                                  "QTableWidget::horizontalHeader::section {"
                                  "    border-bottom: 1px solid #FFFFFF; /* 水平表头底部分隔线 */"
                                  "    border-top: 3px solid #FFFFFF; /* 水平表头顶部分隔线 */"
                                  "    border-left: 1px solid #FFFFFF; /* 水平表头左侧分隔线 */"
                                  "    border-right: none; /* 无右侧分隔线 */"
                                  "}"
                                  "QTableWidget::verticalHeader::section {"
                                  "    border-right: 1px solid #FFFFFF; /* 垂直表头右侧分隔线 */"
                                  "    border-left: 1px solid #FFFFFF; /* 无左侧分隔线 */"
                                  "}"
                                  );
    tableDiaryInfoDM->resize(952, 492);
    tableDiaryInfoDM->horizontalHeader()->setMinimumHeight(40);
    for (int i = 0; i < tableDiaryInfoDM->rowCount(); i++){
        tableDiaryInfoDM->setRowHeight(i, 45);
        for (int j = 0; j < tableDiaryInfoDM->columnCount(); j++)
            tableDiaryInfoDM->setItem(i, j, new QTableWidgetItem(""));
    }
    tableDiaryInfoDM->setColumnWidth(0, 100);
    tableDiaryInfoDM->setColumnWidth(1, 200);
    tableDiaryInfoDM->setColumnWidth(2, 200);
    tableDiaryInfoDM->setColumnWidth(3, 250);
    tableDiaryInfoDM->setColumnWidth(4, 100);
    tableDiaryInfoDM->setColumnWidth(5, 100);

    boxDestinationDM = new QComboBox(this);
    boxDestinationDM->move(890, 218);
    boxDestinationDM->resize(240, 50);
    boxDestinationDM->addItem("全部");
    query.exec("select name from t_place_ranking");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxDestinationDM->addItem(name);
    }
    boxDestinationDM->setStyleSheet(""
                             "QComboBox {"
                             "    border: 2px solid #A8A8A8;"
                             "    border-radius: 5px;"
                             "    padding: 1px 18px 1px 3px;"
                             "    min-width: 4em;"
                             "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                             "                                stop: 0 #E1E1E1, stop: 1.0 #D3D3D3);"
                             "    font: bold 18px 黑体;"
                             "}"
                             "QComboBox:hover {"
                             "    border: 2px solid #7EB6FF;"
                             "}"
                             "QComboBox QAbstractItemView {"
                             "    border: 1px solid #A8A8A8;"
                             "    selection-background-color: #7EB6FF;"
                             "    background: white;"
                             "    outline: 0;"
                             "}"
                             "");
}

vector<DiaryInfo> Diary_Management::search(string keyword, vector<DiaryInfo> diaries){
    vector<DiaryInfo> result;
    if ((mode == 1 && keyword == "全部") || (mode == 2 && keyword.empty()))
        return diaries;   //直接返回所有日记信息
    if (mode == 1) {
        for (const auto& diary : diaries) {
            if (compareStrings(diary.destination, keyword))  //精确查找目的地
                result.push_back(diary);
        }
    }
    else if (mode == 2) {
        vector<MatchedDiary> fuzzyResults = fuzzySearch(keyword, diaries);  //调用fuzzySearch函数进行模糊查找，并限制返回结果最多为10项
        for (const auto& matchedDiary : fuzzyResults) {
            result.push_back(matchedDiary.diary);
            if (result.size() >= 10) break;  //达到10项后停止添加
        }
    }
    return result;
}

bool Diary_Management::compareStrings(const string& str1, const string& str2){
    if (str1.size() != str2.size()) return false;
    for (size_t i = 0; i < str1.size(); ++i){
        if (str1[i] != str2[i]) return false;
    }
    return true;
}

bool Diary_Management::containsSubstring(const string& str, const string& sub){
    for (size_t i = 0; i <= str.size() - sub.size(); ++i) {
        bool match = true;
        for (size_t j = 0; j < sub.size(); ++j) {
            if (str[i + j] != sub[j]) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

vector<MatchedDiary> Diary_Management::fuzzySearch(string keyword, vector<DiaryInfo> diaries){
    vector<MatchedDiary> continuousMatches, partialMatches;
    for (const auto& diary : diaries) {
        if (containsSubstring(diary.diaryName, keyword))  //连续包含模式串
            continuousMatches.push_back({ diary, CONTINUOUS });
        else {  //包含部分模式串
            for (size_t i = 0; i < keyword.size(); ++i) {
                if (containsSubstring(diary.diaryName, keyword.substr(i))) {
                    partialMatches.push_back({ diary, PARTIAL });
                    break; // 防止同一日记因多个子串匹配而重复添加
                }
            }
        }
    }
    //合并结果，优先连续包含的，再补充部分包含的，直到满10个
    vector<MatchedDiary> results;
    results.reserve(10);
    results.insert(results.end(), continuousMatches.begin(), continuousMatches.end());
    while (results.size() < 10 && !partialMatches.empty()) {
        results.push_back(partialMatches.back());
        partialMatches.pop_back();
    }
    return results;
}

void Diary_Management::paintEvent(QPaintEvent*) {
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
    painter.drawRect(250, 0, 1000, 950);
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setOpacity(1);
    painter.setFont(QFont("黑体", 25));
    QRect textRect = painter.boundingRect(QRect(), Qt::TextSingleLine, "当前所在景区/学校:" + place);
    painter.drawText((LENGTH - textRect.width())/2, 100, "当前所在景区/学校:" + place);
}




