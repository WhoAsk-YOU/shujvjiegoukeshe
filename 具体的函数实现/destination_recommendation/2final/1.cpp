#include <iostream>  
#include <vector>  
#include <string>  
#include <algorithm>  
#include <utility> 
#include <iterator> 
#define NUMBER 14

struct Place {
    std::string name;
    int value;//������ѧУ��Ӧ���ȶ�ֵ
};


//................................................................................this is where func1 starts
//func1���ÿ�������һ�����ĺ�������ȫһ���ģ�ֻ��Ҫ��func2����

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

    // ���ƥ��������ͬ������ĸ˳������  
    if (aMatches == bMatches) {
        return a.name < b.name;
    }

    // ����ƥ��������ĵ�����ǰ  
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

    // ����ƥ��̶ȶԽ����������  
    std::sort(results.begin(), results.end(), [&](const Place& a, const Place& b) {
        return comparePlaceMatch(a, b, queryNgrams);
        });

    return results;
}


//.................................................................................this is where func1 ends
//.................................................................................this is where func2 starts

// �ȽϺ��������ڸ���value����  
bool compareById(const Place& a, const Place& b) {
    return a.value > b.value;
}

// ����ǰ10�������Ľṹ��ĺ���  
std::vector<Place> sort(const std::vector<Place>& placeNames) {
    std::vector<Place> sortedData(placeNames); // ���������Ա�����  
    int size = sortedData.size();

    // ����������ڻ����10����ֱ�Ӷ�����������������  
    if (size <= 10) {
        std::sort(sortedData.begin(), sortedData.end(), compareById);
        return sortedData;
    }

    // ���򣬽���ǰ10��Ԫ�ؽ�������  
    std::partial_sort(sortedData.begin(), sortedData.begin() + 10, sortedData.end(), compareById);

    // ����ǰ10���������Ԫ��  
    return std::vector<Place>(sortedData.begin(), sortedData.begin() + 10);
}


//.................................................................................this is where func2 ends

int main()
{

    // ����һ���ṹ������  
    Place spots[NUMBER];//����NUMBER��Place���͵�Ԫ��

    // ��ʼ���ṹ������  ��������������Ե�����
    spots[0].name = "��������Ӱ��";
    spots[0].value = 10;
    spots[1].name = "�ʹ�";
    spots[1].value = 2;
    spots[2].name = "�������ֹ�";
    spots[2].value = 21;
    spots[3].name = "������԰";
    spots[3].value = 5;
    spots[4].name = "��������԰";
    spots[4].value = 13;
    spots[5].name = "���������";
    spots[5].value = 24;
    spots[6].name = "������";
    spots[6].value = 25;
    spots[7].name = "Բ��԰";
    spots[7].value = 17;
    spots[8].name = "������������";
    spots[8].value = 12;
    spots[9].name = "�ú�԰";
    spots[9].value = 7;
    spots[10].name = "�����ѧ������ѧ";
    spots[10].value = 1;
    spots[11].name = "����ɽ";
    spots[11].value = 27;
    spots[12].name = "�����";
    spots[12].value = 9;
    spots[13].name = "��ɽ��԰";
    spots[13].value = 31;


    // ����һ���յ�vector���洢����  
    std::vector<Place> placeNames;

    // �����ṹ�����飬����������ӵ�vector��  
    for (int i = 0; i < NUMBER; i++)
    {
        placeNames.push_back(spots[i]);
    }

    //-----------------------------------------------------------------------func1

    int n = 2; // ����2-grams  ���n���Ը��ģ���Ŀǰ����n=2���Եõ����Ž�

    //������ҵص�
    std::string query;
    std::getline(std::cin, query); // �ӱ�׼�����ȡһ��

    if (empty(query))//�������Ϊ�մ�����ӡ����
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

        // ��������Ľ��  
        for (const auto& result : searchResults) {
            std::cout << result.name << std::endl;
            //std::cout << result.value << std::endl;
        }
    }

    //------------------------------------------------------------------func2 
    // ǰ�治�ÿ��� ���ݺͺ���1��main������һ����

    std::vector<Place> topTen = sort(fuzzySearchPlaces(query, placeNames, n));

    //��������Ľ��
    for (const auto& item : topTen) {
        std::cout << item.name << " " << item.value << std::endl;
    }



    return 0;
}