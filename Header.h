#pragma once
#include <iostream>
#include<string>
#include<fstream>
#include<queue>
#include<stack>
using namespace std;

// Structure qui représente la liste d’adjacence pour un neoud

struct AdjListNode
{
	int ID;
	int weight;
	AdjListNode* next;
};
// Structure qui représente la liste d’adjacence du graph
struct AdjList
{
	int ID;
	AdjListNode* head;
};
// structure représente un graph.
// Un graphe est un tableau(liste chainer) de listes d’adjacence.
// La taille du tableau sera V (nombre de sommets dans le graphe)
// Un graphe peut être dirigé ou indirect
struct Graph
{
	bool Directed;
	int Size;
	AdjList* array;
};

Graph Create_graph(int n, bool Directed);
bool add_node(Graph g, int u);
bool Add_Edge(Graph G, int u, int v, int w);
void afficherGraph(Graph g);
Graph Read_Graph_file(string FileName);
void Write_Graph_file(Graph G, string FileName);
bool IsEdge(Graph G, int u, int v);
int Node_degree(Graph G, int v);
int* Node_Neighbords(Graph G, int v);
int* Common_Neighbords(Graph G, int v, int u, float &Nbr);
int** List2Mat(Graph G);
Graph Mat2List(int** mat, int n, bool directed);
int* dijkstra(Graph g, int src);
Graph graphComp(Graph g);
Graph dir2Ndir(Graph g);
void shortest_path(Graph g, int u, int v);
Graph reverseg(Graph g);
void  dens_graph(Graph g);
void linkprediction(Graph g, int u, int v);