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
    //void sort(QString location, QStringList facilities);

};

#endif // SEARCH_ARCHITECT_H
