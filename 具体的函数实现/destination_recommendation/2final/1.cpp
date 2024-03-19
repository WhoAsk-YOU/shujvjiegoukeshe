#include <iostream>  
#include <vector>  
#include <string>  
#include <algorithm>  
#include <utility> 
#include <iterator> 
#define NUMBER 14

struct Place {
    std::string name;
    int value;//景区或学校对应的热度值
};


//................................................................................this is where func1 starts
//func1不用看，和上一个发的函数是完全一样的，只需要看func2即可

std::vector<std::string> generateNgrams(const std::string& str, int n) {
    std::vector<std::string> ngrams;
    for (size_t i = 0; i <= str.length() - n; ++i) {
        ngrams.push_back(str.substr(i, n));
    }
    return ngrams;
}

bool comparePlaceMatch(const Place& a, const Place& b, const std::vector<std::string>& queryNgrams) {
    std::vector<std::string> aNgrams = generateNgrams(a.name, 2);
    std::vector<std::string> bNgrams = generateNgrams(b.name, 2);

    int aMatches = 0;
    int bMatches = 0;
    for (const auto& queryNgram : queryNgrams) {
        if (std::find(aNgrams.begin(), aNgrams.end(), queryNgram) != aNgrams.end()) {
            ++aMatches;
        }
        if (std::find(bNgrams.begin(), bNgrams.end(), queryNgram) != bNgrams.end()) {
            ++bMatches;
        }
    }

    // 如果匹配数量相同，则按字母顺序排序  
    if (aMatches == bMatches) {
        return a.name < b.name;
    }

    // 返回匹配数量多的地名在前  
    return aMatches > bMatches;
}

std::vector<Place> fuzzySearchPlaces(const std::string& query, const std::vector<Place>& places, int n) {
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
        if (isMatch) {
            results.push_back(place);
        }
    }

    // 根据匹配程度对结果进行排序  
    std::sort(results.begin(), results.end(), [&](const Place& a, const Place& b) {
        return comparePlaceMatch(a, b, queryNgrams);
        });

    return results;
}


//.................................................................................this is where func1 ends
//.................................................................................this is where func2 starts

// 比较函数，用于根据value排序  
bool compareById(const Place& a, const Place& b) {
    return a.value > b.value;
}

// 返回前10个排序后的结构体的函数  
std::vector<Place> sort(const std::vector<Place>& placeNames) {
    std::vector<Place> sortedData(placeNames); // 拷贝数据以便排序  
    int size = sortedData.size();

    // 如果数据少于或等于10个，直接对整个向量进行排序  
    if (size <= 10) {
        std::sort(sortedData.begin(), sortedData.end(), compareById);
        return sortedData;
    }

    // 否则，仅对前10个元素进行排序  
    std::partial_sort(sortedData.begin(), sortedData.begin() + 10, sortedData.end(), compareById);

    // 返回前10个已排序的元素  
    return std::vector<Place>(sortedData.begin(), sortedData.begin() + 10);
}


//.................................................................................this is where func2 ends

int main()
{

    // 定义一个结构体数组  
    Place spots[NUMBER];//包含NUMBER个Place类型的元素

    // 初始化结构体数组  ，这个是用来测试的数据
    spots[0].name = "北京环球影城";
    spots[0].value = 10;
    spots[1].name = "故宫";
    spots[1].value = 2;
    spots[2].name = "北京欢乐谷";
    spots[2].value = 21;
    spots[3].name = "北海公园";
    spots[3].value = 5;
    spots[4].name = "北京动物园";
    spots[4].value = 13;
    spots[5].name = "北京海洋馆";
    spots[5].value = 24;
    spots[6].name = "三里屯";
    spots[6].value = 25;
    spots[7].name = "圆明园";
    spots[7].value = 17;
    spots[8].name = "北京海底世界";
    spots[8].value = 12;
    spots[9].name = "颐和园";
    spots[9].value = 7;
    spots[10].name = "人民大学附属中学";
    spots[10].value = 1;
    spots[11].name = "百望山";
    spots[11].value = 27;
    spots[12].name = "凤凰岭";
    spots[12].value = 9;
    spots[13].name = "中山公园";
    spots[13].value = 31;


    // 创建一个空的vector来存储地名  
    std::vector<Place> placeNames;

    // 遍历结构体数组，并将地名添加到vector中  
    for (int i = 0; i < NUMBER; i++)
    {
        placeNames.push_back(spots[i]);
    }

    //-----------------------------------------------------------------------func1

    int n = 2; // 生成2-grams  这个n可以更改，但目前来看n=2可以得到最优解

    //输入查找地点
    std::string query;
    std::getline(std::cin, query); // 从标准输入读取一行

    if (empty(query))//如果输入为空串，打印所有
    {
        int i;
        for (i = 0; i < NUMBER; i++)
        {
            std::cout << spots[i].name << '\n';
            std::cout << spots[i].value << '\n';
        }
    }
    else
    {
        std::vector<Place> searchResults = fuzzySearchPlaces(query, placeNames, n);

        // 输出排序后的结果  
        for (const auto& result : searchResults) {
            std::cout << result.name << std::endl;
            //std::cout << result.value << std::endl;
        }
    }

    //------------------------------------------------------------------func2 
    // 前面不用看， 内容和函数1的main部分是一样的

    std::vector<Place> topTen = sort(fuzzySearchPlaces(query, placeNames, n));

    //输出排序后的结果
    for (const auto& item : topTen) {
        std::cout << item.name << " " << item.value << std::endl;
    }



    return 0;
}