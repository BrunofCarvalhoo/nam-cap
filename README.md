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

## 💻 Requisitos e Instalação

### Requisitos
- Sistema Operacional: Windows
- Compilador GCC
- Biblioteca libcurl
  ### ✅Como instalar o GCC e a libcurl
  Você deve estar no ambiente **MSYS2 MinGW UCRT64**.  
  Se ainda não tem o MSYS2, instale aqui: [https://www.msys2.org](https://www.msys2.org)
  Abra o terminal `MSYS2 UCRT64` e execute:

  ```bash
  pacman -S mingw-w64-ucrt-x86_64-gcc             # Compilador C
  pacman -S mingw-w64-ucrt-x86_64-curl            # libcurl (acesso à internet)
  ```
  - Talvez seja necessário adicionar a biblioteca libcurl e o GCC no path nas variáveis de ambiente

  
### Compilação e Execução
1. Abra o terminal (PowerShell ou Prompt de Comando)
2. Navegue até a pasta do projeto:
```bash
cd C:\caminho\para\pasta\nam-cap
```

3. Compile o jogo:
```bash
gcc -o pacman_inverso pacman_inverso.c -lgdi32 -lcurl
```

4. Execute o jogo:
```bash
.\pacman_inverso.exe
```

### Observações Importantes
- Certifique-se de estar na pasta correta do projeto antes de compilar
- O arquivo executável será criado na mesma pasta do código fonte
- O jogo salva automaticamente o ranking dos melhores jogadores em `ranking.txt`
- A pontuação é baseada na quantidade de comidas que o Pac-Man não conseguiu comer
