//开始界面
#include "start_widget.h"
#include "ui_start_widget.h"

Start_widget::Start_widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Start_widget)
{
    ui->setupUi(this);
    initWidget();
    connect(buttonLogOn, &QPushButton::clicked, [=]() {  //若点击登录按钮
        logOn();  //登录
    });
    connect(buttonSignIn, &QPushButton::clicked, [=]() {  //若点击注册按钮
        signIn();  //注册
    });
}

Start_widget::~Start_widget() {  //析构函数，释放空间
    delete labelTitle;
    labelTitle = NULL;
    delete buttonLogOn;
    buttonLogOn = NULL;
    delete buttonSignIn;
    buttonSignIn = NULL;
    delete lineEditAccountNumber;
    lineEditAccountNumber = NULL;
    delete lineEditPassword;
    lineEditPassword = NULL;
    delete ui;
}

void Start_widget::logOn() {  //登录
    QString accountNumber = lineEditAccountNumber->text();  //从输入框中获取账户名
    QString password = lineEditPassword->text();  //从输入框中获取密码
    QSqlQuery query;
    if (accountNumber == "") {  //感谢ljc同学的智力贡献，添加了账号和密码不能为空的判断
        QMessageBox::information(this, "登录失败", "登录失败，账号不能为空");  //弹出对话框
        return;
    }
    if (password == "") {
        QMessageBox::information(this, "登录失败", "登录失败，密码不能为空");  //弹出对话框
        return;
    }
    havingSignIn = false;
    query.exec("select account_number,password from t_user");  //从数据库中查询所有的账户名及密码
    while (query.next() && !havingSignIn) {  //记录未遍历完且还未在数据库中匹配到输入的账号
        if (accountNumber == query.value("account_number")) {  //账号匹配
            havingSignIn = true;
            if (password == query.value("password")) {  //密码正确
                query.clear();  //清空数据集
                this->hide();  //隐藏开始界面
                destinationRecommendation = new Destination_Recommendation(accountNumber);  //创建目的地推荐界面的对象
                destinationRecommendation->setGeometry(this->geometry());  //界面位置不会变化
                destinationRecommendation->show();  //显示目的地推荐界面
                connect(destinationRecommendation, &Destination_Recommendation::chooseback, [=]() {  //若在目的地推荐界面点击了返回按钮，接受返回信号
                    this->setGeometry(destinationRecommendation->geometry());
                    this->show();  //重新显示开始界面
                    delete destinationRecommendation;  //销毁目的地推荐界面
                    destinationRecommendation = NULL;
                });
            }
            else  //密码错误
                QMessageBox::information(this, "登陆失败", "密码错误，请重新输入");  //弹出对话框
        }
    }
    if (!havingSignIn)  //未注册过
        QMessageBox::information(this, "登陆失败", "该账号未注册，请先注册");
}

void Start_widget::signIn() {  //注册
    QString accountNumber = lineEditAccountNumber->text();
    QString password = lineEditPassword->text();
    QSqlQuery query;
    if (accountNumber == "") {  //感谢ljc同学的智力贡献，添加了账号和密码不能为空的判断
        QMessageBox::information(this, "注册失败", "注册失败，账号不能为空");  //弹出对话框
        return;
    }
    if (password == "") {
        QMessageBox::information(this, "注册失败", "注册失败，密码不能为空");  //弹出对话框
        return;
    }
    havingSignIn = false;  //是否注册过，即数据库里是否有输入的账号
    query.exec("select account_number from t_user");  //从数据库中查询所有的账户名
    while (query.next() && !havingSignIn) {
        if (accountNumber == query.value("account_number")) {  //数据库中存在账号
            havingSignIn = true;
            QMessageBox::information(this, "注册失败", "注册失败，该账号已注册");
        }
    }
    if (!havingSignIn) {  //若未注册过
        query.prepare("insert into t_user(account_number,password) values(:account_number,:password);");
        query.bindValue(":account_number", accountNumber);
        query.bindValue(":password", password);
        query.exec();  //向数据库中添加账户和密码信息
        QMessageBox::information(this, "注册成功", "注册成功，请登录");
    }
}

void Start_widget::initWidget() {  //初始化开始界面
    setWindowTitle("学生游学系统");
    setFixedSize(LENGTH, WIDTH);
    labelTitle = new QLabel("学生游学系统", this);
    labelTitle->setGeometry(LENGTH / 3, WIDTH / 17, LENGTH / 3, WIDTH / 4);  //设置文本框位置及大小
    labelTitle->setAlignment(Qt::AlignCenter);  //文本框内容居中显示
    labelTitle->setStyleSheet("QLabel{ color: white; font-size: 50px; font-weight: bold; border: none; background: transparent; }");
    buttonLogOn = new QPushButton("登录", this);
    buttonLogOn->move(LENGTH / 3, WIDTH * 5 / 7);  //改变按钮位置
    buttonLogOn->resize(LENGTH / 9, WIDTH / 9);  //改变按钮大小
    buttonLogOn->setFont(QFont("黑体", 30));  //设置按钮字体
    buttonLogOn->setStyleSheet("QPushButton{ background-color: #4a8af4; color: white; border-style: outset; border-width: 2px; border-radius: 10px; border-color: #1c5ecd; font-size: 25px; padding: 6px; }"
                               "QPushButton:hover{ background-color: #1c5ecd; }"
                               "QPushButton:pressed{ background-color: #062f6f; }");
    buttonSignIn = new QPushButton("注册", this);
    buttonSignIn->move(LENGTH - LENGTH / 3 - LENGTH / 9, WIDTH * 5 / 7);
    buttonSignIn->resize(LENGTH / 9, WIDTH / 9);
    buttonSignIn->setFont(QFont("黑体", 30));
    buttonSignIn->setStyleSheet("QPushButton{ background-color: #4a8af4; color: white; border-style: outset; border-width: 2px; border-radius: 10px; border-color: #1c5ecd; font-size: 25px; padding: 6px; }"
                                "QPushButton:hover{ background-color: #1c5ecd; }"
                                "QPushButton:pressed{ background-color: #062f6f; }");
    lineEditAccountNumber = new QLineEdit(this);
    lineEditAccountNumber->setGeometry(LENGTH / 2 - LENGTH / 7, WIDTH * 7 / 19, LENGTH / 3.5, WIDTH / 17);  //设置输入框位置及大小
    lineEditAccountNumber->setFont(QFont("黑体", 15));  //设置输入框字体
    lineEditAccountNumber->setPlaceholderText("请输入账号");  //设置输入框默认显示内容
    lineEditAccountNumber->setClearButtonEnabled(true);  //一键清除功能开启
    lineEditAccountNumber->setMaxLength(10);  //账号最多10位，超过了输不进去
    lineEditAccountNumber->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d+"), this));  //账号只能由数字组成，其他的输不进去
    lineEditAccountNumber->setStyleSheet("QLineEdit{ background-color: white; border-style: solid; border-width: 2px; border-radius: 8px; border-color: #1c5ecd; font-size: 15px; padding: 6px; }");
    lineEditPassword = new QLineEdit(this);
    lineEditPassword->setGeometry(LENGTH / 2 - LENGTH / 7, WIDTH * 10 / 19, LENGTH / 3.5, WIDTH / 17);
    lineEditPassword->setFont(QFont("黑体", 15));
    lineEditPassword->setPlaceholderText("请输入密码");
    lineEditPassword->setClearButtonEnabled(true);
    lineEditPassword->setEchoMode(QLineEdit::Password);
    lineEditPassword->setMaxLength(15);  //密码最多15位，超过了输不进去
    lineEditPassword->setStyleSheet("QLineEdit{ background-color: white; border-style: solid; border-width: 2px; border-radius: 8px; border-color: #1c5ecd; font-size: 15px; padding: 6px; }");

}


void Start_widget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/resource/1.jpeg");
    int windowWidth = this->width();
    int windowHeight = this->height();
    QPixmap scaledPix = pix.scaled(windowWidth, windowHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, scaledPix);
}
