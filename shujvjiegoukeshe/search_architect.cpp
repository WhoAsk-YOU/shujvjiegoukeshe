//场所查询界面
#include "search_architect.h"
#include <iostream>

Search_Architect::Search_Architect(QWidget *parent)
    : QWidget{parent}
{
    initWidget();
    connect(buttonChooseback,&QPushButton::clicked,[=](){  //若点击返回按钮
        emit this->chooseback();  //发出返回选择界面的信号
    });
    connect(buttonSearch,&QPushButton::clicked,[=](){  //若点击查询按钮
        showResult();
    });
}

Search_Architect::~Search_Architect(){
    delete buttonChooseback;
    buttonChooseback = NULL;
    delete buttonSearch;
    buttonSearch = NULL;
    delete boxLocation;
    boxLocation = NULL;
    delete boxType;
    boxType = NULL;
    delete labelType;
    labelType = NULL;
    delete labelLocation;
    labelLocation = NULL;
    delete searchTable;
    searchTable = NULL;
}

void Search_Architect::initWidget(){
    QSqlQuery query;
    QStringList types;
    QStringList horizontalHeaderLabels;
    types<<"全部"<<"商店"<<"饭店"<<"洗手间"<<"图书馆"<<"食堂"<<"超市"<<"咖啡馆"<<"医院"<<"花园"<<"体育馆";
    length = 1400;
    width = 950;
    setWindowTitle("学生游学系统");
    setFixedSize(length,width);

    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, width*8/9);
    buttonChooseback->resize(length/9, width/9);
    buttonChooseback->setFont(QFont("黑体",25));
    buttonSearch = new QPushButton("查询", this);
    buttonSearch->move(1050, 130);
    buttonSearch->resize(80, 45);
    buttonSearch->setFont(QFont("黑体",20));

    boxType = new QComboBox(this);
    boxType->move(750,130);
    boxType->resize(200,45);
    boxType->setFont(QFont("黑体",20));
    boxType->addItems(types);
    boxLocation = new QComboBox(this);
    boxLocation->move(350,130);
    boxLocation->resize(200,45);
    boxLocation->setFont(QFont("黑体",20));
    query.exec("select name from t_architect");
    while(query.next()){
        QString name = query.value(0).toString();
        boxLocation->addItem(name);
    }

    labelLocation = new QLabel("查询场所：",this);
    labelLocation->setGeometry(200, 130, 200, 45);
    labelLocation->setAlignment(Qt::AlignCenter);
    labelLocation->setStyleSheet("QLabel{font:20px;}");
    labelType = new QLabel("设施类型：",this);
    labelType->setGeometry(590, 130, 200, 45);
    labelType->setAlignment(Qt::AlignCenter);
    labelType->setStyleSheet("QLabel{font:20px;}");

    searchTable = new QTableWidget(5,2,this);  //创建5行2列的表格
    searchTable->verticalHeader()->setVisible(false);  //隐藏垂直表头
    searchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  //表格内的数据不允许修改
    searchTable->move(330,300);
    searchTable->resize(752,402);
    searchTable->setFont(QFont("黑体",20));
    searchTable->horizontalHeader()->setMinimumHeight(50);
    for (int i = 0; i < searchTable->rowCount(); i++)  //设置每一行的高度为70像素
        searchTable->setRowHeight(i, 70);
    searchTable->setColumnWidth(0, 500);  // 设置第1列的宽度为500像素
    searchTable->setColumnWidth(1, 250);
    horizontalHeaderLabels << "设施名称" << "到查询场所的距离";
    searchTable->setHorizontalHeaderLabels(horizontalHeaderLabels);
}

Search_Architect::StringList Search_Architect::search(const std::string& type, const StringList& allFacilities) {
    StringList result;
    // 如果设施类型为“全部”，直接返回所有设施名称
    if (type == "全部")
        return allFacilities;

    // 否则，搜索与设施类型相符的设施名（设施类型为设施名的子串）
    for (const std::string& facility : allFacilities) {
        if (facility.size() >= type.size()) { // 如果设施名长度小于设施类型，不符合条件
            bool match = true;
            for (size_t i = 0; i <= facility.size() - type.size(); ++i) {
                match = true;
                for (size_t j = 0; j < type.size(); ++j) {
                    if (tolower(facility[i + j]) != tolower(type[j])) { // 忽略大小写进行匹配
                        match = false;
                        break;
                    }
                }
                if (match) { // 如果找到匹配的子串，则加入结果列表中
                    result.push_back(facility);
                    break; // 终止内部循环，继续下一个设施名的匹配
                }
            }
        }
    }

    return result;
}
/*
void Search_Architect::sort(QString location, QStringList facilities){

}
*/
void Search_Architect::showResult(){
    QSqlQuery query;
    StringList allFacilities, facilities;
    std::string location = boxLocation->currentText().toStdString();
    std::string type = boxType->currentText().toStdString();
    query.exec("select name from t_architect order by architect_id limit 20,50");
    while(query.next()){
        std::string name = query.value(0).toString().toStdString();
        allFacilities.push_back(name);
    }
    facilities = search(type, allFacilities);

    //这里要调用查找和排序函数


    for(int i = 0; i < searchTable->rowCount(); i++){  //将排序后的数据填入表中
        QTableWidgetItem* itemName = new QTableWidgetItem("New Data");
        QTableWidgetItem* itemDistance = new QTableWidgetItem("New value");
        itemName->setTextAlignment(Qt::AlignCenter);
        itemDistance->setTextAlignment(Qt::AlignCenter);
        searchTable->setItem(i, 0, itemName);
        searchTable->setItem(i, 1, itemDistance);
    }
}
