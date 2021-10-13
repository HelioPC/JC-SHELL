# jc-shell

### Um teste das minhas capacidades com algoritmos e estruturas de dados

---

## Tabela de Conteúdos

<!--ts-->
   * [Sobre](#sobre)
      * [Sobre o autor](#sobre-o-autor)
      * [Descrição](#descrição)
   * [Instalação](#uso)
      * [Linux](#uso)
   * [Uso](#uso)
   * [Estado](#estado)
   * [Como ajudar](#como-ajudar)
<!--te-->

## Sobre

`jc-shell` é um projeto da cadeira de Sistemas operativos, do curso de ciências da computação, da faculdade de ciências da universidade Agostinho Neto. Este projeto, consiste num programa de nome `jc-shell`, que permite executar e monitorar lotes de programas em paralelo numa máquina multi-core.

### Sobre o autor

Cordiais saudações para quem estiver a ler, eu sou o `Eliúde Vemba`, um programador nascido no país africano Angola, conhecido em diversas comunidades (Dev, gamers, wiki, youtube, etc...) como `eliude_sweet`, [HelioPc](https://github.com/HelioPC), `hlio_c`, `MASTER_SWEET` e outros, atualmente (07-10-2021) domino todos os aspectos, conceitos, tecnologias e algoritmos envolvidos neste projeto. Apaixonei-me pelo mundo da programação à primeira vista, durante a minha adolescência em 08-2016. Desenvolvi uma paixão inexplicável com a linguagem C, chegando ao ponto de quase dominá-la. Tenho experiência e conhecimento em outras áreas da programação/desenvolvimento como: `web`, `game` e a minha favorita `mobile`.

---

### Descrição

Este projeto, consiste num programa de nome **jc-shell**, que permite executar e monitorar lotes de programas em paralelo numa máquina multi-core. Este projeto, consiste numa shell (**jc-shell**), que lê os comandos de um pipe nomeado (ou pipe com nome) denominado **jcshell-in**. Os comandos são introduzidos no pipe através de um outro programa chamado **jcshell-terminal**.

---

## Uso

NOTA: Este projeto foi feito para distribuições Linux.

Teoricamente, o funcionamento deste projeto está restrito em sistemas unix, funcionando com defeitos imperceptíveis no macOs.

Siga as seguintes para utilizar o **jc-shell**:

### CLONE

Clone este projeto do github.

1. Abra o terminal: CTRL+ALT+T

2. Instale o git:
    ```zsh
    sudo apt install git-all
    ```
3. Faça fork e clone:
    ```zsh
    git clone https://github.com/HelioPC/JC-SHELL.git
    ```

### COMPILE

Compile o projeto.

1. Navegue até ao diretório:
    ```zsh
    cd JC-SHELL/
    ```
2. Compile com:
    ```zsh
    make all
    ```
3. Feche o terminal:
    ```zsh
    exit
    ```

### EXECUTE

Ponha este projeto em funcionamento com os seguintes passos:

1. Abra 4-6 terminais (CTRL+ALT+T), 1 para o **jc-shell** e o restante serão **jcshell-terminal**.
2. Em todos terminais navegue até ao diretório do projeto:
    ```zsh
    cd JC-SHELL/
    ```
3. No terminal do **jc-shell** (à sua escolha), digite:
    ```zsh
    ./jc-shell
    ```
4. Nos terminais restantes digite:
    ```zsh
    ./jcshell-terminal jcshell-in
    ```

### COMANDOS

Neste projeto exitem 6 comandos e as suas sintaxes são as seguintes:

- **`exit-global`**:
    1. Sintaxe: ``exit-global``
    2. Descrição: Encerra todos **jcshell-terminal** e o **jc-shell**.

- **`ls`**:
    1. Sintaxe: ``ls``
    2. Descrição: Apresenta uma lista dos diretórios e ficheiros dentro do diretório do projeto.

- **`clear`**:
    1. Sintaxe: ``clear``
    2. Descrição: Limpa a tela do **jc-shell**.

- **`stats`**:
    1. Sintaxe: ``stats``
    2. Descrição: Apresenta o tempo total de execução dos processos lançados e o número de processos ainda em exução

- **`exit`**:
    1. Sintaxe: ``exit``
    2. Descrição: Encerra o **jcshell-terminal** que digitou.

- **`executável`**:
    1. Sintaxe: ``PATHNAME``
    2. Descrição: Executa o ficheiro executável `PATHNAME`

OBS: Para testes do último comando, o programa `fibonacci` foi compilado, com a seguinte sintaxe: ``fibonacci 123``

## Estado

🚧  **`jc-shell`** 🚀 Estável...  🚧 Versão 1.5.3

## Como ajudar

O projeto é de código aberto, qualquer ajuda é bem vinda, desde que impulsione este projeto para outros níveis.

OBS: O projeto foi inteiramente escrito na linguagem C, se possível que se mantenha assim.
