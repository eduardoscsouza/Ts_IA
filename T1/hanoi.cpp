#include <vector>
#include <set>
#include <queue>
#include <map>
#include <tuple>
#include <iostream>
#include <functional>

#define INF 0x3f3f3f3f

using namespace std;



//checks if the state is final
bool isFinal (vector<int> estado, int n, int m) {
	for (int i = 0; i < n; i++)
		if (estado[i] != m-1) return false;

	return true;
}

//calculates the heuristic cost
int heur1 (vector <int> estado) {
	map <int, int> mapa;
	for (int i = 0; i < estado.size(); i++)
		mapa[estado[i]]++;

	int sum = 0;
	for (auto u : mapa)
		sum += (u.second - 1) * 2;

	return sum;
}

//executes A* to find shortest path to the state
void aStar (int n, int m, function<int(vector<int>)> heur) {
	map < vector<int>, pair<int, int> > pai;
	map < vector<int>, int > distancia;

	//initial state
	vector <int> estado_inicial;

	//min heap of pair <int, vector<int> >
	priority_queue < pair<int, vector<int> >, vector < pair<int, vector<int> > >, greater < pair<int, vector<int> > > > fila;

	//all the discs are at the 0 peg
	for (int i = 0; i < n; i++)
		estado_inicial.push_back(0);

	//puts at the top of the queue the initial state
	fila.emplace(0, estado_inicial);

	//distance from the initial state to it
	distancia[estado_inicial] = 0;

	//the initial state has no father
	pai[estado_inicial] = make_pair(-1, -1);

	while (!fila.empty()) {

		int d;
		vector <int> estado_atual;

		//getting the distace d from the initial state to the actual state
		tie (d, estado_atual) = fila.top();

		fila.pop();

		//checking if it is a final state
		if (isFinal(estado_atual, n, m)) {

			//number of necessary movements
			printf("Movimentos %d\n", distancia[estado_atual]);

			//printing the way to get to the final state
			while (pai[estado_atual] != make_pair(-1, -1)) {

				pair<int, int> par = pai[estado_atual];
				printf("sai da estaca %d entra na estaca %d\n", par.second, estado_atual[par.first]);
				estado_atual[par.first] = par.second;

			}
			//quitting the program once the final state has already been found
			exit(0);
		}

		//set of pegs
		set <int> estaca;

		for (int i = 0; i < n; i++) {

			//checking if the peg is in the set
			if (estaca.count(estado_atual[i]) == 0) {

				//inserting the peg into the set
				estaca.insert(estado_atual[i]);

				for (int j = 0; j < m; j++) {

					if (estaca.count(j))
						continue;

					//creating the new state
					vector <int> novo_estado = estado_atual;
					novo_estado[i] = j;

					if (distancia.count(novo_estado))
						continue;

					//calculting the distance of the new state
					distancia[novo_estado] = distancia[estado_atual] + 1;
					pai[novo_estado] = make_pair(i, estado_atual[i]);

					//inseting into the heap the distace + the preview of the future cost
					fila.emplace(distancia[novo_estado] + heur(novo_estado), novo_estado);
				}
			}
		}
	}
}

/*
void bfs (int n, int m) {
	map < vector<int>, pair<int, int> > pai;
	map < vector<int>, int > distancia;
	vector <int> estado_inicial;
	queue < vector<int> > fila;
	for (int i = 0; i < n; i++) 
		estado_inicial.push_back(0);
	fila.push(estado_inicial);
	distancia[estado_inicial] = 0;
	pai[estado_inicial] = mp(-1, -1);
	while (!fila.empty()) {
		vector <int> estado_atual = fila.front();
		fila.pop();
		if (ehFinal(estado_atual, n, m)) {
			printf("Movimentos %d\n", distancia[estado_atual]);
			while (pai[estado_atual] != mp(-1, -1)) {
				pii par = pai[estado_atual];
				printf("sai da estaca %d entra na estaca %d\n", par.se, estado_atual[par.fi]);
				estado_atual[par.fi] = par.se;
			}
			exit(0);
		}
		set <int> estaca;
		for (int i = 0; i < n; i++) {
			if (!estaca.count(estado_atual[i])) {
				estaca.insert(estado_atual[i]);
				for (int j = 0; j < m; j++) {
					if (estaca.count(j)) continue;
					vector <int> novo_estado = estado_atual;
					novo_estado[i] = j;
					if (distancia.count(novo_estado)) continue;
					distancia[novo_estado] = distancia[estado_atual] + 1;
					fila.push(novo_estado);
					pai[novo_estado] = mp(i, estado_atual[i]);
				}
			}
		}
	}
}*/



int main (int argc, char * argv[]) {

	//bfs(5, 3);
	aStar (5, 3, &heur1);

	return 0;
}
