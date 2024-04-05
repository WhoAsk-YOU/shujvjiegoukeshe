//游学路线规划、场所查询、游学日记管理选择界面
#include "choose_widget.h"

Choose_Widget::Choose_Widget(QString accountNumber, QString place)//构造函数
{
    this->accountNumber = accountNumber;
    this->place = place;
    initWidget();
    connect(buttonChooseback, &QPushButton::clicked, [=]() {  //若点击返回按钮
        emit this->chooseback();  //发出返回目的地推荐界面的信号
    });
    connect(buttonRoute, &QPushButton::clicked, [=]() {  //若点击游学路线规划按钮
        clickRoute();
    });
    connect(buttonDiary, &QPushButton::clicked, [=]() {  //若点击游学日记管理按钮
        clickDiary();
    });
    connect(buttonSearchArchitect, &QPushButton::clicked, [=]() {  //若点击场所查询按钮
        clickSearchArchitect();
    });
}

Choose_Widget::~Choose_Widget() {  //析构函数
    delete buttonSearchArchitect;
    buttonSearchArchitect = NULL;
    delete buttonDiary;
    buttonDiary = NULL;
    delete buttonRoute;
    buttonRoute = NULL;
    delete buttonChooseback;
    buttonChooseback = NULL;
}

void Choose_Widget::clickSearchArchitect() {
    this->hide();
    searchArchitect = new Search_Architect;
    searchArchitect->setGeometry(this->geometry());
    searchArchitect->show();
    connect(searchArchitect, &Search_Architect::chooseback, [=]() {
        this->setGeometry(searchArchitect->geometry());
        this->show();
        delete searchArchitect;
        searchArchitect = NULL;
    });
}

void Choose_Widget::clickDiary() {
    this->hide();
    diaryManagement = new Diary_Management(accountNumber, place);
    diaryManagement->setGeometry(this->geometry());
    diaryManagement->show();
    connect(diaryManagement, &Diary_Management::chooseback, [=]() {
        this->setGeometry(diaryManagement->geometry());
        this->show();
        delete diaryManagement;
        diaryManagement = NULL;
    });
}

void Choose_Widget::clickRoute() {
    this->hide();
    routeStrategy = new Route_Strategy;
    routeStrategy->setGeometry(this->geometry());
    routeStrategy->show();
    connect(routeStrategy, &Route_Strategy::chooseback, [=]() {
        this->setGeometry(routeStrategy->geometry());
        this->show();
        delete routeStrategy;
        routeStrategy = NULL;
    });
}

void Choose_Widget::initWidget() {
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);

    buttonSearchArchitect = new QPushButton("场所查询", this);
    buttonSearchArchitect->move(LENGTH / 3, WIDTH / 4 - 50);
    buttonSearchArchitect->resize(LENGTH / 3, WIDTH / 9);
    buttonSearchArchitect->setFont(QFont("黑体", 25));
    buttonDiary = new QPushButton("游学日记管理", this);
    buttonDiary->move(LENGTH / 3, WIDTH / 2 - 50);
    buttonDiary->resize(LENGTH / 3, WIDTH / 9);
    buttonDiary->setFont(QFont("黑体", 25));
    buttonRoute = new QPushButton("游学路线规划", this);
    buttonRoute->move(LENGTH / 3, WIDTH * 3 / 4 - 50);
    buttonRoute->resize(LENGTH / 3, WIDTH / 9);
    buttonRoute->setFont(QFont("黑体", 25));
    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, WIDTH * 8 / 9);
    buttonChooseback->resize(LENGTH / 9, WIDTH / 9);
    buttonChooseback->setFont(QFont("黑体", 25));
}

void Choose_Widget::paintEvent(QPaintEvent*) {
    QPen pen(Qt::black);  //画笔颜色为黑色
    pen.setWidth(2);  //画笔宽度
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setFont(QFont("黑体", 25));
    painter.drawText(340,100, "当前账户名:" + accountNumber);
    painter.drawText(640,100, "当前所在景区/学校:" + place);
}
