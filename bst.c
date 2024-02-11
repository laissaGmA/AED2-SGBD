#include <stdio.h>
#include <stdlib.h>
#include "merged.h"
#include <string.h>


void inicializar(arvore *raiz) {
	*raiz = NULL;
}


int inicializarTabela(tabela *tab) {
    inicializar(&tab->bst_indices);
    inicializarAVL(&tab->avl_indices);
    inicializarRB(&tab->rb_indices);

    tab->arquivo_dados = fopen("dados.dat", "a+b");
    carregar_arquivo("bst_indices.dat", tab->bst_indices, tab);
	carregar_arquivoAVL("avl_indices.dat", tab->avl_indices, tab);
	carregar_arquivoRB("rb_indices.dat", tab->rb_indices, tab);

    if (tab->arquivo_dados != NULL)
        return 1;
    else
        return 0;
}

void adicionarRegistro(tabela *tab, dado *registro) {
    if (tab->arquivo_dados != NULL) {
        tipo_dado *novo = (tipo_dado *)malloc(sizeof(tipo_dado));
        novo->chave = registro->matricula;

        fseek(tab->arquivo_dados, 0L, SEEK_END);
        novo->indice = ftell(tab->arquivo_dados);

        fwrite(registro, sizeof(dado), 1, tab->arquivo_dados);

        // Adiciona nas árvores
        tab->bst_indices = adicionar(novo, tab->bst_indices, tab);
        tab->avl_indices = adicionarAVL(novo, tab->avl_indices, tab);
        tab->rb_indices = adicionarRB(novo, tab->rb_indices, tab);

        fseek(tab->arquivo_dados, 0L, SEEK_END);  // Move o ponteiro de arquivo de volta ao final
    }
}

tipo_dado *copiar_dados(tipo_dado *registro) {
    tipo_dado *copia = (tipo_dado *)malloc(sizeof(tipo_dado));
    if (copia != NULL) {
        copia->chave = registro->chave;
        copia->indice = registro->indice;
    }
    return copia;
}

// Função adicionar modificada
arvore adicionar(tipo_dado *valor, arvore raiz, tabela *tab) {
    if (raiz == NULL) {
        arvore novo = (arvore)malloc(sizeof(struct no_bst));
        novo->dado = copiar_dados(valor);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (valor->chave > raiz->dado->chave) {
        raiz->dir = adicionar(valor, raiz->dir, tab);
    } else {
        raiz->esq = adicionar(valor, raiz->esq, tab);
    }

    // Move o ponteiro de arquivo ao final
    fseek(tab->arquivo_dados, 0L, SEEK_END);

    // Atualiza o índice do novo nó
    valor->indice = ftell(tab->arquivo_dados);

    // Escreve o registro no arquivo
    fwrite(valor, sizeof(tipo_dado), 1, tab->arquivo_dados);

    // Retorna a raiz atualizada
    return raiz;
}

int altura(arvore raiz) {
	if(raiz == NULL) {
		return 0;
	}
	return 1 + maior(altura(raiz->dir), altura(raiz->esq));
}

int maior(int a, int b) {
	if(a > b)
		return a;
	else
		return b;
}

tipo_dado * maior_elemento(arvore raiz) {
	if(raiz == NULL)
			return NULL;
	if(raiz->dir == NULL)
			return raiz->dado;
	else
			return maior_elemento(raiz->dir);
}

tipo_dado * menor_elemento(arvore raiz) {
	if(raiz == NULL)
			return NULL;
	if(raiz->esq == NULL)
			return raiz->dado;
	else
			return maior_elemento(raiz->esq);
}

void pre_order(arvore raiz, tabela *tab) {
	if(raiz != NULL) {
		imprimir_elemento(raiz, tab);
		pre_order(raiz->esq, tab);
		pre_order(raiz->dir, tab);
	}
}

void pos_order(arvore raiz, tabela *tab) {
	if(raiz != NULL) {
		pos_order(raiz->esq, tab);
		pos_order(raiz->dir, tab);
		imprimir_elemento(raiz, tab);
	}
}

void in_order(arvore raiz, tabela *tab) {
    if (raiz != NULL) {
        in_order(raiz->esq, tab);
        imprimir_elemento(raiz, tab);
        in_order(raiz->dir, tab);
    }
}

void buscar(int chave, arvore raiz, tabela *tab) {
    if (raiz == NULL) {
        printf("Registro com chave %d não encontrado.\n", chave);
        return;
    }

    if (chave == raiz->dado->chave) {
        // Imprima os dados satélite do registro
        imprimir_elemento(raiz, tab);
    } else if (chave < raiz->dado->chave) {
        buscar(chave, raiz->esq, tab);
    } else {
        buscar(chave, raiz->dir, tab);
    }
}

void imprimir_elemento(arvore raiz, tabela *tab) {
    dado *temp = (dado *)malloc(sizeof(dado));
    temp->matricula = 1000;
    printf("indice: %d\n", raiz->dado->indice);

    fseek(tab->arquivo_dados, raiz->dado->indice, SEEK_SET);
    int r = fread(temp, sizeof(dado), 1, tab->arquivo_dados);

    printf("[%d, %d, %s, %s, %d]\n", raiz->dado->chave, r, temp->nome, temp->curso, temp->periodo);
    free(temp);
}


arvore remover (int valor, arvore raiz) {
	if(raiz == NULL) 
		return NULL;
	
	if(raiz->dado->chave == valor) {		
		if(raiz->esq == NULL) {
			return raiz->dir;
		}
		if(raiz->dir == NULL) {
			return raiz->esq;
		}
		raiz->dado = maior_elemento(raiz->esq);
		raiz->esq = remover(raiz->dado->chave, raiz->esq);
		return raiz;
	}	
	if(valor > raiz->dado->chave) {
			raiz->dir = remover(valor, raiz->dir);
	} else {
			raiz->esq = remover(valor, raiz->esq);
	}
	return raiz;
}

dado* ler_dados() {
    dado* registro = (dado*)malloc(sizeof(dado));

    printf("Nome: ");
    scanf("%s", registro->nome);

    printf("Curso: ");
    scanf("%s", registro->curso);

    printf("Periodo: ");
    scanf("%d", &registro->periodo);

    printf("Matricula: ");
    scanf("%d", &registro->matricula);

    return registro;
}

void tirar_enter(char *string) {
	string[strlen(string) -1] = '\0';
}

void salvar_arquivo(char *nome, arvore a) {
    FILE *arq = fopen(nome, "wb");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nome);
        return;
    }

    salvar_auxiliar(a, arq);

    fclose(arq);  // Adiciona esta linha para fechar o arquivo corretamente
}

void salvar_auxiliar(arvore raiz, FILE *arq) {
    if (raiz != NULL) {
        fwrite(raiz->dado, sizeof(tipo_dado), 1, arq);
        salvar_auxiliar(raiz->esq, arq);
        salvar_auxiliar(raiz->dir, arq);
    }
}


void finalizar(tabela *tab) {
    if (tab->arquivo_dados != NULL) {
        fclose(tab->arquivo_dados);
    }

    // Salva os índices antes de encerrar
    salvar_arquivo("bst_indices.dat", tab->bst_indices);
    salvar_arquivoAVL("avl_indices.dat", tab->avl_indices);
    salvar_arquivoRB("rb_indices.dat", tab->rb_indices);
    
    printf("Dados salvos em arquivo.\n");
}

arvore carregar_arquivo(char *nome, arvore a, tabela *tab) {
	FILE *arq;
	arq = fopen(nome, "rb");
	tipo_dado * temp;
	if(arq != NULL) {
		temp = (tipo_dado *) malloc(sizeof(tipo_dado));
		while(fread(temp, sizeof(tipo_dado), 1, arq)) {
			
			a = adicionar(temp, a, tab);	
			temp = (tipo_dado *) malloc(sizeof(tipo_dado));

		}
		fclose(arq);

	}
	return a;
}