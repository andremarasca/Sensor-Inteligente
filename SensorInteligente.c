#include <stdio.h>
#include <stdlib.h>
#define TAM 10 // Tamanho maximo de pecas na memoria
#define POS_P1 3 // Distancia do pistão 1 em relação ao posição de detecção do itemA
#define POS_P2 7 // Distancia do pistão 2 em relação ao posição de detecção do itemB
#define POS_P3 9 // Distancia do pistão 3 em relação ao posição de detecção do itemC
#define ItemA 0 // Codigo qualquer para o itemA
#define ItemB 1 // Codigo qualquer para o itemB
#define ItemC 2 // Codigo qualquer para o itemC

typedef struct nodo
{
    int ItemTipo;
    int posicao;
    struct nodo*proximo;
} Nodo;

typedef struct lista
{
    Nodo *pilha_livres[TAM]; // Pilha de ponteiros para os nodos livres
    int topo; // Para saber o topo da pilha de livres
    Nodo nodos[TAM];
    Nodo *primeiro;
    Nodo *ultimo;
} Lista;

//A função abaixo recebe uma lista e a inicializa
void inicializar(Lista *l)
{
    int i;
    for (i = 0; i < TAM; i++)
    {
        // Inicialmente todos os nodos estão livres
        l->pilha_livres[i] = &(l->nodos[i]);
    }
    // Sendo assim o topo é o idx do ultimo elemento inserido
    l->topo = TAM - 1;
    l->primeiro = NULL;
    l->ultimo = NULL;
}

// A funcao abaixo recebe uma lista e um elemento
// Cria um novo nodo armazena o elemento neste nodo
// e insere na lista.
int inserir (Lista *l, int ItemTipo)
{
    // Ao declarar um novo nodo deve-se pega-lo na pilha de livres
    Nodo *novo = l->pilha_livres[l->topo--];
    novo->ItemTipo = ItemTipo;
    novo->posicao = 0;
    novo->proximo = NULL;
    if(l->primeiro != NULL)
    {
        l->ultimo->proximo = novo;
        l->ultimo = novo;
    }
    else
    {
        l->primeiro = novo;
        l->ultimo = novo;
    }
    return 1;
}

// A funcao abaixo recebe uma lista e um ponteiro de nodo
// Busca pelo anterior a este nodo o remove da lista.
// Inserindo este nodo removido à lista de livres
int remover (Lista *l,Nodo *i)
{
    if(l->primeiro == NULL) return 0;
    Nodo *anterior, *atual;
    anterior = atual = l->primeiro;
    while(atual != NULL && atual != i)
    {
        anterior = atual;
        atual = atual->proximo;
    }
    if(atual != NULL && atual == i)
    {
        anterior->proximo = atual->proximo;
        if(l->primeiro == l->ultimo) l->primeiro = l->ultimo = NULL;
        else if(atual == l->primeiro) l->primeiro = l->primeiro->proximo;
        else if(atual == l->ultimo) l->ultimo = anterior;
        l->pilha_livres[++l->topo] = i;
        return 1;
    }
    return 0;
}

// A função abaixo recebe uma lista e imprime seu conteudo
// Em seguida mostra a visao interna do sistema simulado
// A letra T representa um pistão
// Underline representa espaço vazio
// Outros simbolos representam o tipo do item na dada posição
void imprime(Lista *l)
{
   int i;
    char V[10];
    for(i = 0 ; i < 10; i++)
        V[i] = '_';
    if(l->primeiro != NULL)
    {
        Nodo *tmp = l->primeiro;
        while(tmp)
        {

            if (tmp->ItemTipo == ItemA)
            {
                printf("[A, %d] ", tmp->posicao);
                V[tmp->posicao] = 'A';
            }
            else if (tmp->ItemTipo == ItemB)
            {
                printf("[B, %d] ", tmp->posicao);
                V[tmp->posicao] = 'B';
            }
            else if (tmp->ItemTipo == ItemC)
            {
                printf("[C, %d] ", tmp->posicao);
                V[tmp->posicao] = 'C';
            }
            tmp = tmp->proximo;
        }
    }
    printf("\n");

    for(i = 0; i < 10; i++)
        printf("%c ", V[i]);
    printf("\n");
    for(i = 0; i < 10; i++)
    {
        if (i == POS_P1 || i == POS_P2 || i == POS_P3)
            printf("T ");
        else
            printf("  ");
    }
    printf("\n");
}

// A funcao abaixo recebe uma lista, um nodo e um identificador de eventos
// Inicialmente chama a função que remove o nodo da lista
// Em sequencia no lugar do printf deve-se inserir o evento gerado
// em um buffer de eventos
void gerador_de_eventos (Lista *l, Nodo *i, int evento)
{
    remover(l, i);
    // Gerar um evento na ferramenta de automatos DESLAB
    // significa inseri-lo no buffer do automato
    // em questão, para que ele o execute em seguida.
    if (evento == ItemA)
        printf("Empurrando Item A com o pistao 1\n");
    else if (evento == ItemB)
        printf("Empurrando Item B com o pistao 2\n");
    else if (evento == ItemC)
        printf("Empurrando Item C com o pistao 3\n");
    else
        printf("Evento nao disponivel\n");
}

// A funcao abaixo recebe uma lista
// E para cada chamada dessa função, é adicionado um passo
// Para cada peça desta lista
// Após adicionar o passo é verificado se a peça está no local desejado
// Se estiver é chamada a função que gera o evento de acordo o seu tipo
void atualiza_posicao (Lista *l)
{
    Nodo *i = l->primeiro;
    for(i = l->primeiro; i != NULL; i = i->proximo)
    {
        i->posicao++;
        if(i->posicao == POS_P1 && i->ItemTipo == ItemA)
            gerador_de_eventos(l, i, ItemA);
        else if(i->posicao == POS_P2 && i->ItemTipo == ItemB)
            gerador_de_eventos(l, i, ItemB);
        else if(i->posicao == POS_P3 && i->ItemTipo == ItemC)
            gerador_de_eventos(l, i, ItemC);
    }
}

int main (void)
{
    int i, id;
    char N;
    Lista Lista;
    inicializar(&Lista);

    while(1)
    {
        printf("Gerar evento deteccao de item do tipo A: 0\n");
        printf("Gerar evento deteccao de item do tipo B: 1\n");
        printf("Gerar evento deteccao de item do tipo C: 2\n");
        printf("Andar 1 passo do encoder: Qualquer tecla\n");

        atualiza_posicao(&Lista);
        imprime(&Lista);

        N = getch();
        if (N == '0')
            inserir(&Lista, ItemA);
        else if (N == '1')
            inserir(&Lista, ItemB);
        else if (N == '2')
            inserir(&Lista, ItemC);

        system("cls");

    }
    return 0;
}
