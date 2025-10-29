#include <stdio.h>      // Biblioteca padrão de entrada e saída
#include <string.h>     // Para manipulação de strings (strcmp, strcpy, etc.)
#include <stdlib.h>     // Para uso de malloc e funções de alocação
#include <stdbool.h>    // Para usar tipo booleano (true / false)
#include <locale.h>     // Para configurar acentuação conforme o idioma

// Limites da mochila
#define MAX_ARRAY  10   // Máximo de itens na mochila
#define MAX_NOME  30    // Tamanho máximo do nome de cada item


// Enum para controle da ordem de ordenação dos itens
enum Ordem
{
    Por_Nome = 1,
    Por_Quantidade = 2,
    Por_Prioridade = 3,
    Desordenada = 0
};


// Enum para controle do menu principal
enum Controle_menu
{
    Sair_menu = 0,
    Continuar_menu = 1
};


// Estrutura que representa um item do inventário
typedef struct Item
{
    char nome[MAX_NOME];
    int quantidade;
    int prioridade;
}Item;


// Estrutura que representa a mochila (inventário)
typedef struct Mochila
{
    Item item_na_mochila[MAX_ARRAY];    // Vetor de itens
    int numero_de_itens;                // Quantos itens estão armazenados
    int comparacoes;                    // Contador de comparações (usado na ordenação)
    int em_ordem;                       // Indica se a mochila está ordenada e por qual critério

}Mochila;


// Função auxiliar para imprimir linhas decorativas
void linha(char caractere, int quantidade)
{
    printf("\n");

    for(int i = 0; i < quantidade; i++)
    {
        printf("%c", caractere);
    }

    printf("\n");
}


// Limpa o buffer do teclado (evita erros com scanf e fgets)
void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// Remove o caractere '\n' que o fgets insere ao final da string
void limpar_fgets(char *aux)
{
    aux[strcspn(aux, "\n")] = '\0';
}


// Verifica se a mochila está cheia
bool fila_cheia(Mochila *aux)
{
    return aux->numero_de_itens == MAX_ARRAY;
}


// Verifica se a mochila está vazia
bool fila_vazia(Mochila *aux)
{
    return aux->numero_de_itens == 0;
}


// ----------- FUNÇÕES DE ORDENAÇÃO (Insertion Sort) -----------

// Ordena os itens por nome (ordem alfabética)
void insertion_nome(Mochila *mochila_aux)
{
    mochila_aux->comparacoes = 0;

    for (int i = 1; i < mochila_aux->numero_de_itens; i++) 
    {
        Item chave = mochila_aux->item_na_mochila[i];
        int j = i - 1;

        while (j >= 0 && strcmp(mochila_aux->item_na_mochila[j].nome, chave.nome) > 0) 
        {
            mochila_aux->item_na_mochila[j + 1] = mochila_aux->item_na_mochila[j];
            j--;
        }
        mochila_aux->comparacoes++;

        mochila_aux->item_na_mochila[j + 1] = chave;
    }

    mochila_aux->em_ordem = Por_Nome;
}


// Ordena por quantidade (do menor para o maior)
void insertion_quantidade(Mochila *mochila_aux)
{
    mochila_aux->comparacoes = 0;

    for (int i = 1; i < mochila_aux->numero_de_itens; i++) 
    {
        Item chave = mochila_aux->item_na_mochila[i];
        int j = i - 1;

        while (j >= 0 && mochila_aux->item_na_mochila[j].quantidade > chave.quantidade) 
        {
            mochila_aux->item_na_mochila[j + 1] = mochila_aux->item_na_mochila[j];
            j--;
        }

        mochila_aux->comparacoes++;
            
        mochila_aux->item_na_mochila[j + 1] = chave;
    }

    mochila_aux->em_ordem = Por_Quantidade;
}


// Ordena por prioridade (do menor para o maior)
void insertion_prioridade(Mochila *mochila_aux)
{
    mochila_aux->comparacoes = 0;

    for (int i = 1; i < mochila_aux->numero_de_itens; i++) 
    {
        Item chave = mochila_aux->item_na_mochila[i];
        int j = i - 1;

        while (j >= 0 && mochila_aux->item_na_mochila[j].prioridade > chave.prioridade) 
        {
            mochila_aux->item_na_mochila[j + 1] = mochila_aux->item_na_mochila[j];
            j--;
        }

        mochila_aux->comparacoes++;

        mochila_aux->item_na_mochila[j + 1] = chave;
    }

    mochila_aux->em_ordem = Por_Prioridade;
}


// ----------- FUNÇÕES PRINCIPAIS DO PROGRAMA -----------

// Adiciona um item na mochila (ou soma quantidade se já existir)
void inserir_item(Mochila *mochila_aux, char nome_item[MAX_NOME], int quantidade, int prioridade)
{
    if(fila_cheia(mochila_aux))
    {
        printf("Inventário cheio, impossivel inserir!");
        return;    
    }

    int verificador = 0;    // Marca se o item já existe


    for(int i = 0; i < mochila_aux->numero_de_itens; i++)
    {
        // Se já existe um item com o mesmo nome
        if(strcmp(mochila_aux->item_na_mochila[i].nome, nome_item) == 0)
        {
            // Soma as quantidades
            mochila_aux->item_na_mochila[i].quantidade = mochila_aux->item_na_mochila[i].quantidade + quantidade;

            // Atualiza prioridade se a nova for maior
            if(mochila_aux->item_na_mochila[i].prioridade < prioridade) {
                mochila_aux->item_na_mochila[i].prioridade = prioridade;
            }

            verificador++;
        }
    }

    // Caso o item ainda não exista, adiciona um novo
    if(verificador == 0)
    {
        strcpy(mochila_aux->item_na_mochila[mochila_aux->numero_de_itens].nome, nome_item);
        mochila_aux->item_na_mochila[mochila_aux->numero_de_itens].quantidade = quantidade;
        mochila_aux->item_na_mochila[mochila_aux->numero_de_itens].prioridade = prioridade;
        
        mochila_aux->numero_de_itens++;                
    }

    
   
}


// Remove um item da mochila, caso exista
void remover_item(Mochila *mochila_aux, char nome_aux[MAX_ARRAY])
{
    if(fila_vazia(mochila_aux))
    {
        printf("Inventário vazio, impossivel remover!");
        return;     
    }


    int posicao = 0;
    int item_achado = 0;

    // Busca o item pelo nome
    for(int i = 0; i < mochila_aux->numero_de_itens; i++)
    {
        if(strcmp(mochila_aux->item_na_mochila[i].nome, nome_aux) == 0)
        {
            posicao = i;
            item_achado = 1;
            break;
        }
    }


    // Se encontrado, desloca os elementos para "fechar o buraco"
    if(item_achado != 0)
    {
        for(int i = posicao; i < mochila_aux->numero_de_itens; i++)
        {
            mochila_aux->item_na_mochila[i] = mochila_aux->item_na_mochila[i + 1];
        }
        
        mochila_aux->numero_de_itens--;
        mochila_aux->em_ordem = Desordenada;
    
    }
    else
    {
        printf("Item não achado na lista!");
    }
}


// Exibe o inventário completo
void exibir_inventario(Mochila *mochila_aux)
{
    if (fila_vazia(mochila_aux)) {
    linha('=', 55);
    printf("👜  Inventário vazio!\n");
    linha('=', 55);
    return;
    }

    linha('=', 55);
    printf("🧭  INVENTÁRIO ATUAL\n");
    linha('=', 55);

    printf("%-22s %-12s %-12s\n", "Item", "Quantidade", "Prioridade");
    linha('-', 55);

    for (int i = 0; i < mochila_aux->numero_de_itens; i++) 
    {
        printf("%-20s%-12d%-12d\n",
               mochila_aux->item_na_mochila[i].nome,
               mochila_aux->item_na_mochila[i].quantidade,
               mochila_aux->item_na_mochila[i].prioridade);
    }

    linha('=', 55);
}


// Exibe o menu principal e lê a opção do usuário
int exibir_menu()
{
    int opcao;

    printf("\n");
    printf("=======================================================\n");
    printf("🧭  MENU PRINCIPAL\n");
    printf("=======================================================\n");
    printf("1. Adicionar um item\n");
    printf("2. Remover um item\n");
    printf("3. Listar todos os itens\n");
    printf("4. Ordenar os itens\n");
    printf("5. Buscar item por nome (busca binária)\n");
    printf("0. Sair\n");
    printf("-------------------------------------------------------\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    
    return opcao;
}


// Busca binária por nome (só funciona se a mochila estiver ordenada por nome)
void buscar_item_binaria(Mochila *mochila_aux, char nome_aux[MAX_NOME]) {
    if (mochila_aux->em_ordem != Por_Nome) {
        printf("A mochila precisa estar ordenada por nome antes da busca!\n");
        return;
    }

    int inicio = 0, fim = mochila_aux->numero_de_itens - 1;
    int comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoes++;
        int cmp = strcmp(mochila_aux->item_na_mochila[meio].nome, nome_aux);

        if (cmp == 0) {
            printf("Item encontrado!\n");
            printf("%-20s %-12d %-12d\n",
                   mochila_aux->item_na_mochila[meio].nome,
                   mochila_aux->item_na_mochila[meio].quantidade,
                   mochila_aux->item_na_mochila[meio].prioridade);
            printf("Comparações: %d\n", comparacoes);
            return;
        } else if (cmp < 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    printf("Item não encontrado após %d comparações.\n", comparacoes);
}



// -------------------- FUNÇÃO PRINCIPAL --------------------
int main()
{
    setlocale(LC_ALL, "pt_br");

    int controle_menu = Continuar_menu;
    int entrada_menu = 0;

    char entrada_nome[MAX_NOME];
    int entrada_quantidade = 0;
    int entrada_prioridade = 0;
    int entrada_insertion = 0;

    // Alocação dinâmica da mochila (poderia ser estática, mas aqui é mais flexível)
    Mochila *inventario = malloc(sizeof(Mochila));
    inventario->numero_de_itens = 0;
    inventario->em_ordem = Desordenada;

    // Loop principal do menu
    do
    {
        entrada_menu = 0;
        entrada_quantidade = 0;
        entrada_prioridade = 0;
        
        entrada_menu = exibir_menu();
        limpar_buffer();

        switch (entrada_menu)
        {
        case 1://adicionar item

            linha('=', 55);
            printf("Cadastro de Item!");
            printf("\nEscolha o nome do item: ");
            fgets(entrada_nome, MAX_NOME, stdin);
            limpar_fgets(entrada_nome);
            
            printf("\nQual a sua quantidade? ");
            scanf("%d", &entrada_quantidade);
            limpar_buffer();

            printf("\nQual a sua prioridade? ");
            scanf("%d", &entrada_prioridade);
            limpar_buffer();

            inserir_item(inventario, entrada_nome, entrada_quantidade, entrada_prioridade);
            break;
        
        case 2: //remover item

            linha('=', 55);
            printf("Remover Item!");
            printf("\nDigite o item que deseja remover: ");
            fgets(entrada_nome, MAX_NOME, stdin);
            limpar_fgets(entrada_nome);

            remover_item(inventario, entrada_nome);
            
            break;

        case 3: //listar itens

            exibir_inventario(inventario);

            break;

        case 4: //ordenar itens

            linha('=', 55);
            do
            {
                entrada_insertion = 0;
                printf("Como deseja organizar menu?");
                printf("\n1. Por nome\n2. Por quantidade\n3. Por prioridade\n");

                scanf("%d", &entrada_insertion);
                limpar_buffer();

                if(entrada_insertion > 3 || entrada_insertion < 1)
                {
                    printf("Escolha uma entrada válida!");
                }

            }while(entrada_insertion > 3 || entrada_insertion < 1);
            
            if(entrada_insertion == 1)
                insertion_nome(inventario);
            
            if(entrada_insertion == 2)
                insertion_quantidade(inventario);
            
            if(entrada_insertion == 3)
                insertion_prioridade(inventario);

            break;

        case 5: //buscar por nome

            linha('=', 55);
            printf("Digite o nome do item que deseja buscar: ");
            fgets(entrada_nome, MAX_NOME, stdin);
            limpar_fgets(entrada_nome);

            buscar_item_binaria(inventario, entrada_nome);
            
            break;

        case 0: // Sair do programa
            //sair
            printf("Saindo");
            controle_menu = Sair_menu;
            break;
        
        default: //mensagem de erro!
            printf("Opção inválida!\n");
            break;
        }


    } while (controle_menu != Sair_menu);
    

    free(inventario);
    return 0;
}
