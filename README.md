# Produtor-Consumidor com Pipes e Semáforos

Implementação do problema produtor-consumidor em C++ com duas abordagens:

* Comunicação entre processos utilizando pipes
* Sincronização com threads e semáforos

## Estrutura

```bash
.
├── Makefile
├── bin/
├── build/
└── project/
    └── src/
        ├── pipes/
        │   ├── pipes.cpp
        │   ├── producer.hpp
        │   └── consumer.hpp
        └── threads/
            ├── threads.cpp
            ├── producer.hpp
            └── consumer.hpp
```

## Compilação

Na raiz do projeto:

```bash
make
```

Os executáveis são gerados em `bin/`.

## Execução

### Pipes

```bash
./bin/pipes <quantidade_de_itens>
```

* Cria dois processos com `fork()`
* Comunicação via pipe
* Produtor envia números
* Consumidor lê e verifica se são primos
* O valor `0` indica fim da execução 

### Threads + Semáforos

```bash
./bin/threads <n_produtores> <n_consumidores>
```

* Usa memória compartilhada como buffer
* Controle com três semáforos:

  * exclusão mútua
  * posições vazias
  * posições ocupadas 
* Produtores inserem no buffer
* Consumidores removem e processam

## Limpeza dos arquivos gerados

Na raiz do projeto:

```bash
make clean
```