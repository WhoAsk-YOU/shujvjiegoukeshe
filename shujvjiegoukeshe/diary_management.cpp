//游学日记管理界面
#include "diary_management.h"

Diary_Management::Diary_Management(QString accountNumber, QString place)
{
    this->accountNumber = accountNumber;
    this->place = place;
    initWidget();
    connect(buttonChooseback, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回选择界面的信号
    });
    connect(buttonQueryDestination, &QPushButton::clicked, [=]() {  //若点击目的地查询按钮
        mode = 1;
        showResult();
    });
    connect(buttonQueryDiaryName, &QPushButton::clicked, [=]() {  //若点击日记名查询按钮
        mode = 2;
        showResult();
    });
}

Diary_Management::~Diary_Management() {
    delete labelDestination;
    labelDestination = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
    delete buttonWrite;
    buttonWrite = NULL;
    delete buttonHeatValue;
    buttonHeatValue = NULL;
    delete buttonGoodComments;
    buttonGoodComments = NULL;
    delete buttonQueryDestination;
    buttonQueryDestination = NULL;
    delete buttonQueryDiaryName;
    buttonQueryDiaryName = NULL;
    delete boxDestination;
    boxDestination = NULL;
    delete lineEditDiaryName;
    lineEditDiaryName = NULL;
    delete tableDiaryInfo;
    tableDiaryInfo = NULL;
}

void Diary_Management::showResult() {
    QSqlQuery query;
    /*
    if(buttonHeatValue->isChecked()){
        if(mode == 1)

        else if(mode == 2)
    }
    else if(buttonGoodComments->isChecked()){
        if(mode == 1)

        else if(mode == 2)
    }
    */
    for (int i = 0; i < tableDiaryInfo->rowCount(); i++) {
        QTableWidgetItem* itemName = new QTableWidgetItem("New Data");
        QTableWidgetItem* itemAccountNumber = new QTableWidgetItem("New value");
        QTableWidgetItem* itemDestination = new QTableWidgetItem("New value");
        QTableWidgetItem* itemHeatValue = new QTableWidgetItem("New value");
        QTableWidgetItem* itemGoodComments = new QTableWidgetItem("New value");
        itemName->setTextAlignment(Qt::AlignCenter);
        itemAccountNumber->setTextAlignment(Qt::AlignCenter);
        itemDestination->setTextAlignment(Qt::AlignCenter);
        itemHeatValue->setTextAlignment(Qt::AlignCenter);
        itemGoodComments->setTextAlignment(Qt::AlignCenter);
        tableDiaryInfo->setItem(i, 0, itemName);
        tableDiaryInfo->setItem(i, 1, itemAccountNumber);
        tableDiaryInfo->setItem(i, 2, itemDestination);
        tableDiaryInfo->setItem(i, 3, itemHeatValue);
        tableDiaryInfo->setItem(i, 4, itemGoodComments);
    }
}

void Diary_Management::initWidget() {
    QStringList horizontalHeaderLabels;
    QSqlQuery query;
    length = 1400;
    width = 950;
    setWindowTitle("学生游学系统");
    setFixedSize(length, width);

    labelDestination = new QLabel("游学目的地：", this);
    labelDestination->setGeometry(300, 80, 200, 45);
    labelDestination->setAlignment(Qt::AlignCenter);
    labelDestination->setStyleSheet("QLabel{font:24px;}");

    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, width * 8 / 9);
    buttonChooseback->resize(length / 9, width / 9);
    buttonChooseback->setFont(QFont("黑体", 25));
    buttonWrite = new QPushButton("写日记", this);
    buttonWrite->move(length * 8 / 9, width * 8 / 9);
    buttonWrite->resize(length / 9, width / 9);
    buttonWrite->setFont(QFont("黑体", 25));
    buttonQueryDestination = new QPushButton("查询", this);
    buttonQueryDestination->move(870, 80);
    buttonQueryDestination->resize(80, 45);
    buttonQueryDestination->setFont(QFont("黑体", 20));
    buttonQueryDiaryName = new QPushButton("查询", this);
    buttonQueryDiaryName->move(870, 160);
    buttonQueryDiaryName->resize(80, 45);
    buttonQueryDiaryName->setFont(QFont("黑体", 20));

    buttonHeatValue = new QRadioButton("按热度排序", this);
    buttonHeatValue->setChecked(true);
    buttonHeatValue->move(1000, 120);
    buttonHeatValue->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");
    buttonGoodComments = new QRadioButton("按评价排序", this);
    buttonGoodComments->move(1000, 150);
    buttonGoodComments->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");

    lineEditDiaryName = new QLineEdit(this);
    lineEditDiaryName->setGeometry(320, width * 3 / 19 + 5, 450, width / 17);
    lineEditDiaryName->setFont(QFont("黑体", 15));
    lineEditDiaryName->setPlaceholderText("请输入日记名称");
    lineEditDiaryName->setClearButtonEnabled(true);

    tableDiaryInfo = new QTableWidget(10, 5, this);  //创建10行5列的表格，用于显示日记信息
    tableDiaryInfo->verticalHeader()->setVisible(false);
    tableDiaryInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableDiaryInfo->move(224, 300);
    tableDiaryInfo->resize(952, 492);
    tableDiaryInfo->setFont(QFont("黑体", 14));
    tableDiaryInfo->horizontalHeader()->setMinimumHeight(40);
    for (int i = 0; i < tableDiaryInfo->rowCount(); i++)
        tableDiaryInfo->setRowHeight(i, 45);
    tableDiaryInfo->setColumnWidth(0, 250);
    tableDiaryInfo->setColumnWidth(1, 250);
    tableDiaryInfo->setColumnWidth(2, 250);
    tableDiaryInfo->setColumnWidth(3, 100);
    tableDiaryInfo->setColumnWidth(4, 100);
    horizontalHeaderLabels << "日记名称" << "作者账户名" << "目的地" << "热度" << "好评数";
    tableDiaryInfo->setHorizontalHeaderLabels(horizontalHeaderLabels);

    boxDestination = new QComboBox(this);
    boxDestination->move(490, 80);
    boxDestination->resize(280, 45);
    boxDestination->setFont(QFont("黑体", 17));
    boxDestination->addItem("全部");
    query.exec("select name from t_place_ranking");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxDestination->addItem(name);
    }
}
