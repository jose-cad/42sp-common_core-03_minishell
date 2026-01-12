
# Minishell

_Este projeto foi criado como parte do currículo da 42 por jose-cad._

----------
## 📖 Descrição

O **Minishell** é uma reimplementação simplificada do bash, um dos shells mais utilizados em sistemas Unix. O objetivo deste projeto é compreender profundamente como funcionam processos, descritores de arquivo, redirecionamentos, pipes e sinais em sistemas operacionais.

Este shell permite executar comandos, criar pipelines complexos, redirecionar entrada e saída, expandir variáveis de ambiente e muito mais. É uma jornada técnica que conecta teoria de sistemas operacionais com implementação prática em C.

### 🎯 Objetivos do Projeto

-   Implementar um interpretador de linha de comando funcional
-   Gerenciar processos com `fork()`, `execve()` e `wait()`
-   Manipular descritores de arquivo e redirecionamentos
-   Construir pipelines com múltiplos comandos
-   Processar sinais (SIGINT, SIGQUIT)
-   Implementar builtins essenciais
-   Parsear e expandir tokens complexos

----------
## 🧠 Conceitos Aprendidos

### 1. **Processos e Execução**

-   **Fork e Exec**: Criação de processos filhos com `fork()` e substituição da imagem do processo com `execve()`
-   **Wait e Status**: Sincronização de processos e captura de códigos de saída
-   **Gerenciamento de Recursos**: Limpeza adequada de file descriptors e memória

### 2. **Pipes e Redirecionamentos**

-   **Comunicação entre Processos**: Implementação de pipelines usando `pipe()` e `dup2()`
-   **Redirecionamentos**: Manipulação de stdin/stdout/stderr para arquivos
-   **Heredocs**: Leitura de entrada até encontrar um delimitador

### 3. **Parsing e Lexing**

-   **Tokenização**: Separação de comandos, operadores e palavras
-   **Análise Sintática**: Construção de Abstract Syntax Tree (AST)
-   **Tratamento de Aspas**: Diferenciação entre aspas simples e duplas
-   **Expansão de Variáveis**: Substituição de `$VAR` e `$?`

### 4. **Sinais e Terminal**

-   **Manipulação de Sinais**: Tratamento de Ctrl+C, Ctrl+D e Ctrl+\
-   **Comportamento Interativo**: Diferentes handlers para modo normal vs heredoc
-   **Controle de Terminal**: Uso de `termios` para configurar comportamento

### 5. **Builtins**

Implementação de comandos internos que modificam o estado do shell:

-   `cd`, `pwd` - navegação de diretórios
-   `echo` - output com flag `-n`
-   `env`, `export`, `unset` - gerenciamento de variáveis
-   `exit` - encerramento controlado

----------

## ✨ Funcionalidades

### Parte Obrigatória

✅ Prompt interativo com histórico
✅ Execução de comandos com PATH ou caminho absoluto/relativo
✅ Pipes (`|`) - conecta saída de um comando à entrada do próximo
✅ Redirecionamentos:

-   `<` entrada de arquivo
-   `>` saída para arquivo (sobrescreve)
-   `>>` saída para arquivo (anexa)
-   `<<` heredoc com delimitador
    ✅ Expansão de variáveis (`$VAR`, `$?`)
    ✅ Aspas simples (`'`) e duplas (`"`)
    ✅ Sinais: Ctrl+C, Ctrl+D, Ctrl+\
    ✅ Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

### Parte Bônus

🎁 Operadores lógicos `&&` e `||` com avaliação de curto-circuito
🎁 Subshells com parênteses `( )`
🎁 Wildcards `*` para expansão de nomes de arquivo
🎁 Redirecionamento de stderr `2>`

----------

## 🛠️ Compilação e Execução

### Requisitos

-   GCC ou Clang
-   Make
-   Biblioteca readline (`sudo apt-get install libreadline-dev` no Ubuntu)

### Compilar

bash

```bash
make
```

Isso irá:

1.  Compilar a libft
2.  Compilar todos os arquivos fonte do minishell
3.  Gerar o executável `minishell`

### Executar

bash

```bash
./minishell
```
Você verá o prompt:
```
minishell$
```
### Limpar arquivos compilados

bash

```bash
make clean   # Remove objetos
make fclean  # Remove objetos e executável
make re      # Recompila tudo do zero
```
----------
## 🎮 Como Usar

Após iniciar o minishell, você pode testar todas as funcionalidades implementadas! Aqui estão alguns exemplos:

### Comandos Básicos

bash

```bash
minishell$ ls -la
minishell$ pwd
minishell$ echo "Hello, World!"
```

### Pipes

bash

```bash
minishell$ ls | grep minishell
minishell$ cat file.txt | grep "pattern" | wc -l
```

### Redirecionamentos

bash

```bash
minishell$ echo "test" > output.txt
minishell$ cat < input.txt
minishell$ ls >> log.txt
minishell$ cat << EOF
> linha 1
> linha 2
> EOF
```

### Variáveis de Ambiente

bash

```bash
minishell$ echo $HOME
minishell$ echo $?
minishell$ export MY_VAR="teste"
minishell$ echo $MY_VAR
minishell$ unset MY_VAR
```

### Aspas

bash

```bash
minishell$ echo 'não expande $HOME'
minishell$ echo "expande $HOME"
```

### Operadores Lógicos (Bônus)

bash

```bash
minishell$ ls && echo "sucesso"
minishell$ cat arquivo_inexistente.txt || echo "erro"
minishell$ (cd /tmp && pwd) && pwd
```

### Wildcards (Bônus)

bash

```bash
minishell$ ls *.c
minishell$ echo src/*/*.c
```

**Divirta-se explorando e testando todas as funcionalidades!** 🚀

---

## 🏗️ Arquitetura do Projeto
```
minishell/
├── includes/           # Headers
│   └── minishell.h
├── libft/             # Biblioteca auxiliar
├── srcs/
│   ├── core/          # Inicialização e loop principal
│   ├── lexer/         # Tokenização
│   ├── parser/        # Análise sintática e AST
│   ├── expander/      # Expansão de variáveis e wildcards
│   ├── executor/      # Execução de comandos
│   │   ├── builtin/   # Comandos internos
│   │   ├── pipe/      # Gerenciamento de pipes
│   │   └── redirections/  # Redirecionamentos
│   └── utils/         # Funções auxiliares
└── Makefile
```

----------

## 🔧 Decisões Técnicas e Algoritmos

### 1. **Estrutura de Dados: Abstract Syntax Tree (AST)**

**Justificativa**: Optamos por construir uma AST ao invés de executar diretamente durante o parsing. Isso separa claramente as responsabilidades e permite:

-   Validação sintática completa antes da execução
-   Suporte natural para operadores lógicos (`&&`, `||`)
-   Facilita implementação de subshells
-   Torna o código mais modular e testável

### 2. **Parsing: Recursive Descent Parser**

**Justificativa**: Implementamos um parser descendente recursivo que respeita a precedência de operadores:
**Vantagem**: Estrutura natural que espelha a gramática, facilitando manutenção e extensão.

### 3. **Pipes: Passagem de Bastão**

**Algoritmo**: Para pipelines (`cmd1 | cmd2 | cmd3`), usamos uma técnica de "passagem de bastão":
**Vantagem**: Evita criar todos os pipes antecipadamente, economizando recursos.

### 4. **Heredocs: Processamento Antecipado**

**Decisão**: Heredocs são processados **antes** da execução, em uma fase separada.

**Justificativa**:

-   Permite interromper com Ctrl+C antes de executar qualquer comando
-   Evita bloqueios em pipelines complexos
-   Separa lógica de input do usuário da execução


### 5. **Sinais: Diferentes Contextos**

### 6. **Expansão de Variáveis: Single-Pass**

**Algoritmo**: Percorremos a string uma única vez, expandindo variáveis conforme encontradas:
**Otimização**: Evita múltiplas passadas pela string.

### 7. **Wildcards: Glob Matching Recursivo**

**Algoritmo**: Implementação recursiva de pattern matching:
**Decisão**: Expansão acontece **após** expansão de variáveis mas **antes** de execução.

----------

## 📚 Recursos de Aprendizado

Durante o desenvolvimento, estes recursos foram valiosos:

-   **Man pages**: `man bash`, `man execve`, `man pipe`, `man dup2`
-   **Artigos**: GNU Bash Reference Manual
-   **Debugging**: `valgrind`, `strace`, `gdb`
-  **Café**: `muito bate papo com outros cadetes`na galeria

----------

## 🐛 Debugging e Testes

Para verificar vazamentos de memória:

bash

```bash
make valgrind
```
----------

## ⚠️ Limitações Conhecidas

-   Não implementa operador `;` (ponto e vírgula)
-   Não suporta `\\` (barra invertida) para escape
-   Wildcards não funcionam em aspas
-   Não implementa job control (`&`, `fg`, `bg`)

----------

## 👥 Créditos

Projeto desenvolvido como parte do currículo da **42 São Paulo**.

**Conceitos aprendidos**: Processos, IPC, Parsing, Sinais, File Descriptors, Shell Scripting.

----------

_Sinta-se livre para explorar, testar e se divertir com o minishell! Qualquer comando válido em bash (e não limitado pelo subject) deve funcionar aqui também._ 🎉
