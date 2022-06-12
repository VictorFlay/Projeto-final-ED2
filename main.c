#include "TreeProjFunctions.h"

int main() {
    short opcao = 0;
    do {
        if (opcao != 0) {traca_linha(40); endl;}
        printf(
            "\t\tMENU\n\n"
            "[1] Insercao Desordenada\n"
            "[2] Insercao Ordenada\n"
            "[0] Sair\n"
        ); endl;

        opcao = -1;

        printf("Selecione uma opcao: ");
        scanf("%hd", &opcao); limpa_buffer(); endl;

        switch (opcao) {
            case 0: break;
            case 1: menu_insercao(1); break;
            case 2: menu_insercao(2); break;
            default:
                traca_linha(40); endl;
                printf("Opcao invalida!\n\n");
        }
    }
    while (opcao != 0);

    traca_linha(40); endl;
    printf("Fim de execucao, tecle <Enter> para finalizar");
    getchar();
    return 0;
}
