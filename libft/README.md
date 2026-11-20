# Libft Expanded - The Common Core Journey

## 📝 Visão Geral
Esta não é apenas uma biblioteca estática; é o motor técnico que impulsiona minha jornada no **Common Core da 42**. Esta versão da **Libft** evoluiu a partir do projeto inicial para incluir ferramentas avançadas de parsing, manipulação de arquivos e estruturas de dados complexas, sendo constantemente atualizada para servir como dependência central em projetos subsequentes.

## 🚀 Evolução e Estrutura
Diferente da versão básica, esta biblioteca adota uma organização modular por diretórios para gerenciar de forma eficiente seus mais de 100 arquivos.

### 1. Núcleo de Conversão (`/conversion`)
Extensão das capacidades de transformação de tipos:
- **Bases Numéricas:** Suporte para Binário, Hexadecimal e Octal (`ft_atoi_base`, `ft_itoa_base`).
- **Ponto Flutuante:** Conversão de floats e doubles (`ft_atof`, `ft_ftoa`).
- **Ponteiros:** Tratamento especializado de endereços de memória.

### 2. Estruturas de Dados (`/list` & `/dlist`)
- **Singly Linked Lists:** Implementação padrão de listas encadeadas simples.
- **Doubly Linked Lists:** Inclusão de listas duplamente encadeadas (`ft_dlst...`), permitindo navegação bidirecional e remoção de nós otimizada.

### 3. I/O Avançado (`/gnl` & `/printf`)
Integração de subprojetos fundamentais:
- **Get Next Line:** Leitura de arquivos linha por linha, gerenciando múltiplos *file descriptors* e buffers.
- **ft_printf:** Reimplementação do `printf` da `libc` com suporte a flags, precisão e especificadores customizados, além da variante `ft_dprintf` para escrita em FDs específicos.

### 4. Categorização CType e Strings
- **Ctype:** Verificações específicas como `ft_isspace` e `ft_isalnum_or_underscore` (essencial para parsing de tokens).
- **String Utils:** Funções práticas como `ft_strjoin_free` (para evitar leaks em concatenações sucessivas) e `ft_strcmp`.

### 5. Memória e Alocação
- Gestão rigorosa de memória com `ft_calloc` e proteções em todas as funções de manipulação de memória (`memcpy`, `memmove`, etc).

## 🛠️ Aspectos Técnicos
- **Organização SRC/INC:** Separação clara entre arquivos de implementação (`.c`) e cabeçalhos (`.h`).
- **Makefile Robusto:** Compilação automatizada que gerencia a árvore de diretórios, gerando a biblioteca `libft.a`.
- **Desenvolvimento Contínuo:** Esta biblioteca é um organismo vivo, atualizada conforme as necessidades dos projetos de nível superior.

## 📈 Projetos que utilizam esta Lib
Esta biblioteca é a base de código para:
- **Get_Next_Line** (Integrado)
- **Ft_printf** (Integrado)
- **Born2beRoot** (Scripts e automação)
- *Projetos futuros (Push_swap, So_long, Minishell...)*

---

📌 Autor: **jose-cad** ⚠️ *Nota: Este repositório está em constante evolução conforme o progresso no Common Core.*
