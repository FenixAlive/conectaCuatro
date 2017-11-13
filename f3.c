//ver porque hay error al escribir una palabra
//terminar AI parte dificil
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//constantes
#define N 4   //numero para ganar
#define NJ 2  //numero de jugadores
#define NF 6  //numero de filas
#define NC 7  //numero de columnas
#define OP 21 //oportunidades del jugador
#define BLANCO "\E[m"
#define CYAN "\E[36m"
#define ROJO "\E[31m"
#define AMARILLO "\E[33m"

//Estructura Jugador
struct jugador{
	int num;
	char avatar[7];
	char nombre[10];
	char color[7];
};
//estructura tablero
struct tablero{
	int matriz[NF][NC];
	int cuentaFila[NC];
};
//estructura escanear talero
struct escanear{
 	int hay; //1 si hay alguna, 0 si no hay.
 	int nab; //numero encontrado
 	int vacias[3]; //casillas vacias en el primer encuentro
 	int array[NF-N+1][NC-N+1][N][N];
};
//definicion de funciones
int jugar(struct jugador jug[], int c[]); //funcion principal
struct jugador iniciarjug(struct jugador *jug,int num, char pieza[NJ][7], int c[],int elegir[2][NJ+1]);//inicializa jugadores
void iniciarTab(struct tablero *tab);
void instrucciones(void);
int menu1(void);
int menu2(void);
void dibujarTabla(struct tablero tab, struct jugador jug[]);
int ponerPieza(struct jugador jug, struct tablero tab);
int ponerPiezaAI(struct jugador jug[], struct tablero tab, int n, int dif);
struct escanear revisarTab(struct jugador jug, struct tablero tab, int n);
int revisarGanador(struct jugador jug[],struct tablero tab, int n); 
void esperar(void);
void copiar(char a[], char b[]);//a es el nuevo string

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
            }else{
                c[1]=0;//estoy en 1 vs 2
            }//fin pedir menu 2
			if(c[1] != 4){
			    for(i=0;i<NJ;i++)
				    if(ytn[i] != c[0]){   //si no estan inicializados
					    iniciarjug(&jug[i],i+1,pieza, c,elegir);
					    ytn[i]=c[0];
				    }
				jugar(jug, c);
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
int jugar(struct jugador jug[], int c[]){
	struct tablero tab;
    iniciarTab(&tab);
    int continuar=1, i=0, ok=-2;
    do{
        for(i=0;i<NJ;i++){
            if(continuar){
                ok=-2;
                while(ok==-2){
	                dibujarTabla(tab,jug);
                    if(jug[i].num != NJ+1)
   		                ok=ponerPieza(jug[i],tab);
                    else
    	                ok=ponerPiezaAI(jug,tab,i,c[1]);
                }            
                if(ok==-1)
                    return 0;//si pone 0 el usuario
                tab.matriz[tab.cuentaFila[ok]][ok]=jug[i].num;  //poner pieza en su lugar en la matriz
                tab.cuentaFila[ok]++;   //sumar el contador de fila
            }else{
                return 0;
            }        
            continuar=revisarGanador(jug,tab,i);
        }
    }while(continuar);
	return 0;
}
//funcion para iniciar estructura jugadores
struct jugador iniciarjug(struct jugador *jug, int num, char pieza[2][7], int c[], int elegir[2][NJ+1]){
	int r=-1, i=0;
    char color[3][10]={"1-Rojo","2-Cyan","3-Amarillo"};
    char pintar[3][7]={ROJO,CYAN,AMARILLO};
	//para poner avatar random
	do{
		r=rand()%2;
	}while(!elegir[1][r]);
	elegir[1][r]=0;
    copiar(jug->avatar,pieza[r]);
	//pedir nombre
	if(c[0]==1 || num==2){
	    jug->num=num;//numero jugador
		system("clear");
		printf("\n\n\t\t\tJugador %d",jug->num);
        printf("\n\n\t!!Hola, antes de comenzar elige un nombre: ");
		fflush(stdin);
		scanf("%s",jug->nombre);
		fflush(stdin);
		r=0;
		while(!r){
			for(i=0;i<3;i++)
			    if(elegir[0][i])
                    printf("\n\t%s%s%s",pintar[i],color[i],BLANCO);
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
        copiar(jug->nombre,"cpu");
		do{
			r=rand()%3+1;
		}while(!elegir[0][r-1]);
		elegir[0][r-1]=0;
	}
	copiar(jug->color,pintar[r-1]);
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
    fflush(stdin);
    scanf("%i",&columna);
    fflush(stdin);
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
    }//fin poner pieza
int ponerPiezaAI(struct jugador jug[], struct tablero tab, int n, int dif)
{
    struct escanear escaneado[NJ*2];
    int col=-1;
    int i=0, j=0, lim=N-1, buscar=N-1;
    if(jug[n].num==NJ+1){
        if(dif != 1){
            if(dif==3)
                lim=2;
            do{
                do{
                    escaneado[n+i]=revisarTab(jug[n],tab,buscar);
                    n++;
                }while(n<NJ);
                n=0; 
                i=2;
                buscar--;
           }while(buscar>=lim);
            //si dificultad intermedia
            for(i=0;i<NJ;i++){
                if(escaneado[i].hay && escaneado[i].nab==3){
	            printf("\nvacia 1: %d",escaneado[i].vacias[0]+1);///////////quitar al final
                        return escaneado[i].vacias[0];
        	}
            }
            if(dif==3){
		    int conta=0;
		for(i=2;i<NJ+N-lim;i++){
		    if(escaneado[i].hay && escaneado[i].nab==3){
			for(j=0;j<N-1;j++){
			    if(escaneado[i].vacias[j] != -1){
	  	  printf("\nvacia %d: %d",j+1,escaneado[i].vacias[j]+1);///////quitar al final
		  		conta++;
			    }
			}
		    }
		}
		if(!conta){
			i=NC-N;
			while(i<NC){
			
			}
		}
                //hardcore extra.
            }
    	    esperar();
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
struct escanear revisarTab(struct jugador jug, struct tablero tab, int n){
    //inicializacion
    struct escanear escaner;
    int i[6];//iterador
    int j[3][N];//contador de coincidencias 0=hor, 1=vert, 2=diag
    escaner.hay=0;
    escaner.nab=0;
    for(i[0]=0;i[0]<N-1;i[0]++){
        escaner.vacias[i[0]]=-1;
    }
    //revisa
            int ce=NC-N-2;//donde comienza a buscar columna externa
            int ci=N-N/2-1;     //donde comienza a buscar columna interna
            i[1]=ce;
            while(i[1]>-1){//columna externa
                for(i[0]=0;i[0]<NF-N+1;i[0]++){//fila externa
                    for(i[2]=0;i[2]<3;i[2]++)//inicializar en 0 contador
                        for(i[3]=0;i[3]<N;i[3]++)
                            j[i[2]][i[3]]=0;
                    //contar dentro de los 4 x 4
                    i[3]=ci;
                    while(i[3]>-1){//columna interna
                        for(i[2]=0;i[2]<N;i[2]++){//fila interna
                            escaner.array[i[0]][i[1]][i[2]][i[3]]=tab.matriz[i[0]+i[2]][i[1]+i[3]];
                            if(jug.num==tab.matriz[i[0]+i[2]][i[1]+i[3]]){
                            //si la pieza de la casilla es la del jugador
                                j[0][i[2]]++;//sumador horizontal
                                j[1][i[3]]++;//sumador vertical
                                if(i[2]==i[3])
                                    j[2][0]++;//diagonal
                                if(i[2]+i[3]+1==N)
                                    j[2][1]++;//diagonal invertida
                            }else if(tab.matriz[i[0]+i[2]][i[1]+i[3]] || tab.cuentaFila[i[1]+i[3]]!=(i[0]+i[2]) ){
                            //si la pieza no es la del jugador y no es 0 o si la jugada no se puede completar
                                j[0][i[2]]=N*-1;//sumador horizontal
                                j[1][i[3]]=N*-1;//sumador vertical
                                if(i[2]==i[3])
                                    j[2][0]=N*-1;//diagonal
                                if(i[2]+i[3]+1==N)
                                    j[2][1]=N*-1;//diagonal invertida
                            }
                        }
                    i[3]=(i[3]-ci)*(-1)+ci;
                    if(i[3]>=ci) i[3]++;
                    }
                    //si coincidencias es igual a el numero
                    for(i[2]=0;i[2]<3;i[2]++)
                        for(i[3]=0;i[3]<N;i[3]++)//posicion dentro del 4x4
                            if(j[i[2]][i[3]]==n){
                                escaner.hay=1;
                                escaner.nab=n;
                                i[5]=0;
                                if(!i[2]){
                                    //revision horizontal
                                    printf("\n horizontal");
                                    for(i[4]=0;i[4]<N;i[4]++)//columna 
                                        if(!tab.matriz[i[0]+i[3]][i[1]+i[4]]){
                                            escaner.vacias[i[5]]=i[1]+i[4];
                                            i[5]++;
                                        }
                                }else if(i[2]==1 && tab.cuentaFila[i[0]+i[3]]<NF){
                                    //revision vertical
                                    printf("\n vertical");
                                    escaner.vacias[0]=i[1]+i[3];
                                }else if(i[2]==2){
                                    //revision diagonal
                                    printf("\n diagonal");
                                    for(i[4]=0;i[4]<N;i[4]++){
                                        if(!i[3]){//diagonal
                                            if(!tab.matriz[i[0]+i[4]][i[1]+i[4]] && tab.cuentaFila[i[1]+i[4]]==i[0]+i[4]){
                                                printf("\n diagonal normal");
                                                escaner.vacias[i[5]]=i[1]+i[4];
                                                i[5]++;
                                            }        
                                        }else if(i[3]==1){//diagonal invertida
                                            if(!tab.matriz[i[0]+i[4]][i[1]+(N-1-i[4])] && tab.cuentaFila[i[1]+(N-1-i[4])]==i[0]+i[4]){
                                                printf("\n diagonal invertida");
                                                escaner.vacias[i[5]]=i[1]+(N-1-i[4]);
                                                i[5]++;
                                            }        
                                        }
                                    }
                                }
                            return escaner;
                            }//fin if se encuentra n    
                }//fin fila externa
            i[1]=(i[1]-ce)*(-1)+ce;
            if(i[1]>=ce) i[1]++;
            }//fin while de columna externa
    return escaner;
}//fin escanear
int revisarGanador(struct jugador jug[],struct tablero tab, int n){
    struct escanear escaneo;
    int contador=0;
    int i=0, j=0;
    //revisar si alguien ganó 
    escaneo=revisarTab(jug[n],tab,4);
    if(escaneo.hay && escaneo.nab==4){
	    dibujarTabla(tab,jug);
        if(jug[n].num==NJ+1)
            printf("\n\n\t\t%sLo siento, esta vez te gané, suerte para la proxima%s",jug[n].color,BLANCO);
        else
            printf("\n\n\t\t\t%s!!!Felicidades %s has ganado¡¡¡%s",jug[n].color,jug[n].nombre,BLANCO);
        esperar();
        return 0;
    }
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
} //fin revisar ganador
void copiar(char a[], char b[]){//a es el nuevo string
    char n=0;
   do{
        a[n]=b[n];
        n++;
    }while(b[n-1] != '\0');
}