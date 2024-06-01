//目的地推荐界面
#ifndef DESTINATION_RECOMMENDATION_H
#define DESTINATION_RECOMMENDATION_H

#include "choose_widget.h"

class Destination_Recommendation : public QWidget
{
    Q_OBJECT
public:
    Destination_Recommendation(QString accountNumber);  //带参构造：参数为账号名
    ~Destination_Recommendation();

signals:
    void chooseback();  //返回信号

private slots:
    void showResult();  //展示查询结果
    void tableClicked(int row, int column);  //若点击了表格中的第二列，则根据选中的景区/学校名进入下一个界面

private:
    QString accountNumber;  //当前账户名
    Choose_Widget* chooseWidget = NULL;  //游学路线规划、场所查询、游学日记管理选择界面
    QPushButton* buttonExitDR = NULL;  //退出按钮
    QPushButton* buttonSearchDR = NULL;  //搜索按钮
    QLineEdit* lineEditSearchDR = NULL;  //景区/学校输入框
    QRadioButton* buttonHeatValueDR = NULL;  //按热度排序
    QRadioButton* buttonGoodCommentsDR = NULL;  //按评价排序
    QCheckBox* boxKeyWordDR = NULL;  //是否关键词优先
    QTableWidget* rankingTableDR = NULL;  //排名表格

    void initWidget();  //界面初始化函数
    vector<Place> PlaceSearch(QString query, vector<Place> spots);
    vector<Place> fuzzySearchPlaces(const string& query, const vector<Place>& places, int n);  //查找算法
    StringList generateNgrams(const string& str, int n);
    bool comparePlaceMatch(const Place& a, const Place& b, const StringList& queryNgrams);

    vector<Place> sort(const vector<Place>& placeNames);  //排序算法
    void quickSort(vector<Place>& arr, int left, int right, const StringList& queryNgrams);
    int partition(vector<Place>& arr, int left, int right, const StringList& queryNgrams);
    static bool compareById(const Place& a, const Place& b);

    void paintEvent(QPaintEvent*);  //重写paintEvent函数，用于在屏幕上打印出当前账户名
};

#endif // DESTINATION_RECOMMENDATION_H
