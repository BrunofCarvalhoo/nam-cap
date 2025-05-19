# 🎮 nam-cap

**nam-cap** é uma variação invertida do clássico *Pac-Man*, onde o jogador assume o papel do fantasma em uma caçada emocionante contra o Pac-Man.

---

## 🧠 Introdução

Neste jogo, os papéis tradicionais foram trocados: você é o fantasma! Em vez de fugir, sua missão é perseguir e capturar o Pac-Man, que agora fará de tudo para escapar pelos labirintos do cenário. A proposta traz uma nova dinâmica e adiciona uma camada estratégica à jogabilidade.

---

## 🎯 Objetivo

O objetivo principal é **capturar o Pac-Man** dentro de um tempo limite. Para isso, o jogador deve usar raciocínio rápido, antecipar os movimentos do adversário e traçar rotas inteligentes para cercá-lo.

O Pac-Man é controlado por uma inteligência artificial (IA) e utiliza o labirinto, portais e sua agilidade para escapar. Cabe ao jogador, como fantasma, adaptar-se e planejar a melhor abordagem.

---

## 🛠️ Tecnologias Utilizadas

- Linguagem **C**
- API Windows (GDI+)
- Biblioteca libcurl para integração com API Gemini
- Estrutura de dados: Lista duplamente encadeada circular
- Algoritmo de ordenação: QuickSort para o ranking
- Projeto contido em um único arquivo: `pacman_inverso.c`

---

## 👨‍💻 Integrantes do Grupo

- Bruno Carvalho  
- Guilherme Almeida  
- Igor Couto  
- Júlio César Aragão  
- William Souza  

---
## 🕹️ Como Jogar

### Controles
- Setas direcionais: Movimentar o fantasma
- Selecione a cor do fantasma no menu inicial antes de começar

### Regras
- Capture o Pac-Man antes que ele coma todas as comidas
- Evite o Pac-Man quando ele estiver poderoso (após comer uma fruta rosa)
- Use os portais para se teletransportar pelo mapa
- Quanto menos comidas o Pac-Man comer, melhor sua pontuação

---

## ⚠️ Requisitos para Compilar e Rodar

Para compilar e rodar o jogo, siga os passos abaixo:

### 1. Instale o MSYS2
- Baixe e instale em: https://www.msys2.org

### 2. Abra o terminal **MSYS2 UCRT64**
- Procure por "MSYS2 UCRT64" no menu iniciar e abra esse terminal (não use o cmd ou PowerShell do Windows).

### 3. Instale o GCC e a libcurl
No terminal MSYS2 UCRT64, execute:
```bash
pacman -Syu   # (atualize o sistema, pode pedir para fechar e abrir de novo)
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-curl
```

### 4. Como garantir que o GCC está no PATH do terminal MSYS2 UCRT64

Se ao digitar `gcc --version` no terminal MSYS2 UCRT64 não aparecer a versão do GCC, siga estes passos:

1. No terminal MSYS2 UCRT64, digite:
   ```bash
   echo 'export PATH="/ucrt64/bin:$PATH"' >> ~/.bashrc
   ```
2. Feche o terminal MSYS2 UCRT64.
3. Abra novamente o terminal MSYS2 UCRT64.
4. Agora digite:
   ```bash
   gcc --version
   ```
   Se aparecer a versão do GCC, está tudo certo!

**O que isso faz?**
Esse comando adiciona automaticamente o caminho do compilador e das DLLs do MSYS2 UCRT64 ao PATH toda vez que você abrir o terminal, facilitando a compilação e execução do seu jogo.

### 5. Compile e rode o jogo
No terminal MSYS2 UCRT64, navegue até a pasta do projeto. Por exemplo:
```bash
cd /c/Users/SeuUsuario/Downloads/nam-cap-main
```
Compile o jogo:
```bash
gcc -o pacman_inverso.exe pacman_inverso.c -lgdi32 -lcurl
```
Rode o jogo:
```bash
./pacman_inverso.exe
```

---

## 📁 Arquivos Necessários
A pasta do jogo deve conter:
- `pacman_inverso.c` (código fonte)
- `ranking.txt` (será criado automaticamente)
- (opcional) assets, README, etc.

---

## 🛠️ Solução de Problemas
- Sempre use o terminal **MSYS2 UCRT64** para compilar e rodar o jogo.
- Se der erro de "undefined reference" para funções do curl, confira se instalou o pacote correto (`mingw-w64-ucrt-x86_64-curl`) e se está usando o terminal certo.
- Se der erro de DLL ao rodar, certifique-se de rodar pelo terminal MSYS2 UCRT64, pois ele já configura o ambiente para encontrar as DLLs.
- Se quiser rodar o executável fora do MSYS2, copie também as DLLs necessárias de `/ucrt64/bin` para a pasta do jogo.

---

## Observações
- O jogo salva automaticamente o ranking em `ranking.txt`
- A pontuação é baseada na quantidade de comidas que o Pac-Man não conseguiu comer
