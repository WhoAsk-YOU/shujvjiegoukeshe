//游学路线规划界面
#ifndef ROUTE_STRATEGY_H
#define ROUTE_STRATEGY_H

#include "Structure.h"

class Route_Strategy : public QWidget
{
    Q_OBJECT
public:
    Route_Strategy(QString place);
    ~Route_Strategy();

signals:
    void chooseback();  //返回信号

private slots:
    pair<int, StringList> readyToPaint();  //绘图前的准备工作--计算出最短时间/距离 & 路径

private:
    int mode = 0;  //若未点击查询按钮,mode=0;若点击了目的地(1个)查询按钮,mode=1;若点击了目的地(多个)查询按钮,mode=2
    int totalTimeOrLenth = 0;  //最短距离/时间
    QString place;
    StringList minPath;  //结果路径
    QLabel* labelInitialRS = NULL;  //起始位置
    QLabel* labelDestinationRS = NULL;  //目的地（1个）
    QLabel* labelDestinationsRS = NULL;  //目的地（多个）
    QPushButton* buttonChoosebackRS = NULL;  //返回按钮
    QPushButton* buttonOneQueryRS = NULL;  //查询按钮（1个目的地）
    QPushButton* buttonQueriesRS = NULL;  //查询按钮（多个目的地）
    QRadioButton* buttonShortestTimeRS = NULL;  //最短时间
    QRadioButton* buttonShortestDistanceRS = NULL;  //最短距离
    QComboBox* boxInitialLocationRS = NULL;  //起始位置
    QComboBox* boxDestinationRS = NULL;  //目的地
    QLineEdit* lineEditDestinationsRS = NULL;  //目的地输入框

    void initWidget();  //界面初始化函数

    StringList pointToPointShortestDistance(const string& start, const string& end, const vector<RoadInfo>& roads, int& totalLength);  //点到点的最短距离
    pair<StringList, int> dijkstraLength(const string& start, const string& end, const vector<RoadInfo>& roads);

    StringList pointToPointShortestTime(const string& start, const string& end, const vector<RoadInfo>& roads, float& totalTime);  //点到点的最短时间
    pair<StringList, float> dijkstraTime(const string& start, const string& end, const vector<RoadInfo>& roads);

    StringList multiPointShortestDistance(string start, StringList requiredVertices, vector<RoadInfo> roads, int &totalLength);  //途径多点的最短距离
    void floydWarshallWithPath(unordered_map<string, unordered_map<string, int>>& graph, const StringList& vertices,
                               unordered_map<string, unordered_map<string, string>>& next);  //创建图并使用Floyd - Warshall算法计算所有节点之间的最短路径
    StringList constructPath(const string& u, const string& v, unordered_map<string, unordered_map<string, string>>& next);  //构建完整路径
    pair<int, StringList> solveTSP(const unordered_map<string, unordered_map<string, int>>& graph, const StringList& reqVertices, const string& start);  //使用动态规划方法来解决TSP问题

    void paintEvent(QPaintEvent*);  //重写paintEvent函数，用于在屏幕上打印出路线
};

#endif // ROUTE_STRATEGY_H
