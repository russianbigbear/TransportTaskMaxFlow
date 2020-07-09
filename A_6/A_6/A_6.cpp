#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <list>
#include <locale>

using namespace std;

int N; // кол-во вершин
int count_lifts = 0; //число подъёмов
const int INF = 999; // бесконечность
int source, outlet; // источник и сток

int excess[INF]; //избыточное вещество(еденицы) / поток
int hight[INF]; //высота

/*операция проталкивания */
/*выполняется, когда выполнены условия
1) u - переполнена
2) ребро u->v приналжеит остаточной сети
3) вершина u выше v, тоесть h[u] = h[v] + 1*/
void push(int u, int v, int **graph)
{
	int f = min(excess[u], graph[u][v]); // число вещества для проталкивания, минимальное из избытка в u и сколько можно пустить ещё
	excess[u] -= f; excess[v] += f; // добавим поток в вершину v и отнимем его из вершины u
	graph[u][v] -= f; // уменьшаем излишек в вершине u
	graph[v][u] += f; // увеличиваем излишек в вершине v
}

/*операция поднятия */
/*выполняется, когда выполняются
1) u - переполнена
2) нельзя сделать push ни в какую вершину*/
void lift(int u, int **graph)
{
	count_lifts++; //увеличиваем число подъемов

	int min = 3 * N + 1;  // задание начального минимального вершины

	//находим соседа с минимальной высотой
	for (int i = 0; i < N; i++)
		if (graph[u][i] && (hight[i] < min))
			min = hight[i];
	hight[u] = min + 1; // присваиваем новое значение высоты
}

/*обслуживание вершины(снятие излишка)*/
void discharge(int u, int **graph)
{
	int v = 0;
	while (excess[u] > 0) // пока есть излишек либо операция push, либо lift
	{
		if (graph[u][v] && hight[u] == hight[v] + 1) //ребро есть в остаточной сети и его высота на 1 меньше
		{
			push(u, v, graph);
			v = 0;
			continue;
		}
		v++;
		if (v == N)  // выполнили операцию push для всех соседей, то lift
		{
			lift(u, graph);
			v = 0;
		}
	}
}

/*ввод графа из файла, заданного при помощи матрицы */
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}

/* печать графа c веом рёбер(вывод в graphic.png) */
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

/*инициализация транспортной сети*/
void init(int **graph)
{
	for (int i = 0; i < N; i++)
	{
		if (i == source) continue; // пропускаем исток
		excess[i] = graph[source][i]; //поток
		graph[i][source] += graph[source][i];
	}
	hight[source] = N; // высота истока равна количеству вершин
}

/*------------------------------------------------------------------------------------------------------------------------------*/

/*Главная функия*/
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "rus");
	ifstream ifs; ofstream ofs;

	list<int> l; // список потенциальных соседей
	list<int>::iterator cur; 
	int old;

	int ans;
	do {
		cout << "Редактировать файл ввода? (1-да, 0 -нет)." << endl;
		cin >> ans;
		if (ans == 1) system("input.txt");
	} while (ans != 1 && ans != 0);


	ifs.open("input.txt");
	ifs >> N >> source >> outlet; //вывод из файла количества вершин, истока и стока
	source -= 1; outlet -= 1;

	int **transport_network = new int*[N]; //зануляем матрицу
	for (int i = 0; i < N; i++)
	{
		transport_network[i] = new int[N];
		for (int j = 0; j < N; j++)
			transport_network[i][j] = 0;
	}
	input_matrix(ifs, transport_network);

	do {
		cout << "Вывести граф? (1-да, 0 -нет)." << endl;
		cin >> ans;
		if (ans == 1) print_graph(transport_network);
	} while (ans != 1 && ans != 0);

	ifs.close();

	init(transport_network); // инициализация

	/*Алгоритм проталкивания предпотока*/

	//заносим вершины не исток и сток в список
	for (int i = 0; i < N; i++)
		if (i != source && i != outlet)
			l.push_front(i);
	cur = l.begin();  //задаём начальную вершину

	while (cur != l.end())
	{
		old = hight[*cur]; //сохраняем высоту вершины перед осблуживанием
		discharge(*cur, transport_network); // обслуживаем вершину
		if (hight[*cur] != old) { // если высота была подята( высота увеличилась)
			l.push_front(*cur);
			l.erase(cur);
			cur = l.begin(); // отправляем эту вершину в начало списка
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
	ofs << "Максимальный поток транспортной сети равен: "  << excess[outlet] << endl; // вывод максимального потока( избыток вещества стоке)
	ofs << "Число подъёмов равно: " << count_lifts << endl; // вывод числа подъмов
	ofs.close();

	/*открытие результата*/
	do {
		cout << "Вывести результат? (1-да, вывести 0 -не выводить)." << endl;
		cin >> ans;
		if (ans == 1) system("output.txt");
		
	} while (ans != 1 && ans != 0);

	return 0;
}