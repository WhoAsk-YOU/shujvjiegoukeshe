//场所查询界面
#include "search_architect.h"

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
/*
QStringList Search_Architect::search(QString type, QStringList allFacilities){

}

void Search_Architect::sort(QString location, QStringList facilities){

}
*/
void Search_Architect::showResult(){
    QSqlQuery query;
    QStringList allFacilities, facilities;
    location = boxLocation->currentText();
    type = boxType->currentText();
    query.exec("select name from t_architect limit 15,55");
    while(query.next()){
        QString name = query.value(0).toString();
        allFacilities.append(name);
    }
    //facilities = search(type, allFacilities);

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
