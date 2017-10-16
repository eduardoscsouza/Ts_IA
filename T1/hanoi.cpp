#include <utility>
#include <vector>
#include <queue>
#include <map>
#include <functional>

#include <cstdio>
#include <ctime>
#include <cstring>
#include <cassert>

using namespace std;



//solves hanoi tower for 3 pegs with recursive solution
void recursive (int origin, int destiny, int aux, int quantity, vector<pair<int, int> >& sol) {
  if(quantity == 1) sol.push_back(make_pair(origin, destiny));
  else {
    recursive(origin, aux, destiny, quantity-1, sol);
    recursive(origin, destiny, aux, 1, sol);
    recursive(aux, destiny, origin, quantity-1, sol);
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
	int sum = 0;

	for (unsigned long i = 0; i < estado.size(); i++) 
		aux_vect[estado[i]]++;

	for (int i = 0; i < m - 1; i++) 
			sum += max (0, (aux_vect[i] - 1) * 2 + 1);

	// runs faster without it        ????
	// have to move all discs out of the last peg
	for (int i = n - 1; i >= 0; i--) {
		if (estado[i] != m - 1) {
			sum += (aux_vect[m - 1]) * 2;
			return sum;
		}
		aux_vect[m-1]--;
	}

	return sum;
}

//calculates the heuristic cost
int heur_mis3 (vector<int> estado, int n, int m) {
	vector<int> aux_vect(m, 0);

	for (unsigned long i = 0; i < estado.size(); i++) 
		aux_vect[estado[i]]++;

	
	int sum = 0;
	if (aux_vect[0]) {
		sum += (1 << (aux_vect[0] - 1));
	} 

	sum += max (0, (aux_vect[1] - 1) * 2);

	return sum;
}

//calculates the heuristic cost
// not admissible
int heur2 (vector<int> estado, int n, int m) {
	vector<int> aux_vect(m, 0);
	for (unsigned long i = 0; i < estado.size(); i++)
		aux_vect[estado[i]]++;

	int sum = 0;
	for (int i = 0; i < m - 1; i++) {
		if (aux_vect[i] <= 1) {
			sum += aux_vect[i];
		} else {
			int val = 2;
			while (aux_vect[i]) {
				int quant = min (aux_vect[i], m - 1);
				sum += quant * val;
				aux_vect[i] -= quant;
				val++;
			}
		}
	}

	// runs faster without it        ????
	if ( estado[n - 1] == (m - 1) )	return sum;

	// have to move all discs out of the last peg
	int i = m - 1;
	if (aux_vect[i] <= 1) {
		sum += aux_vect[i];
	} else {
		int val = 2;
		while (aux_vect[i]) {
			int quant = min (aux_vect[i], m - 1);
			sum += quant * val;
			aux_vect[i] -= quant;
			val++;
		}
	}

	return sum;
}

// calculates the heuristic cost
// not admissible
int heur3 (vector<int> estado, int n, int m) {
	vector<int> aux_vect(m, 0);
	for (unsigned long i = 0; i < estado.size(); i++)
		aux_vect[estado[i]]++;

	int sum = 0;
	for (int i = 0; i < m - 1; i++) {
		if (aux_vect[i] <= 1) {
			sum += aux_vect[i];
		} else {
			int val = 2;
			while (aux_vect[i]) {
				int quant = min (aux_vect[i], m - 1);
				sum += quant * val;
				aux_vect[i] -= quant;
				val += 2;
			}
		}
	}

	return sum;
}

// calculates the heuristic cost
// idea from: https://pt.linkedin.com/pulse/an%C3%A1lise-de-algoritmos-busca-na-resolu%C3%A7%C3%A3o-da-torre-penido-maia
// not admissible
int heur4 (vector<int> estado, int n, int m) {
	vector<int> peso(m, 0);
	peso[m - 1] = 0;
	peso[m - 2] = 2;
	for (int i = m - 3; i >= 0; i--) 
		peso[i] = peso[i+1] * 2;

	int sum = 0;
	for (unsigned long i = 0; i < estado.size(); i++) 
		sum += (i + 1) * peso[estado[i]];
	
	return sum;
}

// calculates the heuristic cost
// heur3 + heur4
// not admissible
int heur5 (vector<int> estado, int n, int m) {
	vector<int> peso(m, 0), val(m, 1), quant(m, 0);
	peso[m - 1] = 0;
	peso[m - 2] = 2;
	for (int i = m - 3; i >= 0; i--) 
		peso[i] = peso[i+1] * 2;

	int sum = 0;
	for (unsigned long i = 0; i < estado.size(); i++) {
		sum += (i + 1) * peso[estado[i]] * val[estado[i]];
		quant[estado[i]]++;
		if (val[estado[i]] == 1) {
			quant[estado[i]] = 0;
			val[estado[i]] = 2;
		}
		if (quant[estado[i]] == m - 1) {
			quant[estado[i]] = 0;
			val[estado[i]] += 2;
		}
	}

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

void printSolution(vector<pair<int, int> > sol) {	
	printf("%lu movements\n", sol.size());
	for(unsigned i=0; i<sol.size(); i++)
		printf("%d -> %d\n", sol[i].first, sol[i].second);
	printf("\n");
}

int my_atoi (char *str) {
	int len = strlen(str);

	if (len > 5) {
		return -1;
	}

	int ret = 0;
	for (int i = 0; i < len; i++) {
		if (str[i] >= '0' and str[i] <= '9') {
			ret *= 10;
			ret += str[i] - '0';
		} else {
			return -1;
		}
	}

	return ret;
}

int main (int argc, char * argv[]) {

	int n, m;
	clock_t time_diff;

	if (argc != 3) {
		printf ("argv[1]->n, argv[2]->m\n");
		return 0;
	}

	n = atoi(argv[1]);
	m = atoi(argv[2]);

	if (n == -1 or m == -1) {
		printf ("Invalid args\n");
		return 0;
	}

	printf ("N: %d, M: %d\n", n, m);

	if (m < 3) {
		printf ("M should be >= 3\n");
		return 0;
	}


	vector<pair<int, int> > sol_recursive;
	if (m==3) {
		time_diff = clock();
		recursive(0, 2, 1, n, sol_recursive);
		time_diff = clock() - time_diff;
		printf ("\tRecursive:\t\t");
		printf("%lf seconds\n", (double)time_diff/CLOCKS_PER_SEC);
		//printSolution(sol_recursive);
	}

	time_diff = clock();
	vector<pair<int, int> > sol_bfs = bfs(n, m);
	time_diff = clock() - time_diff;
	printf ("\tBfs:\t\t\t");
	printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol_bfs);
	printf ("sol.size(): %d\n", (int)sol_bfs.size());

	printf ("\nAdmissible:\n\n");

	time_diff = clock();
	vector<pair<int, int> > sol_aStar_heurNull = aStar (n, m, &heurNull);
	time_diff = clock() - time_diff;
	printf ("\taStar (heurNull):\t");
	printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol_aStar_heurNull);
	printf ("sol.size(): %d\n", (int)sol_aStar_heurNull.size());

	time_diff = clock();
	vector<pair<int, int> > sol_aStar_heur1 = aStar (n, m, &heur1);
	time_diff = clock() - time_diff;
	printf ("\taStar (heur1):\t\t");
	printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol_aStar_heur1);
	printf ("sol.size(): %d\n", (int)sol_aStar_heur1.size());

	if (m==3) {
		time_diff = clock();
		vector<pair<int, int> > sol_aStar_heur_mis3 = aStar (n, m, &heur_mis3);
		time_diff = clock() - time_diff;
		printf ("\taStar (heur_mis3):\t");
		printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
		//printSolution(sol_aStar_heur_mis3);
		printf ("sol.size(): %d\n", (int)sol_aStar_heur_mis3.size());
	}

	printf ("\nNon-admissible:\n\n");

	time_diff = clock();
	vector<pair<int, int> > sol_aStar_heur2 = aStar (n, m, &heur2);
	time_diff = clock() - time_diff;
	printf ("\taStar (heur2):\t\t");
	printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol_aStar_heur2);
	printf ("sol.size(): %d\n", (int)sol_aStar_heur2.size());

	time_diff = clock();
	vector<pair<int, int> > sol_aStar_heur3 = aStar (n, m, &heur3);
	time_diff = clock() - time_diff;
	printf ("\taStar (heur3):\t\t");
	printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol_aStar_heur3);
	printf ("sol.size(): %d\n", (int)sol_aStar_heur3.size());

	time_diff = clock();
	vector<pair<int, int> > sol_aStar_heur4 = aStar (n, m, &heur4);
	time_diff = clock() - time_diff;
	printf ("\taStar (heur4):\t\t");
	printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol_aStar_heur4);
	printf ("sol.size(): %d\n", (int)sol_aStar_heur4.size());

	time_diff = clock();
	vector<pair<int, int> > sol_aStar_heur5 = aStar (n, m, &heur5);
	time_diff = clock() - time_diff;
	printf ("\taStar (heur5):\t\t");
	printf("%lf seconds\t", (double)time_diff/CLOCKS_PER_SEC);
	//printSolution(sol_aStar_heur5);
	printf ("sol.size(): %d\n", (int)sol_aStar_heur5.size());
	
	printf ("\n");

	return 0;
}
