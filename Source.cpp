#include"Header.h"

int get_ind(Graph g, int id) {
	for (int i = 0; i < g.Size; i++)
		if (g.array[i].ID == id) return i;
	return -1;
}

Graph Create_graph(int n, bool Directed) {
	Graph a;
	a.Directed = Directed;
	a.Size = n;
	a.array = new AdjList[n];

	for (int i = 0; i < n; i++) 
	{
		a.array[i].ID = -1;
		a.array[i].head = NULL;
	}
	return a;
}

bool add_node(Graph g, int u) {
	int i = 0;
	if (get_ind(g, u) != -1) return 1;
	for (i = 0; i < g.Size && g.array[i].ID != -1; i++);
	if (i == g.Size) return 0;
	g.array[i].ID = u;
	return 1;
}

bool Add_Edge(Graph G, int u, int v, int w) {
	int ind_u = get_ind(G,u);
	int ind_v = get_ind(G,v);
	if (ind_u == -1 || ind_v == -1) {
		cout << " error add_edge : u ou v n'existe pas\n ";
		return false;
	}

	AdjListNode* p = NULL;
	if (G.array[ind_u].head == NULL) {
		G.array[ind_u].head = new AdjListNode;
		G.array[ind_u].head->ID = v;
		G.array[ind_u].head->weight = w;
		G.array[ind_u].head->next = NULL;
	}	
	else {
		p = G.array[ind_u].head;
		if (p->ID == v) return 1;
		while (p->next != NULL)
		{
			if (p->next->ID == v) return 1;
			p = p->next;
		}
		p->next = new AdjListNode;
		p->next->ID = v;
		p->next->weight = w;
		p->next->next = NULL;
	}

	if (!G.Directed) {
		if (G.array[ind_v].head == NULL) {
			G.array[ind_v].head = new AdjListNode;
			G.array[ind_v].head->ID = u;
			G.array[ind_v].head->weight = w;
			G.array[ind_v].head->next = NULL;
		}
		else {
			p = G.array[ind_v].head;
			if (p->ID == v) return 1;
			while (p->next != NULL)
			{
				if (p->next->ID == u) return 1;
				p = p->next;
			}
			p->next = new AdjListNode;
			p->next->ID = u;
			p->next->weight = w;
			p->next->next = NULL;
		}
	}
}

bool IsEdge(Graph G, int u, int v) {
	int ind = get_ind(G, u);
	if (ind == -1) return 0;

	AdjListNode* p = G.array[ind].head;
	while (p != NULL) {
		if (p->ID == v) return 1;
		p = p->next;
	}
	return 0;
}

void afficherGraph(Graph g) {
	string str = "";
	if (g.Directed) str = "-->";
	else str = "<-->";
	for (int i = 0; (i < g.Size && g.array[i].ID != -1); i++) {
		AdjListNode* p = g.array[i].head;
		//if (p == NULL) cout << g.array[i].ID << endl;
		//else 
		while (p != NULL)
		{
			if(g.Directed || get_ind(g,p->ID) >= g.array[i].ID )cout << g.array[i].ID << " " + str + " " << p->ID << endl;
			p = p->next;
		}
	}
}

Graph Read_Graph_file(string FileName) {
	string str = FileName + ".txt";
	ifstream myfile(str);
	if (myfile) {
		string line;
		myfile >> line; myfile >> line; myfile >> line;
		int n = stoi(line);
		myfile >> line; myfile >> line;
		bool b = 0;
		if (line == "True") b = 1;
		Graph g = Create_graph(n, b);

		while (myfile >> line) {
			int u = stoi(line);
			myfile >> line;
			int v = stoi(line);

			add_node(g, u); add_node(g, v);
			Add_Edge(g, u, v, 1);
		}
		myfile.close();
		return g;
	}
	else {
		cout << "Unable to open file";
	}	
}

void Write_Graph_file(Graph G, string FileName) {
	string str = FileName + ".txt";
	ofstream myfile(str);
	if (myfile) {
		str = "False";
		if (G.Directed) str = "True";
		myfile << "Nbr Nodes: " + to_string( G.Size) + "\n";
		myfile << "Directed: " + str ;
		for (int i = 0; i < G.Size && G.array[i].ID != -1; i++) {
			AdjListNode* p = G.array[i].head;
			while (p != NULL)
			{
				if (G.Directed || get_ind(G, p->ID) >= G.array[i].ID)
					myfile <<"\n"+ to_string(G.array[i].ID) + " " + to_string(p->ID);
				p = p->next;
			}
		}
		myfile.close();
	}
	else {
		cout << "Unable to create file";
	}
}

int Node_degree(Graph G, int v) {
	int ind = get_ind(G, v);
	if (ind == -1) return -1;
	int deg = 0;
	AdjListNode* p = G.array[ind].head;
	while (p != NULL) {
		deg++;
		p = p->next;
	}
	return deg;
}

int* Node_Neighbords(Graph G, int v) {
	int ind = get_ind(G, v);
	if (ind == -1) return NULL;

	int n = Node_degree(G, v);
	if (n == 0) return 0;

	int* tab = new int[n];
	AdjListNode* p = G.array[ind].head;
	for (int i = 0; i < n;i++) {
		tab[i] = p->ID;
		p = p->next;
	}
	return tab;
}

int* Common_Neighbords(Graph G, int v, int u, float &Nbr) {
	int* t1 = Node_Neighbords(G, v);
	int* t2 = Node_Neighbords(G, u);
	Nbr = 0;
	if (t1 == NULL || t2 == NULL) return NULL;
	int n1 = Node_degree(G, v);
	int n2 = Node_degree(G, u);
	for (int i = 0; i < n1; i++) {
		bool b = 0;
		for (int j=0;j<n2;j++) 
			if (t2[j] == t1[i]) {
				b = 1;
				Nbr++;
				break;
			}
		if (!b) t1[i] = -1;
	}
	if (Nbr == 0) return NULL;
	int* tab = new int[Nbr];
	int j = 0;
	for (int i = 0; i < n1; i++) {
		if (t1[i] != -1) {
			tab[j] = t1[i];
			j++;
		}
	}
	return tab;
}

int** List2Mat(Graph G) {
	int n = G.Size;
	int** mat = new int* [n];
	for (int i = 0; i < n; i++) {
		mat[i] = new int[n];
		for (int j = 0; j < n; j++) mat[i][j] = 0;
	}

	for (int i = 0; i < n; i++) {
		if (G.array[i].ID == -1) break;
		AdjListNode* p = G.array[i].head;
		while (p != NULL) {
			mat[i][get_ind(G, p->ID)] = p->weight;
			p = p->next;
		}
	}
	return mat;
}

Graph Mat2List(int** mat,int n, bool directed) {
	Graph g = Create_graph(n, directed);
	for (int i = 0; i < n; i++) add_node(g, i);
	for (int i = 0; i < n; i++) 
		for (int j = i; j < n; j++)
			if (mat[i][j] != 0) Add_Edge(g, i, j, mat[i][j]);
	if (directed) {
		for (int i = 1; i < n; i++)
			for (int j = 0; j < i; j++)
				if (mat[i][j] != 0) Add_Edge(g, i, j, mat[i][j]);
	}
	return g;
}

int minDistance(int* dist,int n, bool* sptSet)
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < n; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

void printSolution(Graph g, int dist[], int n)
{
	printf("Vertex \t\t Distance from Source\n");
	for (int i = 0; i < n; i++)
		if (dist[i]==INT_MAX)
			printf("%d \t\t %s\n", g.array[i].ID, "infini");
		else 
			printf("%d \t\t %d\n", g.array[i].ID, dist[i]);
}

///// ----> graph directed -- > not directed <----- //////
Graph dir2Ndir(Graph g) {
	int n = g.Size;
	int** mat = List2Mat(g);
	for (int i = 0; i< n; i++) 
		for (int j = i; j < n; j++) {
			int x = max(mat[i][j], mat[j][i]);
			mat[i][j] = mat[j][i] = x;
		}
	Graph g1 = Create_graph(n, 0);

	for (int i = 0; i < n; i++) {
		add_node(g1, g.array[i].ID);
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (mat[i][j] != 0)
				Add_Edge(g1, g.array[i].ID, g.array[j].ID, 1);

	return g1;


	return g1;
}

Graph reverseg(Graph g) {
	int n = g.Size;
	int** mat = List2Mat(g);
	int** mat2 = new int* [n];
	for (int i = 0; i < n; i++) {
		mat2[i] = new int[n];
		for (int j = 0; j < n; j++) {
			mat2[i][j] = mat[j][i];
		}
	}
	Graph g1 = Create_graph(n,1);

	for (int i = 0; i < n; i++) {
		add_node(g1, g.array[i].ID);
	}
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (mat2[i][j] != 0)
				Add_Edge(g1, g.array[i].ID, g.array[j].ID, 1);

	return g1;
}

///// ----> graph complimentaire <----- //////
Graph graphComp(Graph g) {
	int n = g.Size;
	int** mat =List2Mat(g);
	for (int i=0;i<n;i++)
		for (int j = 0; j < n; j++)
		{
			if (i == j) mat[i][j] = 0;
			else 
				mat[i][j] == 0 ? mat[i][j] = 1 : mat[i][j] = 0;
		}
	return Mat2List(mat, n, g.Directed);
}


///// ----> graph densiter <----- //////

void  dens_graph(Graph g) {
	int n = g.Size;
	int** mat = List2Mat(g);
	float nbr=0;
	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			if (mat[i][j] != 0) nbr++;
	if (g.Directed) {
		for (int i = 1; i < n; i++)
			for (int j = 0; j < i; j++)
				if (mat[i][j] != 0) nbr++;
		nbr =  nbr / ((n - 1) * n);
	}
	else
		nbr =  ((2*nbr)/ ((n - 1) * n));
	cout << "\n la densite du graph g1 est :" << nbr;
}

void linkprediction(Graph g, int u, int v) {
	if (g.Directed == 1) {
		cout << " \nle graph est directed !!";
		return;
	}
	if (IsEdge(g, u, v)) {
		cout << "\n p( " << u << ", " << v << " )" << " = 1";
		return;
	}
	float total = Node_degree(g, u) + Node_degree(g, v);
	float n = 0;
	int* tab = Common_Neighbords(g, u, v, n);
	float res = n / (total-n);
	cout << "\n p( " << u << ", " << v << " )" << " = "<<res;
}

void shortest_path(Graph g,int u, int v) {
	if (get_ind(g, u) == -1 || get_ind(g, v) == -1) {
		cout << "error"; return;
	}
	int* src = dijkstra(g, u);
	string str = "--> " + to_string(v);
	int i = get_ind(g,v);
	while (i != get_ind(g, u)) {
		str = "--> " + to_string(g.array[src[i]].ID) +  str;
		i = src[i];
	}
	str = str;
	cout << "\n le plus court chemin entre " << u << " et " << v << " est : " << str;
}

///// ----> dijkstra <----- //////
int* dijkstra(Graph g, int src)
{
	if (get_ind(g, src) == -1) {
		cout << " error dijkstra : vertex n'exist in graph \n";
		return NULL;
	}
	int n = g.Size;
	int** mat = List2Mat(g);

	int* dist = new int [n]; 
	int* source = new int[n];
	bool* sptSet = new bool[n]; 

	// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < n; i++)
		dist[i] = INT_MAX, sptSet[i] = false;

	// Distance of source vertex from itself is always 0
	dist[get_ind(g,src)] = 0;
	source[get_ind(g, src)] = -1;
	// Find shortest path for all vertices
	for (int count = 0; count < n - 1; count++) {
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in the first iteration.
		int u = minDistance(dist,n, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update distance value of the adjacent vertices of the picked vertex.
		for (int v = 0; v < n; v++) {
			if (v == u) continue;
			if (!sptSet[v] && mat[u][v] && dist[u] + mat[u][v] < dist[v]) {
				dist[v] = dist[u] + mat[u][v];
				source[v] = u;
			}
				
				
		}
			
	}

	// print the constructed distance array
	printSolution(g,dist,n);
	return source;
}

