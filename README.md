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

## 💻 Instalação e Execução

### Método 1 - Instalação Automática (Recomendado)
1. Baixe o arquivo `nam-cap-setup.exe` da pasta de distribuição
2. Execute o instalador
3. Siga as instruções na tela
4. O jogo será instalado automaticamente com todas as dependências necessárias

### Método 2 - Instalação Manual
Se preferir instalar manualmente:

1. **Baixe os arquivos necessários:**
   - `pacman_inverso.exe` (jogo)
   - `libcurl.dll` (biblioteca necessária)
   - `install.bat` (script de instalação)

2. **Execute o script de instalação:**
   - Dê duplo clique em `install.bat`
   - Aguarde a mensagem "Instalação concluída"

3. **Execute o jogo:**
   - Dê duplo clique em `pacman_inverso.exe`

### Solução de Problemas

Se o jogo não iniciar:
1. Verifique se todos os arquivos estão na mesma pasta
2. Execute o `install.bat` novamente
3. Se ainda não funcionar, tente executar como administrador

### Arquivos Necessários
Para o jogo funcionar, você precisa ter:
- `pacman_inverso.exe`
- `libcurl.dll`
- `ranking.txt` (será criado automaticamente)

### Observações
- O jogo salva automaticamente o ranking em `ranking.txt`
- A pontuação é baseada na quantidade de comidas que o Pac-Man não conseguiu comer
- Não é necessário compilar o jogo - use apenas os arquivos fornecidos
