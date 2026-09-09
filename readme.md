# Escalonamento de Tarefas

## Arquivos
- **main.c** — leitura do arquivo de entrada, simulação (rate-monotonic e EDF) e geração da saída.
- **Makefile** — compila o executável `scheduler`.

## Compilar
```
make
```

## Executar
```
./scheduler rate voo.txt
./scheduler edf voo.txt
```
Gera `rate_lcw.out` ou `edf_lcw.out`.

## Testar
Rodar com o arquivo de exemplo e comparar com o gabarito. Testar também
casos de erro (argumentos errados, arquivo inexistente, tarefa malformada).

## Sistema operacional
WSL (Ubuntu) no Windows, com gcc.