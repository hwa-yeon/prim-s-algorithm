#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// graph�� �� vertex���� ��Ÿ��.
class Node {
public:
	int ID;	// vertex�� ID(������ ��ȣ)
	vector <pair<Node*, int>> adjacency_list;	// �ش� vertex�� ������ vertex�� list, ù��° ���ҿ��� ������ vertex, �ι�° ���ҿ��� weight

	Node(int ID) {
		this->ID = ID;
	}
};

class Graph {
private:
	vector<Node*> node_list;	// graph ���� ��� vertex�� list
public:
	// ���ο� vertex�� edge�� �����ϴ� �Լ�
	void insertNode(int ID1, int ID2, int weight) {
		Node* newnode1;
		Node* newnode2;

		// �Էµ� ��ȣ�� vertex�� ���ٸ� ���ο� vertex�� �������.
		if (findNode(ID1) == NULL) {
			newnode1 = new Node(ID1);
			node_list.push_back(newnode1);
		}
		// �Էµ� ��ȣ�� vertx�� �̹� �����Ѵٸ� �� vertex�� ã�� ������ ����
		else
			newnode1 = findNode(ID1);


		// ���� ����
		if (findNode(ID2) == NULL) {
			newnode2 = new Node(ID2);
			node_list.push_back(newnode2);
		}
		else
			newnode2 = findNode(ID2);

		// edge ����, ������ vertex�� edge�� weight�� �Բ� �߰�������
		newnode1->adjacency_list.push_back(pair<Node*, int>(newnode2, weight)); 
		newnode2->adjacency_list.push_back(pair<Node*, int>(newnode1, weight));
	}

	// prim's algorithm
	void prim(int start) {
		vector<int> tree_list; // tree vertex�� list
		vector<pair<int, int>> fringe_list; // fringe vertex�� list, �켱���� ť(unsorted sequence), ù��° ���Ҵ� vertex�� ID, �ι�° ���Ҵ� distance
		vector<int> unseen_list; // unseen vertex�� list

		Node* new_tree = NULL;	// ���ο� tree vertex�� ��Ÿ���� ����(������ vertex�� ã�� ���ο� fringe vertex�� �߰��ϱ� ����)
		int distanceSum = 0;  // prim's algorithm�� �����ϸ� ���� ���� �Ÿ����� ��(��å�� ��ġ�� ���� ����� ����)
		string output = "";  // ���ʷ� ���İ��� ������ ��ȣ�� �߰����ֱ� ���� string�� ����

		// ���� �ش� vertex�� ���� vertex��� tree vertex�� �߰����ְ� ������ vertex���� ��� unseen vertex�� �߰�
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
		// ���� vertex�� ������ vertex���� unseen vertex���� fringe vertex�� �ٲپ���.
		for (int i = 0; i < unseen_list.size(); i++) {
			for (int j = 0; j < new_tree->adjacency_list.size(); j++) {
				if (unseen_list[i] == new_tree->adjacency_list[j].first->ID) {
					fringe_list.push_back(pair<int, int>(unseen_list[i], new_tree->adjacency_list[j].second)); // fringe vertex�� �߰�
					unseen_list.erase(unseen_list.begin() + i);  // unseen vertex���� ����
					i--;	// unseen ���� �� �ݺ����� ���� i�� ���� ���� �ٿ���
					break;
				}
			}
		}

		// ��� vertex�� tree vertex�� �� ������ �ݺ�����
		while (!(tree_list.size() == node_list.size())) {
			// fringe vertex �� ���� ���� distance�� ������ vertex�� ã����. distance�� ���ٸ� �� ���� ID(��ȣ)�� ���� vertex�� ã����.
			int id = fringe_list[0].first;	// ������ ������ vertex�� ID�� �����ϴ� ����
			int distance = fringe_list[0].second;  // ������ ������ vertex�� distance�� �����ϴ� ����
			int idx = 0;  // ������ ������ vertex�� fringe vertex������ index�� �����ϴ� ����(fringe vertex���� �����ϱ� ����)

			for (int i = 1; i < fringe_list.size(); i++) {
				// fringe vertex �� ���� ���� distance�� ������ vertex�� ã����.
				if (fringe_list[i].second < distance) {
					id = fringe_list[i].first;
					distance = fringe_list[i].second;
					idx = i;
				}
				// distance�� ���ٸ� �� ���� ID(��ȣ)�� ���� vertex�� ã����.
				else if ((fringe_list[i].second == distance) && (fringe_list[i].first < id)) {
					id = fringe_list[i].first;
					distance = fringe_list[i].second;
					idx = i;
				}
				else
					continue;
			}

			// ������ ���� vertex�� tree vertex�� �߰�
			tree_list.push_back(id);
			new_tree = findNode(id);
			output += to_string(new_tree->ID);
			output += " ";
			// �� �Ÿ��� ��� �߰��� distance ������
			distanceSum += distance;
			// fringe vertex���� �ش� vertex ����
			fringe_list.erase(fringe_list.begin() + idx);

			// ���� �߰��� tree vertex�� ������ vertex���� unseen vertex���� fringe vertex�� �ٲپ���.
			// �� �� �̹� fringe vertex�� �߰��Ǿ� �ִ� vertex���� ���� �ٲ��� ����. (�ٷ� �Ʒ� �ݺ������� �ش� �۾� ����)
			for (int i = 0; i < unseen_list.size(); i++) {
				for (int j = 0; j < new_tree->adjacency_list.size(); j++) {
					if (unseen_list[i] == new_tree->adjacency_list[j].first->ID) {
						fringe_list.push_back(pair<int, int>(unseen_list[i], new_tree->adjacency_list[j].second)); // fringe vertex�� �߰�
						unseen_list.erase(unseen_list.begin() + i); // unseen vertex���� ����
						i--;	// unseen ���� �� �ݺ����� ���� i�� ���� ���� �ٿ���
						break;
					}
				}
			}
			// ���� �߰��� tree vertex�� ������ vertex�� �� �̹� fringe vertex�� vertex�� ���ؼ� distance�� ���Ͽ� �� ���� ������ ������Ʈ������.
			for (int i = 0; i < fringe_list.size(); i++) {
				for (int j = 0; j < new_tree->adjacency_list.size(); j++) {
					// ���ο� distance�� �� �۴ٸ� ���� �ٲپ��ְ� �ƴ϶�� ������ distance ����
					if ((fringe_list[i].first == new_tree->adjacency_list[j].first->ID) && (fringe_list[i].second > new_tree->adjacency_list[j].second)) {
						fringe_list[i].second = new_tree->adjacency_list[j].second;
						break;
					}
				}
			}
		}
		// prim's algorithm�� �����ϸ� ���� ���� distance �Ÿ����� �հ� ���ʷ� ���İ��� ������ ��ȣ�� �������.
		cout << distanceSum << " " << output << "\n";
	}

	// �ش� ID�� ������ vertex�� ã�� ���� �Լ�
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