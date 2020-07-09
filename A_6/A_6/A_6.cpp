#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <locale>

using namespace std;

int N; // ���-�� ������
int count_lifts = 0; //����� ��������
const int INF = 999; // �������������
int source, outlet; // �������� � ����

int excess[INF]; //���������� ��������(�������) / �����
int hight[INF]; //������

/*�������� ������������� */
/*�����������, ����� ��������� �������
1) u - �����������
2) ����� u->v ���������� ���������� ����
3) ������� u ���� v, ������ h[u] = h[v] + 1*/
void push(int u, int v, int **graph)
{
	int f = min(excess[u], graph[u][v]); // ����� �������� ��� �������������, ����������� �� ������� � u � ������� ����� ������� ���
	excess[u] -= f; excess[v] += f; // ������� ����� � ������� v � ������� ��� �� ������� u
	graph[u][v] -= f; // ��������� ������� � ������� u
	graph[v][u] += f; // ����������� ������� � ������� v
}

/*�������� �������� */
/*�����������, ����� �����������
1) u - �����������
2) ������ ������� push �� � ����� �������*/
void lift(int u, int **graph)
{
	count_lifts++; //����������� ����� ��������

	int min = 3 * N + 1;  // ������� ���������� ������������ �������

	//������� ������ � ����������� �������
	for (int i = 0; i < N; i++)
		if (graph[u][i] && (hight[i] < min))
			min = hight[i];
	hight[u] = min + 1; // ����������� ����� �������� ������
}

/*������������ �������(������ �������)*/
void discharge(int u, int **graph)
{
	int v = 0;
	while (excess[u] > 0) // ���� ���� ������� ���� �������� push, ���� lift
	{
		if (graph[u][v] && hight[u] == hight[v] + 1) //����� ���� � ���������� ���� � ��� ������ �� 1 ������
		{
			push(u, v, graph);
			v = 0;
			continue;
		}
		v++;
		if (v == N)  // ��������� �������� push ��� ���� �������, �� lift
		{
			lift(u, graph);
			v = 0;
		}
	}
}

/*���� ����� �� �����, ��������� ��� ������ ������� */
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}

/* ������ ����� c ���� ����(����� � graphic.png) */
void print_graph(int **graph) {
	ofstream out;
	out.open("graphic.dot");
	out << "digraph G{" << "\n";
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j)
				out << i + 1 << " -> " << j + 1 << "[label = \" " << graph[i][j] << "\"];\n";
	out << "}";
	out.close();
	system("dot -Tpng graphic.dot -o graphic.png ");
	system("graphic.png");
}

/*������������� ������������ ����*/
void init(int **graph)
{
	for (int i = 0; i < N; i++)
	{
		if (i == source) continue; // ���������� �����
		excess[i] = graph[source][i]; //�����
		graph[i][source] += graph[source][i];
	}
	hight[source] = N; // ������ ������ ����� ���������� ������
}

/*------------------------------------------------------------------------------------------------------------------------------*/

/*������� ������*/
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "rus");
	ifstream ifs; ofstream ofs;

	list<int> l; // ������ ������������� �������
	list<int>::iterator cur; 
	int old;

	int ans;
	do {
		cout << "������������� ���� �����? (1-��, 0 -���)." << endl;
		cin >> ans;
		if (ans == 1) system("input.txt");
	} while (ans != 1 && ans != 0);


	ifs.open("input.txt");
	ifs >> N >> source >> outlet; //����� �� ����� ���������� ������, ������ � �����
	source -= 1; outlet -= 1;

	int **transport_network = new int*[N]; //�������� �������
	for (int i = 0; i < N; i++)
	{
		transport_network[i] = new int[N];
		for (int j = 0; j < N; j++)
			transport_network[i][j] = 0;
	}
	input_matrix(ifs, transport_network);

	do {
		cout << "������� ����? (1-��, 0 -���)." << endl;
		cin >> ans;
		if (ans == 1) print_graph(transport_network);
	} while (ans != 1 && ans != 0);

	ifs.close();

	init(transport_network); // �������������

	/*�������� ������������� ����������*/

	//������� ������� �� ����� � ���� � ������
	for (int i = 0; i < N; i++)
		if (i != source && i != outlet)
			l.push_front(i);
	cur = l.begin();  //����� ��������� �������

	while (cur != l.end())
	{
		old = hight[*cur]; //��������� ������ ������� ����� �������������
		discharge(*cur, transport_network); // ����������� �������
		if (hight[*cur] != old) { // ���� ������ ���� ������( ������ �����������)
			l.push_front(*cur);
			l.erase(cur);
			cur = l.begin(); // ���������� ��� ������� � ������ ������
		}
		cur++;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << transport_network[i][j] << " ";
		cout << endl;
	}
	print_graph(transport_network);

	ofs.open("output.txt");
	ofs << "������������ ����� ������������ ���� �����: "  << excess[outlet] << endl; // ����� ������������� ������( ������� �������� �����)
	ofs << "����� �������� �����: " << count_lifts << endl; // ����� ����� �������
	ofs.close();

	/*�������� ����������*/
	do {
		cout << "������� ���������? (1-��, ������� 0 -�� ��������)." << endl;
		cin >> ans;
		if (ans == 1) system("output.txt");
		
	} while (ans != 1 && ans != 0);

	return 0;
}