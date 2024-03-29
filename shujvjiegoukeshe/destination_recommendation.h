//目的地推荐界面
#ifndef DESTINATION_RECOMMENDATION_H
#define DESTINATION_RECOMMENDATION_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QLineEdit>
#include <QSqlQuery>
#include <QRadioButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QCheckBox>
#include <QRegularExpressionValidator>
#include <choose_widget.h>

class Destination_Recommendation : public QWidget
{
    Q_OBJECT
public:
    struct Place {  //景区/学校信息
        std::string name;  //景区或学校名
        int value;  //景区或学校对应的热度值/好评数
    };
    Destination_Recommendation(QString accountNumber);  //带参构造：参数为账号名
    ~Destination_Recommendation();

signals:
    void chooseback();  //返回信号

private slots:
    void showResult();  //展示查询结果
    void tableClicked(int row, int column);  //若点击了表格中的第二列，则根据选中的景区/学校名进入下一个界面

private:
    int length;  //窗口的长
    int width;  //窗口的宽
    bool flag = false;  //用于判断表格里是否显示数据
    QString accountNumber;  //当前账户名
    Choose_Widget* chooseWidget = NULL;  //游学路线规划、场所查询、游学日记管理选择界面
    QPushButton* buttonExit = NULL;  //退出按钮
    QPushButton* buttonSearch = NULL;  //搜索按钮
    QLineEdit* lineEditSearch = NULL;  //景区/学校输入框
    QRadioButton* buttonHeatValue = NULL;  //按热度排序
    QRadioButton* buttonGoodComments = NULL;  //按评价排序
    QCheckBox* boxKeyWord = NULL;  //是否关键词优先
    QTableWidget* rankingTable = NULL;  //排名表格

    void initWidget();  //界面初始化函数

    std::vector<Place> placeSearch(std::string query, std::vector<Place> spots);  //查找算法
    std::vector<std::string> generateNgrams(const std::string& str, int n);
    bool comparePlaceMatch(const Place& a, const Place& b, const std::vector<std::string>& queryNgrams);
    std::vector<Place> fuzzySearchPlaces(const std::string& query, const std::vector<Place>& places, int n);

    std::vector<Place> sort(const std::vector<Place>& placeNames);  //排序算法
    static bool compareById(const Place& a, const Place& b);

    void paintEvent(QPaintEvent*);  //重写paintEvent函数，用于在屏幕上打印出当前账户名
};

#endif // DESTINATION_RECOMMENDATION_H
