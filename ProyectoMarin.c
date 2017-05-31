//hola
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int IDE=0,IDB=0,IDP=0;

typedef struct Persona
{
	char Nombre[30];
	int ID;
	int edad;
	char Sexo;
	Persona *siguiente;
}Persona;

typedef struct Bateria
{
	int nPersonas;
	int Nivel;
	Persona *inicio;
	Persona *ultimo;
	Bateria *siguiente;
}Bateria;


typedef struct Estacion
{
	int ID;
	int nPersonas;
	int nBaterias;
	Bateria *tope;
	Estacion *siguiente;
}Estacion;

//FUNCIONES PILA
Bateria* pop(Bateria **tope){
	Bateria *aux;
	aux = (*tope);
	(*tope) = (*tope)->siguiente;
	return aux;
}
void push(Bateria **tope,Bateria *dato){
	dato->siguiente = (*tope);
	(*tope) = dato;
}



//IMPRESIONES

void ImprimirEstaciones(Estacion **InicioEstacion){
	if(!(*InicioEstacion))
		return;
	printf("\n| Estacion: %d - Baterias: %d - Personas: %d 	|\n",(*InicioEstacion)->ID, (*InicioEstacion)->nBaterias, (*InicioEstacion)->nPersonas );
	return(ImprimirEstaciones(&(*InicioEstacion)->siguiente));
}

//FIN IMPRESIONES

//ALOJAMIENTOS

void CrearEstacion(Estacion **InicioEstacion){
	Estacion *NuevaEstacion =(Estacion*) malloc(sizeof(Estacion));
	if(!NuevaEstacion){
		printf("No se pudo reservar la memoria\n");
		return ;
	}
	NuevaEstacion->ID=++IDE;
	NuevaEstacion->nBaterias=0;
	NuevaEstacion->nPersonas=0;
	NuevaEstacion->tope=NULL;
	NuevaEstacion->siguiente=(*InicioEstacion);
	(*InicioEstacion)=NuevaEstacion;
	printf("Estacion creada con el ID: %d creada con exito\n",IDE );
	system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
}

void CrearBateria(Bateria **tope){
	Bateria *NBateria=(Bateria*) malloc(sizeof(Bateria));
	if(!NBateria){
		printf("No se pudo reservar la memoria\n");
		return;
	}
	if(!(*tope))
		NBateria->Nivel=0;
	else
	NBateria->Nivel=++(NBateria)->Nivel;
	NBateria->nPersonas=0;
	NBateria->inicio=NULL;
	NBateria->ultimo=NULL;
	NBateria->siguiente=(*tope);
	(*tope)=NBateria;
	printf("Nivel %d de bateria creado.\n",NBateria->Nivel );
}

//FIN ALOJAMIENTOS

//BUSQUEDAS

Estacion * BuscarEstacion( Estacion **InicioEstacion,int ID){
	if(!(*InicioEstacion))
		return NULL;
	if((*InicioEstacion)->ID==ID)
		return ((*InicioEstacion));
	return (BuscarEstacion(&(*InicioEstacion)->siguiente,ID));
}

//FIN BUSQUEDAS

//REGISTROS

void IngresarHumano(Bateria **tope, char nombre[30],char sexo,int edad){
 Persona *NuevaPersona= (Persona*) malloc(sizeof(Persona));
 if(!NuevaPersona){
 	printf("No se ha podido agregar a la persona\n");
 	return ;
 }
 strcpy(NuevaPersona->Nombre,nombre);
 NuevaPersona->Sexo=sexo;
 NuevaPersona->edad=edad;
 NuevaPersona->siguiente = NULL;
 NuevaPersona->ID=++IDP;
 if((*tope)->ultimo!=NULL){
 	(*tope)->ultimo->siguiente = NuevaPersona;
 }
 (*tope)->ultimo = NuevaPersona;
 if(!(*tope)->inicio)
 	(*tope)->inicio = NuevaPersona;
 (*tope)->ultimo->siguiente = (*tope)->inicio;
 printf("%s - Ingresado a la capsula correctamente.\n",nombre );
 ++(*tope)->nPersonas;
}


//FIN REGISTROS

//MENUS

void MenuIngresarHumano(Estacion *InicioEstacion){
	char nombre[30],sexo;
	int edad,op;
	while(getchar()!='\n');
	printf("Ingresa el nombre del humano: ");
	scanf("%[^\n]",nombre);
	while(getchar()!='\n');
	printf("Ingresa el Sexo ( M - F ):  ");
	scanf("%c",&sexo);
	while(getchar()!='\n');
	printf("Ingresa la edad: ");
	scanf("%d",&edad);
	do{
		printf("Deseas ingresarlo a la capsula (1 - SI, 2 - NO):  ");
		scanf("%d",&op);	
		if(op==1){
			IngresarHumano(&(InicioEstacion)->tope,nombre,sexo,edad);
			InicioEstacion->nPersonas++;
			system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
		}else if(op>3){
			printf("Opcion invalida!\n");
			system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
		}
	}while(op>3);

	
}

//VERIFICADORES

int VerifiPila(Estacion *InicioEstacion){
	Bateria *aux;
	if(!(InicioEstacion)->tope){
		return 0;
	}
	if(InicioEstacion->tope->nPersonas!=20){
		MenuIngresarHumano(InicioEstacion);
		return 1;
	}
	push(&aux,pop(&InicioEstacion->tope));
	VerifiPila(InicioEstacion);
	push(&InicioEstacion->tope,aux);
	return 2;
}

//FIN VERIFICADORES



void MenuEstacion(Estacion *InicioEstacion){
   int op;
    do{
        printf(" -------------------------------- ");
        printf("\n|  BIENVENIDO A LA ESTACION %d.    |",InicioEstacion->ID);
        printf("\n -------------------------------- ");
        printf("\n|1.-INGRESAR HUMANO.             |");
        printf("\n|2.-ELIMINAR HUMANO.             |");
        printf("\n|3.-ELIMINAR NIVEL.              |");
        printf("\n|4.-SALIR                        |");
        printf("\n -------------------------------- ");
        printf("\nElija una opci%cn: ",162);
        scanf("%d",&op);
        switch(op){
        case 1:
                system("clear");
                //system("cls");
                if(InicioEstacion->tope==NULL){
                	CrearBateria(&InicioEstacion->tope);
                	InicioEstacion->nBaterias++;
                }

                if(VerifiPila(InicioEstacion)==0){
                	if(InicioEstacion->tope->nPersonas==20){
                		CrearBateria(&InicioEstacion->tope);
                		InicioEstacion->nBaterias++;
                	}
                	MenuIngresarHumano(InicioEstacion);
                }


                break;
        case 2:
                system("clear");
                //system("cls");
                //EliminarHumano(&InicioEstacion->tope);
                break;
        case 3: 
        		//EliminarNivel(&InicioEstacion->tope);
        		break;

        case 4: break;
        default:
                printf("\nOpci%cn no valida.\n",162);
        }
    system("clear");
    //system("cls");
    }while(op!=4);
}

void MenuMatrix(Estacion **InicioEstacion){
	   int op;
	   Estacion *aux=NULL;
    do{
        printf(" -------------------------------- ");
        printf("\n|  MATRIX.                        |");
        printf("\n -------------------------------- ");
        printf("\n|1.-INGRESAR ESTACION.            |");
        printf("\n|2.-ADMINISTRAR ESTACION          |");
        printf("\n|3.-SALIR                         |");
        printf("\n -------------------------------- ");
        printf("\nElija una opci%cn: ",162);
        scanf("%d",&op);
        switch(op){
        case 1:
                system("clear");
                //system("cls");
                CrearEstacion(InicioEstacion);
                
                break;
        case 2:
                system("clear");
                //system("cls");
				if(!InicioEstacion){
							printf("No existen estaciones\n");
							return;
						}
						int ID;
						
					        printf(" -------------------------------- ");
					        printf("\n|  ESTACIONES.                        |");
					        printf("\n -------------------------------- ");
					        ImprimirEstaciones(InicioEstacion);
					        printf("NOTA: INGRESE 0 PARA REGRESAR\n");
					        printf("\nElija un ID: ");
					        scanf("%d",&ID);
					        aux=BuscarEstacion(InicioEstacion,ID);
					    if(ID!=0 && aux!=NULL)
                		{	
                			system("clear");
                			MenuEstacion(aux);}
                		else{
                			if(aux==NULL && ID!=0){
                			printf("ID no encontrado\n");
                			system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
                			}
                		}
                		system("clear");
					    //system("cls");
                break;
        case 3: break;
        default:
                printf("\nOpci%cn no valida.\n",162);
        }
    system("clear");
    //system("cls");
    }while(op!=3);

}

void MenuAyuda(Estacion **InicioEstacion){

}


//FIN MENUS



int main(int argc, char const *argv[])
{
	Estacion *InicioEstacion=NULL;
   int op;
    do{
        printf(" -------------------------------- ");
        printf("\n|  BIENVENIDO A LA MATRIX.        |");
        printf("\n -------------------------------- ");
        printf("\n|1.-ADMINISTRAR MATRIX.           |");
        printf("\n|2.-AYUDAR AMIGOS                 |");
        printf("\n|3.-SALIR                         |");
        printf("\n -------------------------------- ");
        printf("\nElija una opci%cn: ",162);
        scanf("%d",&op);
        switch(op){
        case 1:
                system("clear");
                //system("cls");
                MenuMatrix(&InicioEstacion);
                break;
        case 2:
                system("clear");
                //system("cls");
                MenuAyuda(&InicioEstacion);
                break;
        case 3: break;
        default:
                printf("\nOpci%cn no valida.\n",162);
        }
    system("clear");
    //system("cls");
    }while(op!=3);
	return 0;
}

