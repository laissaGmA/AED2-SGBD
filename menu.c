#include <stdio.h>
#include <stdlib.h>
#include "geral.h"  
#include <string.h>

void exibirMenu(tabela *tab);

int main() {
    tabela tab;
    int opcao;

   
    if (!inicializarTabela(&tab)) {
        printf("Erro ao inicializar a tabela.\n");
        return 1;
    }

    do {
        exibirMenu(&tab);

        printf("\nOpcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Altura da árvore BST: %d\n", altura(tab.bst_indices));
                printf("Altura da árvore AVL: %d\n", alturaAVL(tab.avl_indices));
                printf("Altura da árvore RB: %d\n", alturaRB(tab.rb_indices));
                break;
            case 2:
                adicionarRegistro(&tab, ler_dados());
                printf("Registro adicionado com sucesso.\n");
                break;
            case 3:
                {
                 int valor;
                 printf("Digite o numero da matricula do registro a ser remmovido: ");
                 scanf("%d", &valor);
                 tab.bst_indices = remover(valor, tab.bst_indices);
                 tab.avl_indices = removerAVL(valor, tab.avl_indices);
                 tab.rb_indices = removerRB(valor, &tab.rb_indices);
                 printf("Registro removido com sucesso.\n");
                }
                break;
            case 4:
                {
                    int chave;
                    printf("Digite a matricula do registro a ser buscado: ");
                    scanf("%d", &chave);
                    buscarAVL(chave, tab.avl_indices, &tab);
                }
                break;
            case 5:
                printf("Imprimindo em ordem:\n");
                in_order(tab.bst_indices, &tab);
                break;
            case 99:
                printf("Finalizando o Programa...\n");
                finalizar(&tab);
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 99);

    return 0;
}

void exibirMenu(tabela *tab) {
    printf("\nEscolha uma opcao:\n");
    printf("1. Calcular altura das arvores\n");
    printf("2. Adicionar registro\n");
    printf("3. Remover registro\n");
    printf("4. Buscar\n");
    printf("5. Imprimir em ordem\n");
    printf("99. Sair\n");
}