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

using StringList = std::vector<std::string>;//vs里面没有Qstring类型，就先用一个字符串向量代替了，这里的类型转到Qt要修改一下

bool isValidNode(const std::string& node, const std::vector<RoadInfo>& roads) {// 检查输入的节点是否存在于道路信息中  
    for (const auto& road : roads) {
        if (road.start == node || road.end == node) {
            return true;
        }
    }
    return false;
}

std::pair<StringList, int> dijkstra(const std::string& start, const std::string& end, const std::vector<RoadInfo>& roads) {
    // 检查起点和终点是否存在于道路信息中  
    if (!isValidNode(start, roads) || !isValidNode(end, roads)) {
        throw std::invalid_argument("Invalid input: start or end node not found in the road information.");
    }

    // 初始化距离和前一个点映射  
    std::unordered_map<std::string, int> distances;
    std::unordered_map<std::string, std::string> prev;
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;

    // 为所有节点设置初始距离为正无穷大，除了起点  
    for (const auto& road : roads) {
        distances[road.start] = std::numeric_limits<int>::max();
        distances[road.end] = std::numeric_limits<int>::max();
    }
    distances[start] = 0;
    pq.emplace(0, start);

    // 主循环  
    while (!pq.empty()) {
        int currDist = pq.top().first;
        std::string currNode = pq.top().second;
        pq.pop();

        if (currDist > distances[currNode]) {
            continue; // 如果当前距离大于已知最短距离，则跳过  
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

    // 检查是否可达终点  
    if (distances[end] == std::numeric_limits<int>::max()) {
        throw std::runtime_error("No path found from start to end.");
    }

    // 获取最短距离  
    int totallength = distances[end];

    // 重建最短路径  
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

    // 检查输入的节点是否存在于道路信息中  
    validInput = isValidNode(start, roads) && isValidNode(end, roads);

    if (!validInput) {
        std::cout << "Invalid input: start or end node not found in the road information. Please try again." << std::endl;
        std::cin.clear(); // 清除错误状态标志  
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 忽略输入缓冲区的内容  
    }

    std::pair<StringList, int> result = dijkstra(start, end, roads);
    StringList path = result.first;
    totallength = result.second;

    return path;
}

int main() {
    //测试数据
    std::vector<RoadInfo> roads = {
    { "北京", "天津", 7 },
    { "北京", "南京", 9 },
    { "北京", "上海", 10 },
    { "上海", "南京", 15 },
    { "南京", "南极", 34 },
    { "北极", "南极", 9 },
    { "桂林", "西安", 13 },
    { "北京", "桂林", 22 },
    { "上海", "桂林", 31 },
    { "南极", "上海", 65 },
    { "西安", "北京", 3 },
    };

    //输入起点终点
    std::string start, end;
    std::cout << "Enter the start node: ";
    std::cin >> start;
    std::cout << "Enter the end node: ";
    std::cin >> end;


    try {
        int totallength;

        StringList path = pointToPointShortestDistance(start, end, roads, totallength);


        // 输出最短路径  测试用
        std::cout << "Shortest path from " << start << " to " << end << ": ";
        for (const auto& node : path) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
        std::cout << "总长度为: " << totallength << std::endl;
    }
    //两地点间无路线情况
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}