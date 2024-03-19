#include <iostream>  
#include <vector>  
#include <string>  
#include <algorithm>  
#include <utility>  

#define NUMBER 10

struct Place {
    std::string name;
    int value;//景区或学校对应的热度值
};

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

//这个是函数1
std::vector<Place> PlaceSearch(std::string query, std::vector<Place> spots)
{
    int n = 2; // 生成2-grams  这个n可以更改，但目前来看n=2可以得到最优解

    if (empty(query))//如果输入为空串，打印所有
    {
        return spots;
    }
    //std::vector<Place> searchResults = fuzzySearchPlaces(query, spots, n);
    return fuzzySearchPlaces(query, spots, n);
}


int main() 
{
    //输入查找地点
    std::string query;
    std::getline(std::cin, query); // 从标准输入读取一行
    // 定义一个结构体数组  
    Place spots[NUMBER];//包含NUMBER个Place类型的元素，直接在头部更改宏定义

    //初始化结构体数组  ，这个是用来测试的数据，之后把这个替换成那200个数据即可
    spots[0].name = "北京环球影城";
    spots[0].value = 10;
    spots[1].name = "故宫";
    spots[1].value = 2;
    spots[2].name = "北京欢乐谷";
    spots[2].value = 21;
    spots[3].name = "北海公园";
    spots[3].value = 5;
    spots[4].name = "北京动物园";
    spots[4].value = 8;
    spots[5].name = "北京海洋馆";
    spots[5].value = 8;
    spots[6].name = "三里屯";
    spots[6].value = 8;
    spots[7].name = "圆明园";
    spots[7].value = 8;
    spots[8].name = "北京海底世界";
    spots[8].value = 8;
    spots[9].name = "颐和园";
    spots[9].value = 8;

    std::vector<Place> origin_Places;//把数组放进向量里，统一PlaceSearch函数返回值格式
    for (int i = 0; i < NUMBER; i++)
    {
        origin_Places.push_back(spots[i]);
    }
    std::vector<Place> SearchedPlaces = PlaceSearch(query, origin_Places);//调用函数1

    //输出排序后的结果
    for (const auto& result : SearchedPlaces) {
        std::cout << result.name << std::endl;
        std::cout << result.value << std::endl;
    }

    return 0;
}