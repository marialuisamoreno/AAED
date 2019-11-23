#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <locale.h>

#include <list>
#include <stack>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

// Indicar o T da Arvore B aqui:
#define T 2

using namespace std;

typedef struct Item {
    int codigo{};
}Item;

typedef struct No {
    Item item[2*T-1]{};
    struct No *pagina[2*T]{};
    int nInfo{};
}No;

typedef struct ArvB {
    No *raiz;
}ArvB;

/* Funções utilizadas */
void Inicializar (ArvB *arvore);
void Inserir (ArvB *arvore, Item item);
void Pesquisar (No *n, Item item);
Item Remover (No *no, int indice);
void Imprimir (No *no, int h);

Item ProcurarParaExcluir (ArvB *arvore, Item item);
Item Quebrar (No *n, No *dir);
void EmpurrarItens (No *n, int indice);
void PuxarItens (No *n, int indice);
void Unir (No *n, int indice);

void ImprimirArvore (ArvB *arvore);
void PrintAllNodes (No *root);

/* Funcao principal */
int main() {
    printf("Maria Luisa Santos Moreno Sanches\n");
    printf("Analise de Algoritmos e Estrutudas de Dados\n");
    printf("Trabalho sobre Arvore B\n\n");
    ArvB arvore;
    Inicializar(&arvore);

    int menu = 1;
    int opcao;
    int qtd;

    while (menu){
        printf("Menu\n");
        printf("1. Inserir elementos.\n");
        printf("2. Remover elementos.\n");
        printf("3. Buscar elemento.\n");
        printf("4. Visualizar a Arvore.\n");
        printf("5. Sair do programa.\n");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
            printf("INSERCAO\n");
            printf("Digite quantos elementos deseja inserir: ");
            scanf("%d", &qtd);
            for(int i=0; i<qtd; i++){
                printf("Elemento %d: ", i+1);
                Item novo;
                scanf("%d", &novo.codigo);
                Inserir(&arvore, novo);
            }
            printf("Fim da insercao.\n\n");
            break;
            case 2:
            printf("REMOCAO\n");
            printf("Digite quantos elementos deseja remover: ");
            scanf("%d", &qtd);
            for(int i=0; i<qtd; i++){
                printf("Elemento %d: ", i+1);
                Item novo;
                scanf("%d", &novo.codigo);
                ProcurarParaExcluir(&arvore, novo);
            }
            printf("Fim da remocao.\n\n");
            break;
            case 3:
            printf("BUSCA\n");
            printf("Digite quantos elementos deseja buscar: ");
            scanf("%d", &qtd);
            for(int i=0; i<qtd; i++){
                printf("Elemento %d: ", i+1);
                Item novo;
                scanf("%d", &novo.codigo);
                Pesquisar(arvore.raiz, novo);
            }
            printf("Fim da busca.\n\n");
            break;
            case 4:
            printf("VISUALIZACAO\n");
            ImprimirArvore(&arvore);
            printf("Fim da visualizacao.\n\n");
            break;
            case 5:
            printf("Saindo do programa...\n");
            menu = 0;
            break;
            default: printf("Opcao invalida!\n");
        }
    }

    return 0;
}

/* Inicializa a Arvore */
void Inicializar (ArvB *arvore) {
    arvore->raiz = NULL;
}

/* Chama a função para imprimir a Arvore B */
void ImprimirArvore (ArvB *arvore) {
    printf("IMPRIMIR ARVORE B\n\n");
    PrintAllNodes(arvore->raiz);
}

/* Copia um item para uma página da Arvore */
void copyItem (No *no, int index, Item copy) {
    no->item[index].codigo = copy.codigo;
}

/* Função de Inserção dos elementos na Arvore */
void Inserir (ArvB *arvore, Item item) {
    int i, j, indice;

    /* Como o nó raiz da Arvore só pode ter 1 único elemento, os itens são inseridos
       na página 0, e ele só pode ter 2 filhos, 0 e 1. */
    if (arvore->raiz == NULL) {
        No* novo = (No*) malloc(sizeof(No));
        copyItem(novo, 0, item);
        novo->nInfo = 1;
        novo->pagina[0] = NULL;
        novo->pagina[1] = NULL;
        arvore->raiz = novo;
    }

    /* Quando for inserir o segundo elemento em diante: */
    else {
        if (arvore->raiz->nInfo == 2*T-1) {        // é o caso quando a raiz sobe
            No *novo = (No*) malloc(sizeof(No));
            No *dir = (No*) malloc(sizeof(No));

            novo->item[0] = Quebrar(arvore->raiz, dir);
            novo->pagina[0] = arvore->raiz;
            novo->pagina[1] = dir;
            novo->nInfo = 1;
            arvore->raiz = novo;
        }

        No *aux = arvore->raiz;
        // todas as paginas dos nós não-folha tem conteúdo em todos os filhos
        while (aux->pagina[0] != NULL){
            // percorro os itens do nó até saber onde descer (do lado esquerdo de um item maior que o que vai ser inserido)
            for (i=0; i<aux->nInfo; i++){
                if (item.codigo < aux->item[i].codigo) break;
            }

            // maior que o ultimo (nInfo-1) elemento do vetor (ou seja, todos)
            if (i == aux->nInfo){
                // antes de descer, verifica-se se o nó da pagina a ser quebrada
                if (aux->pagina[aux->nInfo]->nInfo == 2*T-1){
                    No *dir = (No*) malloc(sizeof(No));

                    aux->item[aux->nInfo] = Quebrar(aux->pagina[aux->nInfo], dir);
                    aux->pagina[aux->nInfo+1] = dir;
                    aux->nInfo++;
                }
                // destino pode ter sido alterado na quebra da pagina
                if (item.codigo < aux->item[aux->nInfo-1].codigo) aux = aux->pagina[aux->nInfo-1];
                else aux = aux->pagina[aux->nInfo];
            }
            // entrou em uma pagina do lado esquerdo de algum item
            else{
                // quebra se necessario
                if (aux->pagina[i]->nInfo == 2*T-1){
                    EmpurrarItens(aux, i);

                    No *dir = (No*) malloc(sizeof(No));

                    aux->item[i] = Quebrar(aux->pagina[i], dir);
                    aux->pagina[i+1] = dir;
                    aux->nInfo++;
                }

                // destino pode ter mudado
                if (item.codigo < aux->item[i].codigo) aux = aux->pagina[i];
                else aux = aux->pagina[i+1];
            }
        }// sai do while -> encontrei folha

        indice = 0;
        // na folha, quero descobrir em que posicao inserir o item
        while (indice < aux->nInfo && item.codigo > aux->item[indice].codigo) indice++;

        // tenho quer inserir entre dois itens ou no final?
        if (indice < aux->nInfo) EmpurrarItens(aux, indice); // empurro todos a minha direita para entrar no meio

        copyItem(aux, indice, item);
        aux->pagina[indice+1] = NULL;
        aux->nInfo++;
    }
}

/* Função de Quebrar um nó da Arvore B */
Item Quebrar (No *n, No *dir) {
    int med = (n->nInfo-1)/2;
    Item meio = n->item[med];

    dir->nInfo = T-1;
    dir->pagina[med] = n->pagina[n->nInfo];

    int i;
    // copia os itens do final de n para dir
    for (i=0; i<med; i++){
        dir->item[i] = n->item[i + med+1];
        dir->pagina[i] = n->pagina[i + med+1];
    }
    n->nInfo = T-1;

    return meio;
}

/* Função para pesquisar um item na Arvore */
void Pesquisar (No *n, Item item) {
    if (n != NULL) {
        int i;
        for (i=0; i<n->nInfo; i++) {
            if (n->item[i].codigo == item.codigo) {
                Pesquisar(n->pagina[i], item);
                printf("Codigo: %d\n", n->item[i].codigo);
                printf("\n");
            }
            else if (item.codigo < n->item[i].codigo){
                Pesquisar(n->pagina[i], item);
                break;
            }
        }
        if (i == n->nInfo) Pesquisar(n->pagina[i], item);
    }
}

/* Percorre a Arvore até encontrar o item a ser excluido, caso ele exista */
Item ProcurarParaExcluir (ArvB *arvore, Item item) {
    int i;
    No *aux = arvore->raiz;

    // se chegar a null -> item nao existe
    while (aux != NULL){
        // verifica se item esta no nó atual
        for (i=0; i<aux->nInfo; i++){
            // se estiver, remove e ja sai da função
            if (item.codigo == aux->item[i].codigo){
                printf("-Item removido.\n");
                return Remover(aux, i);
            }

            // se encontrar alguem maior, nao preciso continuar percorrendo o mesmo nó e vou descer para uma pagina (i)
            if (item.codigo < aux->item[i].codigo) break;
        }

        // se não encontrei ainda e nao puder descer, item nao existe
        if (aux->pagina[0] == NULL) break;

        // item a ser excluido é maior que o último item do nó (explicado em inserir)
        if (i == aux->nInfo){
            /// antes de descer, a pagina deve ter pelo menos T itens (explicado melhor no relatorio)
            if (aux->pagina[aux->nInfo]->nInfo == T-1) {
                /// pagina da esquerda tambem tem T-1
                if (aux->pagina[aux->nInfo-1]->nInfo == T-1) {
                    /// item desce, unindo paginas e posicoes sao atualizadas
                    Unir(aux, aux->nInfo-1);
                    PuxarItens(aux, aux->nInfo-1);
                    aux->nInfo--;
                }
                /// se esquerda tem pelo menos T itens, passo o mais a direita deles para o pai, que desce para complementar o filho a direita
                else {
                    No *esq = aux->pagina[aux->nInfo-1];
                    No *dir = aux->pagina[aux->nInfo];

                    EmpurrarItens(dir, 0);
                    dir->item[0] = aux->item[aux->nInfo-1];
                    aux->item[aux->nInfo-1] = esq->item[esq->nInfo-1];
                    dir->pagina[0] = esq->pagina[esq->nInfo];
                    esq->nInfo--;
                    dir->nInfo++;
                }
            }
            /// despois de arrumar a pagina para ter pelo menos T itens, desco
            aux = aux->pagina[aux->nInfo];
        }
        /// vou entrar em alguma pagina a esquerda de um item
        else {
            /// analogo ao caso anterior
            if (aux->pagina[i]->nInfo == T-1) {
                /// direita tambem tem T-1 itens
                if (aux->pagina[i+1]->nInfo == T-1) {
                    Unir(aux, i);
                    PuxarItens(aux, i);
                    aux->nInfo--;
                }
                else { /// mais a esquerda da pagina a direita sobe e item que divide as paginas desce para pagina esquerda
                    No *esq = aux->pagina[i];
                    No *dir = aux->pagina[i+1];

                    esq->item[esq->nInfo] = aux->item[i];
                    aux->item[i] = dir->item[0];
                    esq->pagina[esq->nInfo+1] = dir->pagina[0];
                    dir->pagina[0] = dir->pagina[1];
                    PuxarItens(dir, 0);
                    dir->nInfo--;
                    esq->nInfo++;
                }
            }
            aux = aux->pagina[i];
        }
    }
    printf("-Item n�o encontrado.\n");
}

/* Função para remover um item da Arvore */
Item Remover (No *no, int indice) {
    Item texto = no->item[indice];
    /// se folha, remove (lembrando que para ter chegado nessa funcao, todos os nos no caminho ate a raiz tem pelo menos T itens)
    if (no->pagina[0] == NULL) {
        PuxarItens(no, indice);
        no->nInfo--;
    }
     /// se nao-folha, vou ter que emprestar de um filho, que pode ter que emprestar de um filho dele, etc
    else {
        /// posso mandar um item do filho da esquerda subir
        if (no->pagina[indice]->nInfo > T-1) {
            no->item[indice] = Remover(no->pagina[indice], no->pagina[indice]->nInfo-1);
        }
        /// posso mandar um item do filho da direita subir
        else if (no->pagina[indice+1]->nInfo > T-1) {
            no->item[indice] = Remover(no->pagina[indice+1], 0);
        }
        /// ninguem pode subir ainda -> item desce unindo os dois e o problema de remove-lo eh passado para onde ele foi
        else {
            Unir(no, indice);
            PuxarItens(no, indice);
            no->nInfo--;

            No *proximo = no->pagina[indice];
            int med = (proximo->nInfo-1)/2;
            return Remover(proximo, med);
        }
    }
    return texto;
}

/* Operação de Empurrar os itens na Arvore */
void EmpurrarItens (No *n, int indice) {
    int i;
    ///move todos os itens a partir de indice para a direita
    for (i=n->nInfo-1; i>=indice; i--) {
        n->item[i+1] = n->item[i];
        n->pagina[i+2] = n->pagina[i+1];
    }
}

/* Operação de Puxar os itens na Arvore */
void PuxarItens (No *n, int indice) {
    int i;
    ///move todos os itens a partir de indice para a esquerda
    for (i=indice; i<n->nInfo-1; i++) {
        n->item[i] = n->item[i+1];
        n->pagina[i+1] = n->pagina[i+2];
    }
}

/* Operação de Unir os itens na Arvore */
void Unir (No *n, int indice) {
    Item meio = n->item[indice];
    No *esq = n->pagina[indice];
    No *dir = n->pagina[indice+1];

    ///faz o inverso de Quebrar(), manda um item para unir as duas paginas adjacentes a ele (indice e indice+1)
    esq->item[esq->nInfo] = meio;
    esq->pagina[esq->nInfo+1] = dir->pagina[0];
    int i;
    /// aumenta o no, unindo as duas paginas e suas subpaginas
    for (i = esq->nInfo+1; i < 2*T-1; i++) {
        esq->item[i] = dir->item[i-(esq->nInfo+1)];
        esq->pagina[i+1] = dir->pagina[i-(esq->nInfo)];
    }

    esq->nInfo = 2*T-1;
}

/* Funções para imprimir a Arvore B */

int cnt[100];
int levels;
int loc[100][500];
No *ptrs[100][500];

void PrintNode (No *p, int depth, int i, int *pos) {
    int j;
    while ((*pos)<loc[depth][i]) {
        printf(" ");
        (*pos)++;
    }
    printf("[");
    for (j=0; j<p->nInfo-1; j++) {
        printf("%d,", p->item[j].codigo);
        (*pos) += 2;
    }
    printf("%d]", p->item[j].codigo);
    (*pos)+=3;
}

void PrintTop (No *p, int depth, int i, int *pos, int *child) {
    int j,strt,mid;

    mid = loc[depth][i]+p->nInfo;
    for (j=0; j<=p->nInfo; j++) {
        strt = loc[depth+1][*child]+1;
        if (strt>=mid) strt += 2*p->pagina[j]->nInfo-2;

        while ((*pos)<strt) {
            printf(" ");
            (*pos)++;
        }

        if (strt<=mid) printf("/");
        else printf("\\");

        (*pos)++;
        (*child)++;
    }
}

void PrintLine (No *p, int depth, int i, int *pos, int *child) {
    int strt,stop,mid;

    strt = loc[depth+1][*child]+2;
    stop = loc[depth+1][*child+p->nInfo]+p->pagina[p->nInfo]->nInfo*2-2;
    mid = loc[depth][i]+p->nInfo;

    *child += p->nInfo+1;

    while ((*pos)<strt) {
        printf(" ");
        (*pos)++;
    }

    while ((*pos)<=stop) {
        if (*pos==mid) printf("|");
        else printf("_");
        (*pos)++;
    }
}

void DFS(No *p, int depth) {
    int i;

    if (p != NULL) {
        if (depth>levels) levels=depth;
        ptrs[depth][cnt[depth]] = p;
        cnt[depth]++;

        for (i=0; i<=p->nInfo; i++) DFS(p->pagina[i],depth+1);
    }
}

void SetLoc() {
    int i,depth,pos,base,first,last,dist;
    int x;

    depth = levels;
    pos = 0;
    for (i=0; i<cnt[depth]; i++) {
        loc[depth][i] = pos;
        pos += (ptrs[depth][i]->nInfo)*2+2;
    }
    for (depth=levels-1; depth>=0; depth--) {
        base = 0;
        for (i=0; i<cnt[depth]; i++) {
           first = base;
           last  = first+(ptrs[depth][i]->nInfo);
           dist = (loc[depth+1][last] - loc[depth+1][first] + 2*ptrs[depth+1][last]->nInfo)/2;
           loc[depth][i] = loc[depth+1][first]+dist-ptrs[depth][i]->nInfo;
           base += ptrs[depth][i]->nInfo+1;
        }

    }
}

void PrintAllNodes (No *root) {
    int i,depth,pos,child;

    if (root==NULL) printf("EMPTY\n");
    else {
        for (depth=0; depth<10; depth++) {
            cnt[depth]=0;
        }
        levels=0;
        DFS(root,0);
        SetLoc();
        printf("L\n");
        for (depth=0; depth<=levels; depth++) {
            printf("%d",depth);
            pos=-1;
            for (i=0; i<cnt[depth]; i++) PrintNode(ptrs[depth][i],depth,i,&pos);
            printf("\n");

            if (depth < levels) {
                pos=-2;
                child=0;
                for (i=0; i<cnt[depth]; i++) PrintLine(ptrs[depth][i],depth,i,&pos,&child);
                printf("\n");
                pos=-2;
                child=0;
                for (i=0; i<cnt[depth]; i++) PrintTop(ptrs[depth][i],depth,i,&pos,&child);
                printf("\n");
            }
        }
        printf("\n");
    }
}
