#include <stdio.h>
#include <stdlib.h>
#include "geral.h"
#include <string.h>

// Funções da árvore RB
void inicializarRB(arvore_rb *raiz) {
    *raiz = NULL;
}

no_rb *criarNoRB(tipo_dado *valor, cor_no cor) {
    no_rb *novo = (no_rb *)malloc(sizeof(no_rb));
    novo->dado = valor;
    novo->cor = cor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = NULL;
    return novo;
}

void rotacao_esquerdaRB(arvore_rb *raiz, no_rb *x) {
    no_rb *y = x->dir;
    x->dir = y->esq;

    if (y->esq != NULL) {
        y->esq->pai = x;
    }

    y->pai = x->pai;

    if (x->pai == NULL) {
        *raiz = y;
    } else if (x == x->pai->esq) {
        x->pai->esq = y;
    } else {
        x->pai->dir = y;
    }

    y->esq = x;
    x->pai = y;
}

void rotacao_direitaRB(arvore_rb *raiz, no_rb *y) {
    no_rb *x = y->esq;
    y->esq = x->dir;

    if (x->dir != NULL) {
        x->dir->pai = y;
    }

    x->pai = y->pai;

    if (y->pai == NULL) {
        *raiz = x;
    } else if (y == y->pai->esq) {
        y->pai->esq = x;
    } else {
        y->pai->dir = x;
    }

    x->dir = y;
    y->pai = x;
}

void balancearInsercaoRB(arvore_rb *raiz, no_rb *z) {
    while (z != *raiz && z->pai->cor == RED) {
        if (z->pai == z->pai->pai->esq) {
            no_rb *y = z->pai->pai->dir;
            if (y != NULL && y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacao_esquerdaRB(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                rotacao_direitaRB(raiz, z->pai->pai);
            }
        } else {
            no_rb *y = z->pai->pai->esq;
            if (y != NULL && y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacao_direitaRB(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                rotacao_esquerdaRB(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = BLACK;
}

void adicionarRB(tipo_dado *valor, arvore_rb *raiz) {
    no_rb *z = criarNoRB(valor, RED);
    no_rb *y = NULL;
    no_rb *x = *raiz;

    while (x != NULL) {
        y = x;
        if (z->dado->chave < x->dado->chave) {
            x = x->esq;
        } else {
            x = x->dir;
        }
    }

    z->pai = y;

    if (y == NULL) {
        *raiz = z;
    } else if (z->dado->chave < y->dado->chave) {
        y->esq = z;
    } else {
        y->dir = z;
    }

    balancearInsercaoRB(raiz, z);
}

int alturaRB(no_rb *no) {
    if (no == NULL) {
        return 0;
    }

    int altura_esq = alturaRB(no->esq);
    int altura_dir = alturaRB(no->dir);

    return altura_esq > altura_dir ? altura_esq + (no->cor == BLACK ? 1 : 0) : altura_dir + (no->cor == BLACK ? 1 : 0);
}

void balancearRemocaoRB(arvore_rb *raiz, no_rb *x) {
    while (x != *raiz && (x == NULL || x->cor == BLACK)) {
        if (x == x->pai->esq) {
            no_rb *w = x->pai->dir;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                rotacao_esquerdaRB(raiz, x->pai);
                w = x->pai->dir;
            }
            if ((w->esq == NULL || w->esq->cor == BLACK) &&
                (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->dir == NULL || w->dir->cor == BLACK) {
                    if (w->esq != NULL) {
                        w->esq->cor = BLACK;
                    }
                    w->cor = RED;
                    rotacao_direitaRB(raiz, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->dir != NULL) {
                    w->dir->cor = BLACK;
                }
                rotacao_esquerdaRB(raiz, x->pai);
                x = *raiz;
            }
        } else {
            no_rb *w = x->pai->esq;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                rotacao_direitaRB(raiz, x->pai);
                w = x->pai->esq;
            }
            if ((w->dir == NULL || w->dir->cor == BLACK) &&
                (w->esq == NULL || w->esq->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq == NULL || w->esq->cor == BLACK) {
                    if (w->dir != NULL) {
                        w->dir->cor = BLACK;
                    }
                    w->cor = RED;
                    rotacao_esquerdaRB(raiz, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->esq != NULL) {
                    w->esq->cor = BLACK;
                }
                rotacao_direitaRB(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    if (x != NULL) {
        x->cor = BLACK;
    }
}

void transplantarRB(arvore_rb *raiz, no_rb *u, no_rb *v) {
    if (u->pai == NULL) {
        *raiz = v;
    } else if (u == u->pai->esq) {
        u->pai->esq = v;
    } else {
        u->pai->dir = v;
    }

    if (v != NULL) {
        v->pai = u->pai;
    }
}

arvore_rb removerRB(int valor, arvore_rb *raiz) {
    no_rb *z = NULL;
    no_rb *y = NULL;
    no_rb *x = *raiz;

    while (x != NULL) {
        if (x->dado->chave == valor) {
            z = x;
        }

        y = x;
        if (valor < x->dado->chave) {
            x = x->esq;
        } else {
            x = x->dir;
        }
    }

    if (z == NULL) {
        return *raiz;  // Adicione este retorno
    }

    cor_no y_original_cor = y->cor;
    no_rb *x_filho = (z->esq == NULL) ? z->dir : z->esq;

    transplantarRB(raiz, z, x_filho);

    free(z->dado);
    free(z);

    if (y_original_cor == BLACK) {
        balancearRemocaoRB(raiz, x_filho);
    }

    return *raiz;  // Adicione este retorno
}

no_rb *sucessorRB(no_rb *x) {
    while (x != NULL && x->esq != NULL) {
        x = x->esq;
    }
    return x;
}

void pre_orderRB(arvore_rb raiz, tabela *tab) {
    if (raiz != NULL) {
        imprimir_elementoRB(raiz, tab);
        pre_orderRB(raiz->esq, tab);
        pre_orderRB(raiz->dir, tab);
    }
}

void pos_orderRB(arvore_rb raiz, tabela *tab) {
    if (raiz != NULL) {
        pos_orderRB(raiz->esq, tab);
        pos_orderRB(raiz->dir, tab);
        imprimir_elementoRB(raiz, tab);
    }
}

void in_orderRB(arvore_rb raiz, tabela *tab) {
    if (raiz != NULL) {
        in_orderRB(raiz->esq, tab);
        imprimir_elementoRB(raiz, tab);
        in_orderRB(raiz->dir, tab);
    }
}

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

void imprimir_elementoRB(no_rb *raiz, tabela *tab) {
    dado *temp = (dado *)malloc(sizeof(dado));
    temp->matricula = 1000;
    printf("Índice: %d\n", raiz->dado->indice);

    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);
    int r = fread(temp, sizeof(dado), 1, tab->arquivo_dados);

    printf("[%d, %s, %s]\n", raiz->dado->chave, temp->nome, temp->curso);
    free(temp);
}


void salvar_arquivoRB(char *nome, arvore_rb a) {
    FILE *arq;
    arq = fopen(nome, "wb");
    if (arq != NULL) {
        salvar_auxiliarRB(a, arq);
        fclose(arq);
    }
}

void salvar_auxiliarRB(arvore_rb raiz, FILE *arq) {
    if (raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado), 1, arq);
        salvar_auxiliarRB(raiz->esq, arq);
        salvar_auxiliarRB(raiz->dir, arq);
    }
}

arvore_rb carregar_arquivoRB(char *nome, arvore_rb a) {
    FILE *arq;
    arq = fopen(nome, "rb");
    tipo_dado *temp;
    if (arq != NULL) {
        temp = (tipo_dado *)malloc(sizeof(tipo_dado));
        while (fread(temp, sizeof(tipo_dado), 1, arq)) {
            adicionarRB(temp, &a);
            temp = (tipo_dado *)malloc(sizeof(tipo_dado));
        }
        fclose(arq);
    }
    return a;
}
