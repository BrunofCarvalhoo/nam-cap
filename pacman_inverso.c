#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <string.h>

#define LARGURA 800
#define ALTURA 600
#define TAMANHO_CELULA 40
#define LINHAS (ALTURA / TAMANHO_CELULA)
#define COLUNAS (LARGURA / TAMANHO_CELULA)

// Estrutura para o nó da lista duplamente encadeada
typedef struct Node {
    int x, y;
    int tipo; // 0: caminho vazio, 1: parede, 2: comida
    struct Node* prox;
    struct Node* ant;
} Node;

// Estrutura para a lista duplamente encadeada
typedef struct {
    Node* primeiro;
    Node* ultimo;
    int tamanho;
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
int fantasma_x = 1, fantasma_y = 1;
int pacman_x = 10, pacman_y = 10;
int direcao_pacman = 0; // 0: direita, 1: cima, 2: esquerda, 3: baixo
CorFantasma cor_fantasma = VERMELHO; // Cor inicial do fantasma

ListaDupla mapa;
int pontuacao = 0;
int comidas_restantes = 0;

// Buffer para resposta da API
struct MemoryStruct {
    char* memory;
    size_t size;
};

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
        } else {
            novo->ant = lista->ultimo;
            lista->ultimo->prox = novo;
            lista->ultimo = novo;
        }
        lista->tamanho++;
        
        // Se for uma célula do tipo comida, incrementa o contador
        if (tipo == 2) {
            comidas_restantes++;
        }
    }
}

Node* buscarNo(ListaDupla* lista, int x, int y) {
    Node* atual = lista->primeiro;
    while (atual) {
        if (atual->x == x && atual->y == y)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Callback para a resposta da API
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("Sem memória suficiente!\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Função para consultar a API do Gemini
int consultarGeminiAPI(int px, int py, int fx, int fy, ListaDupla* mapa) {
    CURL* curl;
    CURLcode res;
    struct MemoryStruct chunk;
    int direcao = 0; // Direção padrão (direita)

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        char url[512];
        // Usando a chave da API do Gemini fornecida
        sprintf(url, 
            "https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent?key=AIzaSyAgI1cbnClUVnY8ofajlXjV7ZWqkenTuMI");

        // Prepara os dados do mapa para enviar na consulta
        char mapaStr[4096] = "";
        Node* atual = mapa->primeiro;
        
        while (atual) {
            char nodeInfo[64];
            sprintf(nodeInfo, "%d,%d,%d;", atual->x, atual->y, atual->tipo);
            strcat(mapaStr, nodeInfo);
            atual = atual->prox;
        }

        // Cria o corpo da requisição JSON
        char postFields[8192];
        sprintf(postFields, 
            "{\"contents\":[{\"parts\":[{\"text\":\"Você é o Pac-Man em um jogo. "
            "Sua posição atual é (%d,%d). O fantasma está em (%d,%d). "
            "O mapa é representado como 'x,y,tipo' onde tipo 0 é caminho vazio, "
            "tipo 1 é parede e tipo 2 é comida. Mapa: %s. "
            "Escolha uma direção para se mover. Responda APENAS com um número: "
            "0 para direita, 1 para cima, 2 para esquerda, 3 para baixo.\"}]}]}", 
            px, py, fx, fy, mapaStr);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() falhou: %s\n", curl_easy_strerror(res));
        } else {
            // Extrai a direção da resposta Gemini
            char* responseText = chunk.memory;
            
            if (strstr(responseText, "\"0\"") || strstr(responseText, "\"direita\""))
                direcao = 0;
            else if (strstr(responseText, "\"1\"") || strstr(responseText, "\"cima\""))
                direcao = 1;
            else if (strstr(responseText, "\"2\"") || strstr(responseText, "\"esquerda\""))
                direcao = 2;
            else if (strstr(responseText, "\"3\"") || strstr(responseText, "\"baixo\""))
                direcao = 3;
            
            // Busca números na resposta como backup
            if (strstr(responseText, "0")) direcao = 0;
            else if (strstr(responseText, "1")) direcao = 1;
            else if (strstr(responseText, "2")) direcao = 2;
            else if (strstr(responseText, "3")) direcao = 3;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    free(chunk.memory);

    return direcao;
}

// Função para mover o Pac-Man (agora com implementação real do Gemini)
void moverPacman() {
    int nova_x = pacman_x;
    int nova_y = pacman_y;

    // Obter direção do Gemini API
    direcao_pacman = consultarGeminiAPI(pacman_x, pacman_y, fantasma_x, fantasma_y, &mapa);

    // Aplica a direção escolhida
    switch (direcao_pacman) {
    case 0: nova_x += 1; break; // Direita
    case 1: nova_y -= 1; break; // Cima
    case 2: nova_x -= 1; break; // Esquerda
    case 3: nova_y += 1; break; // Baixo
    }

    // Verifica colisão com paredes
    Node* destino = buscarNo(&mapa, nova_x, nova_y);
    if (destino && destino->tipo != 1) {
        pacman_x = nova_x;
        pacman_y = nova_y;

        // Se houver comida, come
        if (destino->tipo == 2) {
            destino->tipo = 0;  // Remove a comida
            pontuacao += 10;
            comidas_restantes--;
        }
    }
}

// Função para criar o mapa
void criarMapa() {
    inicializarLista(&mapa);

    // Cria as bordas
    for (int x = 0; x < COLUNAS; x++) {
        for (int y = 0; y < LINHAS; y++) {
            // Se for borda, cria parede
            if (x == 0 || y == 0 || x == COLUNAS - 1 || y == LINHAS - 1) {
                inserirFim(&mapa, x, y, 1); // Parede
            }
            // Senão, cria comida ou caminho vazio
            else {
                // Alguns obstáculos internos
                if ((x == 3 && y >= 3 && y <= 7) || 
                    (x == 7 && y >= 5 && y <= 10) ||
                    (x >= 10 && x <= 15 && y == 3) ||
                    (x >= 5 && x <= 10 && y == 8)) {
                    inserirFim(&mapa, x, y, 1); // Parede
                }
                // Posição inicial do Pac-Man e fantasma fica vazia
                else if ((x == pacman_x && y == pacman_y) || 
                         (x == fantasma_x && y == fantasma_y)) {
                    inserirFim(&mapa, x, y, 0); // Caminho vazio
                }
                // Resto é comida
                else {
                    inserirFim(&mapa, x, y, 2); // Comida
                }
            }
        }
    }
}

// Função para mudar a cor do fantasma
void mudarCorFantasma(HWND hwnd) {
    cor_fantasma = (cor_fantasma + 1) % NUM_CORES;
    char mensagem[100];
    sprintf(mensagem, "Cor do fantasma alterada para: %s", NOMES_CORES[cor_fantasma]);
    SetWindowText(hwnd, mensagem);
    InvalidateRect(hwnd, NULL, TRUE);
}

// Procedimento para desenhar
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        // Inicializa o mapa
        criarMapa();
        // Configura um timer para mover o Pac-Man
        SetTimer(hwnd, 1, 300, NULL); // A cada 300ms
        break;

    case WM_TIMER:
        // Move o Pac-Man
        moverPacman();
        
        // Verifica colisão entre Pac-Man e fantasma
        if (fantasma_x == pacman_x && fantasma_y == pacman_y) {
            KillTimer(hwnd, 1);
            MessageBox(hwnd, "Game Over! O fantasma capturou o Pac-Man!", "Fim de Jogo", MB_OK);
            PostQuitMessage(0);
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
        // Movimenta o fantasma
        int nova_x = fantasma_x;
        int nova_y = fantasma_y;
        
        switch (wParam) {
        case VK_LEFT:  nova_x -= 1; break;
        case VK_RIGHT: nova_x += 1; break;
        case VK_UP:    nova_y -= 1; break;
        case VK_DOWN:  nova_y += 1; break;
        case 'C':      mudarCorFantasma(hwnd); break; // Pressione 'C' para mudar a cor
        }
        
        // Verifica colisão com paredes
        Node* destino = buscarNo(&mapa, nova_x, nova_y);
        if (destino && destino->tipo != 1) {
            fantasma_x = nova_x;
            fantasma_y = nova_y;
            
            // Verifica colisão entre Pac-Man e fantasma
            if (fantasma_x == pacman_x && fantasma_y == pacman_y) {
                KillTimer(hwnd, 1);
                MessageBox(hwnd, "Game Over! O fantasma capturou o Pac-Man!", "Fim de Jogo", MB_OK);
                PostQuitMessage(0);
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
        
        // Desenha o mapa
        Node* atual = mapa.primeiro;
        while (atual) {
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
                // Desenha um pequeno círculo
                HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
                RECT comida = {
                    atual->x * TAMANHO_CELULA + TAMANHO_CELULA/3,
                    atual->y * TAMANHO_CELULA + TAMANHO_CELULA/3,
                    atual->x * TAMANHO_CELULA + 2*TAMANHO_CELULA/3,
                    atual->y * TAMANHO_CELULA + 2*TAMANHO_CELULA/3
                };
                FillRect(hdc, &comida, whiteBrush);
                DeleteObject(whiteBrush);
                break;
            }
            }
            
            atual = atual->prox;
        }
        
        // Desenha o Pac-Man (amarelo)
        HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
        RECT pacmanRect = {
            pacman_x * TAMANHO_CELULA,
            pacman_y * TAMANHO_CELULA,
            (pacman_x + 1) * TAMANHO_CELULA,
            (pacman_y + 1) * TAMANHO_CELULA
        };
        FillRect(hdc, &pacmanRect, yellowBrush);
        DeleteObject(yellowBrush);
        
        // Desenha o fantasma (com a cor selecionada)
        HBRUSH fantBrush = CreateSolidBrush(VALORES_CORES[cor_fantasma]);
        RECT fantasmaRect = {
            fantasma_x * TAMANHO_CELULA,
            fantasma_y * TAMANHO_CELULA,
            (fantasma_x + 1) * TAMANHO_CELULA,
            (fantasma_y + 1) * TAMANHO_CELULA
        };
        FillRect(hdc, &fantasmaRect, fantBrush);
        DeleteObject(fantBrush);
        
        // Desenha a pontuação e cor atual
        char scoreText[100];
        sprintf(scoreText, "Pontuação: %d | Cor do Fantasma: %s (Pressione 'C' para mudar)", 
                pontuacao, NOMES_CORES[cor_fantasma]);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 10, 10, scoreText, strlen(scoreText));
        
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        // Libera a memória da lista
        Node* atual = mapa.primeiro;
        while (atual) {
            Node* temp = atual;
            atual = atual->prox;
            free(temp);
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

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Pac-Man Inverso - Você é o Fantasma!",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, LARGURA + 16, ALTURA + 38,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}