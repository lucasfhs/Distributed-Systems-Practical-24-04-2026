## Project Overview

This project was developed as part of the Distributed Systems course at CEFET-MG. Its main goal is to explore fundamental Interprocess Communication (IPC) mechanisms, including message passing, multithreading, and synchronization techniques.

The implementation focuses on low-level control using C/C++, providing a deeper understanding of how operating systems handle communication and concurrency. 

Author: Lucas Henrique Ferreira

---
# MakeFile Commands

make run prog=threads arg="<number_producers> <number_consumers>"
make run prog=pipes arg="<numbers_to_generate>"


## Project Structure

```
project/
├── src/
│   ├── main.cpp
│   ├── pipes/
│   └── threads/
├── include/
├── build/
├── Makefile
└── README.md
```

---

## Build

```
make
```

---

## Run

### Pipes (process-based)

```
./bin/pipes <num_values>
```

### Threads (semaphores)

```
./bin/threads <buffer_size> <num_producers> <num_consumers>
```

---

## Clean

```
make clean
```

---

## Features

The project is divided into two main components:

### 1. Producer-Consumer with Pipes

* Implementation of the Producer-Consumer problem using two processes
* Communication through anonymous pipes
* The producer generates increasing random integers
* The consumer receives each number and checks whether it is prime
* The program terminates when a sentinel value (0) is received

---

### 2. Multithreaded Producer-Consumer with Semaphores

* Shared memory implemented as a fixed-size buffer
* Multiple producer and consumer threads
* Synchronization using counting semaphores
* Producers insert random integers into the buffer
* Consumers remove values and perform primality checks
* Proper handling of full and empty buffer states (blocking behavior)

---

## Performance Analysis

* Execution time is measured for different configurations of:

  * Buffer sizes (N)
  * Number of producer threads (Np)
  * Number of consumer threads (Nc)
* Each scenario is executed multiple times to compute average runtime
* Results are presented through performance graphs
* Additional analysis includes buffer occupancy over time

---

## Technologies

* C/C++
* POSIX threads (pthreads)
* Pipes (IPC)
* Semaphores
* Standard libraries for process and thread management

---

## Goals

* Understand IPC mechanisms in practice
* Explore synchronization problems such as race conditions
* Analyze performance trade-offs in concurrent systems
* Gain experience with low-level systems programming

