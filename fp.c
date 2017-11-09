//poner en el jugador 2 vs cpu restricciones en el color
//ver como poner dificultad en otra parte
//ver porque hay error al escribir una palabra
//terminar ganador
//terminar revisar
//terminar AI
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//constantes
#define N 4   //numero para ganar
#define NJ 2  //numero de jugadores
#define NF 6  //numero de filas
#define NC 7  //numero de columnas
#define OP 21 //oportunidades del jugador
#define BLANCO "\E[m"
#define CYAN "\E[36m"
#define ROJO "\E[31m"
#define AMARILLO "\E[33m" //33 amarillo(es mas anaranjado)

//Estructura Jugador
struct jugador{
	int num;
	char avatar[7];
	char nombre[10];
	char color[7];
	int dificultad; //ver posibilidad de meter dificultad en estructura general
};
//estructura tablero
struct tablero{
	int matriz[NF][NC];
	int cuentaFila[NC];
};
//estructura escanear talero
struct escanear{
 	int hay[NJ]; //1 si hay alguna, 0 si no hay.
 	int nab[NJ]; //numero encontrado
 	int vacias[NJ][3][2]; //casillas vacias en el primer encuentro-f,c
 	int array[NF-N+1][NC-N+1][N][N];
};
//definicion de funciones
int jugar(struct jugador jug[]); //funcion principal
struct jugador iniciarjug(struct jugador *jug,int num, char pieza[NJ][7], int c[],int elegir[2][NJ+1]);//inicializa jugadores
void iniciarTab(struct tablero *tab);
void instrucciones(void);
int menu1(void);
int menu2(void);
void dibujarTabla(struct tablero tab, struct jugador jug[]);
int ponerPieza(struct jugador jug, struct tablero tab);
int ponerPiezaAI(struct jugador jug[], struct tablero tab, int n);
struct escanear revisarTab(struct jugador jug[], struct tablero tab);
int revisarGanador(struct jugador jug[],struct tablero tab, int n); 
void esperar(void);

///////Inicio
//main
int main(void){
	srand(time(NULL));
	struct jugador jug[NJ];
	char pieza[NJ][7]={"x","o"};
	int elegir[2][NJ+1];//color, pieza
	int c[2]={0,0}, ytn[NJ]={0,0}, i=0,j=0;
	do{
		c[0]=menu1();
		for(i=0;i<2;i++)  //para inicializar jugador 
			for(j=0;j<NJ+1;j++) //para inicializar colores
				elegir[i][j]=1;
		if(c[0]==1 || c[0]==2){
			if(c[0]==2){
				c[1]=menu2();
				while(c[1]<1 || c[1]>4){
					printf("\n\t\t\tOpción no valida, vuelva a intentarlo.");
					esperar();
					c[1]=menu2();
				}//si me da un numero invalido en el menu
                jug[1].dificultad=c[1];//guardar dificultad
            }else{
                c[1]=0;//estoy en 1 vs 2
            }//fin pedir menu 2
			if(c[1] != 4){
			    for(i=0;i<NJ;i++)
				    if(ytn[i] != c[0]){   //si no estan inicializados
					    iniciarjug(&jug[i],i+1,pieza, c,elegir);
					    ytn[i]=c[0];
				    }
				jugar(jug);
			}
		}else if(c[0]==3)
			instrucciones();
		else if(c[0] != 4){
			printf("\n\t\t\tOpción no valida, vuelva a intentarlo.");
			esperar();
		}
	}while(c[0] != 4);
	//termina while
    system("clear");
	return 0;
}
///////////////funcion principal jugar
int jugar(struct jugador jug[]){
	struct tablero tab;
    iniciarTab(&tab);
    int continuar=1, i=0, ok=-2;
    do{
        for(i=0;i<NJ;i++){
            continuar=revisarGanador(jug,tab,i);
            if(continuar){
                ok=-2;
                while(ok==-2){
	                dibujarTabla(tab,jug);
                    if(jug[i].num != NJ+1)
        	            ok=ponerPieza(jug[i],tab);
                    else
    	                ok=ponerPiezaAI(jug,tab,i);
                }            
                if(ok==-1)//si pone 0 el usuario
                    return 0;
                tab.matriz[tab.cuentaFila[ok]][ok]=jug[i].num;  //poner pieza en su lugar en la matriz
                tab.cuentaFila[ok]++;   //sumar el contador de fila
            }else{
                return 0;
            }        
        }
    }while(continuar);
	return 0;
}
//funcion para iniciar estructura jugadores
struct jugador iniciarjug(struct jugador *jug, int num, char pieza[2][7], int c[], int elegir[2][NJ+1]){
	int r=-1, i=0;
	//para poner avatar random
	do{
		r=rand()%2;
	}while(!elegir[1][r]);
	elegir[1][r]=0;
	strcpy(jug->avatar,pieza[r]);
	//pedir nombre
	if(c[0]==1 || num==2){ //ver como hacer para poner cpu en 1er lugar sin que haya error
	    jug->num=num;//numero jugador
		system("clear");
		printf("\n\n\t\t\tJugador %d",jug->num);
        printf("\n\n\t!!Hola, antes de comenzar ayudame con lo siguiente: ");
		printf("\n\n\tElige un nombre: ");
		fflush(stdin);
		scanf("%s",jug->nombre);
		fflush(stdin);
		r=0;
		while(!r){
			for(i=0;i<3;i++)
			    if(elegir[0][i]){
				    if(i==0)
					    printf("\n\t1-Rojo");
				    if(i==1)
					    printf("\n\t2-Cyan");
				    if(i==2)
					    printf("\n\t3-Amarillo");
			    }
			printf("\n\t\t\tElige un color: ");
			fflush(stdin);
			scanf("%i",&r);
		    fflush(stdin);
			if(r<1 || r>4 || !elegir[0][r-1]){
				printf("\n\tOpción no valida, vuelve a intentarlo");
				esperar();
				r=0;
			}
		}
		elegir[0][r-1]=0;
	}else{
        jug->num=NJ+1;
        jug->dificultad=c[1];//ver forma de quitarla
		strcpy(jug->nombre,"cpu");
		do{
			r=rand()%3+1;
		}while(!elegir[0][r-1]);
	}
	switch(r){
		case 1:
			strcpy(jug->color,ROJO);
			break;
		case 2:
			strcpy(jug->color,CYAN);
			break;
		case 3:
			strcpy(jug->color,AMARILLO);
			break;
		default:
			break;
	}
	return *jug;
}//termina funcion inicar jugador
void iniciarTab(struct tablero *tab){
	int i=0, j=0;
    for(i=0;i<NC;i++){
		tab->cuentaFila[i]=0;
		for(j=0;j<NF;j++)
			tab->matriz[j][i]=0;
    }
}//termina función iniciarTab
int menu1(void){
	int r=0;
	system("clear");
	printf("\n\n\t\tCUATRO EN LINEA, GRAVEDAD CERO \n\n");
	printf("\t\t\tModo de Juego\n\n");
	printf("\t\t 1) Jugador 1 vs Jugador 2. \n");
	printf("\t\t 2) Jugador 1 vs Computadora. \n");
	printf("\t\t 3) Instrucciones de Juego. \n");
	printf("\t\t 4) Salir. \n\n");
	printf("\t\t\tElige una opción: ");
	fflush(stdin);
	scanf("%d",&r);
	return r;
}//fin menu 1
int menu2(void){
	int r2=0;
	system("clear");
	printf("\n\n\t\tCUATRO EN LINEA, GRAVEDAD CERO \n\n");
	printf("\t\t\tNivel de Dificultad:\n\n");
	printf("\t\t 1) Nivel Fácil. \n");
	printf("\t\t 2) Nivel Intermedio. \n");
	printf("\t\t 3) Nivel Difícil. \n");
	printf("\t\t 4) Regresar. \n\n");
	printf("\t\t\tElige una opción: ");
	fflush(stdin);
	scanf("%d",&r2);
	return r2;
}//fin menu 2
void instrucciones(void){
	system("clear");
	printf("\n\n\t\t\tReglas del Juego\n\n\t-El objetivo del juego es alinear cuatro fichas sobre un tablero\n\t formado por seis filas y siete columnas.\n\n\t-Cada uno de los dos jugadores dispone de 21 fichas de color.\n\n\t-Por turnos, los jugadores deben introducir una ficha en la\n\t columna que prefieran (siempre que no esté llena) y ésta\n\t no caerá, sino que se mantendrá en la parte mas alta disponible.\n\n\t-Gana la partida el primero que consiga alinear cuatro fichas\n\t consecutivas en horizontal, vertical o diagonal.\n\n\t-Si todas las columnas estan llenas y nadie ha hecho una fila válida, \n\t entonces hay un empate.");
	esperar();
}//fin instrucciones
void dibujarTabla(struct tablero tabla,struct jugador jug[]){
        int i=0, j=0, k=0;
        //dibujar tabla
        system("clear");
        printf("\n\n\t\t\tCUATRO EN LINEA, GRAVEDAD CERO\n\n");
        printf("\t\t");
        for(i=0;i<NC;i++)
            printf(" _____");
        for(i=-1;i<NF;i++){
            printf("\n\t\t|");
            for(j=0;j<NC;j++)
                printf("     |");
            printf("\n\t\t");
            for(j=0;j<NC;j++){
                if(i==-1)
                    printf("|  %d  ",j+1);
                else{
                  for(k=0;k<NJ;k++){
                    if(tabla.matriz[i][j]==jug[k].num)
                        printf("|""  %s%s%s  ",jug[k].color,jug[k].avatar,BLANCO);
                  }
                  if(!tabla.matriz[i][j])  
                        printf("|     ");
                }
            }
            printf("|\n\t\t|");
            for(j=0;j<NC;j++)
                printf("_____|");
        }
}//fin dibujar tabla
void esperar(void){
    printf("\n\n\t\tPresiona cualquier tecla para continuar: ");
	fflush(stdin);
	getchar();
	getchar();
}//fin esperar
int ponerPieza(struct jugador jug,struct tablero tab){
    int columna=0;
    printf("\n\n\n\t\t\t\tTu Turno %s%s%s\n\n\t\tPuedes presionar 0 en cualquier momento para salir.\n\n\t\tElige una columna: ",jug.color,jug.nombre,BLANCO);
    scanf("%i",&columna);
    columna--;
    if(columna < -1 || columna >= NC){
        printf("\n\n\t\tEl numero que ingresaste no es ninguna columna, elige de nuevo.");
    }else if(tab.cuentaFila[columna] >= NF){
        printf("\n\n\t\tEsa columna esta llena, pero puedes elegir otra.");
    }else{
        return columna;
    }
    esperar();
    return -2;
    }
int ponerPiezaAI(struct jugador jug[], struct tablero tab,int n)
{
    struct escanear escaneado;
    int dif=jug[n].dificultad;
    int col=-1;
    if(jug[n].num==NJ+1){
        if(dif != 1){
            escaneado=revisarTab(jug,tab);
            if(escaneado.hay[0])
                col=escaneado.vacias[0][0][1];
            else if(escaneado.hay[1])
                col=escaneado.vacias[1][0][1];
            else if(dif==3){
                revisarTab(jug,tab);
                //hardcore extra primero poner en la col 3 revisando si el op tiene 2 seguidas de peligro.
                col=0;
            }
        }
        if(col==-1){
            do{
                col=rand()%NC;
            }while(tab.cuentaFila[col]>=NF);
        }
    }
            printf("\n\n\n\n\n\n\n\n\n\n");
    return col;
}
struct escanear revisarTab(struct jugador jug[], struct tablero tab){
    //inicializacion
    struct escanear escaner;
    int i=0, j=0, k=0, l=0;
    for(;i<NJ;i++){
        escaner.hay[i]=0;
        escaner.nab[i]=0;
        for(;j<2;j++)
            for(;k<2;k++){
                escaner.vacias[i][j][k]=0;
            }
    }
    //matriz de matrices de 4x4        
    for(i=0;i<(NF-N+1);i++)
        for(j=0;j<(NC-N+1);j++){
            for(k=0;k<N;k++){
                for(l=0;l<N;l++){
                    escaner.array[i][j][k][l]=tab.matriz[i+k][j+l];
                }
            }
        }
    return escaner;
}

/* 
struct escanear{
 	int hay[NJ]; //1 si hay alguna, 0 si no hay.
 	int nab[NJ]; //numero encontrado
 	int vacias[NJ][3][2]; //casillas vacias en el primer encuentro-f,c
 	int array[NF-N+1][NC-N+1][N][N];
};
 */

int revisarGanador(struct jugador jug[],struct tablero tab, int n){
    struct escanear escaneo;
    int contador=0;
    int i=0, j=0;
    //revisar si alguien ganó 
    /*
    escaneo=revisarTab(jug,tab)
    if(escanear.hay[n] && escanear.nab[n]==4){
	    dibujarTabla(tab,jug);//meter esto en revisarGanador();
        printf("\n\n\t\t!!!Felicidades %s has ganado¡¡¡",jug[n].nombre);
        esperar();
        return 0;
    }
    */
    //revisar si se lleno la tabla
    for(i=0;i<NC;i++)
        if(tab.cuentaFila[i]==NF)
            contador++;
    if(contador==NC){
	    dibujarTabla(tab,jug);
        printf("\n\n\tLo siento, ya se terminaron las oportunidades, juego empatado.");
        esperar();
        return 0;
    }
    return 1;
} 
