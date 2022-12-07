/*	Práctica 1 - Segundo Parcial		Algoritmo de Planificación FCFS (First Come First Served)		Guzmán Peralta Luis Fernando
	Sistemas Operativos											6CM23									Ingeniería en Computación	*/
	
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#define quantum 4

int x = 0, y = 0, y2 = 0;

void creaPCB(void);
void creaTareas(void);
void etiqueta(void);
void muestraTareas(void);
void muestraPCB(void);
void ejecucion(void);
void calculos(void);

struct PCB {
	int id_proceso;
	int num_pag;
	int t_llegada;
	int ciclos;
	int ciclos_aux;
	int memoria;
	int cpuES;
	int nes;
	int archivos;
	int tipo;
	int estado;
	int trp;
	int inicio_SC;
	int duracion_SC;
	int ciclos_SC;
	int enPCB;
	struct PCB *sig;
};

struct PCB_SEMAFORO {
	int id_proceso;
	int num_pag;
	int t_llegada;
	int ciclos;
	int ciclos_aux;
	int memoria;
	int cpuES;
	int nes;
	int archivos;
	int tipo;
	int estado;
	int trp;
	int inicio_SC;
	int duracion_SC;
	int ciclos_SC;
	int enPCB;
	struct PCB_SEMAFORO *sig;
};

struct Tareas {
	int tarea;			//Tarea[n]
	int cont;			//contador de seleccion
	int pag;			//P0, P1, P2, etc
	int sel[10];	
	int pagina[10];		//
	struct Tareas *sig;
};

struct PCB *pcb_inicio, *pcb_nuevo, *pcb_final, *pcb_aux1, *pcb_respaldo, *pcb_aux2, *pcb_aux3, *pcb_aux4;
struct PCB_SEMAFORO *S_inicio, *S_nuevo, *S_final, *S_aux1;
struct Tareas *t_inicio, *t_nuevo, *t_final, *t_aux1;
int cont = 0, auxProceso = 0, auxPagina = 0, auxLlegada = 0, auxCiclos = 0, auxMemoria = 0, auxCPU = 0, auxDispositivos = 0, auxArchivos = 0, auxTipo = 0, auxEstado = 0;

int main() {
	pcb_inicio = NULL;
	pcb_final = NULL;
	S_inicio = NULL;
	S_final = NULL;
	t_inicio = NULL;
	t_final = NULL;
	int opc;
	
	creaTareas();
	muestraTareas();
	creaPCB();
	muestraPCB();
	ejecucion();
	calculos();
	muestraPCB();
	getch();
}

void creaTareas() {
	int i, j, k;
	srand(time(NULL));
	for(i=0; i<5; i++)
	{
		//srand(time(NULL));
		
		
		//getch();
		if(t_inicio == NULL)
		{
			//srand(time(NULL));
			y = 0;
			y = 1 + rand() % 8;
			printf("y = %i\n",y);
			t_inicio = (struct Tareas*) malloc(sizeof(struct Tareas));
			t_inicio->tarea = i+1;
			for(j=0; j<y; j++)
			{
				t_inicio->pagina[j] = j;
				t_inicio->sel[j] = 0 + rand() % 2;
				
				if(t_inicio->sel[j] == 1)
				{
					t_inicio->cont++;
				}
			}
			t_inicio->sig = NULL;
			t_final = t_inicio;
		}
		else
		{
			//srand(time(NULL));
			y2 = 0;
			y2 = 1 + rand() % 8;
			printf("y2 = %i\n",y2);
			t_nuevo = (struct Tareas*) malloc(sizeof(struct Tareas));
			t_nuevo->tarea = i+1;
			for(k=0; k<y2; k++)
			{
				t_nuevo->pagina[k] = k;
				t_nuevo->sel[k] = 0 + rand() % 2;
				if(t_nuevo->sel[k] == 1)
				{
					t_nuevo->cont++;
				}
			}
			t_nuevo->sig = NULL;
			t_final->sig = t_nuevo;
			t_final = t_nuevo;
		}
	}
}

void muestraTareas() {
	int i, j;
	
	printf("\n ::::: TABLA DE MAPA DE PAGINAS :::::");
	printf("\n-------------------------------------");
	t_aux1 = t_inicio;
	while(t_aux1 != NULL)
	{
		printf("\nTarea %i\n",t_aux1->tarea);
		if(t_aux1 == t_inicio)
		{
			for(i=0; i<y; i++)
			{
				printf("J%iP%i\t", t_aux1->tarea, t_aux1->pagina[i]);
				printf("%i\n",t_aux1->sel[i]);
			}
			printf("\nSeleccion: %i", t_aux1->cont);
		}
		else
		{
			for(j=0; j<y2; j++)
			{
				printf("J%iP%i\t", t_aux1->tarea, t_aux1->pagina[j]);
				printf("%i\n",t_aux1->sel[j]);
			}
			printf("\nSeleccion: %i", t_aux1->cont);
		}
		t_aux1 = t_aux1->sig;
		printf("\n");
	}
}

void creaPCB() {
	int i, inc = 0, res;
	
	srand(time(NULL));
	t_aux1 = t_inicio;
	while(t_aux1 != NULL)
	{
		for(i=0; i<9; i++)
		{
			if(t_aux1->sel[i] == 1)
			{
				if(pcb_inicio == NULL)
				{
					pcb_inicio = (struct PCB*) malloc(sizeof(struct PCB));
					pcb_inicio->id_proceso = t_aux1->tarea;
					pcb_inicio->num_pag = t_aux1->pagina[i];
					pcb_inicio->t_llegada = inc;
					pcb_inicio->ciclos = 2 + rand()%9;
					pcb_inicio->ciclos_aux = pcb_inicio->ciclos;
					pcb_inicio->estado = 1;
					pcb_inicio->memoria = 5 + rand()%20;
					pcb_inicio->cpuES = 0 + rand()%2;
					pcb_inicio->nes = 1 + rand()%9;
					pcb_inicio->archivos = 2 + rand()%6;
					pcb_inicio->tipo = 0 + rand()%2;
					pcb_inicio->inicio_SC = 0 + rand()%pcb_inicio->ciclos;
					res = pcb_inicio->ciclos - pcb_inicio->inicio_SC;
					if(pcb_inicio->inicio_SC != 0) 
					{
						pcb_inicio->duracion_SC = 1 + rand()%res;
					}
					else
					{
						pcb_inicio->duracion_SC = 0;
					}
					pcb_inicio->ciclos_SC = 0;
					pcb_inicio->enPCB = 0;
					pcb_inicio->sig = NULL;
					pcb_final = pcb_inicio;
					inc++;
				}
				else
				{
					pcb_nuevo = (struct PCB*) malloc(sizeof(struct PCB));
					pcb_nuevo->id_proceso = t_aux1->tarea;
					pcb_nuevo->num_pag = t_aux1->pagina[i];
					pcb_nuevo->t_llegada = inc;
					pcb_nuevo->ciclos = 2 + rand()%9;
					pcb_nuevo->ciclos_aux = pcb_nuevo->ciclos;
					pcb_nuevo->estado = 1;
					pcb_nuevo->memoria = 5 + rand()%20;
					pcb_nuevo->cpuES = 0 + rand()%2;
					pcb_nuevo->nes = 1 + rand()%9;
					pcb_nuevo->archivos = 1 + rand()%6;
					pcb_nuevo->tipo = 0 + rand()%2;
					pcb_nuevo->inicio_SC = 0 + rand()%pcb_nuevo->ciclos;
					res = pcb_nuevo->ciclos - pcb_nuevo->inicio_SC;
					if(pcb_nuevo->inicio_SC != 0) 
					{
						pcb_nuevo->duracion_SC = 1 + rand()%res;
					}
					else
					{
						pcb_nuevo->duracion_SC = 0;
					}
					pcb_nuevo->ciclos_SC = 0;
					pcb_nuevo->enPCB = 0;
					pcb_nuevo->sig = NULL;
					pcb_final->sig = pcb_nuevo;
					pcb_final = pcb_nuevo;
					inc++;
				}
			}
		}
		t_aux1 = t_aux1->sig;
	}	
}

void muestraPCB() {
	printf("\n\n\t\t\t\t\t::::: BLOQUE DE CONTROL DE PROCESOS (PCB) :::::");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\nPROCESO \tT.LLEGADA \tCICLOS \tESTADO \tMEMORIA \tCPU/E/S \tN. DISP \tN. ARCH \tTIPO \tINICIO SC \tCICLOS SC \tDURACION SC\n");
	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	pcb_aux1 = pcb_inicio;
	while(pcb_aux1 != NULL)
	{
		if(pcb_aux1->enPCB == 0)
		{
			printf("J%iP%i\t\t    %it\t\t   %i\t   %i\t   %i\t\t   %i\t\t   %i\t\t   %i\t\t  %i\t   \t%i   \t%i   \t\t%i",pcb_aux1->id_proceso, pcb_aux1->num_pag, pcb_aux1->t_llegada, pcb_aux1->ciclos, pcb_aux1->estado, pcb_aux1->memoria, pcb_aux1->cpuES, pcb_aux1->nes, pcb_aux1->archivos, pcb_aux1->tipo, pcb_aux1->inicio_SC, pcb_aux1->ciclos_SC, pcb_aux1->duracion_SC);
			printf("\n");
		}
		pcb_aux1 = pcb_aux1->sig;
	}
}

void muestraPCB_Semaforo() {
	printf("\n\n\t\t\t\t ::::: BLOQUE DE CONTROL DE PROCESOS (PCB) DE SEMAFORO :::::");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\nPROCESO \tT.LLEGADA \tCICLOS \tESTADO \tMEMORIA \tCPU/E/S \tN. DISP \tN. ARCH \tTIPO \tINICIO SC \tCICLOS SC \tDURACION SC\n");
	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	S_aux1 = S_inicio;
	while(S_aux1 != NULL)
	{
		if(S_aux1->enPCB == 1)
		{
			printf("J%iP%i\t\t    %it\t\t   %i\t   %i\t   %i\t\t   %i\t\t   %i\t\t   %i\t\t  %i\t   \t%i   \t%i   \t\t%i",S_aux1->id_proceso, S_aux1->num_pag, S_aux1->t_llegada, S_aux1->ciclos, S_aux1->estado, S_aux1->memoria, S_aux1->cpuES, S_aux1->nes, S_aux1->archivos, S_aux1->tipo, S_aux1->inicio_SC, S_aux1->ciclos_SC, S_aux1->duracion_SC);
			printf("\n");
		}
		S_aux1 = S_aux1->sig;
	}
}

void calculos() {
	int i, j, t_inicial = 0, t_retorno_proceso = 0, t_espera=0, num_procesos = 0;
	double TRPS = 0.0, suma_TRP = 0.0;

	pcb_aux1 = pcb_inicio;
	printf("\n\n\t\t     T.R.P.    ");
	printf("\n\t\t-------------    ");
	while(pcb_aux1 != NULL)
	{
		t_retorno_proceso += pcb_aux1->ciclos_aux;
		printf("\n\t\t   J%iP%i = %ims", pcb_aux1->id_proceso, pcb_aux1->num_pag, t_retorno_proceso);
		pcb_aux1 = pcb_aux1->sig;
		num_procesos++;
	}
	
	pcb_aux1 = pcb_inicio;
	t_retorno_proceso = 0;
	printf("\n\n\n\t\t     T.E.    ");
	printf("\n\t\t-------------    ");
	while(pcb_aux1 != NULL)
	{
		t_retorno_proceso += pcb_aux1->ciclos_aux;
		t_espera = t_retorno_proceso - pcb_aux1->ciclos_aux;
		suma_TRP = suma_TRP + t_retorno_proceso;
		printf("\n\t\t   J%iP%i = %ims", pcb_aux1->id_proceso, pcb_aux1->num_pag, t_espera);
		pcb_aux1 = pcb_aux1->sig;
	}
	TRPS = suma_TRP / num_procesos;
	printf("\n\n\t\tSumatoria de los Tiempos de Retorno de Proceso = %0.0fms", suma_TRP);
	printf("\n\t\tTiempo de Retorno Promedio del Sistema (TRPS) = %0.0fms/%i = %2.2fms\n", suma_TRP, num_procesos, TRPS);
}

void ejecucion() {
	int iterador = 0;
	bool band = false;
	
	getch();
	system("cls");
	pcb_aux2 = pcb_inicio;
	
	while(pcb_aux2 != NULL)
	{
		pcb_aux2->estado = 2;
		pcb_aux2 = pcb_aux2->sig;
	}
	muestraPCB();
	getch();
	system("cls");
	
	pcb_aux2 = pcb_inicio;
	while(pcb_aux2 != NULL || band == true)
	{
		pcb_aux2->estado = 3;
		iterador = 0;
		while(pcb_aux2->ciclos != 0 && iterador != quantum)
		{
			pcb_aux2->ciclos = pcb_aux2->ciclos - 1;
			iterador++;
			if(pcb_aux2->inicio_SC != 0)
			{
				pcb_aux2->ciclos_SC++;
			}
			if(pcb_aux2->ciclos_SC == pcb_aux2->inicio_SC && pcb_aux2->inicio_SC != 0)
			{
				if(S_inicio == NULL)
				{
					S_inicio = (struct PCB_SEMAFORO*) malloc(sizeof(struct PCB_SEMAFORO));
					S_inicio->id_proceso = pcb_aux2->id_proceso;
					S_inicio->num_pag = pcb_aux2->num_pag;
					S_inicio->t_llegada = pcb_aux2->t_llegada;
					S_inicio->ciclos = pcb_aux2->ciclos;
					S_inicio->ciclos_aux = pcb_aux2->ciclos_aux;
					S_inicio->estado = pcb_aux2->estado;
					S_inicio->memoria = pcb_aux2->memoria;
					S_inicio->cpuES = pcb_aux2->cpuES;
					S_inicio->nes = pcb_aux2->nes;
					S_inicio->archivos = pcb_aux2->archivos;
					S_inicio->tipo = pcb_aux2->tipo;
					S_inicio->inicio_SC = pcb_aux2->inicio_SC;
					S_inicio->ciclos_SC = pcb_aux2->ciclos_SC;
					S_inicio->duracion_SC = pcb_aux2->duracion_SC;
					S_inicio->enPCB = 1;
					pcb_aux2->enPCB = S_inicio->enPCB;
					S_inicio->sig = NULL;
					S_final = S_inicio;
				}
				else
				{
					S_nuevo = (struct PCB_SEMAFORO*) malloc(sizeof(struct PCB_SEMAFORO));
					S_nuevo->id_proceso = pcb_aux2->id_proceso;
					S_nuevo->num_pag = pcb_aux2->num_pag;
					S_nuevo->t_llegada = pcb_aux2->t_llegada;
					S_nuevo->ciclos = pcb_aux2->ciclos;
					S_nuevo->ciclos_aux = pcb_aux2->ciclos_aux;
					S_nuevo->estado = pcb_aux2->estado;
					S_nuevo->memoria = pcb_aux2->memoria;
					S_nuevo->cpuES = pcb_aux2->cpuES;
					S_nuevo->nes = pcb_aux2->nes;
					S_nuevo->archivos = pcb_aux2->archivos;
					S_nuevo->tipo = pcb_aux2->tipo;
					S_nuevo->inicio_SC = pcb_aux2->inicio_SC;
					S_nuevo->ciclos_SC = pcb_aux2->ciclos_SC;
					S_nuevo->duracion_SC = pcb_aux2->duracion_SC;
					S_nuevo->enPCB = 1;
					pcb_aux2->enPCB = S_nuevo->enPCB;
					S_nuevo->sig = NULL;
					S_final->sig = S_nuevo;
					S_final = S_nuevo;
				}
				system("cls");
				muestraPCB();
				printf("\n");
				muestraPCB_Semaforo();
				getch();
			}
			
			
			if(pcb_aux2->ciclos <= 0)
			{
				pcb_aux2->estado = 5;
				band = false;
			}
			
			system("cls");
			muestraPCB();
			printf("\n");
			muestraPCB_Semaforo();
			getch();
		}
		if(pcb_aux2->ciclos <= 0)
		{
			pcb_aux2->estado = 5;
			band = false;
		}
		if(pcb_aux2->ciclos != 0)
		{
			pcb_aux2->estado = 4;
			band = true;
		}
		pcb_aux2 = pcb_aux2->sig;
		if(pcb_aux2 == NULL)
		{
			pcb_aux2 = pcb_inicio;
			while(pcb_aux2 != NULL)
			{
				if(pcb_aux2->ciclos != 0)
				{
					pcb_aux2->estado = 2;
				}
				pcb_aux2 = pcb_aux2->sig;
			}
			muestraPCB();
			getch();
			system("cls");
			pcb_aux2 = pcb_inicio;
		}
	}
}
