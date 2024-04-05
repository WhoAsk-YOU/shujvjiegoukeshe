//游学路线规划界面
#ifndef ROUTE_STRATEGY_H
#define ROUTE_STRATEGY_H

#include <QWidget>
#include <QPushButton>
#include <QSqlQuery>
#include <QLabel>
#include <QRadioButton>
#include <QComboBox>  //下拉框
#include <QPainter>  //画图
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include "Structure.h"

class Route_Strategy : public QWidget
{
    Q_OBJECT
public:
    explicit Route_Strategy(QWidget* parent = nullptr);
    ~Route_Strategy();

signals:
    void chooseback();  //返回信号

private:
    int mode = 0;  //若未点击查询按钮,mode=0;若点击了目的地(1个)查询按钮,mode=1;若点击了目的地(多个)查询按钮,mode=2

    std::unordered_map<std::string, int> cityIndex;  //建筑名称到索引的映射
    StringList indexCity;  //索引到建筑名称的映射，用于最终输出
    int dist[MAX_CITIES][MAX_CITIES];  //建筑间距离矩阵

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

    StringList pointToPointShortestDistance(const std::string& start, const std::string& end,
                                            const std::vector<RoadInfo>& roads, int& totallength);  //点到点的最短距离
    std::pair<StringList, int> dijkstraLength(const std::string& start, const std::string& end, const std::vector<RoadInfo>& roads);

    StringList pointToPointShortestTime(const std::string& start, const std::string& end,
                                        const std::vector<RoadInfo>& roads, float& totaltime);  //点到点的最短时间
    std::pair<StringList, float> dijkstraTime(const std::string& start, const std::string& end, const std::vector<RoadInfo>& roads);

    std::vector<int> multiPointShortestDistance(const std::string& start, const StringList& mustVisit, int& totallength);  //途径多点的最短距离
    void initDistMatrix();
    void addRoad(const std::string& start, const std::string& end, int length);

    void paintEvent(QPaintEvent*);  //重写paintEvent函数，用于在屏幕上打印出路线

};

#endif // ROUTE_STRATEGY_H
