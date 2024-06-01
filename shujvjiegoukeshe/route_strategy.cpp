//游学路线规划界面
#include "route_strategy.h"
#include <QDebug>

Route_Strategy::Route_Strategy(QString place)
{
    this->place = place;
    initWidget();
    connect(buttonChoosebackRS, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回目的地推荐界面的信号
    });
    connect(buttonOneQueryRS, &QPushButton::clicked, [=]() {  //若点击目的地(1个)查询按钮
        mode = 1;
        tie(totalTimeOrLenth, minPath) = readyToPaint();
        update();  //调用paintEvent函数
    });
    connect(buttonQueriesRS, &QPushButton::clicked, [=]() {  //若点击目的地(多个)查询按钮
        mode = 2;
        tie(totalTimeOrLenth, minPath) = readyToPaint();
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

pair<int, StringList> Route_Strategy::readyToPaint(){
    QSqlQuery query;
    float totalTime;  //最短时间
    int totalLength;  //最短距离
    vector<RoadInfo> roadsInfo;  //所有道路信息
    unordered_set<string> starts;  //储存所有道路的起点
    StringList path;  //用于存储最终的路径
    string initialLocation = boxInitialLocationRS->currentText().toStdString();  //起始位置
    string destination = boxDestinationRS->currentText().toStdString();  //目的地(1个)
    StringList destinations;  //目的地(多个)
    query.exec("select a1.name,a2.name,r.length,r.congestion,r.velocity "
               "from t_road r join t_roadnode a1 on r.start = a1.roadnode_id "
               "join t_roadnode a2 on r.end = a2.roadnode_id");
    while (query.next()) {  //从数据库中把对应信息存入所有道路信息
        RoadInfo roadInfo;
        roadInfo.start = query.value(0).toString().toStdString();
        roadInfo.end = query.value(1).toString().toStdString();
        roadInfo.length = query.value(2).toInt();
        roadInfo.congestion = query.value(3).toFloat();
        roadInfo.velocity = query.value(4).toFloat();
        roadInfo.time = roadInfo.length / (roadInfo.congestion * roadInfo.velocity);
        roadsInfo.push_back(roadInfo);
        starts.insert(roadInfo.start);
    }
    query.clear();
    if (mode == 1) {  //如果查询一个目的地
        if (buttonShortestTimeRS->isChecked()) {  //如果选中了最短时间按钮
            path = pointToPointShortestTime(initialLocation, destination,roadsInfo, totalTime);
            return {totalTime, path};
        }
        else if (buttonShortestDistanceRS->isChecked()) {  //如果选中了最短距离按钮
            path = pointToPointShortestDistance(initialLocation, destination, roadsInfo, totalLength);
            return {totalLength, path};
        }
    }
    else if (mode == 2) {  //如果查询多个目的地
        if (lineEditDestinationsRS->text() == "") {  //多个目的地为空时的判断
            QMessageBox::information(this, "查找失败", "查找失败，目的地不能为空");  //弹出对话框
            return {0, destinations};
        }
        QStringList destinationsTmp = lineEditDestinationsRS->text().split(" ");
        destinationsTmp.removeAll(QString::fromStdString(initialLocation));
        destinationsTmp.removeAll(QString());
        if (destinationsTmp.isEmpty()){
            destinations.push_back(initialLocation);
            return {0, destinations};
        }
        for (const QString& str : destinationsTmp){
            if (!(starts.find(str.toStdString())!= starts.end())){  //如果用户输入的建筑不存在
                QMessageBox::information(this, "查找失败", QString("查找失败，建筑：%1不存在").arg(str));  //弹出对话框
                destinations.clear();
                return {0, destinations};
            }
            destinations.push_back(str.toStdString());
        }
        if (buttonShortestTimeRS->isChecked()) {  //如果选中了最短时间按钮
            path = multiPointShortestTime(initialLocation, destinations, roadsInfo, totalTime);
            return {totalTime, path};
        }
        else if (buttonShortestDistanceRS->isChecked()) {  //如果选中了最短距离按钮
            path = multiPointShortestDistance(initialLocation, destinations, roadsInfo, totalLength);
            return {totalLength, path};
        }
    }
    //return {0, destinations};
}

void Route_Strategy::initWidget() {
    QSqlQuery query;
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);
    labelInitialRS = new QLabel("起始位置：", this);
    labelInitialRS->setGeometry(375, 190, 200, 45);
    labelInitialRS->setAlignment(Qt::AlignCenter);
    labelInitialRS->setStyleSheet("QLabel{font:20px;}");
    labelDestinationRS = new QLabel("目的地(1个)：", this);
    labelDestinationRS->setGeometry(658, 190, 200, 45);
    labelDestinationRS->setAlignment(Qt::AlignCenter);
    labelDestinationRS->setStyleSheet("QLabel{font:20px;}");
    labelDestinationsRS = new QLabel("目的地(多个)：", this);
    labelDestinationsRS->setGeometry(389, 270, 200, 45);
    labelDestinationsRS->setAlignment(Qt::AlignCenter);  //文本框内容居中显示
    labelDestinationsRS->setStyleSheet("QLabel{font:20px;}");  //设置文本框字体
    buttonChoosebackRS = new QPushButton("返回", this);
    buttonChoosebackRS->move(0, WIDTH * 17 / 18);
    buttonChoosebackRS->resize(50, WIDTH / 18);
    buttonChoosebackRS->setStyleSheet(
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
    buttonOneQueryRS = new QPushButton("查询", this);
    buttonOneQueryRS->move(1004, 185);
    buttonOneQueryRS->resize(70, WIDTH / 17);
    buttonOneQueryRS->setStyleSheet("QPushButton {"
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
    buttonQueriesRS = new QPushButton("查询", this);
    buttonQueriesRS->move(1004, 265);
    buttonQueriesRS->resize(70, WIDTH / 17);
    buttonQueriesRS->setStyleSheet("QPushButton {"
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

    boxInitialLocationRS = new QComboBox(this);
    boxInitialLocationRS->move(519, 190);
    boxInitialLocationRS->resize(130, 45);
    boxInitialLocationRS->setFont(QFont("黑体", 20));
    boxInitialLocationRS->setStyleSheet(""
                                    "QComboBox {"
                                    "    border: 2px solid #A8A8A8;"
                                    "    border-radius: 5px;"
                                    "    padding: 1px 18px 1px 3px;"
                                    "    min-width: 4em;"
                                    "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                    "                                stop: 0 #E1E1E1, stop: 1.0 #D3D3D3);"
                                    "    font: bold 18px;"
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
    boxDestinationRS = new QComboBox(this);
    boxDestinationRS->move(818, 190);
    boxDestinationRS->resize(130, 45);
    boxDestinationRS->setFont(QFont("黑体", 20));
    query.exec("select name from t_roadnode order by roadnode_id limit 70");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxInitialLocationRS->addItem(name);
        boxDestinationRS->addItem(name);
    }
    boxDestinationRS->setStyleSheet(""
                        "QComboBox {"
                        "    border: 2px solid #A8A8A8;"
                        "    border-radius: 5px;"
                        "    padding: 1px 18px 1px 3px;"
                        "    min-width: 4em;"
                        "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "                                stop: 0 #E1E1E1, stop: 1.0 #D3D3D3);"
                        "    font: bold 18px;"
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
    buttonShortestTimeRS = new QRadioButton("最短时间", this);
    buttonShortestTimeRS->setChecked(true);
    buttonShortestTimeRS->move(600, 140);
    buttonShortestTimeRS->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");  //设置按钮大小及字体大小
    buttonShortestDistanceRS = new QRadioButton("最短距离", this);
    buttonShortestDistanceRS->move(800, 140);
    buttonShortestDistanceRS->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");
    lineEditDestinationsRS = new QLineEdit(this);
    lineEditDestinationsRS->setGeometry(LENGTH * 3.5 / 14 + 180, WIDTH * 3 / 19 + 115, 450, WIDTH / 17);
    lineEditDestinationsRS->setFont(QFont("黑体", 15));
    lineEditDestinationsRS->setPlaceholderText("请输入多个目的地（用空格分隔）");
    lineEditDestinationsRS->setClearButtonEnabled(true);
    lineEditDestinationsRS->setValidator(new QRegularExpressionValidator(QRegularExpression("^([\u4e00-\u9fa5\\d]+(\\s[\u4e00-\u9fa5\\d]+)*)$"), this));  //只允许输入中文、数字及空格
    lineEditDestinationsRS->setStyleSheet("QLineEdit {"
                    "border: 2px solid #A6C1FF;" // 浅蓝色边框
                    "border-radius: 8px;"         // 圆角
                    "padding: 5px 10px;"          // 内边距
                    "font-size: 15px;"            // 字体大小
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
}

StringList Route_Strategy::pointToPointShortestDistance(const string& start, const string& end, const vector<RoadInfo>& roads, int& totalLength) {
    pair<StringList, int> result = dijkstraLength(start, end, roads);
    StringList path = result.first;
    totalLength = result.second;
    return path;
}

pair<StringList, int> Route_Strategy::dijkstraLength(const string& start, const string& end,const vector<RoadInfo>& roads) {
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
    string curr = end;
    while (curr != start) {
        path.insert(path.begin(), curr);
        curr = prev[curr];
    }
    path.insert(path.begin(), start);
    path.erase(::remove_if(path.begin(), path.end(), [](const string& str) {return str.find("道路节点") != string::npos;}), path.end());
    return { path, totallength };
}

StringList Route_Strategy::pointToPointShortestTime(const string& start, const string& end, const vector<RoadInfo>& roads, float& totalTime) {
    pair<StringList, float> result = dijkstraTime(start, end, roads);
    StringList path = result.first;
    totalTime = result.second;
    return path;
}

pair<StringList, float> Route_Strategy::dijkstraTime(const string& start, const string& end,const vector<RoadInfo>& roads) {
    //初始化时间和前一个点映射
    unordered_map<string, float> time;
    unordered_map<string, string> prev;
    priority_queue<pair<float, string>, vector<pair<float, string>>, greater<pair<float, string>>> pq;

    //为所有节点设置初始距离为正无穷大，除了起点
    for (const auto& road : roads) {
        time[road.start] = numeric_limits<float>::max();
        time[road.end] = numeric_limits<float>::max();
    }
    time[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {  //主循环
        float currDist = pq.top().first;
        string currNode = pq.top().second;
        pq.pop();

        if (currDist > time[currNode])  //如果当前距离大于已知最短时间，则跳过
            continue;

        for (const auto& road : roads) {
            if (road.start == currNode) {
                float alt = currDist + road.time;
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
    string curr = end;
    while (curr != start) {
        path.insert(path.begin(), curr);
        curr = prev[curr];
    }
    path.insert(path.begin(), start);
    path.erase(::remove_if(path.begin(), path.end(), [](const string& str) {return str.find("道路节点") != string::npos;}), path.end());
    return { path, totaltime };
}

void Route_Strategy::floydWarshallWithPathLength(unordered_map<string, unordered_map<string, int>>& graph, const StringList& vertices,
                                           unordered_map<string, unordered_map<string, string>>& next) {
    for (const string& k : vertices) {
        for (const string& i : vertices) {
            for (const string& j : vertices) {
                if (graph[i][k] < INF && graph[k][j] < INF && graph[i][j] > graph[i][k] + graph[k][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    next[i][j] = k;
                }
            }
        }
    }
}

void Route_Strategy::floydWarshallWithPathTime(unordered_map<string, unordered_map<string, float>>& graph, const StringList& vertices,
                                                unordered_map<string, unordered_map<string, string>>& next) {
    for (const string& k : vertices) {
        for (const string& i : vertices) {
            for (const string& j : vertices) {
                if (graph[i][k] < numeric_limits<float>::max() && graph[k][j] < numeric_limits<float>::max() && graph[i][j] > graph[i][k] + graph[k][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    next[i][j] = k;
                }
            }
        }
    }
}

StringList Route_Strategy::constructPath(const string& u, const string& v, unordered_map<string, unordered_map<string, string>>& next) {
    if (next.at(u).at(v) == "")
        return { u, v };
    else {
        StringList path = constructPath(u, next[u][v], next);
        path.pop_back();
        StringList rightPart = constructPath(next[u][v], v, next);
        path.insert(path.end(), rightPart.begin(), rightPart.end());
        return path;
    }
}

pair<int, StringList> Route_Strategy::solveTSPLength(const unordered_map<string, unordered_map<string, int>>& graph, const StringList& reqVertices, const string& start) {
    int n = reqVertices.size();
    vector<vector<int>> dp(1 << n, vector<int>(n, INF));
    vector<vector<int>> parent(1 << n, vector<int>(n, -1));
    for (int i = 0; i < n; ++i)  //初始化DP
        dp[1 << i][i] = graph.at(start).at(reqVertices[i]);
    for (int mask = 1; mask < (1 << n); mask++) {  //填充DP表
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                for (int j = 0; j < n; j++) {
                    if (!(mask & (1 << j)) && graph.at(reqVertices[i]).at(reqVertices[j]) < INF) {
                        int next_mask = mask | (1 << j);
                        int new_cost = dp[mask][i] + graph.at(reqVertices[i]).at(reqVertices[j]);
                        if (new_cost < dp[next_mask][j]) {
                            dp[next_mask][j] = new_cost;
                            parent[next_mask][j] = i;
                        }
                    }
                }
            }
        }
    }
    //寻找返回起点的最小成本
    int min_cost = INF, last_index = -1;
    int final_mask = (1 << n) - 1;
    for (int i = 0; i < n; ++i) {
        if (dp[final_mask][i] + graph.at(reqVertices[i]).at(start) < min_cost) {
            min_cost = dp[final_mask][i] + graph.at(reqVertices[i]).at(start);
            last_index = i;
        }
    }
    //重建路径
    StringList path;
    int current_mask = final_mask;
    while (last_index != -1) {
        path.push_back(reqVertices[last_index]);
        int prev = last_index;
        last_index = parent[current_mask][last_index];
        current_mask -= (1 << prev);
    }

    reverse(path.begin(), path.end());
    path.insert(path.begin(), start);  //开始
    path.push_back(start);  //返回起点
    return { min_cost, path };
}

pair<float, StringList> Route_Strategy::solveTSPTime(const unordered_map<string, unordered_map<string, float>>& graph, const StringList& reqVertices, const string& start) {
    int n = reqVertices.size();
    vector<vector<float>> dp(1 << n, vector<float>(n, numeric_limits<float>::max()));
    vector<vector<float>> parent(1 << n, vector<float>(n, -1));
    for (int i = 0; i < n; ++i)  //初始化DP
        dp[1 << i][i] = graph.at(start).at(reqVertices[i]);
    for (int mask = 1; mask < (1 << n); mask++) {  //填充DP表
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                for (int j = 0; j < n; j++) {
                    if (!(mask & (1 << j)) && graph.at(reqVertices[i]).at(reqVertices[j]) < numeric_limits<float>::max()) {
                        float next_mask = mask | (1 << j);
                        float new_cost = dp[mask][i] + graph.at(reqVertices[i]).at(reqVertices[j]);
                        if (new_cost < dp[next_mask][j]) {
                            dp[next_mask][j] = new_cost;
                            parent[next_mask][j] = i;
                        }
                    }
                }
            }
        }
    }
    //寻找返回起点的最小成本
    float min_cost = numeric_limits<float>::max();
    int last_index = -1;
    int final_mask = (1 << n) - 1;
    for (int i = 0; i < n; ++i) {
        if (dp[final_mask][i] + graph.at(reqVertices[i]).at(start) < min_cost) {
            min_cost = dp[final_mask][i] + graph.at(reqVertices[i]).at(start);
            last_index = i;
        }
    }
    //重建路径
    StringList path;
    int current_mask = final_mask;
    while (last_index != -1) {
        path.push_back(reqVertices[last_index]);
        int prev = last_index;
        last_index = parent[current_mask][last_index];
        current_mask -= (1 << prev);
    }

    reverse(path.begin(), path.end());
    path.insert(path.begin(), start);  //开始
    path.push_back(start);  //返回起点
    return { min_cost, path };
}

StringList Route_Strategy::multiPointShortestDistance(string start, StringList requiredVertices, vector<RoadInfo> roads, int& totalLength){
    StringList pathVertices, fullPath, vertices = { start };
    unordered_map<string, unordered_map<string, int>> graph;
    for (const auto& edge : roads) {  //添加所有顶点到图中
        graph[edge.start][edge.end] = edge.length;
        if (::find(vertices.begin(), vertices.end(), edge.start) == vertices.end()) {
            vertices.push_back(edge.start);
        }
        if (::find(vertices.begin(), vertices.end(), edge.end) == vertices.end()) {
            vertices.push_back(edge.end);
        }
    }
    for (const string& v1 : vertices) {  //初始化图中每个顶点对的距离为无穷大
        for (const string& v2 : vertices) {
            if (graph[v1].find(v2) == graph[v1].end()) {
                graph[v1][v2] = INF;  //没有直接连接的顶点之间的距离设置为无穷大
            }
        }
    }
    //重建路径
    unordered_map<string, unordered_map<string, string>> next;
    for (const auto& v : vertices) {
        for (const auto& w : vertices) {
            next[v][w] = "";
        }
    }
    floydWarshallWithPathLength(graph, vertices, next);

    totalLength = solveTSPLength(graph, requiredVertices, start).first;
    pathVertices = solveTSPLength(graph, requiredVertices, start).second;
    fullPath.push_back(start);
    for (size_t i = 0; i < pathVertices.size() - 1; ++i) {
        StringList segment = constructPath(pathVertices[i], pathVertices[i + 1], next);
        fullPath.insert(fullPath.end(), segment.begin() + 1, segment.end());
    }
    fullPath.erase(::remove_if(fullPath.begin(), fullPath.end(), [](const string& str) {return str.find("道路节点") != string::npos;}), fullPath.end());
    return fullPath;
}

StringList Route_Strategy::multiPointShortestTime(string start, StringList requiredVertices, vector<RoadInfo> roads, float& totalTime){
    StringList pathVertices, fullPath, vertices = { start };
    unordered_map<string, unordered_map<string, float>> graph;
    for (const auto& edge : roads) {  //添加所有顶点到图中
        graph[edge.start][edge.end] = edge.time;
        if (::find(vertices.begin(), vertices.end(), edge.start) == vertices.end()) {
            vertices.push_back(edge.start);
        }
        if (::find(vertices.begin(), vertices.end(), edge.end) == vertices.end()) {
            vertices.push_back(edge.end);
        }
    }
    for (const string& v1 : vertices) {  //初始化图中每个顶点对的距离为无穷大
        for (const string& v2 : vertices) {
            if (graph[v1].find(v2) == graph[v1].end()) {
                graph[v1][v2] = numeric_limits<float>::max();  //没有直接连接的顶点之间的距离设置为无穷大
            }
        }
    }
    //重建路径
    unordered_map<string, unordered_map<string, string>> next;
    for (const auto& v : vertices) {
        for (const auto& w : vertices) {
            next[v][w] = "";
        }
    }
    floydWarshallWithPathTime(graph, vertices, next);

    totalTime = solveTSPTime(graph, requiredVertices, start).first;
    pathVertices = solveTSPTime(graph, requiredVertices, start).second;
    fullPath.push_back(start);
    for (size_t i = 0; i < pathVertices.size() - 1; ++i) {
        StringList segment = constructPath(pathVertices[i], pathVertices[i + 1], next);
        fullPath.insert(fullPath.end(), segment.begin() + 1, segment.end());
    }
    fullPath.erase(::remove_if(fullPath.begin(), fullPath.end(), [](const string& str) {return str.find("道路节点") != string::npos;}), fullPath.end());
    return fullPath;
}

void Route_Strategy::paintEvent(QPaintEvent*) {
    QPen pen(Qt::white);  //画笔颜色为白色
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    QPixmap pix;
    pix.load(":/resource/3.jpg");
    int windowWidth = this->width();
    int windowHeight = this->height();
    QPixmap scaledPix = pix.scaled(windowWidth, windowHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, scaledPix);
    pen.setWidth(0);
    painter.setPen(pen);
    painter.setBrush(Qt::white);
    painter.setOpacity(0.5);
    painter.drawRect(300, 0, 900, 950);
    painter.setOpacity(1);
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setFont(QFont("黑体", 25));
    QRect textRect = painter.boundingRect(QRect(), Qt::TextSingleLine, "当前所在景区/学校:" + place);
    painter.drawText((LENGTH - textRect.width())/2, 100, "当前所在景区/学校:" + place);
    if(mode != 0){
        painter.setFont(QFont("黑体", 18));
        if (buttonShortestTimeRS->isChecked())  //如果选中了最短时间按钮
            painter.drawText(330,850,QString("最短时间为：%1s").arg(totalTimeOrLenth));
        else if (buttonShortestDistanceRS->isChecked())  //如果选中了最短距离按钮
            painter.drawText(330, 850, QString("最短距离为：%1m").arg(totalTimeOrLenth));
        painter.drawText(330, 400, "路线：");
        for (int i = 0, j = 0; i < (int)minPath.size(); i++) {
            if (i % 6 == 0 && i != 0)
                j++;
            if (i == (int)minPath.size() - 1)
                painter.drawText(270 + 130 * (i % 6 + 1), 400 + 100 * j, QString::fromStdString(minPath[i]));
            else
                painter.drawText(270 + 130 * (i % 6 + 1), 400 + 100 * j, QString::fromStdString(minPath[i]) + " ->");
        }
    }
    return;
}

