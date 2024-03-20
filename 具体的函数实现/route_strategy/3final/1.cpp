#include <iostream>  
#include <vector>  
#include <string>  
#include <limits>  
#include <queue>  
#include <unordered_map>  
#include <stdexcept>  

struct RoadInfo {
    std::string start;
    std::string end;
    int length;
};

using StringList = std::vector<std::string>;//vs����û��Qstring���ͣ�������һ���ַ������������ˣ����������ת��QtҪ�޸�һ��

bool isValidNode(const std::string& node, const std::vector<RoadInfo>& roads) {// �������Ľڵ��Ƿ�����ڵ�·��Ϣ��  
    for (const auto& road : roads) {
        if (road.start == node || road.end == node) {
            return true;
        }
    }
    return false;
}

std::pair<StringList, int> dijkstra(const std::string& start, const std::string& end, const std::vector<RoadInfo>& roads) {
    // ��������յ��Ƿ�����ڵ�·��Ϣ��  
    if (!isValidNode(start, roads) || !isValidNode(end, roads)) {
        throw std::invalid_argument("Invalid input: start or end node not found in the road information.");
    }

    // ��ʼ�������ǰһ����ӳ��  
    std::unordered_map<std::string, int> distances;
    std::unordered_map<std::string, std::string> prev;
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;

    // Ϊ���нڵ����ó�ʼ����Ϊ������󣬳������  
    for (const auto& road : roads) {
        distances[road.start] = std::numeric_limits<int>::max();
        distances[road.end] = std::numeric_limits<int>::max();
    }
    distances[start] = 0;
    pq.emplace(0, start);

    // ��ѭ��  
    while (!pq.empty()) {
        int currDist = pq.top().first;
        std::string currNode = pq.top().second;
        pq.pop();

        if (currDist > distances[currNode]) {
            continue; // �����ǰ���������֪��̾��룬������  
        }

        for (const auto& road : roads) {
            if (road.start == currNode) {
                int alt = currDist + road.length;
                if (alt < distances[road.end]) {
                    distances[road.end] = alt;
                    prev[road.end] = road.start;
                    pq.emplace(alt, road.end);
                }
            }
        }
    }

    // ����Ƿ�ɴ��յ�  
    if (distances[end] == std::numeric_limits<int>::max()) {
        throw std::runtime_error("No path found from start to end.");
    }

    // ��ȡ��̾���  
    int totallength = distances[end];

    // �ؽ����·��  
    StringList path;
    std::string curr = end;
    while (curr != start) {
        path.insert(path.begin(), curr);
        curr = prev[curr];
    }
    path.insert(path.begin(), start);

    return { path, totallength };
}

StringList pointToPointShortestDistance(const std::string& start, const std::string& end, const std::vector<RoadInfo>& roads, int& totallength)
{
    bool validInput = false;

    // �������Ľڵ��Ƿ�����ڵ�·��Ϣ��  
    validInput = isValidNode(start, roads) && isValidNode(end, roads);

    if (!validInput) {
        std::cout << "Invalid input: start or end node not found in the road information. Please try again." << std::endl;
        std::cin.clear(); // �������״̬��־  
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // �������뻺����������  
    }

    std::pair<StringList, int> result = dijkstra(start, end, roads);
    StringList path = result.first;
    totallength = result.second;

    return path;
}

int main() {
    //��������
    std::vector<RoadInfo> roads = {
    { "����", "���", 7 },
    { "����", "�Ͼ�", 9 },
    { "����", "�Ϻ�", 10 },
    { "�Ϻ�", "�Ͼ�", 15 },
    { "�Ͼ�", "�ϼ�", 34 },
    { "����", "�ϼ�", 9 },
    { "����", "����", 13 },
    { "����", "����", 22 },
    { "�Ϻ�", "����", 31 },
    { "�ϼ�", "�Ϻ�", 65 },
    { "����", "����", 3 },
    };

    //��������յ�
    std::string start, end;
    std::cout << "Enter the start node: ";
    std::cin >> start;
    std::cout << "Enter the end node: ";
    std::cin >> end;


    try {
        int totallength;

        StringList path = pointToPointShortestDistance(start, end, roads, totallength);


        // ������·��  ������
        std::cout << "Shortest path from " << start << " to " << end << ": ";
        for (const auto& node : path) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
        std::cout << "�ܳ���Ϊ: " << totallength << std::endl;
    }
    //���ص����·�����
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}