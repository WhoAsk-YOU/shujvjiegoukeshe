//场所查询界面
#include "search_architect.h"

Search_Architect::Search_Architect(QWidget *parent)
    : QWidget{parent}
{
    initWidget();
    connect(buttonChoosebackSA,&QPushButton::clicked,[=](){  //若点击返回按钮
        emit this->chooseback();  //发出返回选择界面的信号
    });
    connect(buttonSearchSA,&QPushButton::clicked,[=](){  //若点击查询按钮
        showResult();
    });
}

Search_Architect::~Search_Architect(){
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

void Search_Architect::initWidget(){
    QSqlQuery query;
    QStringList types;
    QStringList horizontalHeaderLabels;
    types<<"全部"<<"商店"<<"饭店"<<"洗手间"<<"图书馆"<<"食堂"<<"超市"<<"咖啡馆"<<"医院"<<"花园"<<"体育馆";
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH,WIDTH);

    buttonChoosebackSA = new QPushButton("返回", this);
    buttonChoosebackSA->move(0, WIDTH*8/9);
    buttonChoosebackSA->resize(LENGTH/9, WIDTH/9);
    buttonChoosebackSA->setFont(QFont("黑体",25));
    buttonSearchSA = new QPushButton("查询", this);
    buttonSearchSA->move(1050, 130);
    buttonSearchSA->resize(80, 45);
    buttonSearchSA->setFont(QFont("黑体",20));

    boxTypeSA = new QComboBox(this);
    boxTypeSA->move(750,130);
    boxTypeSA->resize(200,45);
    boxTypeSA->setFont(QFont("黑体",20));
    boxTypeSA->addItems(types);
    boxLocationSA = new QComboBox(this);
    boxLocationSA->move(350,130);
    boxLocationSA->resize(200,45);
    boxLocationSA->setFont(QFont("黑体",20));
    query.exec("select name from t_architect");
    while(query.next()){
        QString name = query.value(0).toString();
        boxLocationSA->addItem(name);
    }

    labelLocationSA = new QLabel("查询场所：",this);
    labelLocationSA->setGeometry(200, 130, 200, 45);
    labelLocationSA->setAlignment(Qt::AlignCenter);
    labelLocationSA->setStyleSheet("QLabel{font:20px;}");
    labelTypeSA = new QLabel("设施类型：",this);
    labelTypeSA->setGeometry(590, 130, 200, 45);
    labelTypeSA->setAlignment(Qt::AlignCenter);
    labelTypeSA->setStyleSheet("QLabel{font:20px;}");

    searchTableSA = new QTableWidget(5,2,this);  //创建5行2列的表格
    searchTableSA->verticalHeader()->setVisible(false);  //隐藏垂直表头
    searchTableSA->setEditTriggers(QAbstractItemView::NoEditTriggers);  //表格内的数据不允许修改
    searchTableSA->move(330,300);
    searchTableSA->resize(752,402);
    searchTableSA->setFont(QFont("黑体",20));
    searchTableSA->horizontalHeader()->setMinimumHeight(50);
    for (int i = 0; i < searchTableSA->rowCount(); i++)  //设置每一行的高度为70像素
        searchTableSA->setRowHeight(i, 70);
    searchTableSA->setColumnWidth(0, 500);  // 设置第1列的宽度为500像素
    searchTableSA->setColumnWidth(1, 250);
    horizontalHeaderLabels << "设施名称" << "到查询场所的距离";
    searchTableSA->setHorizontalHeaderLabels(horizontalHeaderLabels);
}

StringList Search_Architect::search(const std::string& type, const StringList& allFacilities) {
    StringList result;

    // 如果设施类型为“全部”，直接返回所有设施名称
    if (type == "全部")
        return allFacilities;

    // 否则，搜索与设施类型相符的设施名（设施类型为设施名的子串）
    for (const std::string& facility : allFacilities) {
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

int Search_Architect::dijkstraLength(const std::string& start, const std::string& end, const std::vector<RoadLengthInfo>& roads){
    //初始化距离和前一个点映射
    std::unordered_map<std::string, int> distances;
    std::unordered_map<std::string, std::string> prev;
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;

    //为所有节点设置初始距离为正无穷大，除了起点
    for (const auto& road : roads) {
        distances[road.start] = std::numeric_limits<int>::max();
        distances[road.end] = std::numeric_limits<int>::max();
    }
    distances[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {  //主循环
        int currDist = pq.top().first;
        std::string currNode = pq.top().second;
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

std::vector<std::pair<std::string, int>> Search_Architect::sortPlacesByDistance(const std::string& currentLocation,
                                                                                const std::string& facilityType,
                                                                                const StringList& allFacilities,
                                                                                const std::vector<RoadLengthInfo>& roads){
    std::vector<std::pair<std::string, int>> result;

    //获取当前位置到所有场所的最短距离，并筛选出指定类型的设施
    std::unordered_map<std::string, int> distanceMap;
    for (const auto& facility : allFacilities) {
        if (facility != currentLocation && search(facilityType, { facility }).size() > 0) {
            int distance = dijkstraLength(currentLocation, facility, roads);
            distanceMap[facility] = distance;
        }
    }

    //实现topk排序
    std::vector<std::pair<std::string, int>> sortedPlaces;
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
        std::swap(sortedPlaces[i], sortedPlaces[minIndex]);
    }
    return sortedPlaces;
}

void Search_Architect::showResult(){
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

    query.exec("select name from t_architect order by architect_id limit 20,50");
    while(query.next()){
        std::string name = query.value(0).toString().toStdString();
        allFacilities.push_back(name);
    }

    query.clear();
    query.exec("select a1.name,a2.name,r.length "
               "from t_road r join t_architect a1 on r.start = a1.architect_id "
               "join t_architect a2 on r.end = a2.architect_id");
    while(query.next()){
        RoadLengthInfo roadInfo;
        roadInfo.start = query.value(0).toString().toStdString();
        roadInfo.end = query.value(1).toString().toStdString();
        roadInfo.length = query.value(2).toString().toInt();
        roadsInfo.push_back(roadInfo);
    }

    result = sortPlacesByDistance(location, type, allFacilities, roadsInfo);

    for(int i = 0; i < searchTableSA->rowCount() && i < (int)result.size(); i++){  //将排序后的数据填入表中
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(result[i].first));
        QTableWidgetItem* itemDistance = new QTableWidgetItem(QString::number(result[i].second));
        itemName->setTextAlignment(Qt::AlignCenter);
        itemDistance->setTextAlignment(Qt::AlignCenter);
        searchTableSA->setItem(i, 0, itemName);
        searchTableSA->setItem(i, 1, itemDistance);
    }
}
