#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int IDE=0,IDB=0,IDP=0, impresiones = 0;

typedef struct Persona
{
    char Nombre[30];
    int ID;
    int edad;
    char Sexo;
    struct Persona *siguiente;
}Persona;

typedef struct Bateria
{
    int nPersonas;
    int Nivel;
    Persona *inicio;
    struct Persona *ultimo;
    struct Bateria *siguiente;
}Bateria;


typedef struct Estacion
{
    int ID;
    int nPersonas;
    int nBaterias;
    struct Bateria *tope;
    struct Estacion *siguiente;
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
    printf("\n| Estacion: %d - Baterias: %d - Personas: %d  |\n",(*InicioEstacion)->ID, (*InicioEstacion)->nBaterias, (*InicioEstacion)->nPersonas );
    return(ImprimirEstaciones(&(*InicioEstacion)->siguiente));
}

void MostrarBaterias(Bateria **tope){
    if(!(*tope)){
        return;
    }
    if((*tope)->nPersonas>0){
        printf("\nNivel: %d - Humanos: %d\n",(*tope)->Nivel,(*tope)->nPersonas );
    }
    MostrarBaterias(&(*tope)->siguiente);
}
void ImprimirHumanos (Persona **inicio, Persona **ultimo){
    printf("\nID: %d - Nombre: %s - Edad: %d - Sexo: %c\n",(*inicio)->ID, (*inicio)->Nombre, (*inicio)->edad,(*inicio)->Sexo );
    if((*inicio) ==(*ultimo))
        return;
    return (ImprimirHumanos(&(*inicio)->siguiente,ultimo));

}

void ImprimirHumanosFiltrado (Persona **inicio, Persona **ultimo,char nombre[30],int ID, int Nivel){
    if(!(*inicio))
        return;
    if(strcmp((*inicio)->Nombre,nombre)==0){
    printf("\nID: %d - Nombre: %s - Edad: %d - Sexo: %c - Estacion: %d - Nivel: %d\n",(*inicio)->ID, (*inicio)->Nombre, (*inicio)->edad,(*inicio)->Sexo,ID,Nivel );
    impresiones++;
    }
    if((*inicio) >=(*ultimo))
        return;
    return (ImprimirHumanosFiltrado(&(*inicio)->siguiente,ultimo,nombre,ID,Nivel));

}
void filtrarBateria(Bateria **tope, char nombre[30], int ID){
    Bateria *aux;
    if(!(*tope))
        return;
    ImprimirHumanosFiltrado(&(*tope)->inicio, &(*tope)->ultimo,nombre,ID,(*tope)->Nivel);
    push(&aux,pop(tope));
    filtrarBateria(tope,nombre,ID);
    push(tope,aux);
}
void imprimirFiltrado(Estacion **InicioEstacion, char nombre[30]){
    if(!(*InicioEstacion))
        return; 
    filtrarBateria(&(*InicioEstacion)->tope,nombre,(*InicioEstacion)->ID);
    return imprimirFiltrado(&(*InicioEstacion)->siguiente,nombre);

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

void RecorrerCola(Persona **inicio, Persona **ultimo, Persona **NuevaPersona){

    if((*NuevaPersona)->edad < (*inicio)->edad){
        Persona *aux=(*inicio);
        (*inicio)=(*NuevaPersona);
        (*NuevaPersona)->siguiente=aux;
    }
    if((*inicio)==(*ultimo))
        return;
    return (RecorrerCola(&(*inicio)->siguiente,&(*ultimo),NuevaPersona));
}
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
 if((*tope)->inicio==NULL){   
    (*tope)->inicio=(*tope)->ultimo=NuevaPersona;
    NuevaPersona->siguiente=NuevaPersona;
 }
 else if (NuevaPersona->edad <= (*tope)->inicio->edad ){
    Persona *aux=(*tope)->inicio;
    (*tope)->inicio=NuevaPersona;
    NuevaPersona->siguiente=aux;
    (*tope)->ultimo->siguiente=NuevaPersona;
 }
 else if(NuevaPersona->edad >= (*tope)->ultimo->edad ){
    (*tope)->ultimo->siguiente=NuevaPersona;
    NuevaPersona->siguiente=(*tope)->inicio;
    (*tope)->ultimo=NuevaPersona;
 }else
    RecorrerCola(&(*tope)->inicio->siguiente,&(*tope)->ultimo,&NuevaPersona);
 printf("%s - Ingresado a la capsula correctamente.\n",nombre );
 ++(*tope)->nPersonas;
}
//FIN ALOJAMIENTOS

//ELIMINAR
int eliminarHumano(Persona **inicio, Persona **ultimo, int IDHumano){
    if((*inicio)->ID == IDHumano){
        Persona *aux = (*inicio);
        (*inicio) = (*inicio)->siguiente;
        free(aux);
        return 1;
    }
    if((*ultimo)->ID == IDHumano){       
        Persona *aux = (*ultimo);
        (*ultimo) = (*inicio);
        free (aux);
        return 1;
    }
    if((*inicio)>=(*ultimo))
        return 0;
    return eliminarHumano(&(*inicio)->siguiente,&(*ultimo),IDHumano);
}


void EliminarNivel(Persona **inicio){
    if(!(*inicio))
        return;
    Persona *aux = (*inicio);
    (*inicio) = (*inicio)->siguiente;
    free(aux); 
    return EliminarNivel(&(*inicio)->siguiente);
}
//FIN ELIMINAR

//BUSQUEDAS

Estacion * BuscarEstacion( Estacion **InicioEstacion,int ID){
    if(!(*InicioEstacion))
        return NULL;
    if((*InicioEstacion)->ID==ID)
        return ((*InicioEstacion));
    return (BuscarEstacion(&(*InicioEstacion)->siguiente,ID));
}
Persona * recorrerCapsulas(Persona **inicio, Persona **ultimo, int ID , char nombre[30]){
    if((*inicio)==NULL)
        return NULL;
    if(strcmp((*inicio)->Nombre,nombre)==0 && (*inicio)->ID==ID){
        Persona *aux =(*inicio);
        if((*inicio)->siguiente == (*inicio))
            (*inicio)=(*ultimo)=NULL;
        else
            (*inicio) = (*inicio)->siguiente;
        return aux;
        }
    if((*inicio)==(*ultimo))
        return NULL;
    return (recorrerCapsulas(&(*inicio)->siguiente,&(*ultimo),ID,nombre));
}
Persona * RescatarBateria(Bateria **tope,int ID, char nombre[30]){
    Bateria *aux;
    Persona *auxP;
    if(!(*tope))
        return NULL;
    if((auxP= recorrerCapsulas(&(*tope)->inicio,&(*tope)->ultimo,ID,nombre ))!= NULL){ 
        (*tope)->nPersonas--;
        return auxP;
        }
    push(&aux,pop(tope));
    auxP = RescatarBateria(tope,ID,nombre);
    push(tope,aux);
    return auxP;
}

Persona * Rescatar(Estacion **InicioEstacion, int ID, char nombre[30]){
    Persona *aux;
    if(!(*InicioEstacion))
        return NULL;
    if((aux= RescatarBateria(&(*InicioEstacion)->tope,ID,nombre)) != NULL){
        (*InicioEstacion)->nPersonas--;
        return aux;
    }
    aux = Rescatar(&(*InicioEstacion)->siguiente,ID,nombre);
    return aux;
}

//FIN BUSQUEDAS

//ORDENAMIENTOS
void ordenarCola(Persona **inicio, Persona **ultimo){
    int bandera = 0;
    if((*inicio)->edad > (*inicio)->siguiente->edad){
        bandera = 1;
        Persona *aux=(*inicio),*aux1=(*inicio)->siguiente;
        (*inicio)=(*inicio)->siguiente;
        (*inicio)->siguiente=aux;
        (*inicio)->siguiente->siguiente=aux1;
    }
    if((*inicio)<(*ultimo))
        ordenarCola(&(*inicio)->siguiente,&(*ultimo));
    if(bandera == 1){
        ordenarCola(inicio,ultimo);
    }
}
//FIN ORDENAMIENTOS

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
    if(!(InicioEstacion)->tope)
        return 0;
    if(InicioEstacion->tope->nPersonas!=10){
        MenuIngresarHumano(InicioEstacion);
        return 1;
    }
    push(&aux,pop(&InicioEstacion->tope));
    int band=VerifiPila(InicioEstacion);
    push(&InicioEstacion->tope,aux);
    return band;
}

int eliminarHumanoBateria(Bateria **tope,int nivel, int IDHumano){
    Bateria *aux;
    if(!(*tope))
        return 0;
    if((*tope)->Nivel == nivel){
        if(eliminarHumano(&(*tope)->inicio,&(*tope)->ultimo,IDHumano) == 1){
            (*tope)->nPersonas--;
            return 1;
        }
    }
    push(&aux,pop(&(*tope)));
    int band = eliminarHumanoBateria(&(*tope),nivel,IDHumano);
    push(&(*tope),aux);
    return band;
}

void eliminarNivelBateria(Estacion **InicioEstacion,int nivel){
    Bateria *aux;
    if(!(*InicioEstacion)->tope)
        return ;
    if((*InicioEstacion)->tope->Nivel == nivel){
        if((*InicioEstacion)->tope->inicio != (*InicioEstacion)->tope->ultimo){
            (*InicioEstacion)->tope->ultimo->siguiente=NULL;
            EliminarNivel(&(*InicioEstacion)->tope->inicio);    
        }else
            free((*InicioEstacion)->tope->inicio);
        (*InicioEstacion)->nPersonas= (*InicioEstacion)->nPersonas - (*InicioEstacion)->tope->nPersonas;
        (*InicioEstacion)->tope->nPersonas = 0;
        (*InicioEstacion)->tope->inicio=NULL;
        (*InicioEstacion)->tope->ultimo=NULL;
            return ;
        }
    push(&aux,pop(&(*InicioEstacion)->tope));
    eliminarNivelBateria(&(*InicioEstacion),nivel);
    push(&(*InicioEstacion)->tope,aux);
    return;
}

int SelecNivel(Bateria **tope,int Nivel){
    if(!(*tope))
        return 0;
    if((*tope)->Nivel == Nivel && (*tope)->nPersonas > 0)
        return 1;
    return(SelecNivel(&(*tope)->siguiente,Nivel));
}

void VerifiBateria(Bateria **tope, int sbateria){
    Bateria *aux;
    if(!(*tope))
        return;
    if((*tope)->Nivel == sbateria){
        ImprimirHumanos(&(*tope)->inicio,&(*tope)->ultimo);
        return;
    }
    push(&aux,pop(tope));
    VerifiBateria(tope,sbateria);
    push(tope,aux);
}
//FIN VERIFICADORES

void MenuBaterias( Estacion *InicioEstacion, int sbateria){
    int IDHumano=0;
        printf(" -------------------------------- ");
        printf("\n|  HUMANOS REGISTRADOS.         |");
        printf("\n -------------------------------- ");
        VerifiBateria(&InicioEstacion->tope,sbateria);
        printf("\nSeleccione el ID del humano a eliminar: ");
        while(getchar()!='\n');
        scanf("%d",&IDHumano);
        if(eliminarHumanoBateria(&InicioEstacion->tope,sbateria,IDHumano) == 1){
            printf("\nHumano eliminado con exito!\n");
            InicioEstacion->nPersonas--;
        }else
            printf("\nNo se ha seleccionado un humano valido.\n");
        system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
}

void MenuEstacion(Estacion *InicioEstacion){
   int op,sbateria;
    do{
        printf(" -------------------------------- ");
        printf("\n|  BIENVENIDO A LA ESTACION %d.    |",InicioEstacion->ID);
        printf("\n -------------------------------- ");
        printf("\n|1.-INGRESAR HUMANO.             |");
        printf("\n|2.-ELIMINAR HUMANO.             |");
        printf("\n|3.-ELIMINAR NIVEL.              |");
        printf("\n|4.-REGRESAR                     |");
        printf("\n -------------------------------- ");
        printf("\nElija una opci%cn: ",162);
        scanf("%d",&op);
        switch(op){
        case 1:
                system("clear");
                if(InicioEstacion->tope==NULL){
                    CrearBateria(&InicioEstacion->tope);
                    InicioEstacion->nBaterias++;
                }
                if(VerifiPila(InicioEstacion)==0){
                    if(InicioEstacion->tope->nPersonas==10)
                    {
                        CrearBateria(&InicioEstacion->tope);
                        InicioEstacion->nBaterias++;
                    }
                    MenuIngresarHumano(InicioEstacion);
                }
                break;
        case 2:
                system("clear");
                if(!(InicioEstacion)->tope)
                    printf("No se han registrado baterias\n");
                else if(InicioEstacion->nPersonas>0){
                        printf(" -------------------------------- ");
                        printf("\n|  BATERIAS.                    |");
                        printf("\n -------------------------------- ");
                        MostrarBaterias(&InicioEstacion->tope);
                        printf("\nIngresa el nivel al que quieres acceder: ");
                        scanf("%d",&sbateria);
                        if(SelecNivel(&InicioEstacion->tope,sbateria)==0){
                            printf("No se ingreso un nivel valido\n");
                            system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
                        }else
                            MenuBaterias(InicioEstacion,sbateria);
                }else{
                    printf("No existen humanos en las baterias.\n");
                    system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
                }
                break;
        case 3:
                system("clear");
                if(!(InicioEstacion)->tope)
                    printf("No se han registrado baterias\n");
                else if(InicioEstacion->nPersonas>0){
                        printf(" -------------------------------- ");
                        printf("\n|  BATERIAS.                    |");
                        printf("\n -------------------------------- ");
                        MostrarBaterias(&InicioEstacion->tope);
                        printf("\nIngresa el nivel al que quieras eliminar: ");
                        scanf("%d",&sbateria);
                        if(SelecNivel(&InicioEstacion->tope,sbateria)==0){
                            printf("No se ingreso un nivel valido\n");
                            system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
                        }else
                            eliminarNivelBateria(&InicioEstacion,sbateria);
                }else{
                    printf("No existen humanos en las baterias.\n");
                    system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
                }
                break;
        case 4: break;
        default:
                printf("\nOpci%cn no valida.\n",162);
        }
    system("clear");
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
        printf("\n|3.-REGRESAR                      |");
        printf("\n -------------------------------- ");
        printf("\nElija una opci%cn: ",162);
        scanf("%d",&op);
        switch(op){
        case 1:
                system("clear");
                CrearEstacion(InicioEstacion);
                break;
        case 2:
                system("clear");
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
                break;
        case 3: break;
        default:
                printf("\nOpci%cn no valida.\n",162);
        }
    system("clear");
    }while(op!=3);

}

//MOVER A IMPRESIONES DESPUES V:
void ImprimirAyudantes(Persona **ayudantes){
    if(!(*ayudantes))
        return;
    printf("\n| Nombre: %s - Edad: %d - Sexo: %c  |",(*ayudantes)->Nombre,(*ayudantes)->edad, (*ayudantes)->Sexo );
    return (ImprimirAyudantes(&(*ayudantes)->siguiente));
}

void MenuAyuda(Estacion **InicioEstacion, Persona **ayudantes){
    Persona *aux;
    char nombre[30];
    int idR;
        printf(" -------------------------------- ");
        printf("\n|  AYUDALOS MORFEO!!!          |");
        printf("\n -------------------------------- ");
        if((*ayudantes)){
            printf("\n| AYUDANTES                     |");
            printf("\n--------------------------------" );
            ImprimirAyudantes(ayudantes);
            printf("\n--------------------------------");
        }
    if((*InicioEstacion)->tope->inicio != NULL){
        printf("\nA quien buscas?: ");
        while(getchar()!='\n');
        scanf("%[^\n]",nombre);
        imprimirFiltrado(InicioEstacion,nombre);
        if(impresiones == 0)
            printf("No existe la persona buscada :(.\n");
        impresiones = 0;
        while(getchar()!='\n');
        printf("\nA quien rescataras?: \n");
        scanf("%d",&idR);
        while(getchar()!='\n');
        if((aux=Rescatar(InicioEstacion,idR,nombre))!=NULL){
            printf("Has rescatado a %s de la MATRIX\n",aux->Nombre);
            aux->siguiente=(*ayudantes);
            (*ayudantes)=aux;
        }else
            printf("ID no valido :(\n");
     }else
        printf("\nNo existen humanos en las capsulas actualmente\n");
     system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
}


//FIN MENUS

int main(int argc, char const *argv[]){
    Estacion *InicioEstacion=NULL;
    Persona *ayudantes=NULL;
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
                MenuMatrix(&InicioEstacion);
                break;
        case 2:
                system("clear");
                if(!InicioEstacion){
                    printf("No se han registrado Estaciones\n");
                    system("read -n 1 -s -p \"Presiona una tecla para continuar...\"");
                }
                else
                MenuAyuda(&InicioEstacion, &ayudantes);
                break;
        case 3: break;
        default:
                printf("\nOpci%cn no valida.\n",162);
        }
    system("clear");
    }while(op!=3);
    return 0;
}