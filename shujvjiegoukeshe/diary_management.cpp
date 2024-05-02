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
        diaryRead = new Diary_Read(item->text().toInt());
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

    labelDestinationDM = new QLabel("游学目的地：", this);
    labelDestinationDM->setGeometry(300, 150, 200, 45);
    labelDestinationDM->setAlignment(Qt::AlignCenter);
    labelDestinationDM->setStyleSheet("QLabel{font:24px;}");

    buttonChoosebackDM = new QPushButton("返回", this);
    buttonChoosebackDM->move(0, WIDTH * 8 / 9);
    buttonChoosebackDM->resize(LENGTH / 9, WIDTH / 9);
    buttonChoosebackDM->setFont(QFont("黑体", 25));
    buttonWriteDM = new QPushButton("写日记", this);
    buttonWriteDM->move(LENGTH * 8 / 9, WIDTH * 8 / 9);
    buttonWriteDM->resize(LENGTH / 9, WIDTH / 9);
    buttonWriteDM->setFont(QFont("黑体", 25));
    buttonQueryDestinationDM = new QPushButton("查询", this);
    buttonQueryDestinationDM->move(870, 150);
    buttonQueryDestinationDM->resize(80, 45);
    buttonQueryDestinationDM->setFont(QFont("黑体", 20));
    buttonQueryDiaryNameDM = new QPushButton("查询", this);
    buttonQueryDiaryNameDM->move(870, 230);
    buttonQueryDiaryNameDM->resize(80, 45);
    buttonQueryDiaryNameDM->setFont(QFont("黑体", 20));

    buttonHeatValueDM = new QRadioButton("按热度排序", this);
    buttonHeatValueDM->setChecked(true);
    buttonHeatValueDM->move(1000, 190);
    buttonHeatValueDM->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");
    buttonGoodCommentsDM = new QRadioButton("按评价排序", this);
    buttonGoodCommentsDM->move(1000, 220);
    buttonGoodCommentsDM->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");

    lineEditDiaryNameDM = new QLineEdit(this);
    lineEditDiaryNameDM->setGeometry(320, WIDTH * 3 / 19 + 75, 450, WIDTH / 17);
    lineEditDiaryNameDM->setFont(QFont("黑体", 15));
    lineEditDiaryNameDM->setPlaceholderText("请输入日记名称");
    lineEditDiaryNameDM->setClearButtonEnabled(true);
    lineEditDiaryNameDM->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff]+"), this));  //搜索日记名只允许输入中文字符

    tableDiaryInfoDM = new QTableWidget(10, 6, this);  //创建10行6列的表格，用于显示日记信息
    tableDiaryInfoDM->verticalHeader()->setVisible(false);
    tableDiaryInfoDM->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableDiaryInfoDM->move((LENGTH - 1052)/2, 330);
    tableDiaryInfoDM->resize(1052, 492);
    tableDiaryInfoDM->setFont(QFont("黑体", 14));
    tableDiaryInfoDM->horizontalHeader()->setMinimumHeight(40);
    for (int i = 0; i < tableDiaryInfoDM->rowCount(); i++){
        tableDiaryInfoDM->setRowHeight(i, 45);
        for (int j = 0; j < tableDiaryInfoDM->columnCount(); j++)
            tableDiaryInfoDM->setItem(i, j, new QTableWidgetItem(""));
    }
    tableDiaryInfoDM->setColumnWidth(0, 100);
    tableDiaryInfoDM->setColumnWidth(1, 250);
    tableDiaryInfoDM->setColumnWidth(2, 250);
    tableDiaryInfoDM->setColumnWidth(3, 250);
    tableDiaryInfoDM->setColumnWidth(4, 100);
    tableDiaryInfoDM->setColumnWidth(5, 100);
    horizontalHeaderLabels << "日记编号" << "日记名称" << "作者账户名" << "目的地" << "热度" << "好评数";
    tableDiaryInfoDM->setHorizontalHeaderLabels(horizontalHeaderLabels);

    boxDestinationDM = new QComboBox(this);
    boxDestinationDM->move(490, 150);
    boxDestinationDM->resize(280, 45);
    boxDestinationDM->setFont(QFont("黑体", 17));
    boxDestinationDM->addItem("全部");
    query.exec("select name from t_place_ranking");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxDestinationDM->addItem(name);
    }
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
    QPen pen(Qt::black);  //画笔颜色为黑色
    pen.setWidth(2);  //画笔宽度
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setFont(QFont("黑体", 25));
    painter.drawText(270,100, "当前账户名:" + accountNumber);
    painter.drawText(650,100, "当前所在景区/学校:" + place);
}




