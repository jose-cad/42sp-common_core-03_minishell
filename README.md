# Minishell

_Este projeto foi criado como parte do currículo da 42 por jose-cad._

## Descrição

Um shell simples, inspirado no bash, escrito em C. Lê comandos num prompt interativo, interpreta aspas, variáveis, redirecionamentos e pipes, e executa programas do sistema ou os builtins implementados no próprio projeto.

**Parte obrigatória**

- Prompt com histórico
- Execução de programas pelo `PATH` ou por caminho relativo/absoluto
- Aspas simples (`'`) e duplas (`"`)
- Variáveis de ambiente (`$VAR`) e status de saída (`$?`)
- Redirecionamentos `<`, `>`, `>>` e heredoc `<<`
- Pipes `|`
- Ctrl-C, Ctrl-D e Ctrl-\ como no bash
- Builtins: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

**Bônus**

- `&&` e `||` com parênteses para prioridade
- Wildcard `*` no diretório atual

**Extra:** redirecionamento de erro `2>`.

## Instalação

Requisitos: `cc`, `make` e a biblioteca readline.

```bash
sudo apt-get install libreadline-dev
make
./minishell
```

`make clean`, `make fclean` e `make re` limpam e recompilam.

## Testando

```bash
echo "Olá, $USER"              # expande a variável
echo '$USER'                   # não expande
ls nao_existe
echo $?                        # 2
ls -l | grep minishell | wc -l
echo teste > a.txt && cat < a.txt
echo mais >> a.txt
cat << FIM
linha com $HOME
FIM
(false || echo ok) && echo fim
echo *.c
export NOME=42 && env | grep NOME
unset NOME
exit 3
```

Verificar vazamentos de memória e de file descriptors (a supressão cobre apenas a memória interna do readline):

```bash
make valgrind
```

## Limitações

Seguem o subject: não interpreta `;`, `\` nem aspas não fechadas.

Além disso:

- O wildcard funciona apenas no diretório atual (`*.c`, não `src/*.c`).
- Parâmetros posicionais como `$1` não existem.
