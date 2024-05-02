//场所查询界面
#ifndef SEARCH_ARCHITECT_H
#define SEARCH_ARCHITECT_H

#include "Structure.h"

class Search_Architect : public QWidget
{
    Q_OBJECT
public:
    explicit Search_Architect(QWidget* parent = nullptr);
    ~Search_Architect();

signals:
    void chooseback();  //返回信号

private slots:
    void showResult();  //展示查询结果

private:
    QPushButton* buttonChoosebackSA = NULL;  //返回按钮
    QComboBox* boxLocationSA = NULL;  //要查询的建筑
    QComboBox* boxTypeSA = NULL;  //所有设施类型
    QLabel* labelLocationSA = NULL;  //查询场所
    QLabel* labelTypeSA = NULL;  //设施类型
    QPushButton* buttonSearchSA = NULL;  //查询按钮
    QTableWidget* searchTableSA = NULL;  //距离表格

    void initWidget();  //界面初始化函数
    StringList search(const string& type, const StringList& allFacilities);  //查找设施类型满足要求的设施名
    int dijkstraLength(const string& start, const string& end, const vector<RoadLengthInfo>& roads);
    vector<pair<string, int>> sortPlacesByDistance(const string& currentLocation,const string& facilityType,
                                                   const StringList& allFacilities,const vector<RoadLengthInfo>& roads);
};

#endif // SEARCH_ARCHITECT_H
