#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <limits>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <sstream>
#include <QTextEdit>  //文本编辑框
#include <QRadioButton>  //选择框
#include <QTableWidget>  //表格
#include <QTableWidgetItem>  //表格内的单元格操作
#include <QHeaderView>  //表头
#include <QCheckBox>  //勾选框
#include <QComboBox>  //下拉框
#include <QPainter>  //画图
#include <QWidget>  //窗口
#include <QInputDialog>
#include <QPushButton>  //按钮
#include <QLabel>  //文字标签
#include <QLineEdit>  //输入框
#include <QSqlQuery>  //数据库查询
#include <QRegularExpressionValidator>  //正则表达式匹配
#include <QMessageBox>  //对话框

using namespace std;
using StringList = vector<string>;
const int INF = numeric_limits<int>::max();  //最大的整数
const int LENGTH = 1500;  //窗口长度
const int WIDTH = 950;  //窗口宽度
struct Place {  //景区/学校信息
    string name;  //景区或学校名
    int value;  //景区或学校对应的热度值/好评数
};
struct DiaryInfo {  //日记信息
    string diaryName;  //日记名
    string authorAccount;  //作者账户名
    string destination;  //目的地
    int id;  //日记编号
    int avgScore;  //评分
    int heatValue;  //热度
};
enum MatchType { CONTINUOUS, PARTIAL, NONE };  //匹配类型
struct MatchedDiary {  //匹配结构体
    DiaryInfo diary;
    MatchType type = NONE;
};
struct RoadInfo {  //道路信息
    string start;  //起点
    string end;  //终点
    int length;  //长度(m)
    float congestion;  //拥挤度
    float velocity;  //理想速度
    float time;  //通行时间
};
struct RoadLengthInfo {  //道路长度信息
    string start;  //起点
    string end;  //终点
    int length;  //距离
};
struct Node {
    QChar ch;
    int freq;
    Node* left;
    Node* right;
    Node(QChar c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};
struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

#endif // STRUCTURE_H
