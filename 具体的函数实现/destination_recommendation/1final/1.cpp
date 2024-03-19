#include <iostream>  
#include <vector>  
#include <string>  
#include <algorithm>  
#include <utility>  

#define NUMBER 10

struct Place {
    std::string name;
    int value;//������ѧУ��Ӧ���ȶ�ֵ
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

//����Ǻ���1
std::vector<Place> PlaceSearch(std::string query, std::vector<Place> spots)
{
    int n = 2; // ����2-grams  ���n���Ը��ģ���Ŀǰ����n=2���Եõ����Ž�

    if (empty(query))//�������Ϊ�մ�����ӡ����
    {
        return spots;
    }
    //std::vector<Place> searchResults = fuzzySearchPlaces(query, spots, n);
    return fuzzySearchPlaces(query, spots, n);
}


int main() 
{
    //������ҵص�
    std::string query;
    std::getline(std::cin, query); // �ӱ�׼�����ȡһ��
    // ����һ���ṹ������  
    Place spots[NUMBER];//����NUMBER��Place���͵�Ԫ�أ�ֱ����ͷ�����ĺ궨��

    //��ʼ���ṹ������  ��������������Ե����ݣ�֮�������滻����200�����ݼ���
    spots[0].name = "��������Ӱ��";
    spots[0].value = 10;
    spots[1].name = "�ʹ�";
    spots[1].value = 2;
    spots[2].name = "�������ֹ�";
    spots[2].value = 21;
    spots[3].name = "������԰";
    spots[3].value = 5;
    spots[4].name = "��������԰";
    spots[4].value = 8;
    spots[5].name = "���������";
    spots[5].value = 8;
    spots[6].name = "������";
    spots[6].value = 8;
    spots[7].name = "Բ��԰";
    spots[7].value = 8;
    spots[8].name = "������������";
    spots[8].value = 8;
    spots[9].name = "�ú�԰";
    spots[9].value = 8;

    std::vector<Place> origin_Places;//������Ž������ͳһPlaceSearch��������ֵ��ʽ
    for (int i = 0; i < NUMBER; i++)
    {
        origin_Places.push_back(spots[i]);
    }
    std::vector<Place> SearchedPlaces = PlaceSearch(query, origin_Places);//���ú���1

    //��������Ľ��
    for (const auto& result : SearchedPlaces) {
        std::cout << result.name << std::endl;
        std::cout << result.value << std::endl;
    }

    return 0;
}