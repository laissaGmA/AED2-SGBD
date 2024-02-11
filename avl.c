#include <stdio.h>
#include <stdlib.h>
#include "merged.h"
#include <string.h>

void inicializarAVL(arvore_avl *raiz) {
    *raiz = NULL;
}


arvore_avl balancearAVL(arvore_avl raiz) {
    if (raiz == NULL) {
        return NULL;
    }

    raiz->altura = 1 + maior(alturaAVL(raiz->dir), alturaAVL(raiz->esq));

    int balanceamento = fator_balanceamentoAVL(raiz);

    if (balanceamento > 1) {
        if (fator_balanceamentoAVL(raiz->dir) < 0) {
            return rotacao_direita_esquerdaAVL(raiz);
        } else {
            return rotacao_esquerdaAVL(raiz);
        }
    } else if (balanceamento < -1) {
        if (fator_balanceamentoAVL(raiz->esq) > 0) {
            return rotacao_esquerda_direitaAVL(raiz);
        } else {
            return rotacao_direitaAVL(raiz);
        }
    }

    return raiz;
}

tipo_dado *maior_elementoAVL(arvore_avl raiz) {
    if (raiz == NULL)
        return NULL;

    while (raiz->dir != NULL)
        raiz = raiz->dir;

    return raiz->dado;
}

arvore_avl adicionarAVL(tipo_dado *valor, arvore_avl raiz, tabela *tab) {
    if (raiz == NULL) {
        arvore_avl novo = (arvore_avl)malloc(sizeof(struct no_avl));
        novo->dado = copiar_dados(valor);
        novo->altura = 1;
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (valor->chave > raiz->dado->chave) {
        raiz->dir = adicionarAVL(valor, raiz->dir, tab);
    } else {
        raiz->esq = adicionarAVL(valor, raiz->esq, tab);
    }

    // Move o ponteiro de arquivo ao final
    fseek(tab->arquivo_dados, 0L, SEEK_END);

    // Atualiza o índice do novo nó
    valor->indice = ftell(tab->arquivo_dados);

    // Escreve o registro no arquivo
    fwrite(valor, sizeof(tipo_dado), 1, tab->arquivo_dados);

    raiz->altura = 1 + maior(alturaAVL(raiz->dir), alturaAVL(raiz->esq));

    int balanceamento = fator_balanceamentoAVL(raiz);

    // Rotações AVL
    if (balanceamento > 1) {
        if (valor->chave > raiz->dir->dado->chave) {
            return rotacao_esquerdaAVL(raiz);
        } else {
            return rotacao_direita_esquerdaAVL(raiz);
        }
    }
    if (balanceamento < -1) {
        if (valor->chave < raiz->esq->dado->chave) {
            return rotacao_direitaAVL(raiz);
        } else {
            return rotacao_esquerda_direitaAVL(raiz);
        }
    }

    return raiz;
}

int alturaAVL(no_avl *no) {
    if (no == NULL) {
        return 0;
    }
    return no->altura;
}

int fator_balanceamentoAVL(no_avl *no) {
    if (no == NULL) {
        return 0;
    }
    return alturaAVL(no->dir) - alturaAVL(no->esq);
}

arvore_avl rotacao_direitaAVL(arvore_avl y) {
    arvore_avl x = y->esq;
    arvore_avl T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = 1 + maior(alturaAVL(y->dir), alturaAVL(y->esq));
    x->altura = 1 + maior(alturaAVL(x->dir), alturaAVL(x->esq));

    return x;
}

arvore_avl rotacao_esquerdaAVL(arvore_avl x) {
    arvore_avl y = x->dir;
    arvore_avl T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = 1 + maior(alturaAVL(x->dir), alturaAVL(x->esq));
    y->altura = 1 + maior(alturaAVL(y->dir), alturaAVL(y->esq));

    return y;
}

arvore_avl rotacao_direita_esquerdaAVL(arvore_avl z) {
    z->dir = rotacao_direitaAVL(z->dir);
    return rotacao_esquerdaAVL(z);
}

arvore_avl rotacao_esquerda_direitaAVL(arvore_avl z) {
    z->esq = rotacao_esquerdaAVL(z->esq);
    return rotacao_direitaAVL(z);
}


void pre_orderAVL(arvore_avl raiz, tabela *tab) {
    if (raiz != NULL) {
        imprimir_elementoAVL(raiz, tab);
        pre_orderAVL(raiz->esq, tab);
        pre_orderAVL(raiz->dir, tab);
    }
}

void pos_orderAVL(arvore_avl raiz, tabela *tab) {
    if (raiz != NULL) {
        pos_orderAVL(raiz->esq, tab);
        pos_orderAVL(raiz->dir, tab);
        imprimir_elementoAVL(raiz, tab);
    }
}

void in_orderAVL(arvore_avl raiz, tabela *tab) {
    if (raiz != NULL) {
        in_orderAVL(raiz->esq, tab);
        imprimir_elementoAVL(raiz, tab);
        in_orderAVL(raiz->dir, tab);
    }
}

void imprimir_elementoAVL(arvore_avl raiz, tabela *tab) {
    dado *temp = (dado *)malloc(sizeof(dado));
    temp->matricula = 1000;
    printf("indice: %d\n", raiz->dado->indice);

    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);
    int r = fread(temp, sizeof(dado), 1, tab->arquivo_dados);

    printf("[%d, %d, %s, %s, %d]\n", raiz->dado->chave, r, temp->nome, temp->curso, temp->periodo);
    free(temp);
}

void buscarAVL(int chave, arvore_avl raiz, tabela *tab) {
    if (raiz == NULL) {
        printf("Registro com chave %d nao encontrado.\n", chave);
        return;
    }

    if (chave == raiz->dado->chave) {
        // Imprima os dados satélite do registro
        imprimir_elementoAVL(raiz, tab);
    } else if (chave < raiz->dado->chave) {
        buscarAVL(chave, raiz->esq, tab);
    } else {
        buscarAVL(chave, raiz->dir, tab);
    }
}

arvore_avl removerAVL(int valor, arvore_avl raiz) {
    if (raiz == NULL)
        return NULL;

    if (raiz->dado->chave == valor) {
        if (raiz->esq == NULL) {
            return raiz->dir;
        }
        if (raiz->dir == NULL) {
            return raiz->esq;
        }
        raiz->dado = maior_elementoAVL(raiz->esq);
        raiz->esq = removerAVL(raiz->dado->chave, raiz->esq);
        return balancearAVL(raiz);
    }
    if (valor > raiz->dado->chave) {
        raiz->dir = removerAVL(valor, raiz->dir);
    } else {
        raiz->esq = removerAVL(valor, raiz->esq);
    }

    return balancearAVL(raiz);
}

void salvar_arquivoAVL(char *nome, arvore_avl a) {
    FILE *arq = fopen(nome, "wb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nome);
        return;
    }

    salvar_auxiliarAVL(a, arq);

    fclose(arq);  // Adiciona esta linha para fechar o arquivo corretamente
}

void salvar_auxiliarAVL(arvore_avl raiz, FILE *arq) {
    if (raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado), 1, arq);
        salvar_auxiliarAVL(raiz->esq, arq);
        salvar_auxiliarAVL(raiz->dir, arq);
    }
}

arvore_avl carregar_arquivoAVL(char *nome, arvore_avl a, tabela *tab) {
    FILE *arq;
    arq = fopen(nome, "rb");
    tipo_dado *temp;
    if (arq != NULL) {
        temp = (tipo_dado *)malloc(sizeof(tipo_dado));
        while (fread(temp, sizeof(tipo_dado), 1, arq)) {

            a = adicionarAVL(temp, a, tab);
            a = balancearAVL(a);
            temp = (tipo_dado *)malloc(sizeof(tipo_dado));
        }
        fclose(arq);
    }
    return a;
}