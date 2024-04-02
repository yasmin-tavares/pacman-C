#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main() {
	
    //Iniciação
	allegro_init();
    install_keyboard();
	set_color_depth(32);
	set_window_title("Pacman");
	set_close_button_callback(sair);
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);  
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
		
	
	//Variáveis Locais
	buffer = create_bitmap(width, height);
	fundo  = load_bitmap("../img/fundo.bmp", NULL);
	itens  = load_bitmap("../img/itens.bmp", NULL);
	f28    = load_font("../img/f28.pcx", NULL, NULL);
	f18    = load_font("../img/f18.pcx", NULL, NULL);
	start  = load_sample("../sons/start.wav");
	
	
	sfase  = load_sample("../sons/fase.wav");
	morto  = load_sample("../sons/morto.wav");
	ponto  = load_sample("../sons/ponto.wav");
	fruta  = load_sample("../sons/fruta.wav");
	nhac = load_sample("../sons/pilula.wav");
	macfan = load_sample("../sons/macfan.wav");
	
	int i = 0;
	for(i = 0; i <  4; i++){
		fantasma[i].wx  =  0;
		fantasma[i].wy  = 53 + i*41;
		fantasma[i].x   = 12 + i;		
		fantasma[i].y   = 14;
		fantasma[i].w   = 40;
		fantasma[i].h   = 40;
		fantasma[i].dir =  1;		
	}
	mapa(1);	
	while (!(sai || key[KEY_ESC]))
	{	
		if(hscore < pontos) hscore = pontos;
		antX = p.x;
		antY = p.y;	
		draw_sprite(buffer, fundo, 5, 5);
		if(tTxt) textprintf_ex(buffer, f18, txtX, txtY, 0xffffff, -1, "%i", msg);//mensagem pontos	
		textprintf_right_ex(buffer, f28, 980, 250, 0xffffff, -1,"%i", pontos);   //Pontos
		textprintf_right_ex(buffer, f28, 980, 84, 0xffffff, -1, "%i", hscore);   // hscore
		if(tTxt) tTxt--;  //tempo para exibir a mensagem
		if(vida) controle();
		mapa(0);
		//Exibindo frutas
		if(tFrut++ > 200) tFrut = 0; //tempo para exibir a fruta
		if(tFrut > 150){
			masked_blit(itens, buffer, frutas.wx, frutas.wy + fase*frutas.h, frutas.x, frutas.y, frutas.w, frutas.h);
			//Pegando frutas
			if(p.y == 17 && (p.x == 13 || p.x == 14)){
				msg  = 100 + (fase*100);
				tTxt = 20;
				tFrut = 0;
				pontos+= msg;
				txtX  = frutas.x;
				txtY  = frutas.y + 5; 
				play_sample(fruta, 255, 128, 1000, 0);
			}
		}			
		
		if(vida || inicio) masked_blit(itens, buffer, p.wx + x, p.wy, p.x*28 - 8, p.y*21 - 7, p.w, p.h);
		for(i = 0; i < vida; i++) masked_blit(itens, buffer, 84, 0, 820 + i*45, 600, 42, 44);
		fantasmas();
		draw_sprite(screen, buffer, 0, 0);
		rest(150);
		clear(buffer);
		restart();	
		//Sons	
		 	 if(som == 2){stop_sample(ponto); play_sample(ponto, 255, 128, 1000, 0);}
		else if(som == 3){play_sample(nhac, 255, 128, 1000, 0);}
		som = 0;		
	}
	
	pont_arq = fopen("pontuacao.txt", "a+");
	fprintf(pont_arq,"Pontuação final:	%d", pontos);
	fprintf(pont_arq,"\n");
	fclose(pont_arq);
	
	//Finalização
	destroy_font(f28);
	destroy_font(f18);
	destroy_bitmap(itens);
	destroy_bitmap(fundo);
	destroy_bitmap(buffer);
	destroy_sample(start);
	destroy_sample(ponto);
	destroy_sample(macfan);
	destroy_sample(nhac);
	
	return 0;	
}
END_OF_MAIN();

void sair(){sai= 1;}
END_OF_FUNCTION(sair);
/*___________________________________________________________________________________________________________________________________*/
