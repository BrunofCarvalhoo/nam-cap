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
- Sistema Operacional: Windows 10 ou superior
- Compilador GCC (MinGW)
- Biblioteca libcurl

### Instalação Completa

1. **Instalar o MinGW (GCC)**
   - Baixe o MinGW Installer em: https://sourceforge.net/projects/mingw/
   - Durante a instalação, selecione pelo menos:
     - mingw32-base
     - mingw32-gcc-g++
   - Adicione o caminho `C:\MinGW\bin` às variáveis de ambiente do Windows

2. **Instalar a libcurl**
   - Baixe o pacote completo da libcurl em: https://curl.se/windows/
   - Escolha a versão "curl-8.x.x-win64-mingw.zip"
   - Extraia o conteúdo
   - Copie os arquivos:
     - `bin/libcurl.dll` para `C:\Windows\System32`
     - `include/curl` para `C:\MinGW\include`
     - `lib/libcurl.dll.a` para `C:\MinGW\lib`

3. **Verificar a Instalação**
   - Abra um novo terminal e digite:
   ```bash
   gcc --version
   ```
   - Se aparecer a versão do GCC, a instalação foi bem-sucedida

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

### Solução de Problemas

Se aparecer o erro "libcurl.dll não encontrado":
1. Verifique se o arquivo `libcurl.dll` está em `C:\Windows\System32`
2. Se não estiver, copie-o da pasta `bin` do pacote da libcurl
3. Reinicie o terminal após copiar o arquivo

Se aparecer erro de compilação:
1. Verifique se o GCC está instalado corretamente:
```bash
gcc --version
```
2. Verifique se as variáveis de ambiente estão configuradas:
   - Abra "Editar as variáveis de ambiente do sistema"
   - Em "Variáveis do Sistema", edite "Path"
   - Adicione `C:\MinGW\bin` se não estiver presente

### Arquivos Necessários
Para distribuir o jogo, você precisa incluir:
- `pacman_inverso.exe` (arquivo compilado)
- `libcurl.dll` (da pasta bin da libcurl)
- `ranking.txt` (será criado automaticamente)

### Observações Importantes
- O jogo salva automaticamente o ranking dos melhores jogadores em `ranking.txt`
- A pontuação é baseada na quantidade de comidas que o Pac-Man não conseguiu comer
- Para distribuir o jogo, crie uma pasta com todos os arquivos necessários
- Os jogadores precisarão ter o `libcurl.dll` no mesmo diretório do executável ou em `C:\Windows\System32`
