//游学路线规划界面
#include "route_strategy.h"
#include <queue>

Route_Strategy::Route_Strategy(QWidget* parent)
    : QWidget{ parent }
{
    initWidget();
    connect(buttonChooseback, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回目的地推荐界面的信号
    });
    connect(buttonOneQuery, &QPushButton::clicked, [=]() {  //若点击目的地(1个)查询按钮
        mode = 1;
        update();  //调用paintEvent函数
    });
    connect(buttonQueries, &QPushButton::clicked, [=]() {  //若点击目的地(多个)查询按钮
        mode = 2;
        update();  //调用paintEvent函数
    });
}

Route_Strategy::~Route_Strategy() {
    delete labelInitial;
    labelInitial = NULL;
    delete labelDestination;
    labelDestination = NULL;
    delete labelDestinations;
    labelDestinations = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
    delete buttonOneQuery;
    buttonOneQuery = NULL;
    delete buttonQueries;
    buttonQueries = NULL;
    delete boxInitialLocation;
    boxInitialLocation = NULL;
    delete boxDestination;
    boxDestination = NULL;
    delete buttonShortestDistance;
    buttonShortestDistance = NULL;
    delete buttonShortestTime;
    buttonShortestTime = NULL;
    delete lineEditDestinations;
    lineEditDestinations = NULL;
}

void Route_Strategy::initWidget() {
    QSqlQuery query;
    length = 1400;
    width = 950;
    setWindowTitle("学生游学系统");
    setFixedSize(length, width);

    labelInitial = new QLabel("起始位置：", this);
    labelInitial->setGeometry(150, 80, 200, 45);
    labelInitial->setAlignment(Qt::AlignCenter);
    labelInitial->setStyleSheet("QLabel{font:20px;}");
    labelDestination = new QLabel("目的地(1个)：", this);
    labelDestination->setGeometry(540, 80, 200, 45);
    labelDestination->setAlignment(Qt::AlignCenter);
    labelDestination->setStyleSheet("QLabel{font:20px;}");
    labelDestinations = new QLabel("目的地(多个)：", this);
    labelDestinations->setGeometry(165, 160, 200, 45);
    labelDestinations->setAlignment(Qt::AlignCenter);  //文本框内容居中显示
    labelDestinations->setStyleSheet("QLabel{font:20px;}");  //设置文本框字体

    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, width * 8 / 9);
    buttonChooseback->resize(length / 9, width / 9);
    buttonChooseback->setFont(QFont("黑体", 25));
    buttonOneQuery = new QPushButton("查询", this);
    buttonOneQuery->move(1000, 80);
    buttonOneQuery->resize(80, 45);
    buttonOneQuery->setFont(QFont("黑体", 20));
    buttonQueries = new QPushButton("查询", this);
    buttonQueries->move(1000, 160);
    buttonQueries->resize(80, 45);
    buttonQueries->setFont(QFont("黑体", 20));

    boxInitialLocation = new QComboBox(this);
    boxInitialLocation->move(300, 80);
    boxInitialLocation->resize(200, 45);
    boxInitialLocation->setFont(QFont("黑体", 20));
    boxDestination = new QComboBox(this);
    boxDestination->move(700, 80);
    boxDestination->resize(200, 45);
    boxDestination->setFont(QFont("黑体", 20));
    query.exec("select name from t_architect");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxInitialLocation->addItem(name);
        boxDestination->addItem(name);
    }

    buttonShortestTime = new QRadioButton("最短时间", this);
    buttonShortestTime->setChecked(true);
    buttonShortestTime->move(1120, 120);
    buttonShortestTime->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");  //设置按钮大小及字体大小
    buttonShortestDistance = new QRadioButton("最短距离", this);
    buttonShortestDistance->move(1120, 150);
    buttonShortestDistance->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");

    lineEditDestinations = new QLineEdit(this);
    lineEditDestinations->setGeometry(length * 3.5 / 14 - 8, width * 3 / 19 + 5, length / 2.5, width / 17);
    lineEditDestinations->setFont(QFont("黑体", 15));
    lineEditDestinations->setPlaceholderText("请输入多个目的地（用空格分隔）");
    lineEditDestinations->setClearButtonEnabled(true);
    lineEditDestinations->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff\x20]+"), this));  //只允许输入中文及空格
}
/*
QStringList Route_Strategy::multiPointShortestTime(QString initialLocation, QStringList destinations){

}

QStringList Route_Strategy::multiPointShortestDistance(QString initialLocation, QStringList destinations){

}
*/
Route_Strategy::StringList Route_Strategy::pointToPointShortestDistance(const std::string& start, const std::string& end,
                                                                        const std::vector<RoadInfo>& roads, int& totallength) {
    std::pair<StringList, int> result = dijkstraLength(start, end, roads);
    StringList path = result.first;
    totallength = result.second;
    return path;
}

std::pair<Route_Strategy::StringList, int> Route_Strategy::dijkstraLength(const std::string& start, const std::string& end,
                                                                    const std::vector<RoadInfo>& roads) {
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

Route_Strategy::StringList Route_Strategy::pointToPointShortestTime(const std::string& start, const std::string& end,
                                                                    const std::vector<RoadInfo>& roads, float& totaltime) {
    std::pair<StringList, float> result = dijkstraTime(start, end, roads);
    StringList path = result.first;
    totaltime = result.second;
    return path;
}

std::pair<Route_Strategy::StringList, float> Route_Strategy::dijkstraTime(const std::string& start, const std::string& end,
                                                                          const std::vector<RoadInfo>& roads) {
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


void Route_Strategy::paintEvent(QPaintEvent*) {
    if (mode == 0)  //初始化界面时不进行绘制
        return;

    QPen pen(Qt::black);  //画笔颜色为黑色
    pen.setWidth(2);  //画笔宽度
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setFont(QFont("黑体", 22));

    int totalLength = 0;  //最短距离
    float totalTime = 0;  //最短时间
    QSqlQuery query;
    std::vector<RoadInfo> roadsInfo;  //所有道路信息
    StringList path;  //用于存储最终的路径
    std::string initialLocation = boxInitialLocation->currentText().toStdString();  //起始位置
    std::string destination = boxDestination->currentText().toStdString();  //目的地(1个)
    QStringList destinationsTmp = lineEditDestinations->text().split(" ");
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

    if (buttonShortestTime->isChecked()) {  //如果选中了最短时间按钮
        if(mode == 1)  //如果查询一个目的地
            path = pointToPointShortestTime(initialLocation, destination,roadsInfo, totalTime);
        //else if(mode == 2)  //如果查询多个目的地
        //answer = multiPointShortestTime(initialLocation, destinations);
        painter.drawText(200,700,QString("最短时间为：%1s").arg(totalTime));
    }
    else if (buttonShortestDistance->isChecked()) {  //如果选中了最短距离按钮
        if (mode == 1)  //如果查询一个目的地
            path = pointToPointShortestDistance(initialLocation, destination, roadsInfo, totalLength);
        //else if(mode == 2)  //如果查询多个目的地
        //answer = multiPointShortestDistance(initialLocation, destinations);
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

