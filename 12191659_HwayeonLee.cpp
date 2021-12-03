#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// graph의 각 vertex들을 나타냄.
class Node {
public:
	int ID;	// vertex의 ID(조형물 번호)
	vector <pair<Node*, int>> adjacency_list;	// 해당 vertex와 인접한 vertex의 list, 첫번째 원소에는 인접한 vertex, 두번째 원소에는 weight

	Node(int ID) {
		this->ID = ID;
	}
};

class Graph {
private:
	vector<Node*> node_list;	// graph 내의 모든 vertex의 list
public:
	// 새로운 vertex와 edge를 삽입하는 함수
	void insertNode(int ID1, int ID2, int weight) {
		Node* newnode1;
		Node* newnode2;

		// 입력된 번호의 vertex가 없다면 새로운 vertex를 만들어줌.
		if (findNode(ID1) == NULL) {
			newnode1 = new Node(ID1);
			node_list.push_back(newnode1);
		}
		// 입력된 번호의 vertx가 이미 존재한다면 그 vertex를 찾아 변수에 저장
		else
			newnode1 = findNode(ID1);


		// 위와 동일
		if (findNode(ID2) == NULL) {
			newnode2 = new Node(ID2);
			node_list.push_back(newnode2);
		}
		else
			newnode2 = findNode(ID2);

		// edge 연결, 인접한 vertex에 edge의 weight와 함께 추가시켜줌
		newnode1->adjacency_list.push_back(pair<Node*, int>(newnode2, weight)); 
		newnode2->adjacency_list.push_back(pair<Node*, int>(newnode1, weight));
	}

	// prim's algorithm
	void prim(int start) {
		vector<int> tree_list; // tree vertex의 list
		vector<pair<int, int>> fringe_list; // fringe vertex의 list, 우선순위 큐(unsorted sequence), 첫번째 원소는 vertex의 ID, 두번째 원소는 distance
		vector<int> unseen_list; // unseen vertex의 list

		Node* new_tree = NULL;	// 새로운 tree vertex를 나타내는 변수(인접한 vertex를 찾아 새로운 fringe vertex를 추가하기 위함)
		int distanceSum = 0;  // prim's algorithm을 시행하며 가장 작은 거리들의 합(산책로 설치에 들어가는 비용의 총합)
		string output = "";  // 차례로 거쳐가는 조형물 번호를 추가해주기 위한 string형 변수

		// 만약 해당 vertex가 시작 vertex라면 tree vertex에 추가해주고 나머지 vertex들은 모두 unseen vertex에 추가
		for (int i = 0; i < node_list.size(); i++) {
			if (node_list[i]->ID == start) {
				tree_list.push_back(node_list[i]->ID);
				new_tree = node_list[i];
				output += to_string(new_tree->ID);
				output += " ";
				continue;
			}
			unseen_list.push_back(node_list[i]->ID);
		}
		// 시작 vertex와 인접한 vertex들을 unseen vertex에서 fringe vertex로 바꾸어줌.
		for (int i = 0; i < unseen_list.size(); i++) {
			for (int j = 0; j < new_tree->adjacency_list.size(); j++) {
				if (unseen_list[i] == new_tree->adjacency_list[j].first->ID) {
					fringe_list.push_back(pair<int, int>(unseen_list[i], new_tree->adjacency_list[j].second)); // fringe vertex에 추가
					unseen_list.erase(unseen_list.begin() + i);  // unseen vertex에서 삭제
					i--;	// unseen 삭제 후 반복문을 위한 i의 값도 같이 줄여줌
					break;
				}
			}
		}

		// 모든 vertex가 tree vertex가 될 때까지 반복해줌
		while (!(tree_list.size() == node_list.size())) {
			// fringe vertex 중 가장 작은 distance를 가지는 vertex를 찾아줌. distance가 같다면 더 작은 ID(번호)를 가진 vertex를 찾아줌.
			int id = fringe_list[0].first;	// 위에서 설명한 vertex의 ID를 저장하는 변수
			int distance = fringe_list[0].second;  // 위에서 설명한 vertex의 distance를 저장하는 변수
			int idx = 0;  // 위에서 설명한 vertex의 fringe vertex에서의 index를 저장하는 변수(fringe vertex에서 삭제하기 위함)

			for (int i = 1; i < fringe_list.size(); i++) {
				// fringe vertex 중 가장 작은 distance를 가지는 vertex를 찾아줌.
				if (fringe_list[i].second < distance) {
					id = fringe_list[i].first;
					distance = fringe_list[i].second;
					idx = i;
				}
				// distance가 같다면 더 작은 ID(번호)를 가진 vertex를 찾아줌.
				else if ((fringe_list[i].second == distance) && (fringe_list[i].first < id)) {
					id = fringe_list[i].first;
					distance = fringe_list[i].second;
					idx = i;
				}
				else
					continue;
			}

			// 위에서 구한 vertex를 tree vertex에 추가
			tree_list.push_back(id);
			new_tree = findNode(id);
			output += to_string(new_tree->ID);
			output += " ";
			// 총 거리에 방금 추가된 distance 더해줌
			distanceSum += distance;
			// fringe vertex에서 해당 vertex 삭제
			fringe_list.erase(fringe_list.begin() + idx);

			// 새로 추가된 tree vertex와 인접한 vertex들을 unseen vertex에서 fringe vertex로 바꾸어줌.
			// 이 때 이미 fringe vertex에 추가되어 있는 vertex들은 값을 바꾸지 않음. (바로 아래 반복문에서 해당 작업 수행)
			for (int i = 0; i < unseen_list.size(); i++) {
				for (int j = 0; j < new_tree->adjacency_list.size(); j++) {
					if (unseen_list[i] == new_tree->adjacency_list[j].first->ID) {
						fringe_list.push_back(pair<int, int>(unseen_list[i], new_tree->adjacency_list[j].second)); // fringe vertex에 추가
						unseen_list.erase(unseen_list.begin() + i); // unseen vertex에서 삭제
						i--;	// unseen 삭제 후 반복문을 위한 i의 값도 같이 줄여줌
						break;
					}
				}
			}
			// 새로 추가된 tree vertex와 인접한 vertex들 중 이미 fringe vertex인 vertex에 한해서 distance를 비교하여 더 작은 값으로 업데이트시켜줌.
			for (int i = 0; i < fringe_list.size(); i++) {
				for (int j = 0; j < new_tree->adjacency_list.size(); j++) {
					// 새로운 distance가 더 작다면 값을 바꾸어주고 아니라면 원래의 distance 유지
					if ((fringe_list[i].first == new_tree->adjacency_list[j].first->ID) && (fringe_list[i].second > new_tree->adjacency_list[j].second)) {
						fringe_list[i].second = new_tree->adjacency_list[j].second;
						break;
					}
				}
			}
		}
		// prim's algorithm을 시행하며 가장 작은 distance 거리들의 합과 차례로 거쳐가는 조형물 번호를 출력해줌.
		cout << distanceSum << " " << output << "\n";
	}

	// 해당 ID를 가지는 vertex를 찾기 위한 함수
	Node* findNode(int ID) {
		Node* nownode = NULL;

		for (int i = 0; i < node_list.size(); i++) {
			if (node_list[i]->ID == ID) {
				nownode = node_list[i];
				return nownode;
			}
		}
		return nownode;
	}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	Graph graph = Graph();
	int n, m, q;
	char c;

	cin >> n >> m >> q;

	int ID1, ID2, weight;
	for (int i = 0; i < m; i++) {
		cin >> ID1 >> ID2 >> weight;
		graph.insertNode(ID1, ID2, weight);
	}
	int start;
	for (int i = 0; i < q; i++) {
		cin >> c;
		if (c == 'P') {
			cin >> start;
			graph.prim(start);
		}
	}
	
	return 0;
}