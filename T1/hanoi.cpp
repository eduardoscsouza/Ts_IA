#include <vector>
#include <queue>
#include <map>
#include <functional>

using namespace std;



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
}
*/



//calculates the heuristic cost
int heur_null (vector<int> estado) {
	return 0;
}

//calculates the heuristic cost
int heur1 (vector<int> estado) {
	map <int, int> mapa;
	for (int i = 0; i < estado.size(); i++)
		mapa[estado[i]]++;

	int sum = 0;
	for (auto u : mapa)
		sum += (u.second - 1) * 2;

	return sum;
}

//executes A* to find shortest path to the state
vector<pair<int, int> > aStar (int n, int m, function<int(vector<int>)> heur) {
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
	int dist;
	vector <int> estado_atual(n);
	vector <int> novo_estado(n);
	vector <bool> estaca(m);
	while (!fila.empty()) {
		//getting the distace d from the initial state to the current state
		estado_atual = fila.top().second;
		dist = distancia[estado_atual];
		fila.pop();

		//checking if it is a final state
		if (isFinal(estado_atual, n, m)) {
			//storing the way to get to the final state
			vector<pair<int, int> > sol(dist);
			while (pai[estado_atual] != make_pair(-1, -1)){
				pair<int, int> aux_pai = pai[estado_atual];
				sol[--dist] = make_pair(aux_pai.second, estado_atual[aux_pai.first]);
				estado_atual[aux_pai.first] = aux_pai.second;
			}

			return sol;
		}

		//vector of visited pegspegs
		for (int i = 0; i < n; i++)
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
					distancia[novo_estado] = dist + 1;
					pai[novo_estado] = make_pair(i, estado_atual[i]);

					//inseting into the heap the distace + the preview of the future cost
					fila.emplace(distancia[novo_estado] + heur(novo_estado), novo_estado);
				}
			}
		}
	}

	vector<pair<int, int> > sol(0);
	return sol;
}



void print_solution(vector<pair<int, int> > sol)
{	
	printf("%d movements\n", sol.size());
	for(int i=0; i<sol.size(); i++)
		printf("%d -> %d\n", sol[i].first, sol[i].second);
	printf("\n");
}



int main (int argc, char * argv[]) {

	vector<pair<int, int> > sol;
	deterministic(0, 2, 1, 5, sol);
	print_solution(sol);
	//bfs(5, 3);
	sol = aStar (5, 3, &heur1);
	print_solution(sol);

	return 0;
}
