#include "route_strategy.h"

Route_Strategy::Route_Strategy(QWidget *parent)
    : QWidget{parent}
{
    initWidget();
    connect(buttonChooseback,&QPushButton::clicked,[=](){  //若点击返回按钮
        emit this->chooseback();  //发出返回目的地推荐界面的信号
    });
    connect(buttonOneQuery,&QPushButton::clicked,[=](){  //若点击目的地(1个)查询按钮
        mode = 1;
        update();  //调用paintEvent函数
    });
    connect(buttonQueries,&QPushButton::clicked,[=](){  //若点击目的地(多个)查询按钮
        mode = 2;
        update();  //调用paintEvent函数
    });
}

Route_Strategy::~Route_Strategy(){
    delete labelInitial;
    labelInitial = NULL;
    delete labelDestination;
    labelDestination = NULL;
    delete labelDestinations;
    labelDestinations = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
    delete buttonOneQuery;
    buttonOneQuery = NULL;
    delete buttonQueries;
    buttonQueries = NULL;
    delete boxInitialLocation;
    boxInitialLocation = NULL;
    delete boxDestination;
    boxDestination = NULL;
    delete buttonShortestDistance;
    buttonShortestDistance = NULL;
    delete buttonShortestTime;
    buttonShortestTime = NULL;
    delete lineEditDestinations;
    lineEditDestinations = NULL;
}

void Route_Strategy::initWidget(){
    QSqlQuery query;
    length = 1400;
    width = 950;
    setWindowTitle("学生游学系统");
    setFixedSize(length,width);

    labelInitial = new QLabel("起始位置：",this);
    labelInitial->setGeometry(150, 80, 200, 45);
    labelInitial->setAlignment(Qt::AlignCenter);
    labelInitial->setStyleSheet("QLabel{font:20px;}");
    labelDestination = new QLabel("目的地(1个)：",this);
    labelDestination->setGeometry(540, 80, 200, 45);
    labelDestination->setAlignment(Qt::AlignCenter);
    labelDestination->setStyleSheet("QLabel{font:20px;}");
    labelDestinations = new QLabel("目的地(多个)：",this);
    labelDestinations->setGeometry(165, 160, 200, 45);
    labelDestinations->setAlignment(Qt::AlignCenter);  //文本框内容居中显示
    labelDestinations->setStyleSheet("QLabel{font:20px;}");  //设置文本框字体

    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, width*8/9);
    buttonChooseback->resize(length/9, width/9);
    buttonChooseback->setFont(QFont("黑体",25));
    buttonOneQuery = new QPushButton("查询", this);
    buttonOneQuery->move(1000, 80);
    buttonOneQuery->resize(80, 45);
    buttonOneQuery->setFont(QFont("黑体",20));
    buttonQueries = new QPushButton("查询", this);
    buttonQueries->move(1000, 160);
    buttonQueries->resize(80, 45);
    buttonQueries->setFont(QFont("黑体",20));

    boxInitialLocation = new QComboBox(this);
    boxInitialLocation->move(300,80);
    boxInitialLocation->resize(200,45);
    boxInitialLocation->setFont(QFont("黑体",20));
    boxDestination = new QComboBox(this);
    boxDestination->move(700,80);
    boxDestination->resize(200,45);
    boxDestination->setFont(QFont("黑体",20));
    query.exec("select name from t_architect");
    while(query.next()){
        QString name = query.value(0).toString();
        boxInitialLocation->addItem(name);
        boxDestination->addItem(name);
    }

    buttonShortestTime = new QRadioButton("最短时间",this);
    buttonShortestTime->setChecked(true);
    buttonShortestTime->move(1120,120);
    buttonShortestTime->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");  //设置按钮大小及字体大小
    buttonShortestDistance = new QRadioButton("最短距离",this);
    buttonShortestDistance->move(1120,150);
    buttonShortestDistance->setStyleSheet("QRadioButton::indicator { width: 15px; height: 15px; }""QRadioButton { font-size: 13px; }");

    lineEditDestinations = new QLineEdit(this);
    lineEditDestinations->setGeometry(length*3.5/14-8,width*3/19+5,length/2.5,width/17);
    lineEditDestinations->setFont(QFont("黑体",15));
    lineEditDestinations->setPlaceholderText("请输入多个目的地（用空格分隔）");
    lineEditDestinations->setClearButtonEnabled(true);


}
/*
QStringList Route_Strategy::pointToPointShortestTime(QString initialLocation,QString destination){

}

QStringList Route_Strategy::multiPointShortestTime(QString initialLocation, QStringList destinations){

}


QStringList Route_Strategy::pointToPointShortestDistance(QString initialLocation,QString destination){

}

QStringList Route_Strategy::multiPointShortestDistance(QString initialLocation, QStringList destinations){

}
*/
void Route_Strategy::paintEvent(QPaintEvent *){
    if(mode == 0)  //初始化界面时不进行绘制
        return;
    initialLocation = boxInitialLocation->currentText();  //起始位置
    destination = boxDestination->currentText();  //目的地(1个)
    destinations = (lineEditDestinations->text()).split(" ");  //目的地(多个)
    QStringList answer;
    QSqlQuery query;
/*
    query.exec("select * from t_road");
    while(query.next()){

    }
*/
    /*  这里等上面四个函数实现了才行，否则会引起程序崩溃
    if(buttonShortestTime->isChecked()){
        if(mode == 1)
            answer = pointToPointShortestTime(initialLocation, destination);
        else if(mode == 2)
            answer = multiPointShortestTime(initialLocation, destinations);
    }
    else if(buttonShortestDistance->isChecked()){
        if(mode == 1)
            answer = pointToPointShortestDistance(initialLocation, destination);
        else if(mode == 2)
            answer = multiPointShortestDistance(initialLocation, destinations);
    }
    */
    QPen pen(Qt::black);  //画笔颜色为黑色
    pen.setWidth(2);  //画笔宽度
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setFont(QFont("黑体", 30));
    painter.drawText(400,600,destination);
}

