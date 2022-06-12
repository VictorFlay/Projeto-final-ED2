#include "TreeProjFunctions.h"


/// Rotinas gerais auxiliares

void limpa_buffer(void) {
    while (getchar() != '\n') {}
}

void traca_linha(int linhas) {
    while (linhas-- != 0)
        printf("-");
    endl;
}


/// Rotinas de verifica��o de arquivo

int arquivo_qtd(char *arq_nome) {
    // Conta o n�mero de registros atrav�s do n�mero de caracteres '\n'. Ocorr�ncia de cabe�alho � tratada na chamada da fun��o.

    int qtd = 0;
    char c;

    FILE *arq;
    arq = fopen(arq_nome, "r");
    while ((c = fgetc(arq)) != EOF) {
        if (c == '\n') qtd++;
    }
    fclose(arq);

    return qtd;
}


// Ronina de contagem de tempo

double calculaSegundos(struct timeval tempo1, struct timeval tempo2) {
    return ((tempo2.tv_sec + tempo2.tv_usec/1000000.0) - (tempo1.tv_sec +  tempo1.tv_usec/1000000.0));
}


/// Rotinas de formata��o de dados

double formata_float(char *real) {
    // Trata o campo sal�rio da base dados, que vem com ',' ao inv�s de '.' no valor, o qual a linguagem n�o reconhece como integrante de um float

    char *valor = real;
    while (*real != '\0') {
        if (*real == ',') *real = '.';
        real++;
    }

    return atof(valor);
}

void formata_float_str(char *str) {
    // Ao criar o arquivo ordenado, transforma o '.' do float em ',', conforme o arquivo original.

    char *aux = str;
    int i = 0;

    do {
        if (*aux == ';')
            i++;
        if (i == 5)
            if (*aux == '.')
                *aux = ',';
    }
    while (*aux++ != '\0');
}

void stringToStructDados(char *str, struct DADOS *registro) {
    // Converte os dados da string para o formato da struct

    registro->codigo = atoi(strtok(str, ";"));
    strcpy(registro->nome, strtok(NULL, ";"));
    registro->idade = atoi(strtok(NULL, ";"));
    strcpy(registro->empresa, strtok(NULL, ";"));
    strcpy(registro->departamento, strtok(NULL, ";"));
    registro->salario = formata_float(strtok(NULL, "\n"));
}


/// Rotinas de manipula��o de dados dos arquivos

void entra_dados(struct DADOS *registros, const int *max, char *cabecalho) {
    // Entra os dados do arquivo na vari�vel registros, para que possam ser ordenados

    char str[str_size];
    FILE *arq;

    arq = fopen(arq_dados, "r");

    fgets(str, sizeof(str), arq);
    strcpy(cabecalho, str);         // O cabe�alho n�o � tratado, uma vez que o novo arquivo segue o mesmo molde do antigo

    for (int i = 0; (fgets(str, sizeof(str), arq)) != NULL && i < *max ; i++) {
        // O argumento "i < *max" � �til na realiza��o de testes, n�o sendo necess�rio rodar todos os dados e previnindo erros ao trabalhar com n�mero de valores reduzido

        stringToStructDados(str, registros);
        registros++;
    }

    fclose(arq);

    return;
}

int compara_codigo(const void *a, const void *b) {
    // Argumento de compara��o da fun��o qsort().

    int pri = ((struct DADOS *)a)->codigo;
    int seg = ((struct DADOS *)b)->codigo;

    return (pri - seg);
}

void mostra_dados(struct DADOS *registros, const int max) {
    // Rotina de exibi��o para fins de verifica��o

    for (int i = 0; i < max; i++) {
        printf("Codigo......: %d\n", registros[i].codigo);
        printf("Nome........: "); fputs(registros[i].nome, stdout); endl;
        printf("Idade.......: %d\n", registros[i].idade);
        printf("Empresa.....: "); fputs(registros[i].empresa, stdout); endl;
        printf("Departamento: "); fputs(registros[i].departamento, stdout); endl;
        printf("Salario.....: %g\n", registros[i].salario);
        endl;
    }
}

void ordena_dados(struct DADOS *registros, const int *max, char *cabecalho) {
    // Cria um novo arquivo identico ao original, por�m com seus valores ordenados pelo campo c�digo

    qsort(registros, *max, sizeof(struct DADOS), compara_codigo);
    char str[str_size];

    FILE *arq;
    arq = fopen(arq_dados_ord, "w");
    fputs(cabecalho, arq);
    for (int i = 0; i < *max; i++) {
        sprintf(str, "%d;%s;%hu;%s;%s;%g\n",
                registros->codigo,
                registros->nome,
                registros->idade,
                registros->empresa,
                registros->departamento,
                registros->salario
        );
        formata_float_str(str);
        registros++;
        fputs(str, arq);
    }
    fclose(arq);
}

int OrdenaArquivo(void) {
    // Chamada das fun��es de inser��o de dados do arquivo no vetor, ordena��o de dados e cria��o do arquivo ordenado

    int max = arquivo_qtd(arq_dados) - 1;    // Retira-se um pelo cabecalho

    char cabecalho[str_size];
    dados registros[max];

    entra_dados(registros, &max, cabecalho);
    ordena_dados(registros, &max, cabecalho);

    return 0;
}

int TestaArquivo(void) {
    // Verifica se o arquivo existe e se pode ser aberto
    FILE *arq;
    arq = fopen(arq_dados, "r");

    if (arq == NULL) {
        perror(arq_dados);
        endl;
        return 1;
    }

    fclose(arq);
    return 0;
}

void salvaTestes(int option, double avl, double llrb) {
    char *arquivo;
    char str[19];

    if (option == 1)
        arquivo = "TesteDadosDesordenados.csv";
    else
        arquivo = "TesteDadosOrdenados.csv";

    FILE *arq;
    arq = fopen(arquivo, "a");
    fseek(arq, 0, SEEK_END);
    if (ftell(arq) == 0)
        fputs("AVL;LLRB\n", arq);

    sprintf(str, "%f;%f\n", avl, llrb);
    fputs(str, arq);

    fclose(arq);
}

int menu_insercao(short opcao) {
    traca_linha(40); endl;

    /// Rotina de teste
    if (TestaArquivo()) return 1;   // Verifica se o arquivo de dados existe e se funciona normalmente;



    /// Rotina de controle da fun��o, define o arquivo a ser aberto
    char *arquivo;                  // Recebe o endere�o do arquivo a ser aberto
    switch (opcao) {
    case 1:
        arquivo = arq_dados;        // Macro definida em TreeProjFunctions.h
        printf("\tINSERCAO DESORDENADA\n\n");
        break;

    case 2:
        OrdenaArquivo();            // Cria arquivo ordenado
        arquivo = arq_dados_ord;    // Macro definida em TreeProjFunctions.h
        printf("\tINSERCAO ORDENADA\n\n");
        break;

    default:
        printf("Opcao invalida.\n\n");
        return 1;
    }



    /// Declara��o de vari�veis
    // Declara��o de vari�veis para manipula��o de dados do arquivo
    char str[str_size];
    dados registro;

    // Declara��o de vari�veis para marca��o de tempo
    struct timeval temp_avl_1,  temp_avl_2;
    struct timeval temp_llrb_1, temp_llrb_2;
    double dif_avl, dif_llrb;

    // Declara��o de vari�veis para controle do arquivo
    FILE *arq;
    arq = fopen(arquivo, "r");



    /// Rotinas relacionadas � �rvore AVL
    // Cria��o da �rvore AVL
    arvAVL *raiz_avl;
    raiz_avl = cria_arvAVL();

    gettimeofday(&temp_avl_1, NULL);
    while (fgetc(arq) != '\n') {};          // Remove cabe�alho do arquivo
    while (fgets(str, sizeof(str), arq) != NULL) {
        stringToStructDados(str, &registro);        // Copia os dados da string para seus devidos campos na struct
        insere_arvAVL(raiz_avl, registro);          // Insere a struct na �rvore AVL
    }
    gettimeofday(&temp_avl_2, NULL);
    dif_avl = calculaSegundos(temp_avl_1, temp_avl_2);          // Realiza calcula da diferen�a de tempo

    liberar_arvAVL(raiz_avl);           // Destroi �rvore AVL



    rewind(arq);            // Retorna ponteiro para o �nicio do arquivo, para que esse possa ser lido novamente



    /// Rotinas relacionadas � �rvore LLRB
    // Cria��o da �rvore LLRB
    arvoreLLRB *raiz_llrb;
    raiz_llrb = cria_arvoreLLRB();

    gettimeofday(&temp_llrb_1, NULL);
    while (fgetc(arq) != '\n') {};          // Remove cabe�alho do arquivo
    while (fgets(str, sizeof(str), arq) != NULL) {
        stringToStructDados(str, &registro);            // Copia os dados da string para seus devidos campos na struct
        insere_arvoreLLRB(raiz_llrb, registro);         // Insere a struct na �rvore LLRB
    }
    gettimeofday(&temp_llrb_2, NULL);
    dif_llrb = calculaSegundos(temp_llrb_1, temp_llrb_2);

    liberar_arvoreLLRB(raiz_llrb);          // Destrui��o da �rvore LLRB



    /// Apresenta��o dos c�lculos de tempo
    printf("Arvore AVL.: %f segundos\n", dif_avl);
    printf("Arvore LLRB: %f segundos\n", dif_llrb);
    endl;

    //salvaTestes(opcao, dif_avl, dif_llrb);


    fclose(arq);
    return 0;
}
