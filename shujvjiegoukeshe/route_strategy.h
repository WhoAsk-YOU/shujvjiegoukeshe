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

class Route_Strategy : public QWidget
{
    Q_OBJECT
public:
    explicit Route_Strategy(QWidget* parent = nullptr);
    ~Route_Strategy();
    struct RoadInfo {  //道路信息
        std::string start;  //起点
        std::string end;  //终点
        int length;  //长度(m)
        float congestion;  //拥挤度
        float velocity;  //理想速度
        float time;  //通行时间
    };
    using StringList = std::vector<std::string>;

signals:
    void chooseback();  //返回信号

private:
    int length;  //窗口的长
    int width;  //窗口的宽
    int mode = 0;  //若未点击查询按钮,mode=0;若点击了目的地(1个)查询按钮,mode=1;若点击了目的地(多个)查询按钮,mode=2
    QLabel* labelInitial = NULL;  //起始位置
    QLabel* labelDestination = NULL;  //目的地（1个）
    QLabel* labelDestinations = NULL;  //目的地（多个）
    QPushButton* buttonChooseback = NULL;  //返回按钮
    QPushButton* buttonOneQuery = NULL;  //查询按钮（1个目的地）
    QPushButton* buttonQueries = NULL;  //查询按钮（多个目的地）
    QRadioButton* buttonShortestTime = NULL;  //最短时间
    QRadioButton* buttonShortestDistance = NULL;  //最短距离
    QComboBox* boxInitialLocation = NULL;  //起始位置
    QComboBox* boxDestination = NULL;  //目的地
    QLineEdit* lineEditDestinations = NULL;  //目的地输入框

    void initWidget();  //界面初始化函数

    StringList pointToPointShortestDistance(const std::string& start, const std::string& end,
                                            const std::vector<RoadInfo>& roads, int& totallength);  //点到点的最短距离
    std::pair<StringList, int> dijkstraLength(const std::string& start, const std::string& end, const std::vector<RoadInfo>& roads);

    StringList pointToPointShortestTime(const std::string& start, const std::string& end,
                                        const std::vector<RoadInfo>& roads, float& totaltime);  //点到点的最短时间
    std::pair<StringList, float> dijkstraTime(const std::string& start, const std::string& end, const std::vector<RoadInfo>& roads);
    /*
    StringList multiPointShortestDistance(const std::string& initialLocation, const StringList& destinations,
                                            const std::vector<RoadInfo>& roads, int& totallength);  //途径多点的最短距离
    StringList multiPointShortestTime(const std::string& initialLocation, const StringList& destinations,
                                      const std::vector<RoadInfo>& roads, float& totaltime)  //途径多点的最短时间
    */
    void paintEvent(QPaintEvent*);  //重写paintEvent函数，用于在屏幕上打印出路线

};

#endif // ROUTE_STRATEGY_H
