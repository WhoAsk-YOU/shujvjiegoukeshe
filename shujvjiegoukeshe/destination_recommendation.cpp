//目的地推荐界面
#include "destination_recommendation.h"

Destination_Recommendation::Destination_Recommendation(QString accountNumber)
{
    this->accountNumber = accountNumber;
    initWidget();
    connect(buttonExit,&QPushButton::clicked,[=](){  //若点击退出按钮
        emit this->chooseback();  //发出返回开始界面的信号
    });
    connect(buttonSearch,&QPushButton::clicked,[=](){  //若点击搜索按钮
        flag = true;
        showResult();  //表格会显示数据
    });
    connect(rankingTable,&QTableWidget::cellClicked,[=](int row, int column){  //若点击表格第二列中的某个单元格
        if(flag == true)  //如果表格里内容不为空
            tableClicked(row,column);  //跳转到选择界面
    });
}

Destination_Recommendation::~Destination_Recommendation(){  //析构函数，释放空间
    delete labelAccountnum;
    labelAccountnum = NULL;
    delete buttonExit;
    buttonExit = NULL;
    delete buttonSearch;
    buttonSearch = NULL;
    delete lineEditSearch;
    lineEditSearch = NULL;
    delete buttonHeatValue;
    buttonHeatValue = NULL;
    delete buttonGoodComments;
    buttonGoodComments = NULL;
    delete rankingTable;
    rankingTable = NULL;
}

void Destination_Recommendation::showResult(){
    QSqlQuery query;
    QStringList horizontalHeaderLabels;
    Place spots[200];  //结构数组，储存景区/学校名以及对应的热度/好评数
    std::vector<Place> origin_Places,SearchedPlaces;  //把数组放进向量里，统一PlaceSearch函数返回值格式
    std::string place = lineEditSearch->text().toStdString();
    if(buttonHeatValue->isChecked()){  //若选中了按热度排序
        query.exec("select name,heat_value from t_place_ranking");  //从数据库中拿出name及heat_value的所有字段值
        horizontalHeaderLabels << "排名" << "名称" << "热度";  //更改水平表头
        rankingTable->setHorizontalHeaderLabels(horizontalHeaderLabels);
    }
    else if(buttonGoodComments->isChecked()){  //若选中了按评论排序
        query.exec("select name,good_comments from t_place_ranking");  //从数据库中拿出name及good_comments的所有字段值
        horizontalHeaderLabels << "排名" << "名称" << "好评数";  //更改水平表头
        rankingTable->setHorizontalHeaderLabels(horizontalHeaderLabels);
    }

    for(int i = 0; query.next(); i++){  //遍历数据集，将数据加入结构数组
        spots[i].name = query.value(0).toString().toStdString();  //获取景区/学校的名称
        spots[i].value = query.value(1).toInt();  //获取对应的热度或好评数
        origin_Places.push_back(spots[i]);
    }

    SearchedPlaces = sort(placeSearch(place, origin_Places));
    //这里到时候会调用排序和查找函数

    for(int i = 0; i < rankingTable->rowCount() && i < SearchedPlaces.size(); i++){  //将排序后的数据填入表中
        QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(SearchedPlaces[i].name));
        QTableWidgetItem* itemValue = new QTableWidgetItem(QString::number(SearchedPlaces[i].value));
        itemName->setTextAlignment(Qt::AlignCenter);
        itemValue->setTextAlignment(Qt::AlignCenter);
        rankingTable->setItem(i, 1, itemName);
        rankingTable->setItem(i, 2, itemValue);
    }
}

void Destination_Recommendation::tableClicked(int row, int column){
    if(column==1){
        QTableWidgetItem *item = rankingTable->item(row, column);  //获取到点击单元格的内容，即景区/学校名
        this->hide();  //隐藏目的地推荐界面
        chooseWidget = new Choose_Widget(accountNumber,item->text());
        chooseWidget->setGeometry(this->geometry());  //界面位置不会变化
        chooseWidget->show();  //显示选择界面
        connect(chooseWidget,&Choose_Widget::chooseback,[=](){  //若在选择界面点击了返回按钮，接受返回信号
            this->setGeometry(chooseWidget->geometry());
            this->show();
            delete chooseWidget;
            chooseWidget = NULL;
        });
    }
}

void Destination_Recommendation::initWidget(){  //初始化目的地推荐界面
    QStringList horizontalHeaderLabels;
    length = 1400;
    width = 950;
    setWindowTitle("学生游学系统");
    setFixedSize(length,width);

    labelAccountnum = new QLabel("当前账户名:"+accountNumber,this);
    labelAccountnum->setGeometry(length*2/5+20, 0, length/5, width/6);  //设置文本框位置及大小
    labelAccountnum->setAlignment(Qt::AlignCenter);  //文本框内容居中显示
    labelAccountnum->setStyleSheet("QLabel{font:25px;}");  //设置文本框字体

    buttonExit = new QPushButton("退出", this);
    buttonExit->move(0, width*8/9);
    buttonExit->resize(length/9, width/9);
    buttonExit->setFont(QFont("黑体",25));
    buttonSearch = new QPushButton("搜索", this);
    buttonSearch->move(length/2+length/7, width*3/19);
    buttonSearch->resize(length/17, width/17);
    buttonSearch->setFont(QFont("黑体",21));

    lineEditSearch = new QLineEdit(this);
    lineEditSearch->setGeometry(length/2-length/7,width*3/19,length/3.5,width/17);
    lineEditSearch->setFont(QFont("黑体",15));
    lineEditSearch->setPlaceholderText("请输入景区或学校名/关键词");
    lineEditSearch->setClearButtonEnabled(true);
    lineEditSearch->setMaxLength(20);

    buttonHeatValue = new QRadioButton("按热度排序",this);
    buttonHeatValue->setChecked(true);  //默认选中按热度排序
    buttonHeatValue->move(500,230);
    buttonHeatValue->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");  //设置按钮大小及字体大小
    buttonGoodComments = new QRadioButton("按评价排序",this);
    buttonGoodComments->move(700,230);
    buttonGoodComments->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");

    rankingTable = new QTableWidget(10,3,this);  //创建10行3列的表格，用于显示景区/学校排名
    rankingTable->verticalHeader()->setVisible(false);  //隐藏垂直表头
    rankingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  //表格内的数据不允许修改
    rankingTable->move(length/2-255,300);
    rankingTable->resize(572,492);
    rankingTable->setFont(QFont("黑体",18));
    rankingTable->horizontalHeader()->setMinimumHeight(40);
    for(int i = 0; i < rankingTable->rowCount(); i++){
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(i + 1));
        item->setTextAlignment(Qt::AlignCenter);  //文本居中对齐
        rankingTable->setItem(i, 0, item);
    }
    for (int i = 0; i < rankingTable->rowCount(); i++)  //设置每一行的高度为45像素
        rankingTable->setRowHeight(i, 45);
    rankingTable->setColumnWidth(0, 100);  // 设置第1列的宽度为100像素
    rankingTable->setColumnWidth(1, 320);
    rankingTable->setColumnWidth(2, 150);
    //设置水平表头
    horizontalHeaderLabels << "排名" << "名称" << "热度";
    rankingTable->setHorizontalHeaderLabels(horizontalHeaderLabels);
}

std::vector<Destination_Recommendation::Place> Destination_Recommendation::placeSearch(std::string query, std::vector<Place> spots){
    int n = 3; // 生成3-grams  这个n可以更改，但目前来看n=3可以得到最优解
    if (empty(query))//如果输入为空串，打印所有
        return spots;
    return fuzzySearchPlaces(query, spots, n);
}

std::vector<std::string> Destination_Recommendation::generateNgrams(const std::string& str, int n){
    std::vector<std::string> ngrams;
    for (size_t i = 0; i <= str.length() - n; ++i)
        ngrams.push_back(str.substr(i, n));
    return ngrams;
}

bool Destination_Recommendation::comparePlaceMatch(const Place& a, const Place& b, const std::vector<std::string>& queryNgrams){
    std::vector<std::string> aNgrams = generateNgrams(a.name, 3);
    std::vector<std::string> bNgrams = generateNgrams(b.name, 3);

    int aMatches = 0;
    int bMatches = 0;
    for (const auto& queryNgram : queryNgrams) {
        if (std::find(aNgrams.begin(), aNgrams.end(), queryNgram) != aNgrams.end())
            ++aMatches;
        if (std::find(bNgrams.begin(), bNgrams.end(), queryNgram) != bNgrams.end())
            ++bMatches;
    }

    // 如果匹配数量相同，则按字母顺序排序
    if (aMatches == bMatches)
        return a.name < b.name;

    // 返回匹配数量多的地名在前
    return aMatches > bMatches;
}

std::vector<Destination_Recommendation::Place> Destination_Recommendation::fuzzySearchPlaces(const std::string& query, const std::vector<Place>& places, int n){
    std::vector<Place> results;
    std::vector<std::string> queryNgrams = generateNgrams(query, n);

    for (const auto& place : places) {
        std::vector<std::string> placeNgrams = generateNgrams(place.name, n);
        bool isMatch = false;
        for (const auto& queryNgram : queryNgrams) {
            if (std::find(placeNgrams.begin(), placeNgrams.end(), queryNgram) != placeNgrams.end()) {
                isMatch = true;
                break;
            }
        }
        if (isMatch)
            results.push_back(place);
    }

    // 根据匹配程度对结果进行排序
    std::sort(results.begin(), results.end(), [&](const Place& a, const Place& b) {
        return comparePlaceMatch(a, b, queryNgrams);
    });

    return results;
}

bool Destination_Recommendation::compareById(const Place& a, const Place& b) {
    return a.value > b.value;
}

std::vector<Destination_Recommendation::Place> Destination_Recommendation::sort(const std::vector<Place>& placeNames) {
    std::vector<Destination_Recommendation::Place> sortedData(placeNames); // 拷贝数据以便排序
    int size = sortedData.size();

    // 如果数据少于或等于10个，直接对整个向量进行排序
    if (size <= 10) {
        std::sort(sortedData.begin(), sortedData.end(), compareById);
        return sortedData;
    }

    // 否则，仅对前10个元素进行排序
    std::partial_sort(sortedData.begin(), sortedData.begin() + 10, sortedData.end(), compareById);

    // 返回前10个已排序的元素
    return std::vector<Destination_Recommendation::Place>(sortedData.begin(), sortedData.begin() + 10);
}














