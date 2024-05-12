//目的地推荐界面
#include "destination_recommendation.h"

Destination_Recommendation::Destination_Recommendation(QString accountNumber)
{
    this->accountNumber = accountNumber;
    initWidget();
    showResult();
    connect(buttonExitDR, &QPushButton::clicked, [=]() {  //若点击退出按钮
        emit this->chooseback();  //发出返回开始界面的信号
    });
    connect(buttonSearchDR, &QPushButton::clicked, [=]() {  //若点击搜索按钮
        showResult();  //表格会显示数据
    });
    connect(rankingTableDR, &QTableWidget::cellClicked, [=](int row, int column) {  //若点击表格第二列中的某个单元格
        tableClicked(row, column);  //跳转到选择界面
    });
}

Destination_Recommendation::~Destination_Recommendation() {  //析构函数，释放空间
    delete buttonExitDR;
    buttonExitDR = NULL;
    delete buttonSearchDR;
    buttonSearchDR = NULL;
    delete lineEditSearchDR;
    lineEditSearchDR = NULL;
    delete buttonHeatValueDR;
    buttonHeatValueDR = NULL;
    delete buttonGoodCommentsDR;
    buttonGoodCommentsDR = NULL;
    delete rankingTableDR;
    rankingTableDR = NULL;
    delete boxKeyWordDR;
    boxKeyWordDR = NULL;
}

void Destination_Recommendation::showResult() {
    QSqlQuery query;
    QStringList horizontalHeaderLabels;
    vector<Place> origin_Places, SearchedPlaces;  //把数组放进向量里，统一PlaceSearch函数返回值格式
    string place = lineEditSearchDR->text().toStdString();

    for (int i = 0; i < rankingTableDR->rowCount(); i++) {  //清空表中的内容
        rankingTableDR->setItem(i, 1, new QTableWidgetItem(""));
        rankingTableDR->setItem(i, 2, new QTableWidgetItem(""));
    }

    if (buttonHeatValueDR->isChecked()) {  //若选中了按热度排序
        query.exec("select name,heat_value from t_place_ranking");  //从数据库中拿出name及heat_value的所有字段值
        horizontalHeaderLabels << "排名" << "名称" << "热度";  //更改水平表头
        rankingTableDR->setHorizontalHeaderLabels(horizontalHeaderLabels);
    }
    else if (buttonGoodCommentsDR->isChecked()) {  //若选中了按评论排序
        query.exec("select name,good_comments from t_place_ranking");  //从数据库中拿出name及good_comments的所有字段值
        horizontalHeaderLabels << "排名" << "名称" << "好评数";  //更改水平表头
        rankingTableDR->setHorizontalHeaderLabels(horizontalHeaderLabels);
    }

    while(query.next()) {  //遍历数据集，将数据加入结构数组
        Place spot;
        spot.name = query.value(0).toString().toStdString();  //获取景区/学校的名称
        spot.value = query.value(1).toInt();  //获取对应的热度或好评数
        origin_Places.push_back(spot);
    }

    if (boxKeyWordDR->isChecked())  //如果选中了关键词优先
        SearchedPlaces = fuzzySearchPlaces(place, origin_Places, 4);
    else  //如果没选中关键词优先
        SearchedPlaces = sort(fuzzySearchPlaces(place, origin_Places, 4));  //模糊查找后再排序


    for (int i = 0; i < min(rankingTableDR->rowCount(),(int)SearchedPlaces.size()); i++) {  //将排序后的数据填入表中
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(SearchedPlaces[i].name));
        QTableWidgetItem* itemValue = new QTableWidgetItem(QString::number(SearchedPlaces[i].value));
        itemName->setTextAlignment(Qt::AlignCenter);
        itemValue->setTextAlignment(Qt::AlignCenter);
        rankingTableDR->setItem(i, 1, itemName);
        rankingTableDR->setItem(i, 2, itemValue);
    }
}

void Destination_Recommendation::tableClicked(int row, int column) {
    if (column == 1) {
        if (rankingTableDR->item(row, column)->text().isEmpty())
            return;
        QTableWidgetItem* item = rankingTableDR->item(row, column);  //获取到点击单元格的内容，即景区/学校名
        this->hide();  //隐藏目的地推荐界面
        chooseWidget = new Choose_Widget(accountNumber, item->text());
        chooseWidget->setGeometry(this->geometry());  //界面位置不会变化
        chooseWidget->show();  //显示选择界面
        connect(chooseWidget, &Choose_Widget::chooseback, [=]() {  //若在选择界面点击了返回按钮，接受返回信号
            this->setGeometry(chooseWidget->geometry());
            this->show();
            delete chooseWidget;
            chooseWidget = NULL;
        });
    }
}

void Destination_Recommendation::initWidget() {  //初始化目的地推荐界面
    QStringList horizontalHeaderLabels;
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);
    buttonExitDR = new QPushButton("退出", this);
    buttonExitDR->move(0, WIDTH*17/18);
    buttonExitDR->resize(50, WIDTH / 18);
    buttonExitDR->setStyleSheet(
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
    buttonSearchDR = new QPushButton("搜索", this);
    buttonSearchDR->move(LENGTH / 2 + LENGTH / 7 - 30, WIDTH * 3 / 19 + 90);
    buttonSearchDR->resize(80, WIDTH / 17);
    buttonSearchDR->setStyleSheet("QPushButton {"
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

    lineEditSearchDR = new QLineEdit(this);
    lineEditSearchDR->setGeometry(LENGTH / 2 - LENGTH / 7 -30, WIDTH * 3 / 19 + 90, LENGTH / 3.5, WIDTH / 17);
    lineEditSearchDR->setPlaceholderText("请输入景区或学校名/关键词");
    lineEditSearchDR->setClearButtonEnabled(true);
    lineEditSearchDR->setMaxLength(20);
    lineEditSearchDR->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff]+"), this));  //搜索目的地只允许输入中文字符
    lineEditSearchDR->setStyleSheet("QLineEdit {"
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

    buttonHeatValueDR = new QRadioButton("按热度排序", this);
    buttonHeatValueDR->setChecked(true);  //默认选中按热度排序
    buttonHeatValueDR->move(540, 190);
    buttonHeatValueDR->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");
    buttonGoodCommentsDR = new QRadioButton("按评价排序", this);
    buttonGoodCommentsDR->move(690, 190);
    buttonGoodCommentsDR->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");

    boxKeyWordDR = new QCheckBox("关键词优先", this);
    boxKeyWordDR->setGeometry(840, 171, 100, 60);
    boxKeyWordDR->setFont(QFont("黑体", 12));

    rankingTableDR = new QTableWidget(10, 3, this);  //创建10行3列的表格，用于显示景区/学校排名
    rankingTableDR->verticalHeader()->setVisible(false);  //隐藏垂直表头
    rankingTableDR->setEditTriggers(QAbstractItemView::NoEditTriggers);  //表格内的数据不允许修改
    rankingTableDR->move(LENGTH / 2 - 285, 300);
    //设置水平表头
    horizontalHeaderLabels << "排名" << "名称" << "热度";
    rankingTableDR->setHorizontalHeaderLabels(horizontalHeaderLabels);
    //设置表格样式
    rankingTableDR->setStyleSheet("QTableWidget {"
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
    rankingTableDR->resize(572, 492);
    rankingTableDR->horizontalHeader()->setMinimumHeight(40);
    //设置表格内容和样式
    for (int i = 0; i < rankingTableDR->rowCount(); i++) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(i + 1));
        item->setTextAlignment(Qt::AlignCenter);  //文本居中对齐
        rankingTableDR->setItem(i, 0, item);
        rankingTableDR->setItem(i, 1, new QTableWidgetItem(""));
        rankingTableDR->setItem(i, 2, new QTableWidgetItem(""));
        rankingTableDR->setRowHeight(i, 45);  //设置每一行的高度为45像素
    }
    rankingTableDR->setColumnWidth(0, 100);  // 设置第1列的宽度为100像素
    rankingTableDR->setColumnWidth(1, 320);
    rankingTableDR->setColumnWidth(2, 150);
}

vector<Place> Destination_Recommendation::fuzzySearchPlaces(const string& query, const vector<Place>& places, int n) {
    if (empty(query))  //如果为空串，直接返回
        return places;

    vector<Place> results;
    StringList queryNgrams = generateNgrams(query, n);

    for (const auto& place : places) {
        StringList placeNgrams = generateNgrams(place.name, n);
        bool isMatch = false;
        for (const auto& queryNgram : queryNgrams) {
            if (::find(placeNgrams.begin(), placeNgrams.end(), queryNgram) != placeNgrams.end()) {
                isMatch = true;
                break;
            }
        }
        if (isMatch)
            results.push_back(place);
    }
    quickSort(results, 0, results.size() - 1, queryNgrams);  //根据匹配程度对结果进行排序
    return results;
}

StringList Destination_Recommendation::generateNgrams(const string& str, int n) {
    StringList ngrams;
    for (size_t i = 0; i <= str.length() - n; ++i)
        ngrams.push_back(str.substr(i, n));
    return ngrams;
}

bool Destination_Recommendation::comparePlaceMatch(const Place& a, const Place& b, const StringList& queryNgrams) {
    StringList aNgrams = generateNgrams(a.name, 4);
    StringList bNgrams = generateNgrams(b.name, 4);

    int aMatches = 0;
    int bMatches = 0;
    for (const auto& queryNgram : queryNgrams) {
        if (::find(aNgrams.begin(), aNgrams.end(), queryNgram) != aNgrams.end())
            ++aMatches;
        if (::find(bNgrams.begin(), bNgrams.end(), queryNgram) != bNgrams.end())
            ++bMatches;
    }
    if (aMatches == bMatches)  //如果匹配数量相同，则按字母顺序排序
        return a.name < b.name;
    return aMatches > bMatches;  //返回匹配数量多的地名在前
}

bool Destination_Recommendation::compareById(const Place& a, const Place& b) {
    return a.value > b.value;
}

void Destination_Recommendation::quickSort(vector<Place>& arr, int left, int right, const StringList& queryNgrams) {
    if (left >= right) return;
    int pivotIndex = partition(arr, left, right, queryNgrams);
    quickSort(arr, left, pivotIndex - 1, queryNgrams);
    quickSort(arr, pivotIndex + 1, right, queryNgrams);
}

int Destination_Recommendation::partition(vector<Place>& arr, int left, int right, const StringList& queryNgrams) {
    Place pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; ++j) {
        if (comparePlaceMatch(arr[j], pivot, queryNgrams)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    return i + 1;
}

vector<Place> Destination_Recommendation::sort(const vector<Place>& placeNames) {
    vector<Place> sortedData(placeNames);  //拷贝数据以便排序
    int size = sortedData.size();
    if (size <= 10) {  //如果数据少于或等于10个，直接对整个向量进行冒泡排序（这里选用冒泡排序是因为它简单且仅处理少量数据）
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - 1 - i; ++j) {
                if (compareById(sortedData[j + 1], sortedData[j])) {
                    swap(sortedData[j], sortedData[j + 1]);
                }
            }
        }
    }
    else {
        //找到前10个最大值
        int maxnum;
        int maxdata = 0;
        string maxname = sortedData[0].name;
        for (int i = 0; i < 10; i++)  //用选择排序
        {
            maxdata = sortedData[i].value;
            for (int j = i; j < size; j++)
            {
                if (maxdata < sortedData[j].value)
                {
                    maxdata = sortedData[j].value;
                    maxname = sortedData[j].name;
                    maxnum = j;
                }
            }
            sortedData[maxnum].name = sortedData[i].name;
            sortedData[maxnum].value = sortedData[i].value;
            sortedData[i].name = maxname;
            sortedData[i].value = maxdata;
        }
    }
    //返回前10个已排序的元素
    return vector<Place>(sortedData.begin(), sortedData.begin() + min(10, size));
}

void Destination_Recommendation::paintEvent(QPaintEvent*) {
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
    painter.setOpacity(0.5);  // 设置透明度，0.0为完全透明，1.0为完全不透明
    painter.drawRect(400, 0, 700, 950);
    painter.setOpacity(1.0);
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setFont(QFont("黑体", 25));
    QRect textRect = painter.boundingRect(QRect(), Qt::TextSingleLine, "欢迎用户:" + accountNumber);
    painter.drawText((LENGTH - textRect.width())/2, 100, "欢迎用户:" + accountNumber);
}
