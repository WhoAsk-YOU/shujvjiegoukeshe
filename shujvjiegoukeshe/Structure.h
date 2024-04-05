#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <limits>
#include <string>
#include <vector>
#include <queue>

using StringList = std::vector<std::string>;
const int INF = std::numeric_limits<int>::max();
const int MAX_CITIES = 70;
const int LENGTH = 1400;
const int WIDTH = 950;
struct Place {  //景区/学校信息
    std::string name;  //景区或学校名
    int value;  //景区或学校对应的热度值/好评数
};
struct DiaryInfo{  //日记信息
    std::string diaryName;  //日记名
    std::string authorAccount;  //作者账户名
    std::string destination;  //目的地
    int goodComments;  //好评数
    int heatValue;  //热度
};
struct RoadInfo {  //道路信息
    std::string start;  //起点
    std::string end;  //终点
    int length;  //长度(m)
    float congestion;  //拥挤度
    float velocity;  //理想速度
    float time;  //通行时间
};
struct RoadLengthInfo {  //道路长度信息
    std::string start;  //起点
    std::string end;  //终点
    int length;  //距离
};
struct State { //定义表示状态的结构体，用于分支限界法中
    std::vector<int> path;  //当前路径，存储建筑索引
    int cost;  //当前路径的总成本
    bool visited[MAX_CITIES];  //访问标记数组
    int last;  //最后访问建筑的索引
    bool operator>(const State& other) const {  //优先队列中使用的比较函数，总成本小的优先
        return cost > other.cost;
    }
};

#endif // STRUCTURE_H
