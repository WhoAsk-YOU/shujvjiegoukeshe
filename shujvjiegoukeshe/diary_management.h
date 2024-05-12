//游学日记管理界面
#ifndef DIARY_MANAGEMENT_H
#define DIARY_MANAGEMENT_H

#include "diary_write.h"
#include "diary_read.h"

class Diary_Management : public QWidget
{
    Q_OBJECT
public:
    Diary_Management(QString accountNumber, QString place);
    ~Diary_Management();

signals:
    void chooseback();  //返回信号

private slots:
    void showResult();  //展示查询结果
    void tableClicked(int row, int column);  //若点击了表格中的第一列，则进入读日记界面
    void clickWrite();  //进入写日记界面

private:
    int mode;  //若点击了目的地查询按钮,mode=1;若点击了日记名查询按钮,mode=2
    QString accountNumber;  //当前账户名
    QString place;  //当前所在景区/学校
    Diary_Write* diaryWrite = NULL;
    Diary_Read* diaryRead = NULL;
    QLabel* labelDestinationDM = NULL;  //目的地查找
    QLabel* labelDiaryNameDM = NULL;  //日记名查找
    QLabel* labelDiaryContentDM = NULL;  //全文检索
    QPushButton* buttonChoosebackDM = NULL;  //返回按钮
    QPushButton* buttonWriteDM = NULL;  //写日记按钮
    QPushButton* buttonQueryDestinationDM = NULL;  //查询按钮（目的地日记）
    QPushButton* buttonQueryDiaryNameDM = NULL;  //查询按钮（日记名）
    QPushButton* buttonQueryDiaryContentDM = NULL;  //查询按钮（日记内容）
    QRadioButton* buttonHeatValueDM = NULL;  //按热度排序
    QRadioButton* buttonGoodCommentsDM = NULL;  //按评价排序
    QComboBox* boxDestinationDM = NULL;  //目的地
    QLineEdit* lineEditDiaryNameDM = NULL;  //日记名输入框
    QLineEdit* lineEditDiaryContentDM = NULL;  //日记内容输入框
    QTableWidget* tableDiaryInfoDM = NULL;  //日记信息表格

    void initWidget();  //界面初始化函数

    vector<DiaryInfo> search(string keyword, vector<DiaryInfo> diaries);  //查找函数
    bool compareStrings(const string& str1, const string& str2);  //精确比较两个字符串是否相等
    bool containsSubstring(const string& str, const string& sub);  //子串包含检索
    vector<MatchedDiary> fuzzySearch(string keyword, vector<DiaryInfo> diaries);  //模糊查找

    void paintEvent(QPaintEvent*);  //重写paintEvent函数，用于在屏幕上打印出当前账户名及当前所在景区/学校
};

#endif // DIARY_MANAGEMENT_H
