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
	int opor; //problema para dejar en 21 otra vez este dato
	int dificultad;
};
//estructura tablero
struct tablero{
	int matriz[NF][NC];
	int cuentaFila[NC];
};
//estructura escanear talero
struct escaneo{
 	int nae; //numero a buscar
 	int hay; //1 si hay alguna, 0 si no hay.
 	int vacias[3][2]; //casillas vacias en el primer encuentro-f,c
 	int array[NF-N+1][NC-N+1][N][N];
};

//definicion de funciones
int jugar(struct jugador jug[]); //funcion principal
struct jugador iniciarjug(struct jugador *jug,int num, char pieza[NJ][7], int c[],int elegir[2][3]);//inicializa jugadores
struct tablero iniciarTab(struct tablero *tab);
void instrucciones(void);
int menu1(void);
int menu2(void);
void dibujarTabla(struct tablero tab, struct jugador jug[]);
void esperar(void);
int ponerPieza(struct jugador jug,struct tablero *tab);

///////Funciones

//main
int main(void){
	srand(time(NULL));
	struct jugador jug[NJ];
	char pieza[NJ][7]={"x","o"};
	int elegir[NJ][3];//color, pieza
	int c[2]={0,0}, ytn[NJ]={0,0}, i=0,j=0;
	do{
		c[0]=menu1();
		for(i=0;i<NJ;i++)  //para inicializar jugador 
			for(j=0;j<3;j++) //para inicializar colores
				elegir[i][j]=1;
		if(c[0]==1 || c[0]==2){
			if(c[0]==2){
				c[1]=menu2();
				while(c[1]<1 || c[1]>4){
					printf("\n\t\t\tOpción no valida, vuelva a intentarlo.");
					esperar();
					c[1]=menu2();
				}//si me da un numero invalido en el menu
            }//fin pedir menu 2
			for(i=0;i<NJ;i++)
				if(ytn[i] != c[0]){   //si no estan inicializados
					iniciarjug(&jug[i],i+1,pieza, c,elegir);
					ytn[i]=c[0];
				}
			if(c[1] != 4){
				jugar(jug);
			}
		}else if(c[0]==3)
			instrucciones();
		else if(c[0]==4)
			c[0]=0;
		else{
			printf("\n\t\t\tOpción no valida, vuelva a intentarlo.");
			esperar();
		}
	}while(c[0]);
	//termina while
	return 0;
}
///////////////funcion principal jugar
int jugar(struct jugador jug[]){
	struct tablero tab;
    iniciarTab(&tab);
    int i=0, ok=0;
    while(jug[0].opor || jug[1].opor){
        for(i=0;i<NJ;i++){
            if(1/*revisarGanador(jug,tab)*/){
                while(!ok){
	                dibujarTabla(tab,jug);
                    if(jug[i].num != 3)
        	            ok=ponerPieza(jug[i],&tab);
                    //else
    	                //ok=poner pieza ia(jug,&tab) retornan el numero de columna
                }
                //tab.matriz[tab.cuentaFila[ok]][ok]=jug[i].num;  //poner pieza en su lugar en la matriz
                tab.cuentaFila[ok]++;   //sumar el contador de fila
                jug[i].opor--;  //disminuir contador de oportunidades
            }        
        }
    }
	dibujarTabla(tab,jug);
    printf("\n\n\tLo siento, ya se terminaron las oportunidades, juego empatado.");
    esperar();
	return 0;
}
//funcion para iniciar estructura jugadores
struct jugador iniciarjug(struct jugador *jug, int num, char pieza[2][7], int c[], int elegir[2][3]){
	int r=-1, i=0;
	//para poner avatar random
	if(num==1){
		r=rand()%2;
		elegir[1][r]=0;
	}else{
		if(elegir[1][0])
			r=0;
		else
			r=1;
	}
	strcpy(jug->avatar,pieza[r]);
	//pedir nombre
	if(c[0]==1 || num==1){
	    jug->num=num;//numero jugador
		system("clear");
		printf("\n\n\t\t\tJugador %d",num);
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
			scanf("%d",&r);
			if(r<1 || r>4){
				printf("\n\tOpción no valida, vuelve a intentarlo");
				esperar();
				r=0;
			}
		}
		elegir[0][r-1]=0;
	}else{
        jug->num=3;
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
	jug->opor=OP;
    jug->dificultad=c[1];
	return *jug;
}//termina funcion estructura inicar jugador
struct tablero iniciarTab(struct tablero *tab){
	int i=0, j=0;
    for(i=0;i<NF;i++){
		tab->cuentaFila[j]=0;
		for(j=0;j<NC;j++)
			tab->matriz[i][j]=0;
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
int ponerPieza(struct jugador jug,struct tablero *tab){
    int columna=0;
        printf("\n\n\n\t\t%s%s...%s Elige una columna: ",jug.color,jug.nombre,BLANCO);
        scanf("%d",&columna);
        if(tab->cuentaFila[columna] >= NF){
            printf("\n\n\tEsa columna esta llena, pero puedes elegir otra.");
        }else if(columna < 1 || columna >= NF){
            printf("\n\n\tEl numero que ingresaste no es ninguna columna, elige de nuevo.");
        }else{
            return columna;
        }
    esperar();
    return 0;
    }

