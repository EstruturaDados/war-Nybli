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

#define TAM_NOME 30
#define TAM_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

typedef struct
{
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
    int conquistado; // 0 para não conquistado, 1 para conquistado
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
void cadastrarTerritorios(Territorio *territorios, int n);
void liberarMemoria(Territorio *territorios, char *missaoJogador);

// Funções de interface com o usuário:
void exibirMapa(Territorio *territorios, int n);
void exibirMissao(const char *missao);
void exibirMenu();

// Funções de lógica principal do jogo:
int escolherAtacante(Territorio *territorios, int numTerritorios);
int escolherDefensor(Territorio *territorios, int numTerritorios, int atacante);
int existeDefensorValido(Territorio *territorios, int numTerritorios, int atacante);
int faseAtaque(Territorio *territorios, int numTerritorios);
void atacar(Territorio *atacante, Territorio *defensor);

void atribuirMissao(char *destino, char *missoes[], int totalMissoes, Territorio *mapa, int tamanho);
int verificarMissao(char *missao, Territorio *mapa, int tamanho);
int verificarFimPorTropas(Territorio *mapa, int tamanho);
int existeCor(Territorio *mapa, int tamanho, const char *cor);

// Função utilitária:
void limparBuffer();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main()
{
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.

    srand(time(NULL)); // Inicializa números aleatórios para os dados de ataque e defesa

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    int numTerritorios;
    printf("==================================\n");
    printf("     BEM-VINDO AO JOGO WAR!\n");
    printf("==================================\n");
    printf("\nVamos começar cadastrando os territórios do nosso mundo.\n");

    do
    {
        printf("Digite a quantidade de territórios deseja cadastrar: ");

        if (scanf("%d", &numTerritorios) != 1 || numTerritorios <= 1)
        {
            printf("Entrada inválida. Tente novamente.\n");
            limparBuffer();
        }
        else
        {
            break; // Entrada válida, sai do loop
        }

    } while (1);

    limparBuffer();

    Territorio *territorios = calloc(numTerritorios, sizeof(*territorios)); // Alocação dinâmica de territórios usando calloc para inicializar a memória com zeros.
    char *missaoJogador = malloc(100 * sizeof(char));

    if (territorios == NULL || missaoJogador == NULL)
    {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    char *missoes[] = {
        "Conquistar 3 territórios",
        "Dominar todos os territórios do mapa",
        "Ter 1 território com 10 tropas",
        "Eliminar exército Azul",
        "Eliminar exército Verde"};

    int totalMissoes = 5;

    cadastrarTerritorios(territorios, numTerritorios);
    atribuirMissao(missaoJogador, missoes, totalMissoes, territorios, numTerritorios);
    exibirMapa(territorios, numTerritorios);
    exibirMissao(missaoJogador);

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    // Loop principal do jogo, onde o usuário pode escolher os territórios para atacar e defender, e o jogo continua até que o usuário decida sair.

    int opcao;

    do
    {
        printf("\n--- FASE DE ATAQUE ---\n");
        exibirMenu();

        do
        {
            if (scanf("%d", &opcao) != 1 || opcao < 0 || opcao > 2)
            {
                printf("Entrada inválida. Tente novamente.\n");
                limparBuffer();
            }
            else
            {
                break;
            }
        } while (1);

        limparBuffer();

        switch (opcao)
        {

        case 1:
        {
            printf("\nIniciando fase de ataque...\n");

            if (faseAtaque(territorios, numTerritorios) == 0)
            {
                printf("Jogo encerrado.\n");

                liberarMemoria(territorios, missaoJogador);
                return 0;
            }

            if (verificarMissao(missaoJogador, territorios, numTerritorios))
            {
                printf("\n=================================\n");
                printf("        MISSÃO CUMPRIDA!\n");
                printf("        VOCÊ É O VENCEDOR!\n");
                printf("=================================\n");

                liberarMemoria(territorios, missaoJogador);
                return 0;
            }

            if (verificarFimPorTropas(territorios, numTerritorios))
            {
                printf("\n=================================\n");
                printf("TROPAS INSUFICIENTES!\n");
                printf("O jogo terminou pois ninguém pode mais atacar.\n");
                printf("=================================\n");

                liberarMemoria(territorios, missaoJogador);
                return 0;
            }

            break;
        }

        case 2:
        {
            exibirMissao(missaoJogador);

            printf("\nVocê ainda não cumpriu a missão. Continue a lutar!\n");

            break;
        }

        case 0:
        {
            printf("Saindo...\n");
            printf("Jogo encerrado e memória liberada. Até a próxima!\n");

            liberarMemoria(territorios, missaoJogador);
            break; // Sai do loop principal, permitindo que o programa continue para liberar memória e encerrar corretamente;
        }

        default:
            printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0); // Continua o loop até que o usuário escolha sair (opção 0)

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

//  --- Função para cadastrar os territórios ---
void cadastrarTerritorios(Territorio *territorios, int n)
{
    // --- Laço Principal para cadastro dos territórios ---
    printf("\n==========================================\n");
    printf("    WAR ESTRUTURADO - CADASTRO INICIAL");
    printf("\n==========================================\n");

    for (int i = 0; i < n; i++)
    {
        printf("\n--- Cadastrando Território %d ---\n", i + 1);

        printf("Nome do Território: ");
        fgets(territorios[i].nome, TAM_NOME, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do Exército (ex: Azul, Verde): ");
        fgets(territorios[i].cor, TAM_COR, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        do
        {
            printf("Número de Tropas: ");

            if (scanf("%d", &territorios[i].tropas) != 1 || territorios[i].tropas <= 1)
            {
                printf("Entrada inválida. Tente novamente.\n");
                limparBuffer();
            }
            else
            {
                break;
            }

        } while (1);

        limparBuffer();

        territorios[i].conquistado = 0; // Inicializa o campo conquistado como 0 (não conquistado) para cada território cadastrado
    }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
// --- Função para liberar a memória alocada ---
void liberarMemoria(Territorio *territorios, char *missaoJogador)
{
    free(missaoJogador);
    free(territorios);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// --- Função para exibir o menu de ações ---
void exibirMenu()
{
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// --- Função para exibir o mapa do mundo ---
void exibirMapa(Territorio *territorios, int n)
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

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// --- Função para exibir a missão do jogador ---
void exibirMissao(const char *missao)
{
    printf("\n======================================\n");
    printf("SUA MISSÃO É:\n");
    printf("%s\n", missao);
    printf("======================================\n");
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
// --- Função para realizar a fase de ataque ---
int faseAtaque(Territorio *territorios, int numTerritorios)
{
    int atacante;
    int defensor;

    atacante = escolherAtacante(territorios, numTerritorios);

    if (atacante == -1)
    {
        return 0;
    }

    if (!existeDefensorValido(territorios, numTerritorios, atacante))
    {
        printf("\nNão existem defensores válidos. Encerrando fase de ataque.\n");
        return 0;
    }

    defensor = escolherDefensor(territorios, numTerritorios, atacante);

    atacar(&territorios[atacante], &territorios[defensor]);

    printf("\nPressione Enter para continuar para o próximo turno...");
    limparBuffer();

    exibirMapa(territorios, numTerritorios);

    return 1;
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// --- Função para realizar o ataque entre os territórios ---
void atacar(Territorio *atacante, Territorio *defensor)
{
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor)
    {
        printf("\nVITÓRIA DO ATAQUE!\n");

        int metade = defensor->tropas / 2;

        if (metade < 1)
        {
            metade = 1;
        }

        if (defensor->tropas < metade)
        {
            metade = defensor->tropas;
        }

        atacante->tropas += metade;
        defensor->tropas -= metade;

        if (metade == 1)
        {
            printf("Foi transferida 1 tropa do território %s para o território %s.\n", defensor->nome, atacante->nome);
        }
        else
        {
            printf("Foram transferidas %d tropas do território %s para o território %s.\n", metade, defensor->nome, atacante->nome);
        }

        if (defensor->tropas <= 1)
        {
            strcpy(defensor->cor, atacante->cor);
            defensor->conquistado = 1;

            printf("CONQUISTA! O território %s foi dominado pelo Exército %s!\n", defensor->nome, atacante->cor);
        }
    }

    else if (dadoDefensor > dadoAtacante)
    {
        if (atacante->tropas > 0)
            atacante->tropas--;

        printf("DEFESA VITORIOSA! O atacante %s perdeu 1 tropa.\n", atacante->nome);
    }
    else
    {
        printf("Empate! Nenhuma tropa perdida.\n");
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// --- Função para atribuir uma missão ao jogador ---
void atribuirMissao(char *destino, char *missoes[], int totalMissoes, Territorio *mapa, int tamanho)
{
    int sorteio;
    char *missaoEscolhida;

    do
    {
        sorteio = rand() % totalMissoes;
        missaoEscolhida = missoes[sorteio];

        if (strcmp(missaoEscolhida, "Eliminar exército Azul") == 0 &&
            !existeCor(mapa, tamanho, "Azul"))
            continue;

        if (strcmp(missaoEscolhida, "Eliminar exército Verde") == 0 &&
            !existeCor(mapa, tamanho, "Verde"))
            continue;

        strcpy(destino, missaoEscolhida);
        return;

    } while (1);
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// --- Função para verificar se a missão do jogador foi cumprida ---
int verificarMissao(char *missao, Territorio *mapa, int tamanho)
{
    int i;

    // MISSÃO 1
    if (strcmp(missao, "Conquistar 3 territórios") == 0)
    {
        int count = 0;
        for (i = 0; i < tamanho; i++)
        {
            if (mapa[i].conquistado > 0)
                count++;
        }

        return count >= 3;
    }

    // MISSÃO 2
    if (strcmp(missao, "Dominar todos os territórios do mapa") == 0)
    {
        char *corBase = mapa[0].cor;

        for (int i = 1; i < tamanho; i++)
        {
            if (strcmp(mapa[i].cor, corBase) != 0)
                return 0;
        }

        return 1;
    }

    // MISSÃO 3
    if (strcmp(missao, "Ter 1 território com 10 tropas") == 0)
    {
        for (int i = 0; i < tamanho; i++)
        {
            if (mapa[i].tropas >= 10)
            {
                return 1;
            }
        }

        return 0;
    }

    // MISSÃO 4
    if (strcmp(missao, "Eliminar exército Azul") == 0)
    {
        for (int i = 0; i < tamanho; i++)
        {
            if (strcmp(mapa[i].cor, "AZUL") == 0 ||
                strcmp(mapa[i].cor, "Azul") == 0 ||
                strcmp(mapa[i].cor, "azul") == 0)
                return 0;
        }

        return 1;
    }

    // MISSÃO 5
    if (strcmp(missao, "Eliminar exército Verde") == 0)
    {
        for (int i = 0; i < tamanho; i++)
        {
            if (strcmp(mapa[i].cor, "VERDE") == 0 ||
                strcmp(mapa[i].cor, "Verde") == 0 ||
                strcmp(mapa[i].cor, "verde") == 0)
                return 0;
        }

        return 1;
    }

    return 0;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

// --- Função para limpar o buffer de entrada ---
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// --- Função para escolher o território atacante ---
int escolherAtacante(Territorio *territorios, int numTerritorios)
{
    int atacante;
    do
    {
        do
        {
            printf("Escolha o território atacante (1 a %d, ou 0 para sair): ", numTerritorios);

            if (scanf("%d", &atacante) != 1 || atacante < 0 || atacante > numTerritorios)
            {
                printf("Entrada inválida. Tente novamente.\n");

                limparBuffer();
            }
            else
            {
                break;
            }

        } while (1);

        limparBuffer();

        if (atacante == 0)
        {
            printf("Saindo... Até a próxima!\n");
            return -1; // Sai do programa
        }

        atacante--; // Ajusta índice para 0-based

        if (atacante < 0 || atacante >= numTerritorios)
        {
            printf("Território inválido, tente novamente.\n");
        }
        else if (territorios[atacante].tropas <= 1)
        {
            printf("Tropas insuficientes para atacar, escolha outro território.\n");
        }

    } while (atacante < 0 ||
             atacante >= numTerritorios ||
             (atacante >= 0 && atacante < numTerritorios &&
              territorios[atacante].tropas <= 1));

    return atacante;
}

// --- Função para escolher o território defensor ---
int escolherDefensor(Territorio *territorios, int numTerritorios, int atacante)
{
    int defensor;
    do
    {
        do
        {
            printf("Escolha o território defensor (1 a %d): ", numTerritorios);

            if (scanf("%d", &defensor) != 1 || defensor < 0 || defensor > numTerritorios)
            {
                printf("Entrada inválida. Tente novamente.\n");

                limparBuffer();
            }
            else
            {
                break;
            }

        } while (1);

        limparBuffer();

        defensor--; // Ajusta índice para 0-based

        if (defensor < 0 || defensor >= numTerritorios)
        {
            printf("Território inválido! Tente novamente.\n");
        }
        else if (defensor == atacante)
        {
            printf("Nao pode atacar o mesmo território do atacante! Escolha outro.\n");
        }
        else if (strcmp(territorios[defensor].cor, territorios[atacante].cor) == 0)
        {
            printf("Nao pode atacar um território aliado! Escolha um inimigo.\n");
        }
        else if (territorios[defensor].tropas < 1)
        {
            printf("Território sem tropas, escolha outro defensor.\n");
        }

    } while (
        defensor < 0 ||
        defensor >= numTerritorios ||
        defensor == atacante ||
        (defensor >= 0 && defensor < numTerritorios && territorios[defensor].tropas < 1) ||
        (defensor >= 0 && defensor < numTerritorios && strcmp(territorios[defensor].cor, territorios[atacante].cor) == 0));

    return defensor;
}

// --- Função para verificar se existe um defensor válido ---
int existeDefensorValido(Territorio *territorios, int numTerritorios, int atacante)
{
    for (int i = 0; i < numTerritorios; i++)
    {
        if (i != atacante &&
            territorios[i].tropas > 0 &&
            strcmp(territorios[i].cor, territorios[atacante].cor) != 0)
        {
            return 1;
        }
    }

    return 0;
}

// --- Função para verificar se o jogo deve terminar por falta de tropas suficientes para realizar ataques ---
int verificarFimPorTropas(Territorio *mapa, int tamanho)
{
    int existeAtacante = 0;

    for (int i = 0; i < tamanho; i++)
    {
        if (mapa[i].tropas > 1)
        {
            existeAtacante = 1;
            break; // Ainda pode jogar
        }
    }

    return !existeAtacante;
}

// --- Função para verificar se existe um exército de determinada cor no mapa ---
int existeCor(Territorio *mapa, int tamanho, const char *cor)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (
            (strcmp(cor, "Azul") == 0 &&
             (strcmp(mapa[i].cor, "AZUL") == 0 ||
              strcmp(mapa[i].cor, "Azul") == 0 ||
              strcmp(mapa[i].cor, "azul") == 0)) ||
            (strcmp(cor, "Verde") == 0 &&
             (strcmp(mapa[i].cor, "VERDE") == 0 ||
              strcmp(mapa[i].cor, "Verde") == 0 ||
              strcmp(mapa[i].cor, "verde") == 0)))
        {
            return 1;
        }
    }

    return 0;
}
