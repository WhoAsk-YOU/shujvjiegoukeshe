//游学路线规划界面
#include "route_strategy.h"
#include <iostream>
#include <QDebug>
Route_Strategy::Route_Strategy(QWidget* parent)
    : QWidget{ parent }
{
    initWidget();
    connect(buttonChoosebackRS, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回目的地推荐界面的信号
    });
    connect(buttonOneQueryRS, &QPushButton::clicked, [=]() {  //若点击目的地(1个)查询按钮
        mode = 1;
        update();  //调用paintEvent函数
    });
    connect(buttonQueriesRS, &QPushButton::clicked, [=]() {  //若点击目的地(多个)查询按钮
        mode = 2;
        update();  //调用paintEvent函数
    });
}

Route_Strategy::~Route_Strategy() {
    delete labelInitialRS;
    labelInitialRS = NULL;
    delete labelDestinationRS;
    labelDestinationRS = NULL;
    delete labelDestinationsRS;
    labelDestinationsRS = NULL;
    delete buttonChoosebackRS;
    buttonChoosebackRS = NULL;
    delete buttonOneQueryRS;
    buttonOneQueryRS = NULL;
    delete buttonQueriesRS;
    buttonQueriesRS = NULL;
    delete boxInitialLocationRS;
    boxInitialLocationRS = NULL;
    delete boxDestinationRS;
    boxDestinationRS = NULL;
    delete buttonShortestDistanceRS;
    buttonShortestDistanceRS = NULL;
    delete buttonShortestTimeRS;
    buttonShortestTimeRS = NULL;
    delete lineEditDestinationsRS;
    lineEditDestinationsRS = NULL;
}

void Route_Strategy::initWidget() {
    QSqlQuery query;
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);

    labelInitialRS = new QLabel("起始位置：", this);
    labelInitialRS->setGeometry(150, 80, 200, 45);
    labelInitialRS->setAlignment(Qt::AlignCenter);
    labelInitialRS->setStyleSheet("QLabel{font:20px;}");
    labelDestinationRS = new QLabel("目的地(1个)：", this);
    labelDestinationRS->setGeometry(540, 80, 200, 45);
    labelDestinationRS->setAlignment(Qt::AlignCenter);
    labelDestinationRS->setStyleSheet("QLabel{font:20px;}");
    labelDestinationsRS = new QLabel("目的地(多个)：", this);
    labelDestinationsRS->setGeometry(165, 160, 200, 45);
    labelDestinationsRS->setAlignment(Qt::AlignCenter);  //文本框内容居中显示
    labelDestinationsRS->setStyleSheet("QLabel{font:20px;}");  //设置文本框字体

    buttonChoosebackRS = new QPushButton("返回", this);
    buttonChoosebackRS->move(0, WIDTH * 8 / 9);
    buttonChoosebackRS->resize(LENGTH / 9, WIDTH / 9);
    buttonChoosebackRS->setFont(QFont("黑体", 25));
    buttonOneQueryRS = new QPushButton("查询", this);
    buttonOneQueryRS->move(1000, 80);
    buttonOneQueryRS->resize(80, 45);
    buttonOneQueryRS->setFont(QFont("黑体", 20));
    buttonQueriesRS = new QPushButton("查询", this);
    buttonQueriesRS->move(1000, 160);
    buttonQueriesRS->resize(80, 45);
    buttonQueriesRS->setFont(QFont("黑体", 20));

    boxInitialLocationRS = new QComboBox(this);
    boxInitialLocationRS->move(300, 80);
    boxInitialLocationRS->resize(200, 45);
    boxInitialLocationRS->setFont(QFont("黑体", 20));
    boxDestinationRS = new QComboBox(this);
    boxDestinationRS->move(700, 80);
    boxDestinationRS->resize(200, 45);
    boxDestinationRS->setFont(QFont("黑体", 20));
    query.exec("select name from t_architect");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxInitialLocationRS->addItem(name);
        boxDestinationRS->addItem(name);
    }

    buttonShortestTimeRS = new QRadioButton("最短时间", this);
    buttonShortestTimeRS->setChecked(true);
    buttonShortestTimeRS->move(1120, 120);
    buttonShortestTimeRS->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");  //设置按钮大小及字体大小
    buttonShortestDistanceRS = new QRadioButton("最短距离", this);
    buttonShortestDistanceRS->move(1120, 150);
    buttonShortestDistanceRS->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");

    lineEditDestinationsRS = new QLineEdit(this);
    lineEditDestinationsRS->setGeometry(LENGTH * 3.5 / 14 - 8, WIDTH * 3 / 19 + 5, LENGTH / 2.5, WIDTH / 17);
    lineEditDestinationsRS->setFont(QFont("黑体", 15));
    lineEditDestinationsRS->setPlaceholderText("请输入多个目的地（用空格分隔）");
    lineEditDestinationsRS->setClearButtonEnabled(true);
    lineEditDestinationsRS->setValidator(new QRegularExpressionValidator(QRegularExpression("^([\u4e00-\u9fa5\\d]+(\\s[\u4e00-\u9fa5\\d]+)*)$"), this));  //只允许输入中文、数字及空格
}

StringList Route_Strategy::pointToPointShortestDistance(const std::string& start, const std::string& end,
                                                                        const std::vector<RoadInfo>& roads, int& totallength) {
    std::pair<StringList, int> result = dijkstraLength(start, end, roads);
    StringList path = result.first;
    totallength = result.second;
    return path;
}

std::pair<StringList, int> Route_Strategy::dijkstraLength(const std::string& start, const std::string& end,const std::vector<RoadInfo>& roads) {
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
        if (currDist > distances[currNode])  //如果当前距离大于已知最短距离，则跳过
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

    // 重建最短路径
    StringList path;
    std::string curr = end;
    while (curr != start) {
        path.insert(path.begin(), curr);
        curr = prev[curr];
    }
    path.insert(path.begin(), start);
    return { path, totallength };
}

StringList Route_Strategy::pointToPointShortestTime(const std::string& start, const std::string& end,
                                                    const std::vector<RoadInfo>& roads, float& totaltime) {
    std::pair<StringList, float> result = dijkstraTime(start, end, roads);
    StringList path = result.first;
    totaltime = result.second;
    return path;
}

std::pair<StringList, float> Route_Strategy::dijkstraTime(const std::string& start, const std::string& end,const std::vector<RoadInfo>& roads) {
    //初始化时间和前一个点映射
    std::unordered_map<std::string, float> time;
    std::unordered_map<std::string, std::string> prev;
    std::priority_queue<std::pair<float, std::string>, std::vector<std::pair<float, std::string>>, std::greater<std::pair<float, std::string>>> pq;

    //为所有节点设置初始距离为正无穷大，除了起点
    for (const auto& road : roads) {
        time[road.start] = std::numeric_limits<float>::max();
        time[road.end] = std::numeric_limits<float>::max();
    }
    time[start] = 0;
    pq.emplace(0, start);


    while (!pq.empty()) {  //主循环
        float currDist = pq.top().first;
        std::string currNode = pq.top().second;
        pq.pop();

        if (currDist > time[currNode])  //如果当前距离大于已知最短时间，则跳过
            continue;

        for (const auto& road : roads) {
            if (road.start == currNode) {
                float alt = currDist + road.time;//
                if (alt < time[road.end]) {
                    time[road.end] = alt;
                    prev[road.end] = road.start;
                    pq.emplace(alt, road.end);
                }
            }
        }
    }
    float totaltime = time[end];  //获取最短时间

    // 重建最短路径
    StringList path;
    std::string curr = end;
    while (curr != start) {
        path.insert(path.begin(), curr);
        curr = prev[curr];
    }
    path.insert(path.begin(), start);
    return { path, totaltime };
}

std::vector<int> Route_Strategy::multiPointShortestDistance(const std::string& start, const StringList& mustVisit, int& totallength)
{
    // 必经城市标记
    bool mustVisitIndex[MAX_CITIES] = { false };

    //设置必经城市
    for (const std::string& city : mustVisit) {
        if (cityIndex.count(city)) {
            mustVisitIndex[cityIndex[city]] = true;
        }
    }

    //最短路径
    std::vector<int> minPath;

    // 使用优先队列进行状态的管理
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    // 初始化起始状态
    State startState;
    startState.path.push_back(cityIndex[start]); // 起点城市索引
    startState.cost = 0;
    startState.last = cityIndex[start];
    std::fill(std::begin(startState.visited), std::end(startState.visited), false);
    startState.visited[startState.last] = true;

    pq.push(startState);

    while (!pq.empty()) {
        State currentState = pq.top();
        pq.pop();

        // 如果当前路径已经访问过所有必经城市，则尝试回到起点并更新最小成本
        bool allVisited = true;
        for (const std::string& city : mustVisit) {
            if (!currentState.visited[cityIndex[city]]) {
                allVisited = false;
                break;
            }
        }

        if (allVisited && currentState.last == cityIndex[start]) { // 检查是否回到起点
            if (currentState.cost < totallength) {
                totallength = currentState.cost;
                minPath = currentState.path;
            }
            continue;
        }

        // 扩展当前状态
        for (int next = 0; next < (int)indexCity.size(); ++next) {
            if (!currentState.visited[next] || (allVisited && next == cityIndex[start])) { // 允许回到起点
                if (dist[currentState.last][next] != INF) { // 如果有路可走
                    State newState = currentState;
                    newState.path.push_back(next);
                    newState.cost += dist[currentState.last][next];
                    newState.last = next;
                    newState.visited[next] = true;
                    pq.push(newState);
                }
            }
        }
    }

    return minPath;
}

void Route_Strategy::initDistMatrix() {
    for (int i = 0; i < MAX_CITIES; i++) {
        std::fill(dist[i], dist[i] + MAX_CITIES, INF);
        dist[i][i] = 0;  //自身到自身距离为0
    }
}

void Route_Strategy::addRoad(const std::string& start, const std::string& end, int length) {  //添加道路信息到图中
    if (!cityIndex.count(start)) {
        int newIndex = cityIndex.size();
        cityIndex[start] = newIndex;
        indexCity.push_back(start);
    }
    if (!cityIndex.count(end)) {
        int newIndex = cityIndex.size();
        cityIndex[end] = newIndex;
        indexCity.push_back(end);
    }
    int u = cityIndex[start], v = cityIndex[end];
    dist[u][v] = std::min(dist[u][v], length);  //有向图
}

void Route_Strategy::paintEvent(QPaintEvent*) {
    if (mode == 0)  //初始化界面时不进行绘制
        return;

    QPen pen(Qt::black);  //画笔颜色为黑色
    pen.setWidth(2);  //画笔宽度
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setFont(QFont("黑体", 22));

    int totalLength = INF;  //最短距离
    float totalTime = 0;  //最短时间
    QSqlQuery query;
    std::vector<RoadInfo> roadsInfo;  //所有道路信息
    StringList path;  //用于存储最终的路径(点到点)
    std::vector<int> minPath;
    std::string initialLocation = boxInitialLocationRS->currentText().toStdString();  //起始位置
    std::string destination = boxDestinationRS->currentText().toStdString();  //目的地(1个)
    QStringList destinationsTmp = lineEditDestinationsRS->text().split(" ");
    StringList destinations;  //目的地(多个)

    for (const QString& str : destinationsTmp)
        destinations.push_back(str.toStdString());
    query.exec("select a1.name,a2.name,r.length,r.congestion,r.velocity "
               "from t_road r join t_architect a1 on r.start = a1.architect_id "
               "join t_architect a2 on r.end = a2.architect_id");
    while (query.next()) {  //从数据库中把对应信息存入所有道路信息
        RoadInfo roadInfo;
        roadInfo.start = query.value(0).toString().toStdString();
        roadInfo.end = query.value(1).toString().toStdString();
        roadInfo.length = query.value(2).toString().toInt();
        roadInfo.congestion = query.value(3).toString().toFloat();
        roadInfo.velocity = query.value(4).toString().toFloat();
        roadInfo.time = roadInfo.length / (roadInfo.congestion * roadInfo.velocity);
        roadsInfo.push_back(roadInfo);
    }
    query.clear();
    if (buttonShortestTimeRS->isChecked()) {  //如果选中了最短时间按钮
        if(mode == 1)  //如果查询一个目的地
            path = pointToPointShortestTime(initialLocation, destination,roadsInfo, totalTime);
        //else if(mode == 2){  //如果查询多个目的地

        //}
        painter.drawText(200,700,QString("最短时间为：%1s").arg(totalTime));
    }
    else if (buttonShortestDistanceRS->isChecked()) {  //如果选中了最短距离按钮
        if (mode == 1)  //如果查询一个目的地
            path = pointToPointShortestDistance(initialLocation, destination, roadsInfo, totalLength);
        else if(mode == 2){  //如果查询多个目的地
            initDistMatrix();
            for (RoadInfo road : roadsInfo)
                addRoad(road.start, road.end, road.length);
            minPath = multiPointShortestDistance(initialLocation, destinations, totalLength);
            for(int i : minPath)
                path.push_back(indexCity[i]);
        }
        painter.drawText(200, 700, QString("最短距离为：%1m").arg(totalLength));
    }

    //将结果打印在界面上
    painter.drawText(200, 350, "路线：");
    for (int i = 0, j = 0; i < (int)path.size(); i++) {
        if (i % 5 == 0 && i != 0)
            j++;
        if (i == (int)path.size() - 1)
            painter.drawText(100 + 200 * (i % 5 + 1), 350 + 100 * j, QString::fromStdString(path[i]));
        else
            painter.drawText(100 + 200 * (i % 5 + 1), 350 + 100 * j, QString::fromStdString(path[i]) + "  ->");
    }
}

