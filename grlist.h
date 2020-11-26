#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define vertex int
static int visit[1000];
int topo[1000];
 
typedef struct graph *Graph;
typedef struct node *link;

Graph GRAPHinit(int V); //Cria o grafo
Graph GRAPHrand1(int V, int A); //Cria um grafo aleatorio
Graph GRAPHrand2(int V, int A); //Tambem cria um grafo aleatorio
Graph GRAPHbuildPath(int V); //Cria um grafo caminho
Graph GRAPHbuildComplete(int V); //Cria um grafo completo
Graph GRAPHbuildTour(int V); //Cria um grafo torneio
Graph GRAPHbuildCycle(int V); //Cria um grafo caminho
Graph GRAPHbuildTwo(int V); //Cria um grafo de "2 em 2"
Graph GRAPHbuildGrid(int m, int n); //Cria uma grade dirigida m x n
int GRAPHdestroy(Graph G); //Destroi o grafo
int GRAPHinsertArc(Graph G, vertex v, vertex w); //Insere um arco no grafo
bool GRAPHremoveArc(Graph G, vertex v, vertex w); //Remove um arco no grafo
void GRAPHinvertAdj(Graph G, vertex v); //Inverte a ordem dos vizinhos de um vertice
void GRAPHinvertArc(Graph G); //Inverte toda a lista de adj do grafo
void GRAPHisSink(Graph G); //Exibe um vetor booleano que indica quais vertices sao ou nao soverdouros
void GRAPHisSource(Graph G); //Exibe um vetor booleano que indica quais vertices sao ou nao fontes
int GRAPHindeg(Graph G, vertex v); //Retorna o grau de entrada de um vertice
int GRAPHoutdeg(Graph G, vertex v); //Retorna o grau de saida de um vertice
int DGRAPHoutdeg(Graph G); //Retorna um vetor indexado pelos vertices, contendo seus graus de saida
int DGRAPHindeg(Graph G); //Retorna um vetor indexado pelos vertices, contendo seus graus de entrada
int GRAPHisolated(Graph G, vertex v); //Verifica se um vertice eh isolado
int GRAPHisadj(Graph G, vertex v, vertex w); //Verifica se dois vertices sao vizinhos
int GRAPHisequal(Graph G, Graph H); //Verifica se dois grafos sao iguais
void GRAPHshow(Graph G); //Imprime a lista de adjacencia do grafo
int GRAPHisEdge(Graph G, vertex v, vertex w); //Informa se uma ligacao entre dois vertices eh uma aresta
int GRAPHisUndirected(Graph G); //Informa se um grafo eh nao-dirigido
int GRAPHisTour(Graph G); //Informa se um grafo eh um torneio
int UGRAPHinsertEdge(Graph G, vertex v, vertex w); //Insere uma aresta entre dois vertices em um grafo nao-dirigido
int UGRAPHremoveEdge(Graph G, vertex v, vertex w); //Remove uma aresta entre dois vertices em um grafo nao-dirigido
int UGRAPHdegrees(Graph G); //Retorna um vetor com os graus dos vertices de um grafo nao dirigido
bool isTopoNumbering(Graph G, int topo[]); //Decide se uma numeracao eh topologica
bool isTopoPermut(Graph G, int topo[], int tam); //Decide se uma permutação eh toplogica
bool GRAPHreach(Graph G, vertex s, vertex t); //Decide se existe um caminho entre dois vertices
bool GRAPHcheckWalk(Graph G, int seq[], int tam); //Decide se uma dada sequencia dos vertices de um grafo eh um passeio
bool GRAPHcheckSimplePath(Graph G, int seq[], int tam); //Decide se uma dada sequencia dos vertices eh um caminho simples
bool GRAPHcheckCycle(Graph G, int seq[], int tam); //Verifica se uma sequencia de vertices eh um ciclo
bool GRAPHisTopo(Graph G); //Decide se um grafo eh topologico
bool GRAPHisRootedForest(Graph G); //Decide se um grafo eh uma floresta radicada
int *ConvertNumtoPer(int num[], int tam); //Converte uma numeração para permutação