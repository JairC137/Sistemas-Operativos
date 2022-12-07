#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>


int lineas_codigo=100, Ctareas=4, temp, RR=5,tiempo1=0;

struct Procesos
{
    struct Procesos *siguienteP;
/*    int CSC;  //ciclos en seccion critica
	int state=0;
	struct PCB *direccion;
*/	// state en 0 = W(SEM)  state en 1 = Sec. Crit state en 2 = S(SEM) state en 3 = espera}
	
	int proceso;
   	int nPagina;
    int tLlegada;
    int ciclos;
    int inC;
    int estado;
    int memoria;
    int cpuES;
    int numDisp;
    int archivos;
    int tipo;
    int tfin;
    int iniSC;
    int duraSC;
};

struct semaforo
{
	struct semaforo *siguienteS;
/*	int llegada;
	struct Procesos *EP;
	int semB;
	int SC;
*/
	
};

struct PCB
{
   	struct PCB *siguiente;
   	int proceso;
   	int nPagina;
    int tLlegada;
    int ciclos;
    int inC;
    int estado;
    int memoria;
    int cpuES;
    int numDisp;
    int archivos;
    int tipo;
    int tfin;
    int iniSC;
    int duraSC;
    
};
struct TRP
{
	struct TRP *next;
	int process;
	int page;
	int time;
};

struct tabla_tareas
{
   	struct tabla_tareas *siguiente2;
   	int tarea;
   	int lineas;
    struct tabla_mapa_paginas *loc_PMT;
};
struct mapa_paginas
{
   	struct mapa_paginas *siguiente3;
   	int pagina;
   	int estado;
};
struct tabla_mapa_paginas
{
	struct tabla_mapa_paginas *siguiente4;
    struct mapa_paginas *mp;
};
struct Procesos *P_P =NULL, *Q_P, *N_P, *AUXP;
struct semaforo *P_S =NULL, *Q_S, *N_S, *AUXS;
struct TRP *P_trp =NULL, *Q_trp, *N_trp, *AUX0, *AUX00;
struct PCB *P_mm =NULL, *Q_mm, *N_mm, *AUX, *AUX1, *i, *j;
struct tabla_tareas *P_tt =NULL, *Q_tt, *N_tt, *AUX2;
struct mapa_paginas *P_mp =NULL, *Q_mp, *N_mp, *AUX3;
struct tabla_mapa_paginas *P_tmp =NULL, *Q_tmp, *N_tmp, *AUX4;

mapa_paginas * crear_mp()
{ 
	srand(time(NULL));
	int Cpagina = ceil((temp/lineas_codigo)+0.4999);

	for(int i=0; i<Cpagina; i++){		
      	if(i == 0){
       	 	P_mp = (mapa_paginas *)malloc(sizeof(mapa_paginas));
         	P_mp -> pagina  = i;        		
         	P_mp -> estado   = 0+rand()%2;
         	P_mp -> siguiente3    = NULL;
         	Q_mp = P_mp;
        }else{
      		N_mp = (mapa_paginas *)malloc(sizeof(mapa_paginas));
         	N_mp -> pagina  = i;
         	N_mp -> estado   = 0+rand()%2;
         	N_mp -> siguiente3    = NULL;
         	Q_mp -> siguiente3    = N_mp;
         	Q_mp = N_mp;
   		}	
	}		



   return(P_mp);	
}
tabla_mapa_paginas * crear_tmp()
{
	
   	if(P_tmp == NULL){
      	 P_tmp = (tabla_mapa_paginas *)malloc(sizeof(tabla_mapa_paginas));
         P_tmp -> mp = crear_mp();                                              //mp es un puntero que apunta a la tarea en especifico
         P_tmp -> siguiente4    = NULL;
         Q_tmp = P_tmp;
      }else{
      	N_tmp = (tabla_mapa_paginas *)malloc(sizeof(tabla_mapa_paginas));
         N_tmp -> mp = crear_mp();
         N_tmp -> siguiente4    = NULL;
         Q_tmp -> siguiente4    = N_tmp;
         Q_tmp = N_tmp;
   }
   return(Q_tmp);
}
void crear_tt()
{
	srand(time(NULL));

	for(int i=0; i<Ctareas; i++){
   	if(P_tt == NULL){

      	 P_tt = (tabla_tareas *)malloc(sizeof(tabla_tareas));
         P_tt -> tarea        = i+1;
         temp                 = 50+rand()%550;                              //Cantidad de paginas por tarea (EDITABLE
		 P_tt -> lineas       = temp;
         P_tt -> loc_PMT      = crear_tmp();                                //loc_PMT Es un puntero que apunta al mapa de paginas correspondiente
         P_tt -> siguiente2   = NULL;
         Q_tt = P_tt;
         
      }else{

      	N_tt = (tabla_tareas *)malloc(sizeof(tabla_tareas));
         N_tt -> tarea        = i+1;
         temp                 = 50+rand()%550; 
		 N_tt -> lineas       = temp;       
         N_tt -> loc_PMT      = crear_tmp();
         N_tt -> siguiente2   = NULL;
         Q_tt -> siguiente2   = N_tt;
         Q_tt  = N_tt;
      }
      	
   }
}
void imprimir_tt(){
	printf("\n\nTABLA DE TAREAS");
	AUX2 = P_tt;
	printf("\nTareas\tNo. Lineas");
    do{  	
   		printf("\nJ%i\t%i",AUX2->tarea, AUX2->lineas);
    	AUX2 = AUX2->siguiente2;
    }while(AUX2 != NULL);
}
void imprimir_tmp()
{
	int cont_T=1;
	printf("\nTABLA DE MAPA DE PAGINAS");
	AUX4=P_tmp;
	do
	{
		printf("\n\nTAREA[%i]",cont_T);
		AUX3=AUX4->mp;
		do
		{
            printf("\nJ%iP%i\t%i",cont_T,AUX3->pagina,AUX3->estado);
			AUX3=AUX3->siguiente3;	
		}while(AUX3!=NULL);
		cont_T++;
		AUX4=AUX4->siguiente4;	
	}while(AUX4!=NULL);
}

void crear_PCB()
{
	printf("\n\n\n");
	srand(time(NULL));
	AUX2=P_tt;
	int llegada = 0;
	do{

		AUX4=AUX2->loc_PMT;
		AUX3=AUX4->mp;
		do{

			if(AUX3->estado==1){
				int llega=0;
				int dur=0;
				int auxi=0;
				int vC=0;
				
				if(P_mm == NULL){
      	 			P_mm = (PCB *)malloc(sizeof(PCB));
         			P_mm -> proceso  = AUX2->tarea;
         			P_mm -> nPagina  = AUX3->pagina;
         			P_mm -> tLlegada = llegada;
         			
         			vC=1+rand()%10;
         			
         			P_mm -> ciclos   =  vC;
         			P_mm -> inC      = 1;
         			P_mm -> estado   = 1;
         			P_mm -> memoria  = 50+rand()%150;
         			P_mm -> cpuES    = 0+rand()%3;
         			if(P_mm -> cpuES == 0){
         				P_mm -> numDisp  = 0;
					}else{
						P_mm -> numDisp  = 1+rand()%3;
					}
         			P_mm -> archivos = 0+rand()%11;
         			P_mm -> tipo     = 0+rand()%2;
         			
         			llega = 1+rand()%vC;
         			
         			P_mm -> iniSC  = llega-1;
         			
         			auxi=vC-llega;
         			
         			if(auxi==0){
         				dur=0+rand()%2;
					}else{
						if(auxi>3){
							dur=0+rand()%4;	
						}else{
						   dur=0+rand()%auxi;	
						}
					 	
					}		
					if(dur==0){
						P_mm -> iniSC  = 0;
					}
         			P_mm -> duraSC  = dur;
         			
         			P_mm -> siguiente= NULL;
         			Q_mm = P_mm;     
    			}else{
   	 				N_mm = (PCB *)malloc(sizeof(PCB));
         			N_mm -> proceso  = AUX2->tarea;
         			N_mm -> nPagina  = AUX3->pagina;
         			N_mm -> tLlegada = llegada;
         			
         			vC=1+rand()%10;
         			
         			N_mm -> ciclos   = vC; 
         			N_mm -> inC      = 1;
         			N_mm -> estado   = 1;
         			N_mm -> memoria  = 50+rand()%150;
         			N_mm -> cpuES    = 0+rand()%2;
         			if(N_mm -> cpuES == 0){
         				N_mm -> numDisp  = 0;
					}else{
						N_mm -> numDisp  = 1+rand()%3;
					}
         			N_mm -> archivos = 0+rand()%5;
         			N_mm -> tipo     = 0+rand()%1;
         			
         			llega = 1+rand()%vC;
         			
         			N_mm -> iniSC  = llega-1;
         			
         			auxi=vC-llega;
         			if(auxi==0){
         				dur=0+rand()%2;
					}else{
					 	 if(auxi>3){
							dur=0+rand()%4;	
						}else{
						   dur=0+rand()%auxi;	
						}
					}
					if(dur==0){
						N_mm -> iniSC  = 0;
					}
         			N_mm -> duraSC  = dur;
         			
         			N_mm -> siguiente= NULL;
         			Q_mm -> siguiente= N_mm;     			
         			Q_mm = N_mm;  
      			}
				llegada++;			
			}
			
			AUX3=AUX3->siguiente3;
		}while(AUX3!=NULL);
      	AUX2=AUX2->siguiente2;
    }while(AUX2!=NULL);
}
void imprimir_PCB(){

	printf("\n BLOQUE DE CONTROL DE PROCESOS ");
	AUX = P_mm;
	printf("\nProceso\tT. Llegada\tCiclos\tEstado\tMemoria\tCPU/E/S\tNum_Disp\tNo. Arch\tTipo\tSeccion Critica\t Duracion SC");
   do{
   		bool valImp = true;
   		if(AUX->duraSC!=0){
   			if(AUX->inC>=AUX->iniSC && AUX->inC<(AUX->iniSC+AUX->duraSC)){
   				valImp=false;
			}
		}
   		if(valImp==true){
   			printf("\nJ%iP%i\t%it\t\t%ims\t%i\t%iKB\t%i\t%i\t\t%i\t\t%i\t%i\t\t%i",AUX->proceso, AUX->nPagina, AUX->tLlegada,AUX->ciclos,AUX->estado,AUX->memoria,AUX->cpuES,AUX->numDisp,AUX->archivos,AUX->tipo,AUX->iniSC,AUX->duraSC);
		}	 	
      	AUX = AUX->siguiente;
   }while(AUX != NULL);
   
   	printf("\n PCB (SEMAFORO) ");
	AUXP = P_P;
	printf("\nProceso\tT. Llegada\tCiclos\tEstado\tMemoria\tCPU/E/S\tNum_Disp\tNo. Arch\tTipo\tSeccion Critica\t Duracion SC");
	if(AUXP != NULL){
		do{ 	
   			printf("\nJ%iP%i\t%it\t\t%ims\t%i\t%iKB\t%i\t%i\t\t%i\t\t%i\t%i\t\t%i",AUXP->proceso, AUXP->nPagina, AUXP->tLlegada,AUXP->ciclos,AUXP->estado,AUXP->memoria,AUXP->cpuES,AUXP->numDisp,AUXP->archivos,AUXP->tipo,AUXP->iniSC,AUXP->duraSC);	 	
      		AUXP = AUXP->siguienteP;
   		}while(AUXP != NULL);
	}
}
void TimeReturnProcess()
{
	AUX=P_mm;
	
	do{
		if(P_trp == NULL){
    		P_trp = (TRP *)malloc(sizeof(TRP));
        	P_trp -> process = AUX->proceso;                                       
        	P_trp -> page    = AUX->nPagina;
        	P_trp -> time    = AUX->tfin-AUX->tLlegada;
        	P_trp -> next    = NULL;
        	Q_trp = P_trp;
    	}else{
      		N_trp = (TRP *)malloc(sizeof(TRP));
        	N_trp -> process = AUX->proceso;   
        	N_trp -> page    = AUX->nPagina;
        	N_trp ->time     = AUX->tfin-AUX->tLlegada;
        	Q_trp -> next    = N_trp;
        	Q_trp = N_trp;
        	Q_trp -> next    = NULL;
   		}
   		AUX=AUX->siguiente;
	}while(AUX!=NULL);
}
void imprimir_TRP()
{
	printf("\nTIEMPO DE RETORNO DE PROCESO ");
	AUX0 = P_trp;
	printf("\n");
    do{
 	 	printf("\nJ%iP%i  =\t%i",AUX0->process, AUX0->page, AUX0->time);
      	AUX0 = AUX0->next;
      	
   }while(AUX0 != NULL);
   
}
void TPS()
{
	
	AUX0=P_trp;
	float timeProm=0;
	int nPag=0;

	do
	{
		timeProm=timeProm+AUX0->time;
		AUX0=AUX0->next;
		nPag++;
	}while(AUX0!=NULL);
	timeProm=timeProm/nPag;
	printf("\nTiempo promedio del sistema: %0.3f",timeProm);
	
};
void TablaDeEspera(){
	printf("\n TABLA DE ESPERA ");
	AUX0 = P_trp;
	AUX00 = AUX0;
	printf("\n");
    do{
    	if(AUX0==P_trp){
    		printf("\nJ%iP%i  =\t0",AUX0->process, AUX0->page);
		}else{
			printf("\nJ%iP%i  =\t%i",AUX0->process, AUX0->page, AUX00->time);
			AUX00=AUX00->next;	
		}	 	
      	AUX0 = AUX0->next;
   }while(AUX0 != NULL);
}
void activar(struct PCB *P){
	
/*	if(P_P == NULL){
		P_P = (Procesos *)malloc(sizeof(Procesos));
       	P_P -> CSC           = P->duraSC;                                       
       	P_P -> state         = 1;
       	P_P -> siguienteP    = NULL;
       	Q_P = P_P;
   	}else{
		N_P = (Procesos *)malloc(sizeof(Procesos));
       	N_P -> CSC           = P->duraSC;                                       
       	N_P -> state         = 1;
       	N_P -> siguienteP    = NULL;
       	Q_P -> siguienteP    = N_P;
       	Q_P = N_P;
   	}*/
   	
 	if(P_P == NULL){
      	P_P = (Procesos *)malloc(sizeof(Procesos));
       	P_P -> proceso   = P -> proceso;
       	P_P -> nPagina   = P -> nPagina;
       	P_P -> tLlegada  = P -> tLlegada;		
       	P_P -> ciclos    = P -> ciclos;
        P_P -> inC       = P -> inC;
       	P_P -> estado    = P -> estado;
        P_P -> memoria   = P -> memoria;
        P_P -> cpuES     = P -> cpuES;
 		P_P -> numDisp  = P -> numDisp;			
        P_P -> archivos = P -> archivos;
        P_P -> tipo     = P -> tipo;		
        P_P -> iniSC    = P -> iniSC;
        P_P -> duraSC   = P -> duraSC; 			
        P_P -> siguienteP= NULL;
        Q_P = P_P; 
   	}else{
      	N_P = (Procesos *)malloc(sizeof(Procesos));
       	N_P -> proceso   = P -> proceso;
       	N_P -> nPagina   = P -> nPagina;
       	N_P -> tLlegada  = P -> tLlegada;		
       	N_P -> ciclos    = P -> ciclos;
        N_P -> inC       = P -> inC;
       	N_P -> estado    = P -> estado;
        N_P -> memoria   = P -> memoria;
        N_P -> cpuES     = P -> cpuES;
 		N_P -> numDisp   = P -> numDisp;			
        N_P -> archivos  = P -> archivos;
        N_P -> tipo      = P -> tipo;		
        N_P -> iniSC     = P -> iniSC;
        N_P -> duraSC    = P -> duraSC; 			
        N_P -> siguienteP= NULL;
        Q_P -> siguienteP= N_P;
        Q_P = N_P; 
   	}  	
   	
}
void imprimir_semaforo(){


}
void Semaforo(){
	
/*	if(P_P != NULL){
		if(P_S == NULL){
    		P_S = (semaforo *)malloc(sizeof(semaforo));
			P_S->llegada=0;
			P_S->EP=Crear_EP();
			P_S->semB=1;
			P_S->SC=0;
			P_S->siguienteS = NULL;
			Q_S = P_S;
   		}else{
			N_S = (semaforo *)malloc(sizeof(semaforo));
			N_S->llegada=Q_S->llegada+1;
			N_S->EP=Actualizar_EP();
			N_S->siguienteS = NULL;
			Q_S = N_S;
   		}
   		imprimir_semaforo();
	}*/		
	if(P_P != NULL){
 	   	P_P->ciclos--;
   		P_P->inC++;
   		
   		j=P_mm;
   		do{
   			if(j->proceso == P_P->proceso && j->nPagina == P_P->nPagina ){
   				j->ciclos=P_P->ciclos;
   				j->inC=P_P->inC;
			}
   			j=j->siguiente;	
		}while(j != NULL);
   		
   		
   		if(P_P->inC >= P_P->iniSC+P_P->duraSC){
   			AUXP=P_P;
   			P_P=AUXP->siguienteP;
   			AUXP->siguienteP = NULL;
   			delete AUXP;
		}
	}
		
}

void ordenamientoTM(){

	for(i=P_mm;i->siguiente!=NULL;i=i->siguiente){
		for(j=i->siguiente;j!=NULL;j=j->siguiente){
			if(i->memoria>j->memoria){
				
				N_mm = (PCB *)malloc(sizeof(PCB));
         		N_mm -> proceso  = i->proceso;
         		N_mm -> nPagina  = i->nPagina;
         		N_mm -> tLlegada = i->tLlegada;
         		N_mm -> ciclos   = i->ciclos; 
         		N_mm -> estado   = i->estado;
         		N_mm -> memoria  = i->memoria;
         		N_mm -> cpuES    = i->cpuES;
				N_mm -> numDisp  = i->numDisp;
         		N_mm -> archivos = i->archivos;
         		N_mm -> tipo     = i->tipo;
         		
         		i -> proceso  = j->proceso;
         		i -> nPagina  = j->nPagina;
         		i -> tLlegada = j->tLlegada;
         		i -> ciclos   = j->ciclos; 
         		i -> estado   = j->estado;
         		i -> memoria  = j->memoria;
         		i -> cpuES    = j->cpuES;
				i -> numDisp  = j->numDisp;
         		i -> archivos = j->archivos;
         		i -> tipo     = j->tipo;
 
 				j -> proceso  = N_mm->proceso;
         		j -> nPagina  = N_mm->nPagina;
         		j -> tLlegada = N_mm->tLlegada;
         		j -> ciclos   = N_mm->ciclos; 
         		j -> estado   = N_mm->estado;
         		j -> memoria  = N_mm->memoria;
         		j -> cpuES    = N_mm->cpuES;
				j -> numDisp  = N_mm->numDisp;
         		j -> archivos = N_mm->archivos;
         		j -> tipo     = N_mm->tipo;

			}
		}
	}
}
void ordenamientoNA(){

	for(i=P_mm;i->siguiente!=NULL;i=i->siguiente){
		for(j=i->siguiente;j!=NULL;j=j->siguiente){
			if(i->archivos>j->archivos){
				
				N_mm = (PCB *)malloc(sizeof(PCB));
         		N_mm -> proceso  = i->proceso;
         		N_mm -> nPagina  = i->nPagina;
         		N_mm -> tLlegada = i->tLlegada;
         		N_mm -> ciclos   = i->ciclos; 
         		N_mm -> estado   = i->estado;
         		N_mm -> memoria  = i->memoria;
         		N_mm -> cpuES    = i->cpuES;
				N_mm -> numDisp  = i->numDisp;
         		N_mm -> archivos = i->archivos;
         		N_mm -> tipo     = i->tipo;
         		
         		i -> proceso  = j->proceso;
         		i -> nPagina  = j->nPagina;
         		i -> tLlegada = j->tLlegada;
         		i -> ciclos   = j->ciclos; 
         		i -> estado   = j->estado;
         		i -> memoria  = j->memoria;
         		i -> cpuES    = j->cpuES;
				i -> numDisp  = j->numDisp;
         		i -> archivos = j->archivos;
         		i -> tipo     = j->tipo;
 
 				j -> proceso  = N_mm->proceso;
         		j -> nPagina  = N_mm->nPagina;
         		j -> tLlegada = N_mm->tLlegada;
         		j -> ciclos   = N_mm->ciclos; 
         		j -> estado   = N_mm->estado;
         		j -> memoria  = N_mm->memoria;
         		j -> cpuES    = N_mm->cpuES;
				j -> numDisp  = N_mm->numDisp;
         		j -> archivos = N_mm->archivos;
         		j -> tipo     = N_mm->tipo;

			}
		}
	}
}
void ordenamientoND(){

	for(i=P_mm;i->siguiente!=NULL;i=i->siguiente){
		for(j=i->siguiente;j!=NULL;j=j->siguiente){
			if(i->numDisp>j->numDisp){
				
				N_mm = (PCB *)malloc(sizeof(PCB));
         		N_mm -> proceso  = i->proceso;
         		N_mm -> nPagina  = i->nPagina;
         		N_mm -> tLlegada = i->tLlegada;
         		N_mm -> ciclos   = i->ciclos; 
         		N_mm -> estado   = i->estado;
         		N_mm -> memoria  = i->memoria;
         		N_mm -> cpuES    = i->cpuES;
				N_mm -> numDisp  = i->numDisp;
         		N_mm -> archivos = i->archivos;
         		N_mm -> tipo     = i->tipo;
         		
         		i -> proceso  = j->proceso;
         		i -> nPagina  = j->nPagina;
         		i -> tLlegada = j->tLlegada;
         		i -> ciclos   = j->ciclos; 
         		i -> estado   = j->estado;
         		i -> memoria  = j->memoria;
         		i -> cpuES    = j->cpuES;
				i -> numDisp  = j->numDisp;
         		i -> archivos = j->archivos;
         		i -> tipo     = j->tipo;
 
 				j -> proceso  = N_mm->proceso;
         		j -> nPagina  = N_mm->nPagina;
         		j -> tLlegada = N_mm->tLlegada;
         		j -> ciclos   = N_mm->ciclos; 
         		j -> estado   = N_mm->estado;
         		j -> memoria  = N_mm->memoria;
         		j -> cpuES    = N_mm->cpuES;
				j -> numDisp  = N_mm->numDisp;
         		j -> archivos = N_mm->archivos;
         		j -> tipo     = N_mm->tipo;

			}
		}
	}
}
void ordenamiento2(int a){
if(a==1){
		for(i=P_mm;i->siguiente!=NULL;i=i->siguiente){
		for(j=i->siguiente;j!=NULL;j=j->siguiente){
			if(i->cpuES>j->cpuES){
				
				N_mm = (PCB *)malloc(sizeof(PCB));
         		N_mm -> proceso  = i->proceso;
         		N_mm -> nPagina  = i->nPagina;
         		N_mm -> tLlegada = i->tLlegada;
         		N_mm -> ciclos   = i->ciclos; 
         		N_mm -> estado   = i->estado;
         		N_mm -> memoria  = i->memoria;
         		N_mm -> cpuES    = i->cpuES;
				N_mm -> numDisp  = i->numDisp;
         		N_mm -> archivos = i->archivos;
         		N_mm -> tipo     = i->tipo;
         		
         		i -> proceso  = j->proceso;
         		i -> nPagina  = j->nPagina;
         		i -> tLlegada = j->tLlegada;
         		i -> ciclos   = j->ciclos; 
         		i -> estado   = j->estado;
         		i -> memoria  = j->memoria;
         		i -> cpuES    = j->cpuES;
				i -> numDisp  = j->numDisp;
         		i -> archivos = j->archivos;
         		i -> tipo     = j->tipo;
 
 				j -> proceso  = N_mm->proceso;
         		j -> nPagina  = N_mm->nPagina;
         		j -> tLlegada = N_mm->tLlegada;
         		j -> ciclos   = N_mm->ciclos; 
         		j -> estado   = N_mm->estado;
         		j -> memoria  = N_mm->memoria;
         		j -> cpuES    = N_mm->cpuES;
				j -> numDisp  = N_mm->numDisp;
         		j -> archivos = N_mm->archivos;
         		j -> tipo     = N_mm->tipo;

			}
		}
	}
}else if(a==2){
		for(i=P_mm;i->siguiente!=NULL;i=i->siguiente){
		for(j=i->siguiente;j!=NULL;j=j->siguiente){
			if(i->cpuES<j->cpuES){
				
				N_mm = (PCB *)malloc(sizeof(PCB));
         		N_mm -> proceso  = i->proceso;
         		N_mm -> nPagina  = i->nPagina;
         		N_mm -> tLlegada = i->tLlegada;
         		N_mm -> ciclos   = i->ciclos; 
         		N_mm -> estado   = i->estado;
         		N_mm -> memoria  = i->memoria;
         		N_mm -> cpuES    = i->cpuES;
				N_mm -> numDisp  = i->numDisp;
         		N_mm -> archivos = i->archivos;
         		N_mm -> tipo     = i->tipo;
         		
         		i -> proceso  = j->proceso;
         		i -> nPagina  = j->nPagina;
         		i -> tLlegada = j->tLlegada;
         		i -> ciclos   = j->ciclos; 
         		i -> estado   = j->estado;
         		i -> memoria  = j->memoria;
         		i -> cpuES    = j->cpuES;
				i -> numDisp  = j->numDisp;
         		i -> archivos = j->archivos;
         		i -> tipo     = j->tipo;
 
 				j -> proceso  = N_mm->proceso;
         		j -> nPagina  = N_mm->nPagina;
         		j -> tLlegada = N_mm->tLlegada;
         		j -> ciclos   = N_mm->ciclos; 
         		j -> estado   = N_mm->estado;
         		j -> memoria  = N_mm->memoria;
         		j -> cpuES    = N_mm->cpuES;
				j -> numDisp  = N_mm->numDisp;
         		j -> archivos = N_mm->archivos;
         		j -> tipo     = N_mm->tipo;

			}
		}
	}
}

}
void ordenamiento3(int b){

	for(i=P_mm;i->siguiente!=NULL;i=i->siguiente){
		for(j=i->siguiente;j!=NULL;j=j->siguiente){
			if(i->numDisp>j->numDisp){
				
				N_mm = (PCB *)malloc(sizeof(PCB));
         		N_mm -> proceso  = i->proceso;
         		N_mm -> nPagina  = i->nPagina;
         		N_mm -> tLlegada = i->tLlegada;
         		N_mm -> ciclos   = i->ciclos; 
         		N_mm -> estado   = i->estado;
         		N_mm -> memoria  = i->memoria;
         		N_mm -> cpuES    = i->cpuES;
				N_mm -> numDisp  = i->numDisp;
         		N_mm -> archivos = i->archivos;
         		N_mm -> tipo     = i->tipo;
         		
         		i -> proceso  = j->proceso;
         		i -> nPagina  = j->nPagina;
         		i -> tLlegada = j->tLlegada;
         		i -> ciclos   = j->ciclos; 
         		i -> estado   = j->estado;
         		i -> memoria  = j->memoria;
         		i -> cpuES    = j->cpuES;
				i -> numDisp  = j->numDisp;
         		i -> archivos = j->archivos;
         		i -> tipo     = j->tipo;
 
 				j -> proceso  = N_mm->proceso;
         		j -> nPagina  = N_mm->nPagina;
         		j -> tLlegada = N_mm->tLlegada;
         		j -> ciclos   = N_mm->ciclos; 
         		j -> estado   = N_mm->estado;
         		j -> memoria  = N_mm->memoria;
         		j -> cpuES    = N_mm->cpuES;
				j -> numDisp  = N_mm->numDisp;
         		j -> archivos = N_mm->archivos;
         		j -> tipo     = N_mm->tipo;

			}
		}
	}
}
void prioridades(){
	
	int opc;
	printf("\n Escoja la prioridad \n1. Tamaño de memoria\n2. Tipo de archivo\n3. No. Archivo\n4. Limitados CPU/E/S\n5. No. dispositivos");
	scanf("%i",&opc);
	
	if(opc==1){
		printf("\nOPC 1");
		ordenamientoTM();
	}else if(opc==2){
		int opc2;
		printf("\n1. Sistema\n2. Usuario");
		scanf("%i",&opc2);
		ordenamiento2(opc2);
	}
	else if(opc==3){
		ordenamientoNA();
	}else if(opc==4){
		int opc3;
		printf("\n1. CPU\n2. E\n3. S");
		scanf("%i",&opc3);
		ordenamiento3(opc3);
	}else if(opc==5){
		ordenamientoND();
	}
}
int main()
{
//   printf("\nIngrese la cantidad de tareas ");
//   scanf("%i",&Ctareas);
    crear_tt();
    imprimir_tt();
    imprimir_tmp();
    getch();
    crear_PCB();
    

	
    system("cls");
	imprimir_PCB();
	
//	prioridades();
//	ordenamiento(); 	
	getch();
	AUX1=P_mm;
	// CAMBIO DE ESTADO A LISTO
	do{
		AUX1->estado=2;
		AUX1=AUX1->siguiente;
	}while(AUX1!=NULL);
	
	
	AUX1=P_mm;
	system("cls");
	imprimir_PCB();
	getch();
	bool validacion=false;
	
    do{                      
    	//COMPROBACION DE ESTADO
    	if(AUX1->estado==4){
    		AUX1->estado = 2;
    		system("cls");
			imprimir_PCB();
			getch();
		}
		
   		if(AUX1->estado==2 ){
   		   AUX1->estado = 3;
		}
		
   		
   		int temp = AUX1->ciclos;
   		int I=0;
   		bool valid1=true;
   		bool valid2=true;
   		
   		//comprobacion si hay seccion critica 
   		
   		if(AUX1->duraSC!=0){
   			if(AUX1->inC>=AUX1->iniSC && AUX1->inC<(AUX1->iniSC+AUX1->duraSC)){
   					valid1=false;
   					activar(AUX1);
			   }
		   }	
   		if(AUX1->estado==3 && valid1==true){
   			do{
   				
   				temp--;
   				AUX1->ciclos=temp;
   				AUX1->inC++;
   				system("cls");			
   				Semaforo();
   				
				
							
   				I++;
   				tiempo1++;
   				valid2=true;
   				
   				//comprobacion si hay seccion critica 
   				
   				if(AUX1->duraSC!=0){
   					if(AUX1->inC>=AUX1->iniSC && AUX1->inC<(AUX1->iniSC+AUX1->duraSC)){
   						valid2=false; 						
						activar(AUX1);
			  		 }
		   		}
				imprimir_PCB();
		   		getch();
			}while(I<RR && AUX1->ciclos>0 && valid2==true);
		}

		
        if(AUX1->ciclos==0  ){
        	if(AUX1->estado!=5){
        		AUX1->tfin=tiempo1;   //TFIN SE USA EN EL TIEMPO DE ESPERA
			}
        	AUX1->estado=5;	
		}else{
			AUX1->estado=4;
		}
		
		
		AUX=P_mm;
		validacion=true;
		do{
			if(AUX->estado!=5){
				validacion=false;
			}	
			AUX=AUX->siguiente;
		}while(AUX!=NULL);
		
		
		if(AUX1->siguiente==NULL && validacion==false){
			AUX1=P_mm;
		}else{
			AUX1=AUX1->siguiente;
		}
		
    }while(validacion!=true);
    
    system("cls");
	imprimir_PCB(); 
	getch();
	
//	TablaDeEspera();
    TimeReturnProcess();
    imprimir_TRP();
    getch();
    TPS();
    getch();

}

