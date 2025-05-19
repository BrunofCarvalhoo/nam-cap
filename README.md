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

Para compilar e rodar o jogo, você precisa ter instalado no seu computador:

- **GCC (MinGW)**  
  Baixe e instale em: https://sourceforge.net/projects/mingw/  
  Certifique-se de adicionar o caminho do MinGW (`C:\MinGW\bin`) ao PATH do Windows.

- **libcurl para desenvolvimento**  
  Você pode instalar facilmente pelo MSYS2 (recomendado):  
  1. Instale o MSYS2: https://www.msys2.org  
  2. Abra o terminal `MSYS2 UCRT64`  
  3. Execute:  
     ```bash
     pacman -S mingw-w64-ucrt-x86_64-gcc
     pacman -S mingw-w64-ucrt-x86_64-curl
     ```
  Ou, se usar MinGW puro, baixe a libcurl em https://curl.se/windows/ e copie os arquivos `.dll.a` e `.h` para as pastas do MinGW.

- **DLL da libcurl**  
  O arquivo `libcurl-4.dll` (ou `libcurl-x64.dll` ou `libcurl.dll`) deve estar na mesma pasta do jogo.

---

## 💻 Instalação e Execução

### Método Simples (Recomendado)
1. Baixe a pasta `nam-cap` completa
2. Extraia todos os arquivos para uma pasta de sua preferência
3. Execute o arquivo `jogar.bat` com duplo clique
4. O jogo será compilado e iniciado automaticamente (se os requisitos acima estiverem instalados)

### Arquivos Necessários
A pasta do jogo deve conter:
- `pacman_inverso.c` (código fonte)
- `libcurl-4.dll` (ou `libcurl-x64.dll` ou `libcurl.dll`)
- `jogar.bat` (script para compilar e iniciar o jogo)
- `ranking.txt` (será criado automaticamente)

### Solução de Problemas

Se o jogo não iniciar ou não compilar:
1. Verifique se todos os arquivos estão na mesma pasta
2. Verifique se o GCC (MinGW) está instalado e no PATH
3. Verifique se a libcurl de desenvolvimento está instalada
4. Tente executar o `jogar.bat` como administrador
5. Se aparecer erro sobre DLL, confira o nome do arquivo DLL solicitado e renomeie se necessário

### Observações
- O jogo salva automaticamente o ranking em `ranking.txt`
- A pontuação é baseada na quantidade de comidas que o Pac-Man não conseguiu comer
- O script `jogar.bat` compila e executa o jogo automaticamente, mas depende dos requisitos acima
