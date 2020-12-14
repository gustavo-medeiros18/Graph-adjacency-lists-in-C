#include "grlist.h"

struct node { //Cada node é um valor inteiro e um ponteiro para outro node, que é o vertice vizinho
  vertex w;
  link next;
};

struct graph {
  int V, A; //V = n. de vertices e A = numero de arcos do grafo
  link *adj; //Ponteiro para o vetor de listas
};

static link NEWnode(vertex w, link next) {
  link a = malloc(sizeof(link));
  
  a->w = w;
  a->next = next;

  return a; //Retorna o endereço do no criado
}

Graph GRAPHinit(int V) {
  Graph G = malloc(sizeof(Graph));

  G->V = V;
  G->A = 0;
  G->adj = malloc(V * sizeof(link)); //O ponteiro para o vetor de listas é alocado dinamicamente
  
  vertex v;

  for (v = 0; v < V; v++) { //Todas as posições do vetor de listas recem criado sao inicializadas com NULL
    G->adj[v] = NULL;
  }

  return G;
}

vertex randV(Graph G) { 
  double r;
  
  r = rand( ) / (RAND_MAX + 1.0);
  
  return r * G->V;
}

Graph GRAPHrand1(int V, int A) { 
  Graph G = GRAPHinit(V);
  
  while (G->A < A) {
    vertex v = randV(G);
    vertex w = randV(G);
    
    if (v != w) {
      GRAPHinsertArc(G, v, w);
    }
  }
  
  return G;
}

Graph GRAPHrand2(int V, int A) {
  double prob = (double) A / (V * (V - 1));
  Graph G = GRAPHinit(V);

  srand(time(NULL));

  for (vertex v = 0; v < V; v++)
    for (vertex w = 0; w < V; w++)
      if (v != w)
        if (rand() < prob * (RAND_MAX + 1.0))
          GRAPHinsertArc(G, v, w);
  
  return G;
}

Graph GRAPHbuildPath(int V) {
  Graph G = GRAPHinit(V);
  int v;

  for (v = 0; v < V - 1; v++)
    GRAPHinsertArc(G, v, v + 1);
  
  return G;
}

Graph GRAPHbuildComplete(int V) {
  Graph G = GRAPHinit(V);
  vertex v, w;

  for (v = 0; v < G->V; v++)
    for (w = 0; w < G->V; w++)
      if (w != v)
        GRAPHinsertArc(G, v, w);
  
  return G;
}

Graph GRAPHbuildTour(int V) {
  Graph G = GRAPHinit(V);
  vertex v, w;

  for (v = 0; v < G->V; v++)
    for (w = 0; w < G->V; w++)
      if (v != w && !GRAPHisadj(G, w, v))
        GRAPHinsertArc(G, v, w);

  return G;
}

Graph GRAPHbuildCycle(int V) {
  Graph G = GRAPHbuildPath(V);

  GRAPHinsertArc(G, V - 1, 0);

  return G;
}

Graph GRAPHbuildTwo(int V) {
  Graph G = GRAPHinit(V);
  vertex v; 
  int cont = 0;

  for (v = 0; v < G->V; v++) {
    if (v + 2 >= G->V) {
      GRAPHinsertArc(G, v, cont);
      cont++;
    }
    else
      GRAPHinsertArc(G, v, v + 2);
  }
  
  return G;
}

int isMult(int x, int y) {
  if (x % y == 0)
    return 1;
  else
    return 0;
}

Graph GRAPHbuildGrid(int m, int n) {
  Graph G = GRAPHinit(m * n);
  vertex v;

  for (v = 0; v < G->V; v++) {
    if (!isMult(v + 1, n))
      GRAPHinsertArc(G, v, v + 1);
    
    if (v + 4 <= (G->V - 1))
      GRAPHinsertArc(G, v, v + 4);
  }

  return G;
}

int GRAPHisTour(Graph G) {
  int qtd = G->V * (G->V - 1);
  vertex v;
  link a;

  if (G->A != qtd / 2)
    return 0;

  for (v = 0; v < G->V; v++)
    for (a = G->adj[v]; a != NULL; a = a->next)
      if (v == a->w || GRAPHisadj(G, a->w, v))
        return 0;
  
  return 1;
}

int GRAPHdestroy(Graph G) {
  int i, j;

  for (i = 0; i < G->V; i++) { //Remove todos os arcos possiveis
    for (j = 0; j < G->V; j++)
      GRAPHremoveArc(G, i, j);
  }

  for (i = 0; i < G->V; i++) { //Libera todas as posicoes do vetor de lista (adj)
    G->adj[i] == NULL;
    free(G->adj[i]);
  }

  G = NULL; 
  free(G);

  return 1;
}

int GRAPHinsertArc( Graph G, vertex v, vertex w) {
  link a;

  for (a = G->adj[v]; a != NULL; a = a->next) //Verificacao se o arco que se deseja criar ja existe na lista de adj.
    if (a->w == w)
      return 0; //Nao eh inserido o arco
  
  G->adj[v] = NEWnode(w, G->adj[v]); //Criacao do novo arco
  G->A++;

  return 1; //O arco foi inserido como exito
}

bool GRAPHremoveArc(Graph G, vertex v, vertex w) {
  link prev, a;

  /*Caso o vertice v nao se ligue a nenhum outro vertice*/
  if (G->adj[v] == NULL)
    return false;

  /*Caso o vertice w for o primeiro da lista do vertice v*/
  if (G->adj[v]->w == w) {
    a = G->adj[v];
    G->adj[v] = a->next;

    free(a);
    G->A--;

    return true;
  }

  /*Procura do vertice w na lista de adjacencia do vertice v*/
  for ((prev = G->adj[v], a = G->adj[v]->next); a; (prev = a, a = a->next)) {
    if (a == NULL)
      return false;
    else if (a->w == w) {
      prev->next = a->next;
      
      free(a);
      G->A--;

      return true;
    }
  }
}

void GRAPHinvertAdj(Graph G, vertex v) {
  int tam = 0, *vet, i = 0, j;
  link a;

  for (a = G->adj[v]; a != NULL; a = a->next) //Obtem a quantidade de vizinhos de um vertice
    tam++;

  vet = (int *)calloc(tam, sizeof(int)); //Aloca um vetor para armazenar os vizinhos de um vertice

  for (a = G->adj[v]; a != NULL; a = a->next) { //Guarda no vetor os vizinho de um vertice
    vet[i] = a->w;
    i++;
  }

  for (i = 0; i < tam; i++)
    GRAPHremoveArc(G, v, vet[i]); //Remove todos os vizinhos do vertice
  
  for (i = 0; i < tam; i++)
    GRAPHinsertArc(G, v, vet[i]); //Insere novamente os vizinhos do vertice, mas na ordem inversa
}

void GRAPHinvertArc(Graph G) {
  int i;

  for (i = 0; i < G->V; i++)
    GRAPHinvertAdj(G, i); //Inverte todos os vertices possiveis do grafo
}

void GRAPHisSink(Graph G) {
  vertex v;
  bool *isSink;

  isSink = calloc(G->V, sizeof(bool));

  for (v = 0; v < G->V; ++v)
    (G->adj[v]) ? (isSink[v] = 0) : (isSink[v] = 1);

  for (v = 0; v < G->V; v++)
    isSink[v] ? (printf("[%d]: TRUE\n", v)) : (printf("[%d]: FALSE\n", v));
}

void GRAPHisSource(Graph G) {
  vertex v;
  vertex isSource[G->V];

  for (int i = 0; i < G->V; ++i) {
    isSource[i] = 1;

    for (v = 0; v < G->V; ++v) {
      if (G->adj[v] == NULL)
        isSource[v] = 0;

      for (vertex q = 0; (q < G->V) && (isSource[v] == 1); ++q) {
        for (link a = G->adj[q]; a != NULL; a = a->next)
          if (a->w == v) {
            isSource[v] = 0;

            break;
          }
      }
    }
  }

  printf("\nSources: ");

  for (int i = 0; i < G->V; ++i)
    if (isSource[i] == 1)
        printf("%d ", i);
  
  printf("\n");
}

int GRAPHindeg(Graph G, vertex v) {
  int indeg = 0;
  vertex i;
  link a;

  for (i = 0; i < G->V; i++)
    for (a = G->adj[i]; a; a = a->next)
      (a->w == v) ? (++indeg) : (indeg = indeg);
  
  return indeg;
}

int GRAPHoutdeg(Graph G, vertex v) {
  link a;
  int count = 0;

  for (a = G->adj[v]; a != NULL; a = a->next)
    count++;
  
  return count;
}

int *DGRAPHoutdeg(Graph G) {
  int *vet = (int *)calloc(G->V, sizeof(int));
  int i;
  link a;

  for (i = 0; i < G->V; i++)
    vet[i] = 0;

  for (i = 0; i < G->V; i++) {
    for (a = G->adj[i]; a != NULL; a = a->next)
      vet[i]++;
  }

  return vet;
}

int *DGRAPHindeg(Graph G) {
  int i, *vet;
  link a;

  vet = calloc(G->V, sizeof(int));

  for (i = 0; i < G->V; i++)
    vet[i] = 0;
  
  for (i = 0; i < G->V; i++) {
    for (a = G->adj[i]; a != NULL; a = a->next)
      vet[a->w]++;
  }

  return vet;
}

int GRAPHisolated(Graph G, vertex v) {
  int in = GRAPHindeg(G, v);
  int out = GRAPHoutdeg(G, v);

  if (in == 0 && out == 0)
    return 1;
  else
    return 0;
}

bool GRAPHisadj(Graph G, vertex v, vertex w) {
  link a;

  for (a = G->adj[v]; a != NULL; a = a->next)
    if (a->w == w)
      return true;
  
  return false;
}

int GRAPHisequal(Graph G, Graph H) {
  int i, j = 0, cont = 0, *vet1, *vet2;
  link a;

  if ((G->V != H->V) || (G->A != H->A))
    return 0;
  
  vet1 = (int *)calloc(G->A, sizeof(int));
  vet2 = (int *)calloc(G->A, sizeof(int));

  for (i = 0; i < G->V; i++) {
    for (a = G->adj[i]; a != NULL; a = a->next) {
      vet1[j] = a->w;
      j++;
    }
  }

  j = 0;

  for (i = 0; i < H->V; i++) {
    for (a = H->adj[i]; a != NULL; a = a->next) {
      vet2[j] = a->w;
      j++;
    }
  }

  for (i = 0; i < G->A; i++) {
    if (vet1[i] == vet2[i])
      cont++;
  }

  if (cont == G->A)
    return 1;
  
  return 0;
}

bool GRAPHisEdge(Graph G, vertex v, vertex w) {
  if (GRAPHisadj(G, v, w) && GRAPHisadj(G, w, v))
    return true;
  else
    return false;
}

int UGRAPHinsertEdge(Graph G, vertex v, vertex w) {
  GRAPHinsertArc(G, v, w);
  GRAPHinsertArc(G, w, v);

  return 1;
}

bool GRAPHisUndirected(Graph G) {
  link a;
  vertex v;

  for (v = 0; v < G->V; v++)
    for (a = G->adj[v]; a != NULL; a = a->next)
      if (!GRAPHisEdge(G, v, a->w))
        return false;
  
  return true;
}

int UGRAPHremoveEdge(Graph G, vertex v, vertex w) {
  if (GRAPHremoveArc(G, v, w) && GRAPHremoveArc(G, w, v))
    return 1;
  else
    return 0;
}

int *UGRAPHdegrees(Graph G) {
  int *degs;
  vertex v;
  link a;

  degs = calloc(G->V, sizeof(int));

  for (v = 0; v < G->V; v++)
    degs[v] = 0;

  for (v = 0; v < G->V; v++) {
    for (a = G->adj[v]; a != NULL; a = a->next)
      degs[v]++;
  }

  return degs;
}

bool isTopoNumbering(Graph G, int topo[]) {
  vertex v;
  link a;

  for (v = 0; v < G->V; v++)
    for (a = G->adj[v]; a != NULL; a = a->next)
      if (topo[v] >= topo[a->w])
        return false;
  
  return true;
}

static void reachR(Graph G, vertex v) { //Determina quais vertices sao acessiveis a partir de v
  visit[v] = 1;
  link a;

  for (a = G->adj[v]; a != NULL; a = a->next)
    if (visit[a->w] == 0)
      reachR(G, a->w);
}

bool GRAPHreach(Graph G, vertex s, vertex t) {
  vertex v;

  for (v = 0; v < G->V; v++)
    visit[v] = 0; //Marca todos os vertices como nao visitados
  
  reachR(G, s); //Marca todos os vertices que sao acessiveis a partir de s

  if (visit[t] == 0)
    return false;
  else
    return true;
}

bool GRAPHcheckWalk(Graph G, int seq[], int n) {
  int i;

  for (i = 0; i < n - 1; i++)
    if (!GRAPHisadj(G, seq[i], seq[i + 1]))
      return false;
  
  return true;
}

int VerifyRepV(int vet[], int tam) { //Verifica se ha vertices repetidos em uma sequencia de vertices
  int i, j, cont;

  for (i = 0; i < tam; i++) {
    cont = 0;

    for (j = 0; j < tam; j++) {
      if (vet[i] == vet[j])
        cont++;
      
      if(cont > 1)
        return 1; //Ha repeticao de valores no vetor
    }
  }

  return 0; //Nao ha repeticao de valores no vetor
}

bool GRAPHcheckSimplePath(Graph G, int seq[], int tam) {
  if(VerifyRepV(seq, tam))
    return false;
  
  if(!GRAPHcheckWalk(G, seq, tam))
    return false;
  
  return true;
}

bool VerifyRepArc(Graph G, int seq[], int tam) { //Verifica se em uma sequencia de vertices ha arcos repetidos
  Graph H = GRAPHinit(G->V);
  int i = 0, j = 1;

  while (i < tam - 1 &&  j < tam) {
    if(!GRAPHinsertArc(H, seq[i], seq[j])) {
      return true; //Ha repeticao de arcos na sequencia
      
      GRAPHdestroy(H);
    }
      
    i++;
    j++;
  }

  GRAPHdestroy(H);

  return false; //Nao ha repeticao de arcos na sequencia
}

bool GRAPHcheckCycle(Graph G, int seq[], int tam) {
  if(seq[0] != seq[tam - 1])
    return false;
    
  if(tam < 3)
    return false;
    
  if(VerifyRepArc(G, seq, tam))
    return false;
    
  if(!GRAPHcheckWalk(G, seq, tam))
    return false;
  
  return true;
}

int ObtainIndex(int x, int vet[], int tam) {
  int i;

  for (i = 0; i < tam; i++) {
    if (vet[i] == x)
      return i;
  }

  return 0;
}

bool isTopoPermut(Graph G, int topo[], int tam) {
  int i, j;

  for (i = 0; i < G->V; i++)
    for (j = 0; j < G->V; j++)
      if(GRAPHisadj(G, i, j))
        if(ObtainIndex(i, topo, tam) >= ObtainIndex(j, topo, tam))
          return false;
  
  return true;
}

int *ConvertNumtoPer(int num[], int tam) {
  int *per, i;

  per = (int *)calloc(tam, sizeof(int));

  for (i = 0; i < tam; i++)
    per[i] = ObtainIndex(i, num, tam);
  
  return per;
}

bool GRAPHisTopo(Graph G) {
  int indeg[1000]; //Guarda os graus de entrada dos vertices
  vertex v;
  link a;

  for (v = 0; v < G->V; v++)
    indeg[v] = 0; 
  
  for (v = 0; v < G->V; v++)
    for (a = G->adj[v]; a != NULL; a = a->next)
      indeg[a->w] += 1; //Preenchimento do vetor com os graus de entrada dos vertices
  
  vertex fila[1000]; //Fila que guarda os vertices que sao fontes (indeg[v] == 0)
  int comeco = 0, fim = 0;

  for (v = 0; v < G->V; v++)
    if (indeg[v] == 0)
      fila[fim++] = v; //Os vertices que sao fontes sao guardados na fila

  int cnt = 0; //Rotulo que vai sendo atribuido aos vertices conforme eles vao sendo removidos virtualmente

  while (comeco < fim) { //Nesse while, todos os vizinhos da fila de fontes sao transformados em fontes e removidos virtualmente
    v = fila[comeco++];
    topo[v] = cnt++; //topo[] = Guarda a numeracao topologica dos vertices

    for (a = G->adj[v]; a != NULL; a = a->next) { //Os arcos v-w vao sendo gradualmente removidos virtualmente, a fim de que cada vizinho de v torne-se uma fonte
      indeg[a->w] -= 1;

      if (indeg[a->w] == 0)
        fila[fim++] = a->w; //Os vertices que tornaram-se fontes sao adicionados à fila de fontes
    }
  }

  return cnt >= G->V;
}

bool GRAPHisRootedForest(Graph G) {
  vertex v;
  link a;

  if(!GRAPHisTopo(G))
    return false;
  
  for (v = 0; v < G->V; v++)
    if(GRAPHindeg(G, v) > 1)
      return false;
  
  return true;
}

vertex GRAPHgiveRoot(Graph G) {
  vertex v;
  vertex isRoot[G->V];

  for (int i = 0; i < G->V; ++i) {
    isRoot[i] = 1;

    for (v = 0; v < G->V; ++v) {
      if (G->adj[v] == NULL)
        isRoot[v] = 0;

      for (vertex q = 0; (q < G->V) && (isRoot[v] == 1); ++q) {
        for (link a = G->adj[q]; a != NULL; a = a->next)
          if (a->w == v) {
            isRoot[v] = 0;

            break;
          }
      }
    }
  }

  for (int i = 0; i < G->V; i++)
    if (isRoot[i])
      return i;
  
  return -1;
}

void GRAPHshow(Graph G) {
  link a;
  int i;

  for (i = 0; i < G->V; i++) {
    printf("[%d]: ", i);

    for (a = G->adj[i]; a != NULL; a = a->next)
      printf("%d, ", a->w);
    
    printf("\n");

  }
}