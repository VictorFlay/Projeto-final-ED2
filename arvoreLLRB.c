#include <stdio.h>
#include <stdlib.h>
#include "TreeProjFunctions.h"

#define RED 1
#define BLACK 0

struct NO {
    int info;
    dados conteudo;
    struct NO *esq;
    struct NO *dir;
    int cor;
};


// Funções auxiliares
int cor(struct NO *H) {
    if (H == NULL) {
        return BLACK;
    }
    else {
        return H->cor;
    }
}

void trocaCor(struct NO *H) {
    H->cor = !H->cor;
    if (H->esq != NULL) {
        H->esq->cor = !H->esq->cor;
    }
    if (H->dir != NULL) {
        H->dir->cor = !H->dir->cor;
    }
}

struct NO *rotacionaEsquerda(struct NO *A) {
    struct NO *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
};

struct NO *rotacionaDireita(struct NO *A) {
    struct NO *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
};

struct NO *move2EsqRED(struct NO *H) {
    trocaCor(H);
    if (cor(H->dir->esq) == RED) {
        H->dir = rotacionaDireita(H->dir);
        H = rotacionaEsquerda(H);
        trocaCor(H);
    }
    return H;
};

struct NO *move2DirRED(struct NO *H) {
    trocaCor(H);
    if (cor(H->esq->esq) == RED) {
        H = rotacionaDireita(H);
        trocaCor(H);
    }
    return H;
};

struct NO *balancear(struct NO *H) {
    if (cor(H->dir) == RED) {
        H = rotacionaEsquerda(H);
    }
    if (H->esq != NULL && cor(H->dir) == RED && cor(H->esq->esq) == RED) {
        H = rotacionaDireita(H);
    }
    if (cor(H->esq) == RED && cor(H->dir) == RED) {
        trocaCor(H);
    }
    return H;
};


// Rotinas de manipulação de árvore

arvoreLLRB *cria_arvoreLLRB() {
    arvoreLLRB *raiz = (arvoreLLRB*) malloc(sizeof(arvoreLLRB));
    if (raiz != NULL) {
        *raiz = NULL;
    }
    return raiz;
}

void libera_NO_LLRB(struct NO *no) {
    if (no == NULL) {
        return;
    }
    libera_NO_LLRB(no->esq);
    libera_NO_LLRB(no->dir);
    free(no);
    no = NULL;
}

void liberar_arvoreLLRB(arvoreLLRB *raiz) {
    if (raiz == NULL) {
        return;
    }
    libera_NO_LLRB(*raiz);
    free(raiz);
}


// Rotinas de consulta

int vazia_arvoreLLRB(arvoreLLRB *raiz) {
    if (raiz == NULL) {
        return 1;
    }
    if (*raiz == NULL) {
        return 1;
    }
    return 0;
}

int altura_arvoreLLRB(arvoreLLRB *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    if (*raiz == NULL) {
        return 0;
    }
    int alt_esq = altura_arvoreLLRB(&((*raiz)->esq));
    int alt_dir = altura_arvoreLLRB(&((*raiz)->dir));
    if (alt_esq > alt_dir) {
        return (alt_esq + 1);
    }
    else {
        return (alt_dir + 1);
    }
}

int totalNO_arvoreLLRB(arvoreLLRB *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    if (*raiz == NULL) {
        return 0;
    }
    int alt_esq = totalNO_arvoreLLRB(&((*raiz)->esq));
    int alt_dir = totalNO_arvoreLLRB(&((*raiz)->dir));
    return (alt_esq + alt_dir + 1);
}


// Rotinas complementares

struct NO *insereNO(struct NO *H, dados valor, int *resp) {
    if (H == NULL) {
        struct NO *novo;
        novo = (struct NO*) malloc(sizeof(struct NO));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        novo->conteudo = valor;
        novo->info = valor.codigo;
        novo->cor = RED;
        novo->dir = NULL;
        novo->esq = NULL;
        *resp = 1;
        return novo;
    }
    if (valor.codigo == H->info) {
        *resp = 0;
    }
    else {
        if (valor.codigo < H->info) {
            H->esq = insereNO(H->esq, valor, resp);
        }
        else {
            H->dir = insereNO(H->dir, valor, resp);
        }
    }
    if (cor(H->dir) == RED && cor(H->esq) == BLACK) {
        H = rotacionaEsquerda(H);
    }
    if (cor(H->esq) == RED && cor(H->esq->esq) == RED) {
        H = rotacionaDireita(H);
    }
    if (cor(H->esq) == RED && cor(H->dir) == RED) {
        trocaCor(H);
    }
    return H;
};

struct NO *procuraMenor(struct NO *atual) {
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
};

struct NO *removeMenor(struct NO *H) {
    if (H->esq == NULL) {
        free(H);
        return NULL;
    }
    if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK) {
        H = move2EsqRED(H);
    }
    H->esq = removeMenor(H->esq);
    return balancear(H);
};

struct NO *removeNO(struct NO *H, int valor) {
    if (valor < H->info) {
        if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK) {
            H = move2EsqRED(H);
        }
        H->esq = removeNO(H->esq, valor);
    }
    else {
        if (cor(H->esq) == RED) {
            H = rotacionaDireita(H);
        }
        if (valor == H->info && (H->dir == NULL)) {
            free(H);
            return NULL;
        }
        if (cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK) {
            H = move2DirRED(H);
        }
        if (valor == H->info) {
            struct NO *x = procuraMenor(H->dir);
            H->info = x->info;
            H->dir = removeMenor(H->dir);
        }
        else {
            H->dir = removeNO(H->dir, valor);
        }
    }
    return balancear(H);
};


// Rotinas de manipulação de dados

int insere_arvoreLLRB(arvoreLLRB *raiz, dados valor) {
    int resp;
    *raiz = insereNO(*raiz, valor, &resp);
    if ((*raiz) != NULL) {
        (*raiz)->cor = BLACK;
    }
    return resp;
}

int remove_arvoreLLRB(arvoreLLRB *raiz, int valor) {
    if (consulta_arvoreLLRB(raiz, valor)) {
        struct NO *H = *raiz;
        *raiz = removeNO(H, valor);
        if (*raiz != NULL) {
            (*raiz)->cor = BLACK;
        }
        return 1;
    }
    else {
        return 0;
    }
}

int consulta_arvoreLLRB(arvoreLLRB *raiz, int valor) {
    if (raiz == NULL) {
        return 0;
    }
    struct NO *atual = *raiz;
    while (atual != NULL) {
        if (valor == atual->info) {
            return 1;
        }
        if (valor > atual->info) {
            atual = atual->dir;
        }
        else {
            atual = atual->esq;
        }
    }
    return 0;
}


// Rotinas de ordenação

void preOrdem_arvoreLLRB(arvoreLLRB *raiz) {
    if (raiz == NULL) {
        return;
    }
    if (*raiz != NULL) {
        printf("%d\n", (*raiz)->info);
        preOrdem_arvoreLLRB(&((*raiz)->esq));
        preOrdem_arvoreLLRB(&((*raiz)->dir));
    }
}

void emOrdem_arvoreLLRB(arvoreLLRB *raiz) {
    if (raiz == NULL) {
        return;
    }
    if (*raiz != NULL) {
        emOrdem_arvoreLLRB(&((*raiz)->esq));
        printf("%d\n", (*raiz)->info);
        emOrdem_arvoreLLRB(&((*raiz)->dir));
    }
}

void posOrdem_arvoreLLRB(arvoreLLRB *raiz) {
    if (raiz == NULL) {
        return;
    }
    if (*raiz != NULL) {
        posOrdem_arvoreLLRB(&((*raiz)->esq));
        posOrdem_arvoreLLRB(&((*raiz)->dir));
        printf("%d\n", (*raiz)->info);
    }
}
