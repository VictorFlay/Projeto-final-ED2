#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define arq_dados     "Dados/massaDados.csv"
#define arq_dados_ord "Dados/massaDadosOrdenada.csv"

#define endl printf("\n")
#define str_size 201

struct DADOS {
    int codigo;
    char nome[30];
    short idade;
    char empresa[30];
    char departamento[30];
    double salario;
};

typedef struct DADOS dados;

/// Rotinas de Arvore AVL
typedef struct NO *arvAVL;

arvAVL *cria_arvAVL();
void liberar_arvAVL(arvAVL *raiz);

int vazia_arvAVL(arvAVL *raiz);
int altura_arvAVL(arvAVL *raiz);
int totalNO_arvAVL(arvAVL *raiz);

void preOrdem_arvAVL(arvAVL *raiz);
void emOrdem_arvAVL(arvAVL *raiz);
void posOrdem_arvAVL(arvAVL *raiz);

int insere_arvAVL(arvAVL *raiz, dados valor);
int remove_arvAVL(arvAVL *raiz, int valor);
int consulta_arvAVL(arvAVL *raiz, int valor);


/// Rotinas da Arvore LLRB
typedef struct NO *arvoreLLRB;

arvoreLLRB *cria_arvoreLLRB();
void liberar_arvoreLLRB(arvoreLLRB *raiz);

int vazia_arvoreLLRB(arvoreLLRB *raiz);
int altura_arvoreLLRB(arvoreLLRB *raz);
int totalNO_arvoreLLRB(arvoreLLRB *raiz);

int insere_arvoreLLRB(arvoreLLRB *raiz, dados valor);
int remove_arvoreLLRB(arvoreLLRB *raiz, int valor);
int consulta_arvoreLLRB(arvoreLLRB *raiz, int valor);

void preOrdem_arvoreLLRB(arvoreLLRB *raiz);
void emOrdem_arvoreLLRB(arvoreLLRB *raiz);
void posOrdem_arvoreLLRB(arvoreLLRB *raiz);


/// Rotinas gerais auxiliares
void limpa_buffer(void);
void traca_linha(int linhas);

/// Rotinas de verificação de arquivo
int arquivo_qtd(char *arq_nome);

/// Ronina de contagem de tempo
double calculaSegundos(struct timeval tempo1, struct timeval tempo2);

/// Rotinas de formatação de dados
double formata_float(char *real);
void formata_float_str(char *str);
void stringToStructDados(char *str, struct DADOS *registro);

/// Rotinas de manipulação de dados dos arquivos
void entra_dados(struct DADOS *registros, const int *max, char *cabecalho);
int compara_codigo(const void *a, const void *b);
void mostra_dados(struct DADOS *registros, const int max);
void ordena_dados(struct DADOS *registros, const int *max, char *cabecalho);
int OrdenaArquivo(void);
int TestaArquivo(void);
int menu_insercao(short opcao);
