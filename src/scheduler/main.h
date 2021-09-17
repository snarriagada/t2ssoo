#pragma once

// Define the struct

typedef struct rafaga {
  int duracion;
  int restante;
  struct rafaga *next;
} Rafaga;

Rafaga* rafaga_init(int duracion);

typedef struct process {
  int pid;
  char** nombre;
  int fabrica;
  char* estado;
  struct process *next;
  int tiempo_inicio;
  int cantidad_rafagas;
  //Rafaga* rafaga_actual;
  //int** rafagas;
  int quantum;
  int *rafagas;
  
  // Estadisticas
  int elecciones;
  int interrupciones;
  int turnaround_time;
  int response_time;
  int waiting_time;

} Process;

Process* process_init(int pid, char **nombre, int tiempo_inicio, int fabrica, int cantidad_rafagas);

typedef struct queue {
    int cantidad;
    Process *inicio;
    Process *last;
} Queue;

Queue* queue_init(int cantidad);
int quantum(Queue* queue, int q, Process* process);


typedef struct cpu {
    Process *exec;
} Cpu;

Cpu* cpu_init();


