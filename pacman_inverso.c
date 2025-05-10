#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define LARGURA 1000
#define ALTURA 800
#define TAMANHO_CELULA 35  // Células maiores para labirinto mais espaçado
#define LINHAS (ALTURA / TAMANHO_CELULA)
#define COLUNAS (LARGURA / TAMANHO_CELULA)

// Estrutura para o nó da lista duplamente encadeada
typedef struct Node {
    int x, y;
    int tipo; // 0: caminho vazio, 1: parede, 2: comida, 3: portal, 4: fruta rosa
    struct Node* prox;
    struct Node* ant;
} Node;

// Estrutura para o portal
typedef struct {
    int x_origem;
    int y_origem;
    int x_destino;
    int y_destino;
} Portal;

// Estrutura para a lista duplamente encadeada
typedef struct {
    Node* primeiro;
    Node* ultimo;
    int tamanho;
    Portal portal; // Informações do portal
} ListaDupla;

// Cores disponíveis para o fantasma
typedef enum {
    VERMELHO,
    LARANJA,
    AZUL,
    ROSA,
    VERDE,
    ROXO,
    NUM_CORES
} CorFantasma;

// Nomes das cores para exibição
const char* NOMES_CORES[] = {
    "Vermelho",
    "Laranja",
    "Azul",
    "Rosa",
    "Verde",
    "Roxo"
};

// Valores RGB das cores
const COLORREF VALORES_CORES[] = {
    RGB(255, 0, 0),     // Vermelho
    RGB(255, 165, 0),   // Laranja
    RGB(0, 0, 255),     // Azul
    RGB(255, 192, 203), // Rosa
    RGB(0, 255, 0),     // Verde
    RGB(128, 0, 128)    // Roxo
};

// Variáveis globais
int fantasma_x = 2, fantasma_y = 2;  // Posição inicial do fantasma ajustada
int pacman_x = 15, pacman_y = 15;    // Posição inicial do Pac-Man ajustada
int direcao_pacman = 0; // 0: direita, 1: cima, 2: esquerda, 3: baixo
CorFantasma cor_fantasma = VERMELHO; // Cor inicial do fantasma
int movimento_fantasma = 0; // Contador para retardar o movimento do fantasma
BOOL jogoIniciado = FALSE; // Flag para controlar o início do jogo
BOOL pacman_poderoso = FALSE; // Flag para indicar se o Pac-Man comeu uma fruta rosa
DWORD tempoPoder = 0; // Tempo em que o poder começou
int proximaFruta = 0; // Contador para próxima aparição de fruta rosa

ListaDupla mapa;
int pontuacao = 0;
int comidas_restantes = 0;

// Definição de IDs para os botões do menu
#define ID_BOTAO_VERMELHO 101
#define ID_BOTAO_LARANJA  102
#define ID_BOTAO_AZUL     103
#define ID_BOTAO_ROSA     104
#define ID_BOTAO_VERDE    105
#define ID_BOTAO_ROXO     106
#define ID_BOTAO_INICIAR  107

// Handles para botões e elementos de interface
HWND botoes[NUM_CORES];
HWND botaoIniciar;

// Funções da lista duplamente encadeada
void inicializarLista(ListaDupla* lista) {
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    lista->tamanho = 0;
}

Node* criarNo(int x, int y, int tipo) {
    Node* novo = (Node*)malloc(sizeof(Node));
    if (novo) {
        novo->x = x;
        novo->y = y;
        novo->tipo = tipo;
        novo->prox = NULL;
        novo->ant = NULL;
    }
    return novo;
}

void inserirFim(ListaDupla* lista, int x, int y, int tipo) {
    Node* novo = criarNo(x, y, tipo);
    if (novo) {
        if (lista->primeiro == NULL) {
            lista->primeiro = novo;
            lista->ultimo = novo;
            // Torna a lista circular
            novo->prox = novo;
            novo->ant = novo;
        } else {
            novo->ant = lista->ultimo;
            novo->prox = lista->primeiro;
            lista->ultimo->prox = novo;
            lista->primeiro->ant = novo;
            lista->ultimo = novo;
        }
        lista->tamanho++;
        
        if (tipo == 2) {
            comidas_restantes++;
        }
    }
}

Node* buscarNo(ListaDupla* lista, int x, int y) {
    if (lista->primeiro == NULL) {
        return NULL;
    }
    
    Node* atual = lista->primeiro;
    do {
        if (atual->x == x && atual->y == y)
            return atual;
        atual = atual->prox;
    } while (atual != lista->primeiro);
    
    return NULL;
}

// Função auxiliar para verificar se é parede
int ehParede(int x, int y) {
    // Bordas do mapa
    if (x <= 0 || y <= 0 || x >= COLUNAS - 1 || y >= LINHAS - 1) {
        return 1;
    }
    
    // Paredes internas do labirinto - mais espaçadas
    return ((x == 5 && y >= 4 && y <= 12) || 
            (x == 10 && y >= 6 && y <= 16) ||
            (x >= 8 && x <= 15 && y == 4) ||
            (x >= 6 && x <= 13 && y == 10) ||
            (x >= 18 && x <= 24 && y == 8) ||
            (x == 18 && y >= 10 && y <= 18) ||
            (x >= 10 && x <= 20 && y == 15) ||
            (x == 15 && y >= 18 && y <= 22));
}

// Função para calcular a distância entre dois pontos
int calcularDistancia(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y2 - y1);
}

// Função para encontrar a comida mais próxima
Node* encontrarComidaProxima(int x, int y) {
    if (mapa.primeiro == NULL) return NULL;
    
    Node* atual = mapa.primeiro;
    Node* comidaProxima = NULL;
    Node* frutaProxima = NULL;
    int menorDistanciaComida = INT_MAX;
    int menorDistanciaFruta = INT_MAX;
    
    do {
        if (atual->tipo == 2) { // Se for comida normal
            int distancia = calcularDistancia(x, y, atual->x, atual->y);
            if (distancia < menorDistanciaComida) {
                menorDistanciaComida = distancia;
                comidaProxima = atual;
            }
        } else if (atual->tipo == 4) { // Se for fruta rosa
            int distancia = calcularDistancia(x, y, atual->x, atual->y);
            if (distancia < menorDistanciaFruta) {
                menorDistanciaFruta = distancia;
                frutaProxima = atual;
            }
        }
        atual = atual->prox;
    } while (atual != mapa.primeiro);
    
    // Prioriza frutas rosas sobre comidas normais
    if (frutaProxima != NULL) {
        return frutaProxima;
    }
    
    return comidaProxima;
}

// Função para mover o Pac-Man
void moverPacman() {
    // Verifica se o poder da fruta rosa expirou
    if (pacman_poderoso) {
        DWORD tempoAtual = GetTickCount();
        if (tempoAtual - tempoPoder > 3000) { // 3 segundos
            pacman_poderoso = FALSE;
        }
    }

    // Verifica se o fantasma está próximo (distância de 5 células)
    int distanciaFantasma = calcularDistancia(pacman_x, pacman_y, fantasma_x, fantasma_y);
    int nova_x = pacman_x;
    int nova_y = pacman_y;
    int melhor_direcao = direcao_pacman;
    
    // Verifica se o pacman está em modo poderoso
    if (pacman_poderoso) {
        // Vai em direção ao fantasma quando está poderoso
        if (fantasma_x > pacman_x && !ehParede(pacman_x + 1, pacman_y)) {
            melhor_direcao = 0; // Direita
            nova_x = pacman_x + 1;
        } else if (fantasma_x < pacman_x && !ehParede(pacman_x - 1, pacman_y)) {
            melhor_direcao = 2; // Esquerda
            nova_x = pacman_x - 1;
        } else if (fantasma_y > pacman_y && !ehParede(pacman_x, pacman_y + 1)) {
            melhor_direcao = 3; // Baixo
            nova_y = pacman_y + 1;
        } else if (fantasma_y < pacman_y && !ehParede(pacman_x, pacman_y - 1)) {
            melhor_direcao = 1; // Cima
            nova_y = pacman_y - 1;
        } else {
            // Tenta encontrar um caminho alternativo
            int direcoes_tentadas[4] = {0};
            int contador_tentativas = 0;
            
            while (contador_tentativas < 4) {
                int dir = rand() % 4;
                if (direcoes_tentadas[dir]) continue;
                direcoes_tentadas[dir] = 1;
                contador_tentativas++;
                
                nova_x = pacman_x;
                nova_y = pacman_y;
                
                switch (dir) {
                    case 0: nova_x += 1; break; // Direita
                    case 1: nova_y -= 1; break; // Cima
                    case 2: nova_x -= 1; break; // Esquerda
                    case 3: nova_y += 1; break; // Baixo
                }
                
                if (!ehParede(nova_x, nova_y)) {
                    melhor_direcao = dir;
                    break;
                }
            }
        }
    }
    // Se o fantasma estiver próximo e o pacman não estiver poderoso, tenta fugir
    else if (distanciaFantasma <= 5) {
        // Tenta encontrar a direção que aumenta a distância do fantasma
        int maiorDistancia = -1;
        
        // Testa todas as direções possíveis
        for (int dir = 0; dir < 4; dir++) {
            int test_x = pacman_x;
            int test_y = pacman_y;
            
            switch (dir) {
                case 0: test_x += 1; break; // Direita
                case 1: test_y -= 1; break; // Cima
                case 2: test_x -= 1; break; // Esquerda
                case 3: test_y += 1; break; // Baixo
            }
            
            // Verifica se a posição é válida
            if (!ehParede(test_x, test_y)) {
                int novaDistancia = calcularDistancia(test_x, test_y, fantasma_x, fantasma_y);
                if (novaDistancia > maiorDistancia) {
                    maiorDistancia = novaDistancia;
                    melhor_direcao = dir;
                    nova_x = test_x;
                    nova_y = test_y;
                }
            }
        }
    } else {
        // Se não estiver em perigo, procura comida ou frutas
        Node* comidaProxima = encontrarComidaProxima(pacman_x, pacman_y);
        
        if (comidaProxima) {
            // Algoritmo para contornar paredes e se aproximar do alvo
            int melhor_movimento = -1;
            int melhor_distancia = INT_MAX;
            
            // Verifica todas as direções possíveis
            for (int dir = 0; dir < 4; dir++) {
                int test_x = pacman_x;
                int test_y = pacman_y;
                
                switch (dir) {
                    case 0: test_x += 1; break; // Direita
                    case 1: test_y -= 1; break; // Cima
                    case 2: test_x -= 1; break; // Esquerda
                    case 3: test_y += 1; break; // Baixo
                }
                
                // Verifica se pode mover para esta posição
                if (!ehParede(test_x, test_y)) {
                    int distancia = calcularDistancia(test_x, test_y, comidaProxima->x, comidaProxima->y);
                    
                    // Se estiver em um beco sem saída, evita voltar nessa direção
                    if (direcao_pacman == (dir + 2) % 4) {
                        distancia += 2; // Penalidade por dar meia volta
                    }
                    
                    if (distancia < melhor_distancia) {
                        melhor_distancia = distancia;
                        melhor_movimento = dir;
                    }
                }
            }
            
            if (melhor_movimento != -1) {
                melhor_direcao = melhor_movimento;
                switch (melhor_direcao) {
                    case 0: nova_x = pacman_x + 1; break; // Direita
                    case 1: nova_y = pacman_y - 1; break; // Cima
                    case 2: nova_x = pacman_x - 1; break; // Esquerda
                    case 3: nova_y = pacman_y + 1; break; // Baixo
                }
            }
        }
    }
    
    // Atualiza a direção e posição
    direcao_pacman = melhor_direcao;
    
    // Verifica colisão com paredes e portal
    Node* destino = buscarNo(&mapa, nova_x, nova_y);
    if (destino) {
        if (destino->tipo == 1) { // É parede
            // Se encontrar parede, tenta outra direção
            direcao_pacman = (direcao_pacman + 1) % 4;
            return;
        }
        
        // Se encontrar um portal
        if (destino->tipo == 3) {
            if (nova_x == mapa.portal.x_origem && nova_y == mapa.portal.y_origem) {
                pacman_x = mapa.portal.x_destino;
                pacman_y = mapa.portal.y_destino;
            } else {
                pacman_x = mapa.portal.x_origem;
                pacman_y = mapa.portal.y_origem;
            }
        } else {
            pacman_x = nova_x;
            pacman_y = nova_y;

            // Se houver comida, come
            if (destino->tipo == 2) {
                destino->tipo = 0;  // Remove a comida
                pontuacao += 10;
                comidas_restantes--;
            } 
            // Se for fruta rosa, come e fica poderoso
            else if (destino->tipo == 4) {
                destino->tipo = 0;  // Remove a fruta
                pacman_poderoso = TRUE;
                tempoPoder = GetTickCount(); // Marca o tempo atual
                pontuacao += 50;
            }
        }
    } else if (!ehParede(nova_x, nova_y)) {
        pacman_x = nova_x;
        pacman_y = nova_y;
    }
}

// Função para criar o mapa
void criarMapa() {
    inicializarLista(&mapa);
    
    // Configuração do portal - posições mais estratégicas
    mapa.portal.x_origem = 3;
    mapa.portal.y_origem = 3;
    mapa.portal.x_destino = COLUNAS - 4;
    mapa.portal.y_destino = LINHAS - 4;

    // Array para marcar posições ocupadas
    int posicoes_ocupadas[COLUNAS][LINHAS] = {0};

    // Marca todas as paredes
    for (int x = 0; x < COLUNAS; x++) {
        for (int y = 0; y < LINHAS; y++) {
            if (ehParede(x, y)) {
                inserirFim(&mapa, x, y, 1); // Parede
                posicoes_ocupadas[x][y] = 1;
            }
        }
    }

    // Adiciona os portais
    inserirFim(&mapa, mapa.portal.x_origem, mapa.portal.y_origem, 3);
    inserirFim(&mapa, mapa.portal.x_destino, mapa.portal.y_destino, 3);
    posicoes_ocupadas[mapa.portal.x_origem][mapa.portal.y_origem] = 1;
    posicoes_ocupadas[mapa.portal.x_destino][mapa.portal.y_destino] = 1;

    // Marca posições iniciais do Pac-Man e fantasma
    posicoes_ocupadas[pacman_x][pacman_y] = 1;
    posicoes_ocupadas[fantasma_x][fantasma_y] = 1;

    // Agora adiciona comidas apenas em posições vazias e que NÃO são paredes
    for (int x = 1; x < COLUNAS - 1; x++) {
        for (int y = 1; y < LINHAS - 1; y++) {
            if (!posicoes_ocupadas[x][y] && !ehParede(x, y)) {
                inserirFim(&mapa, x, y, 2); // Comida
            }
        }
    }
    
    // Inicializa o contador de frutas
    proximaFruta = 100; // Primeira fruta aparecerá após um tempo
}

// Função para gerar frutas rosas aleatoriamente
void gerarFrutaRosa() {
    // Encontra uma posição vazia para colocar a fruta
    int x, y;
    BOOL posicaoValida = FALSE;
    int tentativas = 0;
    
    while (!posicaoValida && tentativas < 100) {
        x = rand() % (COLUNAS - 2) + 1;
        y = rand() % (LINHAS - 2) + 1;
        
        // Verifica se a posição é válida (não é parede, comida, portal, etc.)
        if (!ehParede(x, y)) {
            Node* no = buscarNo(&mapa, x, y);
            if (no && no->tipo == 0) { // Posição vazia
                no->tipo = 4; // Converte para fruta rosa
                posicaoValida = TRUE;
            } else if (!no) {
                // Se o nó não existe, cria um novo para a fruta
                inserirFim(&mapa, x, y, 4);
                posicaoValida = TRUE;
            }
        }
        tentativas++;
    }
    
    // Reseta o contador de próxima fruta (entre 10 e 20 segundos)
    proximaFruta = 100 + (rand() % 100);
}

// Função para iniciar o jogo
void iniciarJogo(HWND hwnd) {
    jogoIniciado = TRUE;
    
    // Remove os botões do menu
    for (int i = 0; i < NUM_CORES; i++) {
        DestroyWindow(botoes[i]);
    }
    DestroyWindow(botaoIniciar);
    
    // Inicializa o mapa
    criarMapa();
    
    // Configura um timer para mover o Pac-Man
    SetTimer(hwnd, 1, 100, NULL);
    
    // Atualiza a janela
    char titulo[100];
    sprintf(titulo, "Nam-Pac - Jogo em andamento - Fantasma: %s", NOMES_CORES[cor_fantasma]);
    SetWindowText(hwnd, titulo);
    InvalidateRect(hwnd, NULL, TRUE);
}

// Função para criar os botões do menu
void criarMenu(HWND hwnd) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    int larguraBotao = 150;
    int alturaBotao = 40;
    int espacamento = 20; // Diminuído o espaçamento entre botões
    int xInicial = (LARGURA - larguraBotao) / 2;
    int yInicial = 200; // Posição inicial dos botões mais perto do topo
    
    // Cria os botões para cada cor
    for (int i = 0; i < NUM_CORES; i++) {
        botoes[i] = CreateWindow(
            "BUTTON", NOMES_CORES[i],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            xInicial, yInicial + (i * (alturaBotao + espacamento)),
            larguraBotao, alturaBotao,
            hwnd, (HMENU)(LONG_PTR)(ID_BOTAO_VERMELHO + i), hInstance, NULL
        );
    }
    
    // Cria o botão de iniciar
    botaoIniciar = CreateWindow(
        "BUTTON", "Iniciar Jogo",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        xInicial, yInicial + (NUM_CORES * (alturaBotao + espacamento)),
        larguraBotao, alturaBotao,
        hwnd, (HMENU)(LONG_PTR)ID_BOTAO_INICIAR, hInstance, NULL
    );
    
    // Define o primeiro botão (vermelho) como selecionado por padrão
    SendMessage(botoes[0], BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE);
}

// Procedimento para desenhar
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        // Inicializa o menu
        criarMenu(hwnd);
        break;

    case WM_COMMAND:
        // Verifica qual botão foi pressionado
        if (LOWORD(wParam) >= ID_BOTAO_VERMELHO && LOWORD(wParam) <= ID_BOTAO_ROXO) {
            // Atualiza a cor do fantasma
            cor_fantasma = (CorFantasma)(LOWORD(wParam) - ID_BOTAO_VERMELHO);
            
            // Destaca o botão selecionado e desmarca os outros
            for (int i = 0; i < NUM_CORES; i++) {
                SendMessage(botoes[i], BM_SETSTYLE, 
                           (i == cor_fantasma) ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON, TRUE);
            }
            
            // Atualiza a mensagem na barra de título
            char titulo[100];
            sprintf(titulo, "Nam-Pac - Cor selecionada: %s", NOMES_CORES[cor_fantasma]);
            SetWindowText(hwnd, titulo);
        } 
        else if (LOWORD(wParam) == ID_BOTAO_INICIAR) {
            // Inicia o jogo com timer mais rápido (100ms ao invés de 200ms)
            iniciarJogo(hwnd);
            SetTimer(hwnd, 1, 100, NULL); // Timer mais rápido para o Pac-Man
        }
        break;

    case WM_TIMER:
        if (!jogoIniciado) break;
        
        // Atualiza o contador para gerar frutas rosas
        proximaFruta--;
        if (proximaFruta <= 0) {
            gerarFrutaRosa();
        }
        
        // Move o Pac-Man
        moverPacman();
        
        // Verifica colisão entre Pac-Man e fantasma
        if (fantasma_x == pacman_x && fantasma_y == pacman_y) {
            if (pacman_poderoso) {
                // Pacman venceu ao pegar o fantasma quando poderoso
                KillTimer(hwnd, 1);
                MessageBox(hwnd, "Vitória! O Pac-Man capturou o fantasma!", "Fim de Jogo", MB_OK);
                PostQuitMessage(0);
            } else {
                // Fantasma capturou o Pacman
                KillTimer(hwnd, 1);
                MessageBox(hwnd, "Game Over! O fantasma capturou o Pac-Man!", "Fim de Jogo", MB_OK);
                PostQuitMessage(0);
            }
        }
        
        // Verifica se todas as comidas foram consumidas
        if (comidas_restantes == 0) {
            KillTimer(hwnd, 1);
            MessageBox(hwnd, "Vitória! O Pac-Man consumiu todas as comidas!", "Fim de Jogo", MB_OK);
            PostQuitMessage(0);
        }
        
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_KEYDOWN:
        if (!jogoIniciado) break;
        
        // Troca a cor do fantasma se pressionar 'C'
        if (wParam == 'C' || wParam == 'c') {
            cor_fantasma = (cor_fantasma + 1) % NUM_CORES;
            char titulo[100];
            sprintf(titulo, "Nam-Pac - Jogo em andamento - Fantasma: %s", NOMES_CORES[cor_fantasma]);
            SetWindowText(hwnd, titulo);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        
        // Controla o movimento do fantasma (mais lento)
        movimento_fantasma++;
        if (movimento_fantasma < 2) { // Movimento a cada 2 pressionamentos
            break;
        }
        movimento_fantasma = 0;

        // Movimenta o fantasma
        int nova_x = fantasma_x;
        int nova_y = fantasma_y;
        
        switch (wParam) {
        case VK_LEFT:  nova_x -= 1; break;
        case VK_RIGHT: nova_x += 1; break;
        case VK_UP:    nova_y -= 1; break;
        case VK_DOWN:  nova_y += 1; break;
        }
        
        // Verifica colisão com paredes
        if (ehParede(nova_x, nova_y)) {
            break; // Se for parede, não move
        }
        
        // Verifica colisão com paredes e portal
        Node* destino = buscarNo(&mapa, nova_x, nova_y);
        if (destino) {
            if (destino->tipo == 1) { // É parede
                break; // Não permite atravessar parede
            }
            
            // Se encontrar um portal
            if (destino->tipo == 3) {
                if (nova_x == mapa.portal.x_origem && nova_y == mapa.portal.y_origem) {
                    // Teleporta da origem para o destino
                    fantasma_x = mapa.portal.x_destino;
                    fantasma_y = mapa.portal.y_destino;
                } else {
                    // Teleporta do destino para a origem
                    fantasma_x = mapa.portal.x_origem;
                    fantasma_y = mapa.portal.y_origem;
                }
            } else {
                fantasma_x = nova_x;
                fantasma_y = nova_y;
            }
            
            // Verifica colisão entre Pac-Man e fantasma
            if (fantasma_x == pacman_x && fantasma_y == pacman_y) {
                if (pacman_poderoso) {
                    // Pacman venceu ao pegar o fantasma quando poderoso
                    KillTimer(hwnd, 1);
                    MessageBox(hwnd, "Vitória! O Pac-Man capturou o fantasma!", "Fim de Jogo", MB_OK);
                    PostQuitMessage(0);
                } else {
                    // Fantasma capturou o Pacman
                    KillTimer(hwnd, 1);
                    MessageBox(hwnd, "Game Over! O fantasma capturou o Pac-Man!", "Fim de Jogo", MB_OK);
                    PostQuitMessage(0);
                }
            }
        } else if (!ehParede(nova_x, nova_y)) {
            // Se não há nó no destino mas não é parede, pode mover
            fantasma_x = nova_x;
            fantasma_y = nova_y;
            
            // Verifica colisão entre Pac-Man e fantasma
            if (fantasma_x == pacman_x && fantasma_y == pacman_y) {
                if (pacman_poderoso) {
                    // Pacman venceu ao pegar o fantasma quando poderoso
                    KillTimer(hwnd, 1);
                    MessageBox(hwnd, "Vitória! O Pac-Man capturou o fantasma!", "Fim de Jogo", MB_OK);
                    PostQuitMessage(0);
                } else {
                    // Fantasma capturou o Pacman
                    KillTimer(hwnd, 1);
                    MessageBox(hwnd, "Game Over! O fantasma capturou o Pac-Man!", "Fim de Jogo", MB_OK);
                    PostQuitMessage(0);
                }
            }
        }
        
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        // Pinta o fundo
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &ps.rcPaint, blackBrush);
        DeleteObject(blackBrush);
        
        if (jogoIniciado) {
            // Desenha o mapa
            Node* atual = mapa.primeiro;
            do {
                RECT celula = { 
                    atual->x * TAMANHO_CELULA, 
                    atual->y * TAMANHO_CELULA, 
                    (atual->x + 1) * TAMANHO_CELULA, 
                    (atual->y + 1) * TAMANHO_CELULA 
                };
                
                // Desenha com base no tipo
                switch (atual->tipo) {
                case 0: // Caminho vazio
                    // Não desenha nada (fica preto)
                    break;
                case 1: { // Parede
                    HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 180));
                    FillRect(hdc, &celula, blueBrush);
                    DeleteObject(blueBrush);
                    break;
                }
                case 2: { // Comida
                    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
                    // Desenhar um círculo pequeno para a comida
                    int raio = TAMANHO_CELULA / 6;
                    int centroX = atual->x * TAMANHO_CELULA + TAMANHO_CELULA / 2;
                    int centroY = atual->y * TAMANHO_CELULA + TAMANHO_CELULA / 2;
                    SelectObject(hdc, whiteBrush);
                    Ellipse(hdc, centroX - raio, centroY - raio, centroX + raio, centroY + raio);
                    DeleteObject(whiteBrush);
                    break;
                }
                case 3: { // Portal
                    HBRUSH portalBrush = CreateSolidBrush(RGB(255, 0, 255));
                    FillRect(hdc, &celula, portalBrush);
                    DeleteObject(portalBrush);
                    break;
                }
                case 4: { // Fruta rosa
                    HBRUSH rosaBrush = CreateSolidBrush(RGB(255, 20, 147)); // Rosa forte
                    // Desenhar um círculo maior para a fruta
                    int raio = TAMANHO_CELULA / 3;
                    int centroX = atual->x * TAMANHO_CELULA + TAMANHO_CELULA / 2;
                    int centroY = atual->y * TAMANHO_CELULA + TAMANHO_CELULA / 2;
                    SelectObject(hdc, rosaBrush);
                    Ellipse(hdc, centroX - raio, centroY - raio, centroX + raio, centroY + raio);
                    DeleteObject(rosaBrush);
                    break;
                }
                }
                
                atual = atual->prox;
            } while (atual != mapa.primeiro);
            
            // Desenha o Pac-Man (amarelo) como um círculo
            HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
            int centroX = pacman_x * TAMANHO_CELULA + TAMANHO_CELULA / 2;
            int centroY = pacman_y * TAMANHO_CELULA + TAMANHO_CELULA / 2;
            int raio = TAMANHO_CELULA / 2 - 2; // Um pouco menor que a célula
            
            SelectObject(hdc, yellowBrush);
            Ellipse(hdc, centroX - raio, centroY - raio, centroX + raio, centroY + raio);
            DeleteObject(yellowBrush);
            
            // Desenha o fantasma (com a cor selecionada) como um círculo
            HBRUSH fantBrush = CreateSolidBrush(VALORES_CORES[cor_fantasma]);
            centroX = fantasma_x * TAMANHO_CELULA + TAMANHO_CELULA / 2;
            centroY = fantasma_y * TAMANHO_CELULA + TAMANHO_CELULA / 2;
            
            SelectObject(hdc, fantBrush);
            Ellipse(hdc, centroX - raio, centroY - raio, centroX + raio, centroY + raio);
            
            // Desenha os olhos do fantasma
            HBRUSH olhoBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, olhoBrush);
            
            // Olho esquerdo
            int raioOlho = TAMANHO_CELULA / 10;
            int olhoEsqX = centroX - raio/2;
            int olhoY = centroY - raio/4;
            Ellipse(hdc, olhoEsqX - raioOlho, olhoY - raioOlho, 
                   olhoEsqX + raioOlho, olhoY + raioOlho);
            
            // Olho direito
            int olhoDirX = centroX + raio/2;
            Ellipse(hdc, olhoDirX - raioOlho, olhoY - raioOlho, 
                   olhoDirX + raioOlho, olhoY + raioOlho);
            
            // Pupila dos olhos (preta)
            HBRUSH pupilaBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, pupilaBrush);
            
            int raioPupila = TAMANHO_CELULA / 20;
            Ellipse(hdc, olhoEsqX - raioPupila, olhoY - raioPupila, 
                   olhoEsqX + raioPupila, olhoY + raioPupila);
            Ellipse(hdc, olhoDirX - raioPupila, olhoY - raioPupila, 
                   olhoDirX + raioPupila, olhoY + raioPupila);
            
            DeleteObject(fantBrush);
            DeleteObject(olhoBrush);
            DeleteObject(pupilaBrush);
            
            // Desenha a pontuação e status do poder
            char scoreText[150];
            if (pacman_poderoso) {
                sprintf(scoreText, "Pontuação: %d | Fantasma: %s | PACMAN PODEROSO!", pontuacao, NOMES_CORES[cor_fantasma]);
            } else {
                sprintf(scoreText, "Pontuação: %d | Fantasma: %s", pontuacao, NOMES_CORES[cor_fantasma]);
            }
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            TextOut(hdc, 10, 10, scoreText, strlen(scoreText));
        } else {
            // Desenha o menu inicial
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 0)); // Texto amarelo
            
            // Título principal grande
            HFONT fonteTitulo = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                         DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                                         CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                                         DEFAULT_PITCH | FF_SWISS, "Arial");
            HFONT fonteAntiga = (HFONT)SelectObject(hdc, fonteTitulo);
            
            char tituloJogo[] = "Nam-Pac";
            RECT areaTexto = {0, 50, LARGURA, 120}; // Ajustado a posição do título
            DrawText(hdc, tituloJogo, -1, &areaTexto, DT_CENTER | DT_SINGLELINE);
            
            // Subtítulo
            SelectObject(hdc, fonteAntiga);
            DeleteObject(fonteTitulo);
            
            HFONT fonteSubtitulo = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                                            CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                                            DEFAULT_PITCH | FF_SWISS, "Arial");
            fonteAntiga = (HFONT)SelectObject(hdc, fonteSubtitulo);
            
            char subtitulo[] = "Selecione a cor do fantasma e inicie o jogo";
            areaTexto.top = 130; // Ajustado a posição do subtítulo
            areaTexto.bottom = 170;
            DrawText(hdc, subtitulo, -1, &areaTexto, DT_CENTER | DT_SINGLELINE);
            
            SelectObject(hdc, fonteAntiga);
            DeleteObject(fonteSubtitulo);
        }
        
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        // Libera a memória da lista
        if (mapa.primeiro) {
            Node* atual = mapa.primeiro;
            do {
                Node* temp = atual;
                atual = atual->prox;
                free(temp);
                if (atual == mapa.primeiro) break;
            } while (1);
        }
        
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    const char CLASS_NAME[] = "PacManInverso";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Erro ao registrar a classe da janela!", "Erro", MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Nam-Pac - Cor selecionada: Vermelho", // Título inicial mostrando a cor padrão
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, LARGURA + 16, ALTURA + 38,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Erro ao criar a janela!", "Erro", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}