#include <stdio.h>

#include "../file_manager/manager.h"
#include "main.h"

Process* process_init(int pid, char **nombre, int tiempo_inicio, int fabrica, int cantidad_rafagas){
    Process* new_process = malloc(sizeof(Process));
    new_process -> pid = pid;
    new_process -> nombre = nombre;
    new_process -> fabrica = fabrica;
    new_process -> estado = "READY";
    new_process -> tiempo_inicio = tiempo_inicio;
    new_process -> cantidad_rafagas = cantidad_rafagas;
    new_process -> quantum = 0;
    new_process -> elecciones = 0;
    new_process -> interrupciones = 0;
    new_process -> turnaround_time = 0;
    new_process -> response_time = 0;
    new_process -> waiting_time = 0;
    //new_process -> rafaga_actual = NULL;
    new_process->rafagas = calloc(cantidad_rafagas, sizeof(int));
    printf("retornando nuevo proceso\n");


    
    
    return new_process;
};

Queue* queue_init(int cantidad){
    Queue* new_queue = malloc(sizeof(Queue));
    new_queue -> cantidad = cantidad;
    new_queue -> inicio = NULL;
    new_queue -> last = NULL;
    return new_queue;
};

Cpu* cpu_init(){
  Cpu* cpu = malloc(sizeof(Cpu));
  cpu -> exec = NULL;
  return cpu;
};

Rafaga* rafaga_init(int duracion){
  Rafaga* new_rafaga = malloc(sizeof(Rafaga));
  new_rafaga -> duracion = duracion;
  new_rafaga -> restante = duracion;
  new_rafaga -> next = NULL;
  return new_rafaga;
};

// función scheduler:
// encontrar el process FIFO que este ready y extraerlo de la cola

void scheduler(Queue* queue, Cpu* cpu){
    //------ sale el process se la CPU al final de la cola
    queue -> last -> next = cpu -> exec;
    queue -> last = cpu -> exec;
    cpu -> exec = NULL;

    Process* elegido = queue->inicio;
    while(elegido != NULL){
      // encontrar el primero que este ready
      if(elegido->estado = "READY"){
        break;
      }
      elegido = elegido->next;
    }
    //-----------------------------------------------------
    
    // aqui calcular quantum del proceso elegido (almacenar o retornar el quantum)

    // sacamos el process de la cola, arreglando el ->next de su antecesor
    // tambien cubrimos el caso si sacamos el inicio o final de la cola
    Process* aux = queue->inicio;
    while(aux != NULL){
      if(elegido == queue -> inicio){
        queue->inicio = elegido->next;
      }
      if(elegido == queue -> last){ //solo pasaria cuando hay 1 process
        queue->last = NULL;
      }
      if(aux -> next == elegido){
        aux -> next = elegido->next;
        break;
      }
      aux = aux->next;
    }

    if(elegido != NULL){
      elegido -> next = NULL;
    }
    cpu -> exec = elegido;
}

int quantum(Queue* queue, int q, Process* process){
  int fabrica = process->fabrica;
  int n = 0;
  int f = 0;
  int fabricas_actuales[4] = {1,2,3,4};

  //printf("la fabrica en cuestion es %i\n", fabrica);
  Process* aux = queue->inicio;
  while(aux != NULL){
    if(aux->fabrica == fabrica){
      n++;
    }
    int indice = (aux->fabrica)-1;
    //printf("el indice es: %i\n ",indice);
    fabricas_actuales[indice] = NULL; // asumiendo que fabricas son del 1 al 4
    aux = aux->next;
  }

  // f: las que efectivamente hay en cola
  for (int i = 0; i < 4; i++){
    if(fabricas_actuales[i] == NULL){
      f++;
    }

  }
  //printf("el Q es: %i\n ",q);
  //printf("el n es: %i\n ",n);    
  //printf("el f es: %i\n ",f);

  int quantum = (q)/(n*f);
  return quantum;
}

void enviar_final_cola(Queue* queue, Process* process){
    //printf("funcion enviar final cola\n");
    if(queue->inicio == NULL){
      //printf("132");
      if(queue->last == NULL){
        printf("la cola esta vacía. %s queda al inicio y final\n",process->nombre);
        queue->inicio = process;
        queue->last = process;
      }
    }else{
      queue->last->next = process;
      queue->last = process;
      printf("la cola tiene procesos. %s queda al final\n",process->nombre);
    }
  }

// darle la raiz de la cola
void free_process(Process* proceso){
  printf("free de %s\n", proceso->nombre);
  if(proceso->next){
    free_process(proceso->next);
  }
  free(proceso);
}

void free_queue(Queue* queue){
  free(queue);
}

int main(int argc, char **argv)
{
  int q = 100;
  printf("Hello T2!\n");
  printf("argv 0: %s\n", argv[0]); //scheduler
  printf("argv 1: %s\n", argv[1]); //input.txt
  printf("argv 2: %s\n", argv[2]); //output.csv
  if(argv[3] != NULL){
    printf("argv 3: %s\n", argv[3]); // Q
    q = atoi(argv[3]);
  }

  Cpu* cpu = cpu_init();
  //Process* new_process;
  Queue* new_queue;
  Queue* process_queue;

  int contador_pid = 0;

  InputFile *file = read_file(argv[1]);

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
        contador_pid++;
        //printf("%s\n", line[0]);
        Process* new_process = process_init(contador_pid, line[0], atoi(line[1]), atoi(line[2]), 2*(atoi(line[3]))-1);
        //printf("Nuevo proceso de id: %i, nombre: %s, estado: %s\n", new_process->pid, new_process->nombre, new_process->estado);
        printf("Nuevo proceso de id: %i\n",new_process->pid);
        //printf("nombre: %s\n",new_process->nombre);
        //printf("estado: %s\n",new_process->estado);
        
        for (int i = 0; i < 2*(atoi(line[3]))-1; i++){
          //printf("rafaga[%i] : %i\n",i,atoi(line[4+i]));
          new_process->rafagas[i] = atoi(line[4+i]);
          //printf("new_process->rafagas[%i] %i\n",i, new_process->rafagas[i]);
        }
        

        /*
        int *rafagas[atoi(line[3])];
        for (int i = 0; i < 2*(atoi(line[3]))-1; i++){
          printf("i: %i input: %i\n",i, atoi(line[4+i]));
          int* raf = atoi(line[4+i]);
          //rafagas[i] = atoi(line[4+i]);
          rafagas[i] = raf;
        }
        // int *array_ptr = rafagas;
    
        new_process->rafagas = rafagas;
        // rafagas[0] seria un puntero que podemos referir en los struct process
      
        printf("rafaga: %i \n", new_process->rafagas[0]);
        printf("rafaga: %i \n", new_process->rafagas[1]);
        printf("rafaga: %i \n", new_process->rafagas[2]);
        printf("rafaga: %i \n", new_process->rafagas[3]);
        printf("rafaga: %i \n", new_process->rafagas[4]);
        */

        if(contador_pid == 1){
          new_queue = queue_init(file->len);
          process_queue = queue_init(file->len);
          //printf("Nueva cola de largo: %i\n", new_queue->cantidad);
          new_queue->inicio = new_process;
          //printf("new_queue->inicio->rafaga 0]: %i\n", new_queue->inicio->rafagas[0]);
          new_queue->last = new_process;
          //printf("el primero y ultimo de la fila es %s\n", new_queue->inicio->nombre);
          
        }else{
          new_queue->last->next = new_process;
          new_queue->last = new_process;
          //printf("new_queue->last->rafaga 0]: %i\n", new_queue->last->rafagas[0]);

        }
  }


  
  int tiempo = 0;
  int termino = 0;
  while(1){
    if(termino == 1){
      break;
    }
    /*
     1: si hay proceso en la cpu se debe hacer lo siguiente:
        1.1 IF proceso cede cpu: pasa a waiting y se va al final de la cola
        1.2 ELSE IF proceso termina ejecución, pasa a FINISHED y sale del sistema
        1.3 ELSE IF proceso consume su quantum, pasa a READY y se va al final de la cola
        1.4 ELSE proceso continua runnin
     2: Procesos creados entran a la cola, incluyendo el que sale de la CP
     3: Si no hay proceso en la CPU, se elije uno que pase a RUNNIN
     4: Se actualizan estadistica
     5: Los procesos WAITING que terminaron su I/O Burst pasan a READY
    */

    // 1: si hay proceso en la cpu se debe hacer lo siguiente:
    if(cpu->exec != NULL){
      //    1.1 IF proceso cede cpu: pasa a WAITING y se va al final de la cola
      // encuentro la rafaga actual, resto uno y comparo con 0.
      int indice_rafaga = 0;
      for (int i = 0; i < cpu->exec->cantidad_rafagas; i++){
        if(cpu->exec->rafagas[i] != 0){
          indice_rafaga = i;
          break;
        }
      }
      cpu->exec->rafagas[indice_rafaga] = cpu->exec->rafagas[indice_rafaga] - 1;
      if(cpu->exec->rafagas[indice_rafaga] == 0){
        if(indice_rafaga == (cpu->exec->cantidad_rafagas - 1)){
        //    1.2 ELSE IF proceso termina ejecución, pasa a FINISHED y sale del sistema
          cpu->exec->estado = "FINISHED";
          printf("[t = %i] process %s pasa a FINISHED\n", tiempo, cpu->exec->nombre);
          cpu->exec->turnaround_time = (tiempo - cpu->exec->tiempo_inicio);
          printf("turnaround time %i\n", cpu->exec->turnaround_time);
          printf("elecciones %i\n", cpu->exec->elecciones);
          printf("interrupciones %i\n", cpu->exec->interrupciones);
          cpu->exec = NULL;
          if(process_queue->inicio == NULL){
            if(process_queue->last == NULL){
              termino = 1;
            }
          }
        }else{
          cpu->exec->estado = "WAITING";
          printf("[t = %i] process %s pasa a WAITING\n", tiempo, cpu->exec->nombre);
          enviar_final_cola(process_queue, cpu->exec);
          cpu->exec = NULL;
        }
      }else if(cpu->exec->quantum == 1){
        //    1.3 ELSE IF proceso consume su quantum, pasa a READY y se va al final de la cola
        cpu->exec->quantum = cpu->exec->quantum - 1;
        if(cpu->exec->quantum == 0){
          cpu->exec->estado = "READY";
          printf("[t = %i] process %s pasa a READY\n", tiempo, cpu->exec->nombre);
          cpu->exec->interrupciones++;
          enviar_final_cola(process_queue, cpu->exec);
          cpu->exec = NULL;
        }
      }else{
        //    1.4 ELSE proceso continua running
        cpu->exec->quantum = cpu->exec->quantum - 1;
      }
    }
    // 2: Procesos creados entran a la cola, incluyendo el que sale de la CPU

    /*
                       C O D I G O     L E G A D O
    */

    Process* prioridad;
    int menor_n_fabrica = 10;
    char menor_n_proceso;
    
    // contamos cuantos son los procesos a ingresar
    int ctdad_procesos_init = 0;
    //printf("* new_queue->inicio->rafagas[0]: %i\n", new_queue->inicio->rafagas[0]);
    Process* aux_init = new_queue->inicio;
    //printf("* new_queue->inicio->rafagas[0]: %i\n", new_queue->inicio->rafagas[0]);
    //printf("* aux_init rafagas[0]: %i\n", aux_init->rafagas[0]);
    while(aux_init != NULL){
      if(aux_init->tiempo_inicio == tiempo){
        ctdad_procesos_init++;
      }
      aux_init = aux_init->next;
    }
    
    //printf("Existen %i procesos con tiempo de inicio en este segundo\n", ctdad_procesos_init);
  
    for (int i = 0; i < ctdad_procesos_init; i++){
      //recorrer new_queue
      //traspasar de new_queue a process_queue el de mayor prioridad
      //printf("1\n");
      Process* aux = new_queue->inicio;
      while(aux != NULL){
        //printf("2\n");
        if(aux->tiempo_inicio == tiempo){
          //printf("3\n");
          if(aux->fabrica <= menor_n_fabrica){
            //printf("4\n");
            prioridad = aux;
            }
          }
        aux = aux -> next;
      }
      //printf("5\n");
  
      // aqui ya elegimos el Process* prioridad
        Process* aux2 = new_queue->inicio;
        //printf("6\n");
        while(aux2 != NULL){
          //printf("7\n");
          if(prioridad == new_queue -> inicio){
            //printf("8\n");
            new_queue->inicio = prioridad->next;
          }
          //printf("9\n");
          if(prioridad == new_queue -> last){ //solo pasaria cuando hay 1 process
            //printf("10\n");
            new_queue->last = NULL;
          }
          //printf("11\n");
          if(aux2 -> next == prioridad){
            //printf("12\n");
            aux2 -> next = prioridad->next;
            //printf("13\n");
            break;
          }
          //printf("14\n");
          aux2 = aux2 -> next;
          //printf("15\n");
        }
        //printf("16\n");
        if(prioridad != NULL){
          prioridad -> next = NULL;
        }
        //printf("el proceso %s salio de new_queue  con rafaga[0]: %i\n", prioridad->nombre, prioridad->rafagas[0]);
        // hasta aqui ya sacamos el proceso_priorida de new_queue
        //falta ingresarlo a process_queue
          
        prioridad->estado = "READY"; // creo que ya esta en READY
        //printf("17\n");
        if (process_queue -> inicio == NULL){
          printf("asignando inicio y final de process queue\n");
          process_queue -> inicio = prioridad;
          process_queue -> last = prioridad;
          prioridad->tiempo_inicio = tiempo;
        }else{
          printf("asignando final de process queue\n");
          process_queue -> last -> next = prioridad;
          process_queue -> last = prioridad;
          prioridad->tiempo_inicio = tiempo;
        }
        //printf("18\n");
        prioridad = NULL;
        printf("el proceso %s es el primero de process_queue\n", process_queue->inicio->nombre);
        printf("el proceso %s es el ultimo de process_queue\n", process_queue->last->nombre);
    }
    // 3: Si no hay proceso en la CPU, se elije uno que pase a RUNNING
    if(process_queue->inicio == NULL){
      //printf("No hay procesos en cola momentaneamente.\n");
      
    }else if(cpu->exec == NULL){
      // caso base: inicio READY
      //printf("382\n");
      if(process_queue->inicio->estado == "READY"){
        //printf("391\n");
        int quantum1 = quantum(process_queue, q, process_queue->inicio);
        cpu->exec = process_queue->inicio;
        cpu->exec->quantum = quantum1;
        cpu->exec->estado = "RUNNING";
        printf("[t = %i] process %s pasa a RUNNING\n", tiempo, cpu->exec->nombre);
        cpu->exec->elecciones++;
        process_queue->inicio = cpu->exec->next;
        if(process_queue->inicio == NULL){
          process_queue->last = NULL;
        }
        cpu->exec->next = NULL;
        //printf("399\n");
      }else{
        //printf("401\n");
        Process* aux_ready = process_queue->inicio;
        while(aux_ready->next != NULL){
          //printf("404\n");
          if(aux_ready->next->estado == "READY"){
            int quantum1 = quantum(process_queue, q, aux_ready->next);
            cpu->exec = aux_ready->next;
            cpu->exec->quantum = quantum1;
            cpu->exec->estado = "RUNNING";
            printf("[t = %i] process %s pasa a RUNNING\n", tiempo, cpu->exec->nombre);
            cpu->exec->elecciones++;
            aux_ready->next = cpu->exec->next;
            cpu->exec->next = NULL;
          }
          aux_ready = aux_ready->next;
        }
        //printf("415\n");
      }
    }

    // 4: Se actualizan estadisticas

    // 5: Los procesos WAITING que terminaron su I/O Burst pasan a READY
    Process* aux_waiting = process_queue->inicio;
    while(aux_waiting != NULL){
      //printf("425\n");
      if(aux_waiting->estado == "WAITING"){
        //printf("427\n");
        int indice_rafaga = 0;
        for (int i = 0; i < aux_waiting->cantidad_rafagas; i++){
          if(aux_waiting->rafagas[i] != 0){
            indice_rafaga = i;
            break;
          }
        }
        //printf("435\n");
        aux_waiting->rafagas[indice_rafaga] = aux_waiting->rafagas[indice_rafaga]-1;
        if(aux_waiting->rafagas[indice_rafaga] == 0){
          //printf("438\n");
          aux_waiting->estado = "READY";
        }
      }
      //printf("442\n");
      aux_waiting = aux_waiting->next;
    }
  tiempo++;
  }
  printf("a\n");
  //free_process(process_queue->inicio);
  printf("b\n");
  //free_queue(new_queue);
  printf("c\n");
  //free_queue(process_queue);
}

