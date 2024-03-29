//场所查询界面
#ifndef SEARCH_ARCHITECT_H
#define SEARCH_ARCHITECT_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSqlQuery>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

class Search_Architect : public QWidget
{
    Q_OBJECT
public:
    explicit Search_Architect(QWidget* parent = nullptr);
    ~Search_Architect();
    struct RoadLengthInfo {  //道路信息
        std::string start;  //起点
        std::string end;  //终点
        int length;  //距离
    };
    using StringList = std::vector<std::string>;

signals:
    void chooseback();  //返回信号

private slots:
    void showResult();  //展示查询结果

private:
    int length;  //窗口的长
    int width;  //窗口的宽
    QPushButton* buttonChooseback = NULL;  //返回按钮
    QComboBox* boxLocation = NULL;  //要查询的建筑
    QComboBox* boxType = NULL;  //所有设施类型
    QLabel* labelLocation = NULL;  //查询场所
    QLabel* labelType = NULL;  //设施类型
    QPushButton* buttonSearch = NULL;  //查询按钮
    QTableWidget* searchTable = NULL;  //距离表格

    void initWidget();  //界面初始化函数
    StringList search(const std::string& type, const StringList& allFacilities);  //查找设施类型满足要求的设施名
    int dijkstraLength(const std::string& start, const std::string& end, const std::vector<RoadLengthInfo>& roads);
    std::vector<std::pair<std::string, int>> sortPlacesByDistance(const std::string& currentLocation,const std::string& facilityType,
                                                                  const StringList& allFacilities,const std::vector<RoadLengthInfo>& roads);
};

#endif // SEARCH_ARCHITECT_H
