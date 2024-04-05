//游学日记管理界面
#include "diary_management.h"

Diary_Management::Diary_Management(QString accountNumber, QString place)
{
    this->accountNumber = accountNumber;
    this->place = place;
    initWidget();
    connect(buttonChoosebackDM, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回选择界面的信号
    });
    connect(buttonQueryDestinationDM, &QPushButton::clicked, [=]() {  //若点击目的地查询按钮
        mode = 1;
        showResult();
    });
    connect(buttonQueryDiaryNameDM, &QPushButton::clicked, [=]() {  //若点击日记名查询按钮
        mode = 2;
        showResult();
    });
}

Diary_Management::~Diary_Management() {
    delete labelDestinationDM;
    labelDestinationDM = NULL;
    delete buttonChoosebackDM;
    buttonChoosebackDM = NULL;
    delete buttonWriteDM;
    buttonWriteDM = NULL;
    delete buttonHeatValueDM;
    buttonHeatValueDM = NULL;
    delete buttonGoodCommentsDM;
    buttonGoodCommentsDM = NULL;
    delete buttonQueryDestinationDM;
    buttonQueryDestinationDM = NULL;
    delete buttonQueryDiaryNameDM;
    buttonQueryDiaryNameDM = NULL;
    delete boxDestinationDM;
    boxDestinationDM = NULL;
    delete lineEditDiaryNameDM;
    lineEditDiaryNameDM = NULL;
    delete tableDiaryInfoDM;
    tableDiaryInfoDM = NULL;
}

void Diary_Management::showResult() {
    std::string destination = boxDestinationDM->currentText().toStdString();
    std::string diaryName = lineEditDiaryNameDM->text().toStdString();
    QSqlQuery query;

    /*
    if(buttonHeatValue->isChecked()){
        query.exec("select title,from t_diary");
        if(mode == 1)

        else if(mode == 2)
    }
    else if(buttonGoodComments->isChecked()){
        if(mode == 1)

        else if(mode == 2)
    }
    */
    for (int i = 0; i < tableDiaryInfoDM->rowCount(); i++) {
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
        tableDiaryInfoDM->setItem(i, 0, itemName);
        tableDiaryInfoDM->setItem(i, 1, itemAccountNumber);
        tableDiaryInfoDM->setItem(i, 2, itemDestination);
        tableDiaryInfoDM->setItem(i, 3, itemHeatValue);
        tableDiaryInfoDM->setItem(i, 4, itemGoodComments);
    }
}

void Diary_Management::initWidget() {
    QStringList horizontalHeaderLabels;
    QSqlQuery query;
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);

    labelDestinationDM = new QLabel("游学目的地：", this);
    labelDestinationDM->setGeometry(300, 150, 200, 45);
    labelDestinationDM->setAlignment(Qt::AlignCenter);
    labelDestinationDM->setStyleSheet("QLabel{font:24px;}");

    buttonChoosebackDM = new QPushButton("返回", this);
    buttonChoosebackDM->move(0, WIDTH * 8 / 9);
    buttonChoosebackDM->resize(LENGTH / 9, WIDTH / 9);
    buttonChoosebackDM->setFont(QFont("黑体", 25));
    buttonWriteDM = new QPushButton("写日记", this);
    buttonWriteDM->move(LENGTH * 8 / 9, WIDTH * 8 / 9);
    buttonWriteDM->resize(LENGTH / 9, WIDTH / 9);
    buttonWriteDM->setFont(QFont("黑体", 25));
    buttonQueryDestinationDM = new QPushButton("查询", this);
    buttonQueryDestinationDM->move(870, 150);
    buttonQueryDestinationDM->resize(80, 45);
    buttonQueryDestinationDM->setFont(QFont("黑体", 20));
    buttonQueryDiaryNameDM = new QPushButton("查询", this);
    buttonQueryDiaryNameDM->move(870, 230);
    buttonQueryDiaryNameDM->resize(80, 45);
    buttonQueryDiaryNameDM->setFont(QFont("黑体", 20));

    buttonHeatValueDM = new QRadioButton("按热度排序", this);
    buttonHeatValueDM->setChecked(true);
    buttonHeatValueDM->move(1000, 190);
    buttonHeatValueDM->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");
    buttonGoodCommentsDM = new QRadioButton("按评价排序", this);
    buttonGoodCommentsDM->move(1000, 220);
    buttonGoodCommentsDM->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 15px; }");

    lineEditDiaryNameDM = new QLineEdit(this);
    lineEditDiaryNameDM->setGeometry(320, WIDTH * 3 / 19 + 75, 450, WIDTH / 17);
    lineEditDiaryNameDM->setFont(QFont("黑体", 15));
    lineEditDiaryNameDM->setPlaceholderText("请输入日记名称");
    lineEditDiaryNameDM->setClearButtonEnabled(true);
    lineEditDiaryNameDM->setValidator(new QRegularExpressionValidator(QRegularExpression("[\u4e00-\u9fff]+"), this));  //搜索日记名只允许输入中文字符

    tableDiaryInfoDM = new QTableWidget(10, 5, this);  //创建10行5列的表格，用于显示日记信息
    tableDiaryInfoDM->verticalHeader()->setVisible(false);
    tableDiaryInfoDM->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableDiaryInfoDM->move(224, 350);
    tableDiaryInfoDM->resize(952, 492);
    tableDiaryInfoDM->setFont(QFont("黑体", 14));
    tableDiaryInfoDM->horizontalHeader()->setMinimumHeight(40);
    for (int i = 0; i < tableDiaryInfoDM->rowCount(); i++)
        tableDiaryInfoDM->setRowHeight(i, 45);
    tableDiaryInfoDM->setColumnWidth(0, 250);
    tableDiaryInfoDM->setColumnWidth(1, 250);
    tableDiaryInfoDM->setColumnWidth(2, 250);
    tableDiaryInfoDM->setColumnWidth(3, 100);
    tableDiaryInfoDM->setColumnWidth(4, 100);
    horizontalHeaderLabels << "日记名称" << "作者账户名" << "目的地" << "热度" << "好评数";
    tableDiaryInfoDM->setHorizontalHeaderLabels(horizontalHeaderLabels);

    boxDestinationDM = new QComboBox(this);
    boxDestinationDM->move(490, 150);
    boxDestinationDM->resize(280, 45);
    boxDestinationDM->setFont(QFont("黑体", 17));
    boxDestinationDM->addItem("全部");
    query.exec("select name from t_place_ranking");
    while (query.next()) {
        QString name = query.value(0).toString();
        boxDestinationDM->addItem(name);
    }
}

void Diary_Management::paintEvent(QPaintEvent*) {
    QPen pen(Qt::black);  //画笔颜色为黑色
    pen.setWidth(2);  //画笔宽度
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setFont(QFont("黑体", 25));
    painter.drawText(300,100, "当前账户名:" + accountNumber);
    painter.drawText(620,100, "当前所在景区/学校:" + place);
}




