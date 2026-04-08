// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// #define MAX_TERRITORIOS 5
#define TAM_NOME 30
#define TAM_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

typedef struct
{
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

void limparBuffer();
void cadastrarTerritorios(Territorio *territorios, int n);
void exibirTerritorios(Territorio *territorios, int n);
void atacar(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *territorios);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main()
{
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    srand(time(NULL)); // Inicializa números aleatórios para os dados de ataque e defesa

    int numTerritorios;
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &numTerritorios);
    limparBuffer();

    Territorio *territorios = calloc(numTerritorios, sizeof(*territorios)); // Alocação dinâmica de territórios usando calloc para inicializar a memória com zeros.
    if (territorios == NULL)
    {
        printf("Erro ao alocar memória.\n"); // Imprime mensagem de erro para o usuário caso a alocação de memória falhe
        return 1;                            // Retorna 1 para indicar erro na alocação de memória
    }

    cadastrarTerritorios(territorios, numTerritorios);
    exibirTerritorios(territorios, numTerritorios);

    // Variáveis para armazenar as escolhas do usuário para ataque e defesa
    int atacante;
    int defensor;

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // Loop principal do jogo, onde o usuário pode escolher os territórios para atacar e defender, e o jogo continua até que o usuário decida sair.
    do
    {
        printf("\n--- FASE DE ATAQUE ---\n");
        // Escolha do atacante
        do
        {
            printf("Escolha o território atacante (1 a %d, ou 0 para sair): ", numTerritorios);
            scanf("%d", &atacante);
            limparBuffer();

            // Verifica se o usuário deseja sair do jogo
            if (atacante == 0)
            {
                // Saindo do jogo: libera memória
                liberarMemoria(territorios); // Libera a memória alocada para os territórios antes de sair do programa
                printf("Jogo encerrado e memória liberada. Até a próxima\n");
                return 0; // Sai do programa
            }

            atacante--; // Ajusta o índice
            if (atacante < 0 || atacante >= numTerritorios)
                printf("Território inválido, tente novamente.\n");
            else if (territorios[atacante].tropas < 2)
                printf("Tropas insuficientes para atacar, escolha outro território.\n");

        } while (atacante < 0 || atacante >= numTerritorios || territorios[atacante].tropas < 2);

        // Escolha do defensor
        do
        {
            printf("Escolha o território defensor (1 a %d): ", numTerritorios);
            scanf("%d", &defensor);
            limparBuffer();
            defensor--; // Ajusta o índice

            if (defensor < 0 || defensor >= numTerritorios)
                printf("Território inválido! Tente novamente.\n");
            else if (defensor == atacante)
                printf("Nao pode atacar o mesmo território! Escolha outro.\n");
            else if (territorios[defensor].tropas < 1)
                printf("Território sem tropas, escolha outro defensor.\n");
        } while (defensor < 0 || defensor >= numTerritorios || defensor == atacante || territorios[defensor].tropas < 1);

        // Ataque
        atacar(&territorios[atacante], &territorios[defensor]);

        printf("\nPressione Enter para continuar para o próximo turno...");
        getchar();

        exibirTerritorios(territorios, numTerritorios); // Exibe o mapa atualizado após cada turno de ataque, mostrando o estado atual dos territórios, incluindo suas cores e número de tropas.

    } while (1); // Loop infinito para continuar o jogo até que o usuário decida sair

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(territorios);
    return 0;
}
// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

// --- Função para limpar o buffer de entrada ---
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// --- Função para cadastrar territórios ---
void cadastrarTerritorios(Territorio *territorios, int n)
{
    // --- Laço Principal para cadastro dos territórios ---
    printf("\n=========================================\n");
    printf("   WAR ESTRUTURADO - CADASTRO INICIAL");
    printf("\n=========================================\n");

    for (int i = 0; i < n; i++)
    {
        printf("\n--- Cadastrando Território %d ---\n", i + 1);

        printf("Nome do Território: ");
        fgets(territorios[i].nome, TAM_NOME, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do Exército (ex: Azul, Verde): ");
        fgets(territorios[i].cor, TAM_COR, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBuffer();
    }
}

// --- Função para exibir territórios ---
void exibirTerritorios(Territorio *territorios, int n)
{
    // Exibe os territórios cadastrados

    printf("\n=========================================\n");
    printf("      MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("=========================================\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d. %s (Exército %s, Tropas: %d)\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

// --- Função de ataque ---
void atacar(Territorio *atacante, Territorio *defensor) // A função de ataque recebe dois ponteiros para estruturas do tipo Territorio, representando o território atacante e o território defensor.
{
    int dadoAtacante = rand() % 6 + 1; // Gera um número aleatório entre 1 e 6 para o atacante, simulando o lançamento de um dado.
    int dadoDefensor = rand() % 6 + 1; // Gera um número aleatório entre 1 e 6 para o defensor, da mesma forma que gera o dado do atacante.

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante); // Imprime o resultado do dado do atacante, mostrando o nome do território atacante e o valor do dado rolado.
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor); // Imprime o resultado do dado do defensor, mostrando o nome do território defensor e o valor do dado rolado.

    // Determina o resultado da batalha comparando os valores dos dados do atacante e do defensor. O território com o valor mais alto vence a batalha, e o perdedor perde uma tropa. Em caso de empate, nenhuma tropa é perdida.
    if (dadoAtacante > dadoDefensor) // Verifica se o atacante venceu a batalha.
    {
        defensor->tropas--; // O defensor perde uma tropa, decrementando o número de tropas do território defensor em 1.
        printf("VITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");

        if (defensor->tropas == 0) // Verifica se o defensor ficou sem tropas após a batalha. Se o número de tropas do defensor for igual a zero, significa que o território foi conquistado pelo atacante.
        {
            strcpy(defensor->cor, atacante->cor);
            printf("CONQUISTA! O território %s foi dominado pelo Exército %s!\n", defensor->nome, atacante->cor);
        }
    }
    // Se o defensor vencer a batalha, o atacante perde uma tropa.
    else if (dadoDefensor > dadoAtacante)
    {
        atacante->tropas--; // O atacante perde uma tropa, decrementando o número de tropas do território atacante em 1.
        printf("DEFESA VITORIOSA! O atacante %s perdeu 1 tropa.\n", atacante->nome);

        if (atacante->tropas == 0) // Verifica se o atacante ficou sem tropas após a batalha. Se o número de tropas do atacante for igual a zero, significa que o território foi conquistado pelo defensor.
        {
            strcpy(atacante->cor, defensor->cor); // O território atacante é conquistado pelo defensor, então a cor do território atacante é atualizada para a cor do defensor usando a função strcpy para copiar a string da cor do defensor para a cor do atacante.
            printf("CONQUISTA! O território %s foi dominado pelo Exército %s!\n", atacante->nome, defensor->cor);
        }
        // Em caso de empate, nenhuma tropa é perdida e o resultado da batalha é declarado como um empate.
    }
    else
    {
        printf("Empate! Nenhuma tropa perdida.\n");
    }
}

// --- Função para liberar memória ---
void liberarMemoria(Territorio *territorios)
{
    free(territorios);
}
