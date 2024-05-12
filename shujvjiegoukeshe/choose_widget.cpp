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
    searchArchitect = new Search_Architect(place);
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
    routeStrategy = new Route_Strategy(place);
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
    buttonSearchArchitect->setStyleSheet("QPushButton{ background-color: #4a8af4; color: white; border-style: outset; border-width: 2px; border-radius: 10px; border-color: #1c5ecd; font-size: 25px; padding: 6px; }"
                                         "QPushButton:hover{ background-color: #1c5ecd; }"
                                         "QPushButton:pressed{ background-color: #062f6f; }");
    buttonDiary = new QPushButton("游学日记管理", this);
    buttonDiary->move(LENGTH / 3, WIDTH / 2 - 50);
    buttonDiary->resize(LENGTH / 3, WIDTH / 9);
    buttonDiary->setFont(QFont("黑体", 25));
    buttonDiary->setStyleSheet("QPushButton{ background-color: #4a8af4; color: white; border-style: outset; border-width: 2px; border-radius: 10px; border-color: #1c5ecd; font-size: 25px; padding: 6px; }"
                               "QPushButton:hover{ background-color: #1c5ecd; }"
                               "QPushButton:pressed{ background-color: #062f6f; }");
    buttonRoute = new QPushButton("游学路线规划", this);
    buttonRoute->move(LENGTH / 3, WIDTH * 3 / 4 - 50);
    buttonRoute->resize(LENGTH / 3, WIDTH / 9);
    buttonRoute->setFont(QFont("黑体", 25));
    buttonRoute->setStyleSheet("QPushButton{ background-color: #4a8af4; color: white; border-style: outset; border-width: 2px; border-radius: 10px; border-color: #1c5ecd; font-size: 25px; padding: 6px; }"
                               "QPushButton:hover{ background-color: #1c5ecd; }"
                               "QPushButton:pressed{ background-color: #062f6f; }");
    buttonChooseback = new QPushButton("返回", this);
    buttonChooseback->move(0, WIDTH * 17 / 18);
    buttonChooseback->resize(50, WIDTH / 18);
    buttonChooseback->setStyleSheet(
        "QPushButton {"
        "    background-color: #ff9444;" // 设置背景颜色为较浅的偏橙色
        "    border-style: outset;"       // 边框样式
        "    border-width: 2px;"          // 边框宽度
        "    border-radius: 10px;"        // 边框圆角
        "    border-color: #555555;"      // 边框颜色
        "    font: bold 25px;"             // 字体大小和粗细
        "    min-width: 3em;"             // 调整按钮的最小宽度
        "    padding: 4px;"               // 调整内边距
        "    color: white;"                // 字体颜色
        "}"
        "QPushButton:hover {"
        "    background-color: #ffa944;"  // 悬停时的背景颜色调整为更浅的偏橙色
        "}"
        "QPushButton:pressed {"
        "    background-color: #ff8c44;"  // 鼠标按下时的背景颜色调整为较深的偏橙色
        "    border-style: inset;"
        "}"
        );

}

void Choose_Widget::paintEvent(QPaintEvent*) {
    QPen pen(Qt::white);  //画笔颜色为白色
    pen.setWidth(0);  //画笔宽度
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/3.jpg");
    int windowWidth = this->width();
    int windowHeight = this->height();
    QPixmap scaledPix = pix.scaled(windowWidth, windowHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, scaledPix);
    painter.setRenderHint(QPainter::Antialiasing);  //设置抗锯齿能力，画面更清晰
    painter.setPen(pen);  //使用pen画图
    painter.setBrush(Qt::white);
    painter.setOpacity(0.5);
    painter.drawRect(400, 0, 700, 950);
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setOpacity(1);
    painter.setFont(QFont("黑体", 25));
    QRect textRect = painter.boundingRect(QRect(), Qt::TextSingleLine, "当前所在景区/学校:" + place);
    painter.drawText((LENGTH - textRect.width())/2, 100, "当前所在景区/学校:" + place);
}
