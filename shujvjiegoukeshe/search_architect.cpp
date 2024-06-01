//场所查询界面
#include "search_architect.h"

Search_Architect::Search_Architect(QString place)
{
    this->place = place;
    initWidget();
    connect(buttonChoosebackSA, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回选择界面的信号
    });
    connect(buttonSearchSA, &QPushButton::clicked, [=]() {  //若点击查询按钮
        showResult();
    });
}

Search_Architect::~Search_Architect() {
    delete buttonChoosebackSA;
    buttonChoosebackSA = NULL;
    delete buttonSearchSA;
    buttonSearchSA = NULL;
    delete boxLocationSA;
    boxLocationSA = NULL;
    delete boxTypeSA;
    boxTypeSA = NULL;
    delete labelTypeSA;
    labelTypeSA = NULL;
    delete labelLocationSA;
    labelLocationSA = NULL;
    delete searchTableSA;
    searchTableSA = NULL;
}

void Search_Architect::showResult() {
    QSqlQuery query;
    StringList allFacilities;  //所有设施信息
    std::vector<RoadLengthInfo> roadsInfo;  //所有道路信息
    std::string location = boxLocationSA->currentText().toStdString();  //要查询的位置
    std::string type = boxTypeSA->currentText().toStdString();  //要查询的设施
    std::vector<std::pair<std::string, int>> result;

    for (int i = 0; i < searchTableSA->rowCount(); i++) {  //清空表中的内容
        searchTableSA->setItem(i, 0, new QTableWidgetItem(""));
        searchTableSA->setItem(i, 1, new QTableWidgetItem(""));
    }

    query.exec("select name from t_roadnode order by roadnode_id limit 20,50");  //从数据库表里查询所有设施信息
    while (query.next()) {
        std::string name = query.value(0).toString().toStdString();
        allFacilities.push_back(name);
    }
    query.clear();
    query.exec("select a1.name,a2.name,r.length "
               "from t_road r join t_roadnode a1 on r.start = a1.roadnode_id "
               "join t_roadnode a2 on r.end = a2.roadnode_id");  //从数据库表里查询所有道路信息
    while (query.next()) {
        RoadLengthInfo roadInfo;
        roadInfo.start = query.value(0).toString().toStdString();
        roadInfo.end = query.value(1).toString().toStdString();
        roadInfo.length = query.value(2).toInt();
        roadsInfo.push_back(roadInfo);
    }

    result = sortPlacesByDistance(location, type, allFacilities, roadsInfo);

    for (int i = 0; i < searchTableSA->rowCount() && i < (int)result.size(); i++) {  //将排序后的数据填入表中
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(result[i].first));
        QTableWidgetItem* itemDistance = new QTableWidgetItem(QString::number(result[i].second));
        itemName->setTextAlignment(Qt::AlignCenter);
        itemDistance->setTextAlignment(Qt::AlignCenter);
        searchTableSA->setItem(i, 0, itemName);
        searchTableSA->setItem(i, 1, itemDistance);
    }
}

void Search_Architect::initWidget() {
    QSqlQuery query;
    QStringList types;
    QStringList horizontalHeaderLabels;
    types << "全部" << "商店" << "饭店" << "洗手间" << "图书馆" << "食堂" << "超市" << "咖啡馆" << "医院" << "花园" << "体育馆";
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);
    buttonChoosebackSA = new QPushButton("返回", this);
    buttonChoosebackSA->move(0, WIDTH * 17 / 18);
    buttonChoosebackSA->resize(50, WIDTH / 18);
    buttonChoosebackSA->setStyleSheet(
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
    buttonSearchSA = new QPushButton("查询", this);
    buttonSearchSA->move(LENGTH / 2 - 35, 750);
    buttonSearchSA->resize(70, WIDTH / 17);
    buttonSearchSA->setStyleSheet("QPushButton {"
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
    boxTypeSA = new QComboBox(this);
    boxTypeSA->move(874, 230);
    boxTypeSA->resize(130, 45);
    boxTypeSA->addItems(types);
    boxTypeSA->setStyleSheet(""
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
    boxLocationSA = new QComboBox(this);
    boxLocationSA->move(554, 230);
    boxLocationSA->resize(130, 45);
    query.exec("select name from t_roadnode order by roadnode_id limit 70");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxLocationSA->addItem(name);
    }
    boxLocationSA->setStyleSheet(""
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
    labelLocationSA = new QLabel("场所：", this);
    labelLocationSA->setGeometry(430, 230, 200, 45);
    labelLocationSA->setAlignment(Qt::AlignCenter);
    labelLocationSA->setStyleSheet("QLabel{font:20px;}");
    labelTypeSA = new QLabel("场所附近设施类型：", this);
    labelTypeSA->setGeometry(689, 230, 200, 45);
    labelTypeSA->setAlignment(Qt::AlignCenter);
    labelTypeSA->setStyleSheet("QLabel{font:20px;}");

    searchTableSA = new QTableWidget(5, 2, this);  //创建5行2列的表格
    searchTableSA->verticalHeader()->setVisible(false);  //隐藏垂直表头
    searchTableSA->setEditTriggers(QAbstractItemView::NoEditTriggers);  //表格内的数据不允许修改
    searchTableSA->move(LENGTH / 2 - 251, 300);
    searchTableSA->setFont(QFont("黑体", 20));
    horizontalHeaderLabels << "设施名称" << "到场所的距离(m)";
    searchTableSA->setHorizontalHeaderLabels(horizontalHeaderLabels);
    searchTableSA->setStyleSheet("QTableWidget {"
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
    searchTableSA->resize(502, 402);
    searchTableSA->horizontalHeader()->setMinimumHeight(50);
    for (int i = 0; i < searchTableSA->rowCount(); i++)  //设置每一行的高度为70像素
        searchTableSA->setRowHeight(i, 70);
    searchTableSA->setColumnWidth(0, 300);  // 设置第1列的宽度为300像素
    searchTableSA->setColumnWidth(1, 200);
}

StringList Search_Architect::search(const string& type, const StringList& allFacilities) {
    StringList result;

    // 如果设施类型为“全部”，直接返回所有设施名称
    if (type == "全部")
        return allFacilities;

    // 否则，搜索与设施类型相符的设施名（设施类型为设施名的子串）
    for (const string& facility : allFacilities) {
        if (facility.size() >= type.size()) { // 如果设施名长度小于设施类型，不符合条件
            bool match = true;
            for (size_t i = 0; i <= facility.size() - type.size(); ++i) {
                match = true;
                for (size_t j = 0; j < type.size(); ++j) {
                    if (tolower(facility[i + j]) != tolower(type[j])) { // 忽略大小写进行匹配
                        match = false;
                        break;
                    }
                }
                if (match) { // 如果找到匹配的子串，则加入结果列表中
                    result.push_back(facility);
                    break; // 终止内部循环，继续下一个设施名的匹配
                }
            }
        }
    }
    return result;
}

int Search_Architect::dijkstraLength(const string& start, const string& end, const vector<RoadLengthInfo>& roads) {
    //初始化距离和前一个点映射
    unordered_map<string, int> distances;
    unordered_map<string, string> prev;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    //为所有节点设置初始距离为正无穷大，除了起点
    for (const auto& road : roads) {
        distances[road.start] = numeric_limits<int>::max();
        distances[road.end] = numeric_limits<int>::max();
    }
    distances[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {  //主循环
        int currDist = pq.top().first;
        string currNode = pq.top().second;
        pq.pop();
        if (currDist > distances[currNode])  // 如果当前距离大于已知最短距离，则跳过
            continue;
        for (const auto& road : roads) {
            if (road.start == currNode) {
                int alt = currDist + road.length;
                if (alt < distances[road.end]) {
                    distances[road.end] = alt;
                    prev[road.end] = road.start;
                    pq.emplace(alt, road.end);
                }
            }
        }
    }
    int totallength = distances[end];  //获取最短距离
    return totallength;
}

vector<pair<string, int>> Search_Architect::sortPlacesByDistance(const string& currentLocation, const string& facilityType,
                                                                 const StringList& allFacilities, const vector<RoadLengthInfo>& roads) {
    vector<pair<string, int>> result;

    //获取当前位置到所有场所的最短距离，并筛选出指定类型的设施
    unordered_map<string, int> distanceMap;
    for (const auto& facility : allFacilities) {
        if (facility != currentLocation && search(facilityType, { facility }).size() > 0) {
            int distance = dijkstraLength(currentLocation, facility, roads);
            distanceMap[facility] = distance;
        }
    }

    //实现topk排序
    vector<pair<string, int>> sortedPlaces;
    for (const auto& entry : distanceMap)
        sortedPlaces.push_back(entry);

    //使用选择排序对结果进行排序
    for (int i = 0; i < std::min(10, static_cast<int>(sortedPlaces.size())); ++i) {
        int minIndex = i;
        for (int j = i + 1; j < (int)sortedPlaces.size(); ++j) {
            if (sortedPlaces[j].second < sortedPlaces[minIndex].second) {
                minIndex = j;
            }
        }
        swap(sortedPlaces[i], sortedPlaces[minIndex]);
    }
    return sortedPlaces;
}

void Search_Architect::paintEvent(QPaintEvent*) {
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
