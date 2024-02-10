#ifndef MERGED_H
#define MERGED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct registro {
    char nome[81];
    char curso[51];
    int periodo;
    int matricula;
} dado;

typedef struct indice {
    int chave;
    int indice;  // posição no arquivo do primeiro byte do registro
} tipo_dado;

// Definições para Árvore Binária de Busca (BST)
typedef struct no_bst {
    tipo_dado *dado;
    struct no_bst *esq, *dir;
} no_bst;

typedef no_bst *arvore;

// Definições para Árvore AVL
typedef struct no_avl {
    tipo_dado *dado;
    int altura;
    struct no_avl *esq, *dir;
} no_avl;

typedef no_avl *arvore_avl;

// Definições para Árvore Rubro-Negra (RB)
typedef enum {
    RED,
    BLACK
} cor;

typedef struct no_rb {
    tipo_dado *dado;
    cor cor;
    struct no_rb *esq, *dir, *pai;
} no_rb;

typedef no_rb *arvore_rb;

typedef struct tabela {
    FILE *arquivo_dados;
    arvore bst_indices;
    arvore_avl avl_indices;
    arvore_rb rb_indices;
} tabela;

int inicializarTabela(tabela *tab);
void finalizar(tabela *tab);
void adicionarRegistro(tabela *tab, dado *registro);
void inicializar(arvore *raiz);
arvore adicionar(tipo_dado *valor, arvore raiz);
int altura(arvore raiz);
int maior(int a, int b);
tipo_dado *maior_elemento(arvore raiz);
tipo_dado *menor_elemento(arvore raiz);
void pre_order(arvore raiz, tabela *tab);
void pos_order(arvore raiz, tabela *tab);
void in_order(arvore raiz, tabela *tab);
void buscar(int chave, arvore raiz, tabela *tab);
arvore remover(int valor, arvore raiz);
void imprimir_elemento(arvore raiz, tabela *tab);
dado *ler_dados();
void tirar_enter(char *string);
void salvar_arquivo(char *nome, arvore a);
void salvar_auxiliar(arvore raiz, FILE *arq);
arvore carregar_arquivo(char *nome, arvore a);

// Definições para Árvore AVL

int inicializarTabelaAVL(tabela *tab);
void finalizarAVL(tabela *tab);
void adicionarRegistroAVL(tabela *tab, dado *registro);
void inicializarAVL(arvore_avl *raiz);
arvore_avl adicionarAVL(tipo_dado *valor, arvore_avl raiz);
int alturaAVL(no_avl *no);
arvore_avl balancearAVL(arvore_avl raiz);
int fator_balanceamentoAVL(no_avl *no);
tipo_dado *maior_elementoAVL(arvore_avl raiz);
tipo_dado *menor_elementoAVL(arvore_avl raiz);
void pre_orderAVL(arvore_avl raiz, tabela *tab);
void pos_orderAVL(arvore_avl raiz, tabela *tab);
void in_orderAVL(arvore_avl raiz, tabela *tab);
arvore_avl removerAVL(int valor, arvore_avl raiz);
void imprimir_elementoAVL(arvore_avl raiz, tabela *tab);
void buscarAVL(int chave, arvore_avl raiz, tabela *tab);
dado *ler_dadosAVL();
void tirar_enterAVL(char *string);
void salvar_arquivoAVL(char *nome, arvore_avl a);
void salvar_auxiliarAVL(arvore_avl raiz, FILE *arq);
arvore_avl carregar_arquivoAVL(char *nome, arvore_avl a);
arvore_avl rotacao_direitaAVL(arvore_avl y);
arvore_avl rotacao_esquerdaAVL(arvore_avl x);
arvore_avl rotacao_direita_esquerdaAVL(arvore_avl z);
arvore_avl rotacao_esquerda_direitaAVL(arvore_avl z);

// Definições para Árvore Rubro-Negra (RB)

int inicializarTabelaRB(tabela *tab);
void finalizarRB(tabela *tab);
void adicionarRegistroRB(tabela *tab, dado *registro);
void inicializarRB(arvore_rb *raiz);
arvore_rb adicionarRB(tipo_dado *valor, arvore_rb raiz);
arvore_rb balancearInsercaoRB(arvore_rb raiz, no_rb *novo);
arvore_rb encontrarMenorRB(arvore_rb raiz);
int alturaRB(no_rb *no);
int corDoNo(no_rb *no);
void pre_orderRB(arvore_rb raiz, tabela *tab);
void pos_orderRB(arvore_rb raiz, tabela *tab);
void in_orderRB(arvore_rb raiz, tabela *tab);
void imprimir_elementoRB(arvore_rb raiz, tabela *tab);
void buscarRB(int chave, arvore_rb raiz, tabela *tab);
arvore_rb removerRB(int valor, arvore_rb raiz);
void balancearRemocaoRB(arvore_rb *raiz, no_rb *x, no_rb *pai_x);
void tirar_enterRB(char *string);
void salvar_arquivoRB(char *nome, arvore_rb a);
void salvar_auxiliarRB(arvore_rb raiz, FILE *arq);
arvore_rb carregar_arquivoRB(char *nome, arvore_rb a);
arvore_rb rotacaoDireitaRB(arvore_rb raiz, no_rb *x);
arvore_rb rotacaoEsquerdaRB(arvore_rb raiz, no_rb *y);

#endif // MERGED_H