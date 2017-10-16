#include <utility>
#include <vector>
#include <queue>
#include <map>
#include <functional>

#include <cstdio>
#include <ctime>

using namespace std;



//solves hanoi tower for 3 pegs with deterministic solution
void deterministic (int origin, int destiny, int aux, int quantity, vector<pair<int, int> >& sol) {
  if(quantity == 1) sol.push_back(make_pair(origin, destiny));
  else {
    deterministic(origin, aux, destiny, quantity-1, sol);
    deterministic(origin, destiny, aux, 1, sol);
    deterministic(aux, destiny, origin, quantity-1, sol);
  }
}



//checks if the state is final
bool isFinal (vector<int> estado, int n, int m) {
	for (int i = 0; i < n; i++)
		if (estado[i] != m-1) return false;

	return true;
}

//executes BFS to find shortest path to final the state
vector<pair<int, int> > bfs (int n, int m) {
	//maps to store movements tree and distances
	map < vector<int>, pair<int, int> > pai;
	map < vector<int>, int > distancia;

	//initial state, all pegs at 0
	vector <int> estado_inicial(n);
	for (int i = 0; i < n; i++)
		estado_inicial[i] = 0;
	distancia[estado_inicial] = 0;
	pai[estado_inicial] = make_pair(-1, -1);

	//queue for the algorithm
	queue < vector<int> > fila;
	fila.push(estado_inicial);

	vector <int> estado_atual(n);
	vector <int> novo_estado(n);
	vector <bool> estaca(m);
	while (!fila.empty()) {
		//getting the first element in the queue
		estado_atual = fila.front();
		fila.pop();

		//checking if it is a final state
		if (isFinal(estado_atual, n, m)) {
			//storing the way to get to the final state
			int dist = distancia[estado_atual];
			vector<pair<int, int> > sol(dist);
			pair<int, int> aux_end = make_pair(-1, -1);
			while (pai[estado_atual] != aux_end){
				pair<int, int> aux_pai = pai[estado_atual];
				sol[--dist] = make_pair(aux_pai.second, estado_atual[aux_pai.first]);
				estado_atual[aux_pai.first] = aux_pai.second;
			}

			return sol;
		}

		//vector of visited pegspegs
		for (int i = 0; i < m; i++)
			estaca[i] = false;

		for (int i = 0; i < n; i++) {
			//checking if the peg was already "used" by other disc
			if (!estaca[estado_atual[i]]) {
				estaca[estado_atual[i]] = true;
				for (int j = 0; j < m; j++) {
					//verifying if other iteration had already added movements coming out of this peg
					if (estaca[j])
						continue;

					//creating the new state
					novo_estado = estado_atual;
					novo_estado[i] = j;

					//verifying of it hadn't passed through this state already 
					if (distancia.count(novo_estado))
						continue;

					//calculting the distance of the new state
					distancia[novo_estado] = distancia[estado_atual] + 1;
					pai[novo_estado] = make_pair(i, estado_atual[i]);

					//inserting new state on queue
					fila.push(novo_estado);
				}
			}
		}
	}

	vector<pair<int, int> > sol(0);
	return sol;
}



//calculates the heuristic cost
int heurNull (vector<int> estado, int n, int m) {
	return 0;
}

//calculates the heuristic cost
int heur1 (vector<int> estado, int n, int m) {
	vector<int> aux_vect(m, 0);
	for (unsigned long i = 0; i < estado.size(); i++)
		aux_vect[estado[i]]++;

	int sum = 0;
	for (int i = 0; i < m - 1; i++)
		if (aux_vect[i])
			sum += (aux_vect[i] - 1) * 2;

	// Code runs faster without it        ????
	/* have to move all discs out of the last peg*/
/*	bool heavier_in_last = (estado[estado.size() - 1] == m - 1);
	if (!heavier_in_last and aux_vect[m-1])
		sum += (aux_vect[m - 1] - 1) * 2;
*/
	return sum;
}

//executes A* to find shortest path to the final state
vector<pair<int, int> > aStar (int n, int m, function<int(vector<int>, int, int)> heur) {
	//maps to store movements tree and distances
	map < vector<int>, pair<int, int> > pai;
	map < vector<int>, int > distancia;

	//initial state, all pegs at 0
	vector <int> estado_inicial(n);
	for (int i = 0; i < n; i++)
		estado_inicial[i] = 0;
	distancia[estado_inicial] = 0;
	pai[estado_inicial] = make_pair(-1, -1);

	//heap for the algorithm
	priority_queue < pair<int, vector<int> >, vector < pair<int, vector<int> > >, greater < pair<int, vector<int> > > > fila;
	fila.emplace(0, estado_inicial);

	//auxiliar variables
	vector <int> estado_atual(n);
	vector <int> novo_estado(n);
	vector <bool> estaca(m);
	while (!fila.empty()) {
		//getting the distace d from the initial state to the current state
		estado_atual = fila.top().second;
		fila.pop();

		//checking if it is a final state
		if (isFinal(estado_atual, n, m)) {
			//storing the way to get to the final state
			int dist = distancia[estado_atual];
			vector<pair<int, int> > sol(dist);
			pair<int, int> aux_end = make_pair(-1, -1);
			while (pai[estado_atual] != aux_end){
				pair<int, int> aux_pai = pai[estado_atual];
				sol[--dist] = make_pair(aux_pai.second, estado_atual[aux_pai.first]);
				estado_atual[aux_pai.first] = aux_pai.second;
			}

			return sol;
		}

		//vector of visited pegspegs
		for (int i = 0; i < m; i++)
			estaca[i] = false;

		for (int i = 0; i < n; i++) {
			//checking if the peg was already "used" by other disc
			if (!estaca[estado_atual[i]]) {
				estaca[estado_atual[i]] = true;
				for (int j = 0; j < m; j++) {
					//verifying if other iteration had already added movements coming out of this peg
					if (estaca[j])
						continue;

					//creating the new state
					novo_estado = estado_atual;
					novo_estado[i] = j;

					//verifying of it hadn't passed through this state already 
					if (distancia.count(novo_estado))
						continue;

					//calculting the distance of the new state
					distancia[novo_estado] = distancia[estado_atual] + 1;
					pai[novo_estado] = make_pair(i, estado_atual[i]);

					//inseting into the heap the distace + the preview of the future cost
					fila.emplace(distancia[novo_estado] + heur(novo_estado, n, m), novo_estado);
				}
			}
		}
	}

	vector<pair<int, int> > sol(0);
	return sol;
}



void printSolution(vector<pair<int, int> > sol)
{	
	printf("%lu movements\n", sol.size());
	for(unsigned i=0; i<sol.size(); i++)
		printf("%d -> %d\n", sol[i].first, sol[i].second);
	printf("\n");
}



int main (int argc, char * argv[]) {

	int n, m;
	vector<pair<int, int> > sol;
	clock_t time_diff;

	n = 10;
	m = 3;

	if (m==3) {
		time_diff = clock();
		deterministic(0, 2, 1, n, sol);
		time_diff = clock() - time_diff;
		printf ("Deterministic:\t\t");
		printf("%lf seconds\n", (double)time_diff/CLOCKS_PER_SEC);
		//printSolution(sol);
	}

	time_diff = clock();
	sol = bfs(n, m);
	time_diff = clock() - time_diff;
	printf ("Bfs:\t\t\t");
	printf("%lf seconds\n", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol);

	time_diff = clock();
	sol = aStar (n, m, &heurNull);
	time_diff = clock() - time_diff;
	printf ("aStar (heurNull):\t");
	printf("%lf seconds\n", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol);

	time_diff = clock();
	sol = aStar (n, m, &heur1);
	time_diff = clock() - time_diff;
	printf ("aStar (heur1):\t\t");
	printf("%lf seconds\n", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol);

	return 0;
}
