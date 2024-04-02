/*___________________________________________________________________________________________________________________________________*/
												/* CONSIDERAÇÕES FINAIS DO TRABALHO: */
												
//1º possível problema: Structs usadas como (no lugar de) "objetos".
//	Pelas aulas ministradas e pesquisas que fiz, estruturas C utilizam a função malloc para alocar memória, porém, o uso de alocação 
// de memória nesse jogo não se tornou necessário. Tanto em C quanto em C++ é possível se utilizar structs sem aplicação do malloc. 
// Por exemplo:
	
	//#include <stdio.h>
	//#include <conio.h>
	//int main(void)
//{    
  	/*Criando a struct  
		  struct ficha_de_aluno
		  {
		    char nome[50];
		    char disciplina[30];
		    float nota_prova1;
		    float nota_prova2;  
		  };
		  
   	Criando a variável aluno que será do tipo struct ficha_de_aluno:
			       
  		struct ficha_de_aluno aluno;
 
	  	printf("\n---------- Cadastro de aluno -----------\n\n\n");
	  	printf("Nome do aluno ......: ");
	  	fflush(stdin); 
	 
	  /*usaremos o comando fgets() para ler strings, no caso o nome do aluno e a disciplina
	   fgets(variavel, tamanho da string, entrada) como estamos lendo do teclado a entrada é stdin (entrada padrão),
	   porém em outro caso, a entrada tambem poderia ser um arquivo  */
	 
	  /*fgets(aluno.nome, 40, stdin);
	   
	  printf("Disciplina ......: ");
	  fflush(stdin); 
	  fgets(aluno.disciplina, 40, stdin);
	   
	  printf("Informe a 1a. nota ..: ");
	  scanf("%f", &aluno.nota_prova1);
	   
	  printf("Informe a 2a. nota ..: ");
	  scanf("%f", &aluno.nota_prova2);  
	   
	  printf("\n\n --------- Lendo os dados da struct ---------\n\n");
	  printf("Nome ...........: %s", aluno.nome);
	  printf("Disciplina .....: %s", aluno.disciplina);
	  printf("Nota da Prova 1 ...: %.2f\n" , aluno.nota_prova1);
	  printf("Nota da Prova 2 ...: %.2f\n" , aluno.nota_prova2);
	   
	  getch();
	  return(0);
	} */

// Como podemos ver no código C acima não se utiliza malloc para se utilizar as structs. 
// No trabalho abaixo, foram utilizadas structs partindo desse pressuposto. 

// 2º possível problema: Métodos de limpeza do buffer. 
		// clear(buffer) x  fflush(stdin)
		// Funciona com qualquer uma das duas funções, então não faz diferença qual das duas usar. 
		
// 3º Dev c++: interpretando a linguagem como outra linguagem.
// 	É possível que o compilador ignore completamente o formato do arquivo e a linguagem aplicada a este, interpretando-o como C++ ao invés de C.
// ENTRETANTO ao testar o código desenvolvido via terminal em uma máquina virtual Linux, ou seja, fora do ambiente Dev C++, o mesmo rodou como
// C sem problema algum. 		

/*___________________________________________________________________________________________________________________________________*/

#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

struct campos{int wx,wy,x,y,w,h,dir,bk;};   // posições e proporções dos campos que os componentes percorrem.
struct campos p = {42, 0,14,23,42,44,5}, fantasma[4], frutas = {166, 0,370,352,50,40};

//funções
void sair();
void fantasmas();
void mapa(int);
void controle();
void restart();

//Ponteiros usados no código

SAMPLE *ponto , *nhac, *start, *sfase, *fruta, *macfan, *morto;
BITMAP *buffer, *itens, *fundo;
FONT   *f28, *f18;
FILE *pont_arq;

//Variáveis Globais
int x      =   0;
int sai    =   0;
int som    =   0;
int mov    =   0;
int msg    =   0;
int tTxt   =   0;
int txtX   =   0;
int txtY   =   0;
int antX   =   0;
int antY   =   0;
int next   =   0;
int fase   =   0;
int vida   =   0;
int macFan = 100;
int vidas  =   3;
int morre  =   0; 
int inicio =   0;
int tFrut  =   0;
int tPower =   0; 
int width  =1048;
int height = 670;
int pontos =   0;
int hscore =1000;
int map[31][28] ;

// Funções desenvolvidas
	//Fantasmas
void fantasmas(){
	int i;	
	for(i = 0; i < 4; i++){
		
		//Resetando os fantasmas 
		if(fantasma[i].x == 13 && (fantasma[i].y == 13 || fantasma[i].y == 14)) tPower ? (fantasma[i].bk = 1) : (fantasma[i].bk = 0);
		
		if(fantasma[i].bk){
			     if(fantasma[i].y  ==  5 && fantasma[i].x > 9 && fantasma[i].x < 18)                   {fantasma[i].dir = 0;}//grande  T encima  
			else if(fantasma[i].y  == 23 && fantasma[i].x >   1  && fantasma[i].x <  4)                {fantasma[i].dir = 0;}  //7 abaixo
			else if(fantasma[i].y  == 23 && fantasma[i].x >  23  && fantasma[i].x < 26)                {fantasma[i].dir = 2;}  //F abaixo 
			else if(fantasma[i].y  <  15 && fantasma[i].y > 11 && (fantasma[i].x == 9 || fantasma[i].x == 18)){fantasma[i].dir = 1;}//sobe fatasmas até a caixa 
			else if((fantasma[i].y == 17)&& fantasma[i].x > 9 && fantasma[i].x < 18)                   {fantasma[i].dir = 2;}   //embaixo da caixa frutas
			else if((fantasma[i].y == 20 || fantasma[i].y == 26) && fantasma[i].x > 14 && fantasma[i].x < 18) {fantasma[i].dir = 2;}  //Ts centrais dir-> cima e baixo
			else if((fantasma[i].y == 20 || fantasma[i].y == 26) && fantasma[i].x >  9 && fantasma[i].x < 13) {fantasma[i].dir = 0;}  //Ts centrais esq-> cima e baixo								
			else if(fantasma[i].y  >  14 && map[fantasma[i].y-1][fantasma[i].x] != 1)                   fantasma[i].dir = 1; // centraliza para cima				
			else if(fantasma[i].y  <  13 &&(fantasma[i].x != 9 || fantasma[i].y == 5) && fantasma[i].x != 18 && map[fantasma[i].y+1][fantasma[i].x] != 1) fantasma[i].dir = 3; // centraliza para baixo
			else if(fantasma[i].x  <  13 && map[fantasma[i].y][fantasma[i].x+1] != 1)                   fantasma[i].dir = 2; // centraliza para direita	
			else if(fantasma[i].x  >  13 && map[fantasma[i].y][fantasma[i].x-1] != 1)                   fantasma[i].dir = 0; // centraliza para esquerda
			
		}else{
			//saindo da caixa
			if(fantasma[i].x  >  10 && fantasma[i].x  < 17  &&  fantasma[i].y == 14) fantasma[i].dir  = rand()%4;
			else if((fantasma[i].y == 12 || fantasma[i].y == 13) && (fantasma[i].x == 13 || fantasma[i].x == 14))  fantasma[i].dir = 1;
			
			//Fantasmas travessando a tela
	  		else if(fantasma[i].x <  1) fantasma[i].x = 27;                  //saída pela esquerda
			else if(fantasma[i].x > 26 && fantasma[i].dir == 2) fantasma[i].x =  1; //Saída pela direita	
			
			//(Blinky, fantasma vermelho) seguir os movimentos do pacman
			else if(!tPower && i == 3 && fantasma[3].x < p.x && fantasma[3].dir != 0 && map[fantasma[3].y][fantasma[3].x+1] != 1) fantasma[3].dir = 2;
			else if(!tPower && i == 3 && fantasma[3].x > p.x && fantasma[3].dir != 2 && map[fantasma[3].y][fantasma[3].x-1] != 1) fantasma[3].dir = 0;
			else if(!tPower && i == 3 && fantasma[3].y < p.y && fantasma[3].dir != 1 && map[fantasma[3].y+1][fantasma[3].x] != 1) fantasma[3].dir = 3;
			else if(!tPower && i == 3 && fantasma[3].y > p.y && fantasma[3].dir != 3 && map[fantasma[3].y-1][fantasma[3].x] != 1) fantasma[3].dir = 1;
			
			// (Pinky, fantasma rosa) se posiciona em um ponto que é 32 pixels a frente do Pac-Man
			else if(i == 2 && fantasma[2].x < p.x+16 && fantasma[2].dir != 0 && map[fantasma[2].y][fantasma[2].x+1] != 1) fantasma[2].dir = 2; 
			else if(i == 2 && fantasma[2].x > p.x+16 && fantasma[2].dir != 2 && map[fantasma[2].y][fantasma[2].x-1] != 1) fantasma[2].dir = 0; 
			else if(i == 2 && fantasma[2].y < p.y+16 && fantasma[2].dir != 1 && map[fantasma[2].y+1][fantasma[2].x] != 1) fantasma[2].dir = 3; 
			else if(i == 2 && fantasma[2].y > p.y+16 && fantasma[2].dir != 3 && map[fantasma[2].y-1][fantasma[2].x] != 1) fantasma[2].dir = 1; 
			
			//(Inky, fantasma azul) similar ao rosa.
			else if(i == 1 && fantasma[1].x < p.x-8 && fantasma[1].dir != 0 && map[fantasma[1].y][fantasma[1].x+1] != 1) fantasma[1].dir = 2; 
			else if(i == 1 && fantasma[1].x > p.x-8 && fantasma[1].dir != 2 && map[fantasma[1].y][fantasma[1].x-1] != 1) fantasma[1].dir = 0; 
			else if(i == 1 && fantasma[1].y < p.y-8 && fantasma[1].dir != 1 && map[fantasma[1].y+1][fantasma[1].x] != 1) fantasma[1].dir = 3; 
			else if(i == 1 && fantasma[1].y > p.y-8 && fantasma[1].dir != 3 && map[fantasma[1].y-1][fantasma[1].x] != 1) fantasma[1].dir = 1; 	
			else {
				
				//(Clyde,fantasma laranja) movimento aleatório.
				// Movimento para a direita 
				if(fantasma[i].dir == 2 && map[fantasma[i].y][fantasma[i].x+1] == 1){
					if(map[fantasma[i].y-1][fantasma[i].x] != 1 && map[fantasma[i].y+1][fantasma[i].x] != 1)
						(rand()%2) ? (fantasma[i].dir = 1) : (fantasma[i].dir = 3);
					else (map[fantasma[i].y-1][fantasma[i].x] != 1) ? (fantasma[i].dir = 1) : (fantasma[i].dir = 3);
				}
				// Movimento para a esquerda  
				if(fantasma[i].dir == 0 && map[fantasma[i].y][fantasma[i].x-1] == 1){
					if(map[fantasma[i].y-1][fantasma[i].x] != 1 && map[fantasma[i].y+1][fantasma[i].x] != 1)
						(rand()%2) ? (fantasma[i].dir = 1) : (fantasma[i].dir = 3);
					else (map[fantasma[i].y-1][fantasma[i].x] != 1) ? (fantasma[i].dir = 1) : (fantasma[i].dir = 3);
				}
				// Movimento para baixo 
				if(fantasma[i].dir == 3 && map[fantasma[i].y+1][fantasma[i].x] == 1){
					if(map[fantasma[i].y][fantasma[i].x-1] != 1 && map[fantasma[i].y][fantasma[i].x+1] != 1)
						(rand()%2) ? (fantasma[i].dir = 0) : (fantasma[i].dir = 2);
					else (map[fantasma[i].y][fantasma[i].x-1] != 1) ? (fantasma[i].dir = 0) : (fantasma[i].dir = 2);
				}
			 	// Movimento para cima
				if(fantasma[i].dir == 1 && map[fantasma[i].y-1][fantasma[i].x] == 1){
					if(map[fantasma[i].y][fantasma[i].x-1] != 1 && map[fantasma[i].y][fantasma[i].x+1] != 1)
						(rand()%2) ? (fantasma[i].dir = 0) : (fantasma[i].dir = 2);
					else (map[fantasma[i].y][fantasma[i].x-1] != 1) ? (fantasma[i].dir = 0) : (fantasma[i].dir = 2);
				}
			}
		}
		
		
	         if(fantasma[i].dir == 0 && map[fantasma[i].y][fantasma[i].x-1] != 1) fantasma[i].x--; 	//esq
		else if(fantasma[i].dir == 1 && map[fantasma[i].y-1][fantasma[i].x] != 1) fantasma[i].y--;  //cima 
		else if(fantasma[i].dir == 2 && map[fantasma[i].y][fantasma[i].x+1] != 1) fantasma[i].x++;  //dir
		else if(fantasma[i].dir == 3 && map[fantasma[i].y+1][fantasma[i].x] != 1) fantasma[i].y++;  //baixo
		
		//Exibição dos fantasmas
		if(fantasma[i].bk)     masked_blit(itens, buffer, fantasma[i].dir*41, 217, fantasma[i].x*28 - 8, fantasma[i].y*21 - 7, fantasma[i].w, fantasma[i].h); //olhos
		else if(tPower) masked_blit(itens, buffer, 164, 176, fantasma[i].x*28 - 8, fantasma[i].y*21 - 7, fantasma[i].w, fantasma[i].h);	  //Azul
		else            masked_blit(itens, buffer, fantasma[i].wx + fantasma[i].dir*41, fantasma[i].wy, fantasma[i].x*28 - 8, fantasma[i].y*21 - 7, fantasma[i].w, fantasma[i].h); // normal	
		
		//Colisão com fantasmas
		if(vida && !fantasma[i].bk && (p.x == fantasma[i].x && p.y == fantasma[i].y || antX == fantasma[i].x && antY == fantasma[i].y)){
			if(tPower){
				fantasma[i].bk = 1;
				macFan *= 2;
				tTxt = 30;
				pontos+= macFan;
				msg  = macFan;
				txtX = fantasma[i].x*28-fantasma[i].w/2;
				txtY = fantasma[i].y*21;
				play_sample(macfan, 255, 128, 1000, 0);
				rest(500);
			}else morre = 1;		 	
	 	}
	}
}

	// Mapa	
void mapa(int mod){
	int i, j;

	char mp[31][28] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
					   {1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
					   {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
					   {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
					   {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
					   {1,1,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,4,4,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,0,0,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {0,0,0,0,0,0,2,2,2,2,1,0,0,0,0,0,0,1,2,2,2,2,0,0,0,0,0,0},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
					   {1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
					   {1,2,2,2,1,1,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,1,1,2,2,2,1},
					   {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
					   {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
					   {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
					   {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
					   {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
					   {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
					   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};	
	next = 0;				   					   
	for(i = 0; i < 31; i++){
		for(j = 0; j < 28; j++){
			if(mod)	map[i][j] = mp[i][j];
			if(map[i][j] == 2) masked_blit(itens, buffer,   10, 10, j*28 + 10, i*21 + 16,  6,  6);//Pontos	
			if(map[i][j] == 3 && mov < 3) masked_blit(itens, buffer, 127,  0, j*28 +  3, i*21 + 10, 18, 18);//Nhac
			if(map[i][j]) next++;	
		}
	}					
	
}

// Reiniciar	
	void restart(){
	int i;
	if(morre || !vida) stop_sample(sfase);
	if(inicio){
		play_sample(start, 255, 128, 1000, 0);	
		vida = vidas;
		rest(5000);
		play_sample(sfase, 255, 128, 1000, 1);
		inicio = 0;
		pontos = 0;
		fase = 0;		
	}
	if(key[KEY_SPACE] && !vida){
		while(key[KEY_SPACE]) ; inicio = 1;
	}
	if(next == 568){
		fase++;
		stop_sample(sfase);
		stop_sample(nhac);	
		for(i = 0; i < 5; i++){
			draw_sprite(screen, fundo, 5, 5);
			rest(500);
			rectfill(screen, 0, 0, 780, 680, 0x00);
			rest(500);
		}
	}else if(morre && vida){
		vida--;
		stop_sample(sfase);
		rest(500);
		play_sample(morto, 255, 128, 1000, 0);
		for(i = 0; i < 7; i++){
			draw_sprite(buffer, fundo, 5, 5);	
			masked_blit(itens, buffer, p.wx + i*p.w, 261, p.x*28 - 8 - inicio*16, p.y*21 - 7, p.w, p.h);
			mapa(0);		
			draw_sprite(screen, buffer, 0, 0);			
			rest(200);
		fflush(stdin);
		}
	}

	if(inicio || (next == 568) || morre){
		if(!morre) mapa(1);
		tPower = 0;
		tFrut  = 0;
		p.x = 14;
		p.y = 23;
		p.dir = 4;
		morre = 0;
		macFan = 100; 
		tTxt  = 0;
		for(i = 0; i < 4; i++){
			fantasma[i].x = 12 + i;
			fantasma[i].y = 14;
			fantasma[i].bk = 0;
		}
		if(!inicio) play_sample(sfase, 255, 128, 1000, 1);
	}
}

// Controle do jogo
void controle(){
	if(key[KEY_LEFT] && map[p.y][p.x-1] != 1) p.dir = 0;
	if(key[KEY_UP]   && map[p.y-1][p.x] != 1) p.dir = 1;
	if(key[KEY_RIGHT]&& map[p.y][p.x+1] != 1) p.dir = 2;
	if(key[KEY_DOWN] && map[p.y+1][p.x] != 1) p.dir = 3;	
	
	if(p.dir == 0 && map[p.y][p.x-1] != 1){p.x--; p.wx = 0;}
	if(p.dir == 1 && map[p.y-1][p.x] != 1) p.y--;
	if(p.dir == 2 && map[p.y][p.x+1] != 1){p.x++; p.wx = 41;}
	if(p.dir == 3 && map[p.y+1][p.x] != 1) p.y++;
	
	//Abrindo e fechando a boca 
	if(mov > 4) { x = 0; mov = 0;}
	if(mov++ > 1) x = 42;
		
	//Atravessando a tela
	if(p.x > 26 &&  p.dir) p.x =  0;
	if(p.x <  1 && !p.dir) p.x = 27;
	
	//Recolhe objetos
	if(map[p.y][p.x] == 2){pontos+= 10; som = 2; map[p.y][p.x] = 0;}
	//pegando nhacs
	if(map[p.y][p.x] == 3){  
		som = 3; 
		pontos+= 50;
		tPower = 50;
		map[p.y][p.x] = 0;			
	}
	if(tPower) tPower--;
	if(tPower == 1) stop_sample(nhac);
}
