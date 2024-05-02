//游学路线规划界面
#include "route_strategy.h"
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
    float totalTime = 0;  //最短时间
    int totalLength = INF;  //最短距离
    vector<RoadInfo> roadsInfo;  //所有道路信息
    unordered_set<string> starts;  //储存所有道路的起点
    StringList path;  //用于存储最终的路径
    string initialLocation = boxInitialLocationRS->currentText().toStdString();  //起始位置
    string destination = boxDestinationRS->currentText().toStdString();  //目的地(1个)
    StringList destinations;  //目的地(多个)
    query.exec("select a1.name,a2.name,r.length,r.congestion,r.velocity "
               "from t_road r join t_architect a1 on r.start = a1.architect_id "
               "join t_architect a2 on r.end = a2.architect_id");
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
            return {(int)totalTime, path};
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
            //path = pointToPointShortestTime(initialLocation, destination,roadsInfo, totalTime);
            //return {(int)totalTime, path};
        }
        else if (buttonShortestDistanceRS->isChecked()) {  //如果选中了最短距离按钮
            path = multiPointShortestDistance(initialLocation, destinations, roadsInfo, totalLength);
            return {totalLength, path};
        }
    }
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
    string curr = end;
    while (curr != start) {
        path.insert(path.begin(), curr);
        curr = prev[curr];
    }
    path.insert(path.begin(), start);
    return { path, totaltime };
}

void Route_Strategy::floydWarshallWithPath(unordered_map<string, unordered_map<string, int>>& graph, const StringList& vertices,
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

pair<int, StringList> Route_Strategy::solveTSP(const unordered_map<string, unordered_map<string, int>>& graph, const StringList& reqVertices, const string& start) {
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

StringList Route_Strategy::multiPointShortestDistance(string start, StringList requiredVertices, vector<RoadInfo> roads, int& totalLength){
    StringList pathVertices, fullPath;
    unordered_map<string, unordered_map<string, int>> graph;
    StringList vertices = { start };
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
    floydWarshallWithPath(graph, vertices, next);

    totalLength = solveTSP(graph, requiredVertices, start).first;
    pathVertices = solveTSP(graph, requiredVertices, start).second;
    fullPath.push_back(start);
    for (size_t i = 0; i < pathVertices.size() - 1; ++i) {
        StringList segment = constructPath(pathVertices[i], pathVertices[i + 1], next);
        fullPath.insert(fullPath.end(), segment.begin() + 1, segment.end());
    }
    return fullPath;
}

void Route_Strategy::paintEvent(QPaintEvent*) {
    if (mode == 0)  //初始化界面&用户输入不符合规定时不进行绘制
        return;
    QPen pen(Qt::black);  //画笔颜色为黑色
    pen.setWidth(2);  //画笔宽度
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setFont(QFont("黑体", 22));
    if (buttonShortestTimeRS->isChecked())  //如果选中了最短时间按钮
        painter.drawText(200,800,QString("最短时间为：%1s").arg(totalTimeOrLenth));
    else if (buttonShortestDistanceRS->isChecked())  //如果选中了最短距离按钮
        painter.drawText(200, 800, QString("最短距离为：%1m").arg(totalTimeOrLenth));
    painter.drawText(200, 300, "路线：");
    for (int i = 0, j = 0; i < (int)minPath.size(); i++) {
        if (i % 5 == 0 && i != 0)
            j++;
        if (i == (int)minPath.size() - 1)
            painter.drawText(100 + 200 * (i % 5 + 1), 300 + 100 * j, QString::fromStdString(minPath[i]));
        else
            painter.drawText(100 + 200 * (i % 5 + 1), 300 + 100 * j, QString::fromStdString(minPath[i]) + "  ->");
    }
}

