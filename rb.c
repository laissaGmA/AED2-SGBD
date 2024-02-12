#define NIL NULL

#include <stdio.h>
#include <stdlib.h>
#include "merged.h"
#include <string.h>

void inicializarRB(arvore_rb *raiz) {
    *raiz = NULL;
}

// Função para calcular a altura de uma árvore rubro-negra
int alturaRB(arvore_rb raiz) {
    if (raiz == NULL) {
        return 0;
    } else {
        int altura_esquerda = alturaRB(raiz->esq);
        int altura_direita = alturaRB(raiz->dir);
        return 1 + ((altura_esquerda > altura_direita) ? altura_esquerda : altura_direita);
    }
}



int maiorRB(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

tipo_dado *maior_elementoRB(arvore_rb raiz) {
    if (raiz == NULL)
        return NULL;

    while (raiz->dir != NULL)
        raiz = raiz->dir;

    return raiz->dado;
}

arvore_rb adicionarRB(tipo_dado *valor, arvore_rb raiz, tabela *tab) {
    if (raiz == NULL) {
        no_rb *novo = (no_rb *)malloc(sizeof(no_rb));

        // Aloca uma nova cópia de valor
        tipo_dado *novo_dado = copiar_dados(valor);

        novo->dado = novo_dado;
        novo->cor = RED;  // Novos nós são inicializados como vermelhos
        novo->esq = NULL;
        novo->dir = NULL;
        novo->pai = NULL;  // Inicializamos o pai como NULL

        return novo;
    }

    if (valor->chave > raiz->dado->chave) {
        raiz->dir = adicionarRB(valor, raiz->dir, tab);
        raiz->dir->pai = raiz;
    } else {
        raiz->esq = adicionarRB(valor, raiz->esq, tab);
        raiz->esq->pai = raiz;
    }

    // Move o ponteiro de arquivo ao final
    fseek(tab->arquivo_dados, 0L, SEEK_END);

    // Atualiza o índice do novo nó
    valor->indice = ftell(tab->arquivo_dados);

    // Escreve o registro no arquivo
    fwrite(valor, sizeof(tipo_dado), 1, tab->arquivo_dados);

    // Balanceamento na inserção ocorre no final da função
    return balancearInsercaoRB(raiz, raiz->dir);
}

no_rb *criarNo(tipo_dado *valor) {
    no_rb *novo = (no_rb *)malloc(sizeof(no_rb));
    if (novo != NULL) {
        novo->dado = valor;
        novo->pai = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->cor = RED;  // Novos nós são sempre vermelhos por padrão
    }
    return novo;
}

// rotacaoDireita
arvore_rb rotacaoDireita(arvore_rb raiz, no_rb *x) {
    no_rb *y = x->esq;
    x->esq = y->dir;
    if (y->dir != NULL) {
        y->dir->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        raiz = y;
    } else if (x == x->pai->esq) {
        x->pai->esq = y;
    } else {
        x->pai->dir = y;
    }
    y->dir = x;
    x->pai = y;

    return raiz;
}

// rotacaoEsquerda
arvore_rb rotacaoEsquerda(arvore_rb raiz, no_rb *y) {
    no_rb *x = y->dir;
    y->dir = x->esq;
    if (x->esq != NULL) {
        x->esq->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == NULL) {
        raiz = x;
    } else if (y == y->pai->esq) {
        y->pai->esq = x;
    } else {
        y->pai->dir = x;
    }
    x->esq = y;
    y->pai = x;

    return raiz;
}

// Função para balancear a árvore após a inserção
arvore_rb balancearInsercaoRB(arvore_rb raiz, no_rb *novo) {
    while (novo->pai != NULL && novo->pai->cor == RED) {
        if (novo->pai == novo->pai->pai->esq) {
            no_rb *tio = novo->pai->pai->dir;

            if (tio != NULL && tio->cor == RED) {
                // Caso 1: O tio é vermelho
                novo->pai->cor = BLACK;
                tio->cor = BLACK;
                novo->pai->pai->cor = RED;
                novo = novo->pai->pai;
            } else {
                if (novo == novo->pai->dir) {
                    // Caso 2: O tio é preto e o nó é um filho à direita
                    novo = novo->pai;
                    raiz = rotacaoEsquerda(raiz, novo);
                }

                // Caso 3: O tio é preto e o nó é um filho à esquerda
                novo->pai->cor = BLACK;
                novo->pai->pai->cor = RED;
                raiz = rotacaoDireita(raiz, novo->pai->pai);
            }
        } 
    }

    // Caso especial: A raiz deve ser sempre preta
    if (raiz != NULL) {
        raiz->cor = BLACK;
    }

    return raiz;
}

int corDoNo(no_rb *no) {
    // Retorna a cor do nó ou BLACK se o nó for NULL
    return (no == NULL) ? BLACK : no->cor;
}

// Função para imprimir em ordem
void in_orderRB(arvore_rb raiz, tabela *tab) {
    if (raiz != NULL) {
        in_orderRB(raiz->esq, tab);
        imprimir_elementoRB(raiz, tab);
        in_orderRB(raiz->dir, tab);
    }
}

// Função para imprimir um elemento da árvore rubro-negra
void buscarRB(int chave, arvore_rb raiz, tabela *tab) {
    if (raiz == NULL) {
        printf("Registro com chave %d nao encontrado.\n", chave);
        return;
    }

    if (chave == raiz->dado->chave) {
        // Imprima os dados satélite do registro
        imprimir_elementoRB(raiz, tab);
    } else if (chave < raiz->dado->chave) {
        buscarRB(chave, raiz->esq, tab);
    } else {
        buscarRB(chave, raiz->dir, tab);
    }
}

void imprimir_elementoRB(arvore_rb raiz, tabela *tab) {
    dado *temp = (dado *)malloc(sizeof(dado));
    temp->matricula = 1000;
    printf("indice: %d\n", raiz->dado->indice);

    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);
    int r = fread(temp, sizeof(dado), 1, tab->arquivo_dados);

    printf("[%d, %d, %s, %s, %d]\n", raiz->dado->chave, r, temp->nome, temp->curso, temp->periodo);
    free(temp);
}

arvore_rb removerRB(int valor, arvore_rb raiz) {
    if (raiz == NULL) {
        return NULL;
    }

    if (valor < raiz->dado->chave) {
        raiz->esq = removerRB(valor, raiz->esq);
    } else if (valor > raiz->dado->chave) {
        raiz->dir = removerRB(valor, raiz->dir);
    } else {
        // Nó a ser removido
        if (raiz->esq == NULL && raiz->dir == NULL) {
            // Caso 1: Nó sem filhos
            free(raiz->dado);
            free(raiz);
            return NULL;
        } else if (raiz->esq == NULL || raiz->dir == NULL) {
            // Caso 2: Nó com um filho
            arvore_rb filho = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            free(raiz->dado);
            free(raiz);
            return filho;
        } else {
            // Caso 3: Nó com dois filhos
            arvore_rb substituto = encontrarMenorRB(raiz->dir);
            raiz->dado = substituto->dado;
            raiz->dir = removerRB(substituto->dado->chave, raiz->dir);
        }
    }

    // Após a remoção, é necessário verificar e ajustar as propriedades da árvore rubro-negra
    // Implemente os casos específicos de balanceamento aqui

    return raiz;
}

// Função auxiliar para encontrar o nó com a menor chave em uma subárvore
arvore_rb encontrarMenorRB(arvore_rb raiz) {
    while (raiz->esq != NULL) {
        raiz = raiz->esq;
    }
    return raiz;
}

// Função para balancear a árvore após a remoção
arvore_rb balancearAposRemocao(arvore_rb raiz, arvore_rb pai, int lado) {
    if (raiz == NULL) {
        return NULL;
    }

    // Implementação dos casos específicos de balanceamento após a remoção
    if (corDoNo(raiz->esq) == BLACK &&
        (corDoNo(raiz->esq->esq) == BLACK || (raiz->esq->esq == NULL)) &&
        (corDoNo(raiz->esq->dir) == BLACK || (raiz->esq->dir == NULL))) {
        // Caso 3: Sobrinho Externo do Irmão é Vermelho
        // Troque as cores do irmão e do sobrinho externo
        // Faça uma rotação para a direita
        if (raiz->esq != NULL) {
            raiz->esq->cor = RED;
            if (raiz->esq->dir != NULL) {
                raiz->esq->dir->cor = BLACK;
            }
            raiz = rotacaoDireita(raiz, raiz->esq); // Corrija a chamada aqui
        }
    } else if (corDoNo(raiz->esq) == BLACK &&
               (corDoNo(raiz->esq->esq) == BLACK || (raiz->esq->esq == NULL)) &&
               (corDoNo(raiz->esq->dir) == RED)) {
        // Caso 4: Sobrinho Interno do Irmão é Vermelho
        // Troque as cores do pai e do irmão
        // Faça uma rotação para a direita
        // Torne o sobrinho interno preto
        if (raiz->esq != NULL) {
            raiz->esq->cor = RED;
            pai->cor = BLACK;
            raiz = rotacaoDireita(raiz, raiz); // Corrija a chamada aqui
            if (raiz->dir != NULL) {
                raiz->dir->cor = BLACK;
            }
        }
    }

    return raiz;
}


// Função para salvar a árvore rubro-negra em um arquivo
void salvar_arquivoRB(char *nome, arvore_rb a) {
    FILE *arq = fopen(nome, "wb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nome);
        return;
    }

    salvar_auxiliarRB(a, arq);

    fclose(arq);  // Adiciona esta linha para fechar o arquivo corretamente
}

// Função auxiliar para salvar a árvore rubro-negra em um arquivo
void salvar_auxiliarRB(arvore_rb raiz, FILE *arq) {
    if (raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado), 1, arq);
        salvar_auxiliarRB(raiz->esq, arq);
        salvar_auxiliarRB(raiz->dir, arq);
    }
}

// Função para carregar a árvore rubro-negra de um arquivo
arvore_rb carregar_arquivoRB(char *nome, arvore_rb raiz, tabela *tab) {
    FILE *arq;
    arq = fopen(nome, "rb");
    tipo_dado *temp;
    if (arq != NULL) {
        temp = (tipo_dado *)malloc(sizeof(tipo_dado));
        while (fread(temp, sizeof(tipo_dado), 1, arq)) {

            raiz = adicionarRB(temp, raiz, tab);
            raiz = balancearInsercaoRB(raiz, raiz->dir);  // Aplicar balanceamento após a carga
            temp = (tipo_dado *)malloc(sizeof(tipo_dado));
        }
        fclose(arq);
    }
    return raiz;
}