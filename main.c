/* Keep these headers */
#include <tice.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/ce/graphx.h>
#include <lib/ce/fileioc.h>
#include <lib/ce/keypadc.h>
//TODO: guns?
//      DOORS
//      ELEVATOR
//      PICKUPS
//      DOGS
//      SOME FORM OF LEVEL PARSER
//      LEVEL

void draw(void);

int randv;
int randv2;
int health= 100;
void init(void);
void data_init(void);
typedef struct{
	float x;
	float y;
} Point2D;
typedef struct {
	Point2D pointLoLt;
	Point2D pointUpRt;
	uint8_t wc1;
	uint8_t wc2;
} Brush2D;
typedef struct {
	float ang;
	Point2D pos;
} PlayerData;
typedef struct{
	uint8_t sp[14][9];
} SprMap;
typedef struct{
	int id;
	int min_x;
	int max_x;
	Point2D pos;
	float spwidth;
	float spheight;
	bool ena;
	SprMap sp;
	bool fliptick;
	bool rendered;
} SpriteDt;

typedef struct{
	Point2D pos;
	int sp_id;
	bool vis;
	float hp;
	bool dead;
	bool pickup;
	bool canfire;
} NaziData;
int ammo = 8;
SprMap spr_nazi_norm_a;
SprMap spr_nazi_shoot_a;
SprMap spr_nazi_die_a;
PlayerData plyr;
const int num_brushes = 1;
const int num_sprites = 1;
const int num_nazis = 1;
const float max_dist=7;
Brush2D brushArry[num_brushes];
SpriteDt spriteArry[num_sprites];
NaziData naziArray[num_nazis];
float getangscan(int xscan);
float getangscan(int xscan){
	float tmpang;
	float ang;
	tmpang=plyr.ang;
	
	ang=((float)(xscan)-160)/160*3.1415/4+tmpang;
	return ang;
}
void init_spr_ids(void);
void init_spr_ids(){
	int i;
	for(i = 0; i < num_sprites; i++){
		spriteArry[i].id=i;
		spriteArry[i].min_x=0;
		spriteArry[i].max_x=0;
		
	}
}
void init_nazi(int ind,int brushind);
void init_nazi(int ind,int brushind){
	naziArray[ind].pos.x=-3;
	naziArray[ind].pos.y=-3;
	naziArray[ind].sp_id=brushind;
	naziArray[ind].vis=false;
	naziArray[ind].hp=25;
	naziArray[ind].dead=false;
	naziArray[ind].canfire=false;
	naziArray[ind].pickup=false;
	
	spriteArry[naziArray[ind].sp_id].sp=spr_nazi_norm_a;
}
void res_spr_x(void);
void res_spr_x(){
	int i;
	for(i = 0; i < num_sprites; i++){
		spriteArry[i].min_x=0;
		spriteArry[i].max_x=0;
		spriteArry[i].rendered=false;	
	}
}
void handleNazi(int ind){
	
	int xscan;
	float dist;
	bool hit;
	float ang;
	float xt;
	float yt;
	
	int brush_ind;
	float distance = sqrt((plyr.pos.x-naziArray[ind].pos.x)*(plyr.pos.x-naziArray[ind].pos.x)+(plyr.pos.y-naziArray[ind].pos.y)*(plyr.pos.y-naziArray[ind].pos.y));
	if(distance==0){
		distance=1;
	}
		dist = 1/distance;
		hit=false;
		while(dist <1&&hit==false){
			xt=plyr.pos.x;
			yt=plyr.pos.y;
			xt+=(naziArray[ind].pos.x-plyr.pos.x)*dist;
			yt+=(naziArray[ind].pos.y-plyr.pos.y)*dist;
			for(brush_ind = 0; brush_ind < num_brushes; brush_ind += 1){
				if(xt<brushArry[brush_ind].pointUpRt.x && xt > brushArry[brush_ind].pointLoLt.x&&yt<brushArry[brush_ind].pointUpRt.y&&yt>brushArry[brush_ind].pointLoLt.y){
					hit=true;
				}
			}
			
			dist+=1/distance;
		}
	
	spriteArry[naziArray[ind].sp_id].pos.x=naziArray[ind].pos.x;
	spriteArry[naziArray[ind].sp_id].pos.y=naziArray[ind].pos.y;
	if(naziArray[ind].hp>0){
	if(hit!=true){
		
		naziArray[ind].vis=true;
		
		
	}
		if(distance <= 2&&naziArray[ind].canfire==true && naziArray[ind].vis==true){
			if(randv<=8){
		
			spriteArry[naziArray[ind].sp_id].sp=spr_nazi_shoot_a;
			naziArray[ind].canfire=false;
				naziArray[ind].vis=false;
				if(randv>2){
					health-=15;
				}
			}
			//fired
				
		}else{
			
		spriteArry[naziArray[ind].sp_id].sp=spr_nazi_norm_a;
	}
	if(naziArray[ind].vis==true&&naziArray[ind].canfire==false &&distance>2){
		if(randv<=8){
			xt=naziArray[ind].pos.x-(naziArray[ind].pos.x-plyr.pos.x)/(4*distance);
			yt=naziArray[ind].pos.y-(naziArray[ind].pos.y-plyr.pos.y)/(4*distance);
		hit=false;
		    for(brush_ind = 0; brush_ind < num_brushes; brush_ind += 1){
				if(xt<brushArry[brush_ind].pointUpRt.x && xt > brushArry[brush_ind].pointLoLt.x&&yt<brushArry[brush_ind].pointUpRt.y&&yt>brushArry[brush_ind].pointLoLt.y){
					hit=true;
				}
			}
		if(hit==false){
			naziArray[ind].pos.x=xt;
			naziArray[ind].pos.y=yt;
		}
		}
	}
	if(distance <= 2 && naziArray[ind].vis==true){
		
		naziArray[ind].canfire=true;	
	}else{
		naziArray[ind].canfire=false;
	}
	}else{
		if(naziArray[ind].pickup==false){spriteArry[naziArray[ind].sp_id].sp=spr_nazi_die_a;}
		
		if(distance<=1&&naziArray[ind].pickup==false){
			spriteArry[naziArray[ind].sp_id].ena=false;
			naziArray[ind].pickup=true;
			if(randv<=8){
				ammo+=4;
			}
			else{
				ammo+=8;
			}
			if(ammo>99){
				ammo=99;
			}
		}
	}
}
int lives = 3;
void d_init(){
	init_spr_ids();
	brushArry[0].pointLoLt.x=-5;
	brushArry[0].pointLoLt.y=4;
	brushArry[0].pointUpRt.x=5;
	brushArry[0].pointUpRt.y=5;
	brushArry[0].wc1=gfx_blue;
	brushArry[0].wc2=gfx_black;
	spriteArry[0].pos.x=-3;
	spriteArry[0].pos.y=-3;
	spriteArry[0].spheight=1;
	spriteArry[0].spwidth=0.75;
	spriteArry[0].ena=true;
	spriteArry[0].fliptick=true;

	*/
	data_init();
	init_nazi(0,0);

}

void data_init(){
	int isc1=0;
	int isc2=0;
	spr_nazi_norm_a.sp[0][0]=gfx_pink;
	spr_nazi_norm_a.sp[0][1]=gfx_pink;
	spr_nazi_norm_a.sp[0][2]=gfx_black;
	spr_nazi_norm_a.sp[0][3]=gfx_black;
	spr_nazi_norm_a.sp[0][4]=gfx_black;
	spr_nazi_norm_a.sp[0][5]=gfx_black;
	spr_nazi_norm_a.sp[0][6]=gfx_black;
	spr_nazi_norm_a.sp[0][7]=gfx_pink;
	spr_nazi_norm_a.sp[0][8]=gfx_pink;
	spr_nazi_norm_a.sp[1][0]=gfx_pink;
	spr_nazi_norm_a.sp[1][1]=gfx_black;
	spr_nazi_norm_a.sp[1][2]=gfx_black;
	spr_nazi_norm_a.sp[1][3]=gfx_black;
	spr_nazi_norm_a.sp[1][4]=gfx_black;
	spr_nazi_norm_a.sp[1][5]=gfx_black;
	spr_nazi_norm_a.sp[1][6]=gfx_black;
	spr_nazi_norm_a.sp[1][7]=gfx_black;
	spr_nazi_norm_a.sp[1][8]=gfx_pink;
	spr_nazi_norm_a.sp[2][0]=gfx_pink;
	spr_nazi_norm_a.sp[2][1]=gfx_black;
	spr_nazi_norm_a.sp[2][2]=gfx_yellow;
	spr_nazi_norm_a.sp[2][3]=gfx_blue;
	spr_nazi_norm_a.sp[2][4]=gfx_yellow;
	spr_nazi_norm_a.sp[2][5]=gfx_blue;
	spr_nazi_norm_a.sp[2][6]=gfx_yellow;
	spr_nazi_norm_a.sp[2][7]=gfx_black;
	spr_nazi_norm_a.sp[2][8]=gfx_pink;
	spr_nazi_norm_a.sp[3][0]=gfx_pink;
	spr_nazi_norm_a.sp[3][1]=gfx_pink;
	spr_nazi_norm_a.sp[3][2]=gfx_yellow;
	spr_nazi_norm_a.sp[3][3]=gfx_yellow;
	spr_nazi_norm_a.sp[3][4]=gfx_yellow;
	spr_nazi_norm_a.sp[3][5]=gfx_yellow;
	spr_nazi_norm_a.sp[3][6]=gfx_yellow;
	spr_nazi_norm_a.sp[3][7]=gfx_pink;
	spr_nazi_norm_a.sp[3][8]=gfx_pink;
	spr_nazi_norm_a.sp[4][0]=gfx_pink;
	spr_nazi_norm_a.sp[4][1]=gfx_black;
	spr_nazi_norm_a.sp[4][2]=gfx_black;
	spr_nazi_norm_a.sp[4][3]=gfx_black;
	spr_nazi_norm_a.sp[4][4]=gfx_yellow;
	spr_nazi_norm_a.sp[4][5]=gfx_black;
	spr_nazi_norm_a.sp[4][6]=gfx_black;
	spr_nazi_norm_a.sp[4][7]=gfx_black;
	spr_nazi_norm_a.sp[4][8]=gfx_pink;
	spr_nazi_norm_a.sp[5][0]=gfx_pink;
	spr_nazi_norm_a.sp[5][1]=gfx_orange;
	spr_nazi_norm_a.sp[5][2]=gfx_black;
	spr_nazi_norm_a.sp[5][3]=gfx_orange;
	spr_nazi_norm_a.sp[5][4]=gfx_black;
	spr_nazi_norm_a.sp[5][5]=gfx_orange;
	spr_nazi_norm_a.sp[5][6]=gfx_orange;
	spr_nazi_norm_a.sp[5][7]=gfx_orange;
	spr_nazi_norm_a.sp[5][8]=gfx_black;
	spr_nazi_norm_a.sp[6][0]=gfx_orange;
	spr_nazi_norm_a.sp[6][1]=gfx_orange;
	spr_nazi_norm_a.sp[6][2]=gfx_black;
	spr_nazi_norm_a.sp[6][3]=gfx_black;
	spr_nazi_norm_a.sp[6][4]=gfx_orange;
	spr_nazi_norm_a.sp[6][5]=gfx_orange;
	spr_nazi_norm_a.sp[6][6]=gfx_black;
	spr_nazi_norm_a.sp[6][7]=gfx_orange;
	spr_nazi_norm_a.sp[6][8]=gfx_orange;
	spr_nazi_norm_a.sp[7][0]=gfx_orange;
	spr_nazi_norm_a.sp[7][1]=gfx_orange;
	spr_nazi_norm_a.sp[7][2]=gfx_black;
	spr_nazi_norm_a.sp[7][3]=gfx_orange;
	spr_nazi_norm_a.sp[7][4]=gfx_black;
	spr_nazi_norm_a.sp[7][5]=gfx_orange;
	spr_nazi_norm_a.sp[7][6]=gfx_black;
	spr_nazi_norm_a.sp[7][7]=gfx_orange;
	spr_nazi_norm_a.sp[7][8]=gfx_orange;
	spr_nazi_norm_a.sp[8][0]=gfx_orange;
	spr_nazi_norm_a.sp[8][1]=gfx_orange;
	spr_nazi_norm_a.sp[8][2]=gfx_yellow;
	spr_nazi_norm_a.sp[8][3]=gfx_yellow;
	spr_nazi_norm_a.sp[8][4]=gfx_orange;
	spr_nazi_norm_a.sp[8][5]=gfx_black;
	spr_nazi_norm_a.sp[8][6]=gfx_black;
	spr_nazi_norm_a.sp[8][7]=gfx_yellow;
	spr_nazi_norm_a.sp[8][8]=gfx_black;
	
	spr_nazi_norm_a.sp[9][0]=gfx_pink;
	spr_nazi_norm_a.sp[9][1]=gfx_black;
	spr_nazi_norm_a.sp[9][2]=gfx_yellow;
	spr_nazi_norm_a.sp[9][3]=gfx_yellow;
	spr_nazi_norm_a.sp[9][4]=gfx_black;
	spr_nazi_norm_a.sp[9][5]=gfx_orange;
	spr_nazi_norm_a.sp[9][6]=gfx_black;
	spr_nazi_norm_a.sp[9][7]=gfx_black;
	spr_nazi_norm_a.sp[9][8]=gfx_pink;
	spr_nazi_norm_a.sp[10][0]=gfx_pink;
	spr_nazi_norm_a.sp[10][1]=gfx_pink;
	spr_nazi_norm_a.sp[10][2]=gfx_orange;
	spr_nazi_norm_a.sp[10][3]=gfx_orange;
	spr_nazi_norm_a.sp[10][4]=gfx_black;
	spr_nazi_norm_a.sp[10][5]=gfx_orange;
	spr_nazi_norm_a.sp[10][6]=gfx_black;
	spr_nazi_norm_a.sp[10][7]=gfx_pink;
	spr_nazi_norm_a.sp[10][8]=gfx_pink;
	spr_nazi_norm_a.sp[11][0]=gfx_pink;
	spr_nazi_norm_a.sp[11][1]=gfx_pink;
	spr_nazi_norm_a.sp[11][2]=gfx_blue;
	spr_nazi_norm_a.sp[11][3]=gfx_blue;
	spr_nazi_norm_a.sp[11][4]=gfx_black;
	spr_nazi_norm_a.sp[11][5]=gfx_blue;
	spr_nazi_norm_a.sp[11][6]=gfx_black;
	spr_nazi_norm_a.sp[11][7]=gfx_pink;
	spr_nazi_norm_a.sp[11][8]=gfx_pink;
	spr_nazi_norm_a.sp[12][0]=gfx_pink;
	spr_nazi_norm_a.sp[12][1]=gfx_pink;
	spr_nazi_norm_a.sp[12][2]=gfx_blue;
	spr_nazi_norm_a.sp[12][3]=gfx_blue;
	spr_nazi_norm_a.sp[12][4]=gfx_black;
	spr_nazi_norm_a.sp[12][5]=gfx_blue;
	spr_nazi_norm_a.sp[12][6]=gfx_blue;
	spr_nazi_norm_a.sp[12][7]=gfx_black;
	spr_nazi_norm_a.sp[12][8]=gfx_pink;
	spr_nazi_norm_a.sp[13][0]=gfx_pink;
	spr_nazi_norm_a.sp[13][1]=gfx_pink;
	spr_nazi_norm_a.sp[13][2]=gfx_blue;
	spr_nazi_norm_a.sp[13][3]=gfx_blue;
	spr_nazi_norm_a.sp[13][4]=gfx_black;
	spr_nazi_norm_a.sp[13][5]=gfx_blue;
	spr_nazi_norm_a.sp[13][6]=gfx_blue;
	spr_nazi_norm_a.sp[13][7]=gfx_black;
	spr_nazi_norm_a.sp[13][8]=gfx_pink;
	
	spriteArry[0].sp=spr_nazi_norm_a;
	spr_nazi_shoot_a.sp[0][0]=gfx_pink;
	spr_nazi_shoot_a.sp[0][1]=gfx_pink;
	spr_nazi_shoot_a.sp[0][2]=gfx_black;
	spr_nazi_shoot_a.sp[0][3]=gfx_black;
	spr_nazi_shoot_a.sp[0][4]=gfx_black;
	spr_nazi_shoot_a.sp[0][5]=gfx_black;
	spr_nazi_shoot_a.sp[0][6]=gfx_black;
	spr_nazi_shoot_a.sp[0][7]=gfx_pink;
	spr_nazi_shoot_a.sp[0][8]=gfx_pink;
	spr_nazi_shoot_a.sp[1][0]=gfx_pink;
	spr_nazi_shoot_a.sp[1][1]=gfx_black;
	spr_nazi_shoot_a.sp[1][2]=gfx_black;
	spr_nazi_shoot_a.sp[1][3]=gfx_black;
	spr_nazi_shoot_a.sp[1][4]=gfx_black;
	spr_nazi_shoot_a.sp[1][5]=gfx_black;
	spr_nazi_shoot_a.sp[1][6]=gfx_black;
	spr_nazi_shoot_a.sp[1][7]=gfx_black;
	spr_nazi_shoot_a.sp[1][8]=gfx_pink;
	spr_nazi_shoot_a.sp[2][0]=gfx_pink;
	spr_nazi_shoot_a.sp[2][1]=gfx_black;
	spr_nazi_shoot_a.sp[2][2]=gfx_black;
	spr_nazi_shoot_a.sp[2][3]=gfx_black;
	spr_nazi_shoot_a.sp[2][4]=gfx_red;
	spr_nazi_shoot_a.sp[2][5]=gfx_black;
	spr_nazi_shoot_a.sp[2][6]=gfx_black;
	spr_nazi_shoot_a.sp[2][7]=gfx_black;
	spr_nazi_shoot_a.sp[2][8]=gfx_pink;
	spr_nazi_shoot_a.sp[3][0]=gfx_pink;
	spr_nazi_shoot_a.sp[3][1]=gfx_black;
	spr_nazi_shoot_a.sp[3][2]=gfx_yellow;
	spr_nazi_shoot_a.sp[3][3]=gfx_red;
	spr_nazi_shoot_a.sp[3][4]=gfx_orange;
	spr_nazi_shoot_a.sp[3][5]=gfx_red;
	spr_nazi_shoot_a.sp[3][6]=gfx_yellow;
	spr_nazi_shoot_a.sp[3][7]=gfx_black;
	spr_nazi_shoot_a.sp[3][8]=gfx_pink;
	spr_nazi_shoot_a.sp[4][0]=gfx_pink;
	spr_nazi_shoot_a.sp[4][1]=gfx_black;
	spr_nazi_shoot_a.sp[4][2]=gfx_red;
	spr_nazi_shoot_a.sp[4][3]=gfx_orange;
	spr_nazi_shoot_a.sp[4][4]=gfx_white;
	spr_nazi_shoot_a.sp[4][5]=gfx_orange;
	spr_nazi_shoot_a.sp[4][6]=gfx_red;
	spr_nazi_shoot_a.sp[4][7]=gfx_black;
	spr_nazi_shoot_a.sp[4][8]=gfx_pink;
	
	spr_nazi_shoot_a.sp[5][0]=gfx_black;
	spr_nazi_shoot_a.sp[5][1]=gfx_red;
	spr_nazi_shoot_a.sp[5][2]=gfx_orange;
	spr_nazi_shoot_a.sp[5][3]=gfx_white;
	spr_nazi_shoot_a.sp[5][4]=gfx_white;
	spr_nazi_shoot_a.sp[5][5]=gfx_white;
	spr_nazi_shoot_a.sp[5][6]=gfx_orange;
	spr_nazi_shoot_a.sp[5][7]=gfx_red;
	spr_nazi_shoot_a.sp[5][8]=gfx_black;
		
	spr_nazi_shoot_a.sp[6][0]=gfx_orange;
	spr_nazi_shoot_a.sp[6][1]=gfx_orange;
	spr_nazi_shoot_a.sp[6][2]=gfx_red;
	spr_nazi_shoot_a.sp[6][3]=gfx_orange;
	spr_nazi_shoot_a.sp[6][4]=gfx_white;
	spr_nazi_shoot_a.sp[6][5]=gfx_orange;
	spr_nazi_shoot_a.sp[6][6]=gfx_red;
	spr_nazi_shoot_a.sp[6][7]=gfx_orange;
	spr_nazi_shoot_a.sp[6][8]=gfx_orange;
	spr_nazi_shoot_a.sp[7][0]=gfx_black;
	spr_nazi_shoot_a.sp[7][1]=gfx_black;
	spr_nazi_shoot_a.sp[7][2]=gfx_black;
	spr_nazi_shoot_a.sp[7][3]=gfx_red;
	spr_nazi_shoot_a.sp[7][4]=gfx_orange;
	spr_nazi_shoot_a.sp[7][5]=gfx_red;
	spr_nazi_shoot_a.sp[7][6]=gfx_black;
	spr_nazi_shoot_a.sp[7][7]=gfx_black;
	spr_nazi_shoot_a.sp[7][8]=gfx_black;
	spr_nazi_shoot_a.sp[8][0]=gfx_pink;
	spr_nazi_shoot_a.sp[8][1]=gfx_pink;
	spr_nazi_shoot_a.sp[8][2]=gfx_orange;
	spr_nazi_shoot_a.sp[8][3]=gfx_orange;
	spr_nazi_shoot_a.sp[8][4]=gfx_red;
	spr_nazi_shoot_a.sp[8][5]=gfx_orange;
	spr_nazi_shoot_a.sp[8][6]=gfx_orange;
	spr_nazi_shoot_a.sp[8][7]=gfx_pink;
	spr_nazi_shoot_a.sp[8][8]=gfx_pink;
	spr_nazi_shoot_a.sp[9][0]=gfx_pink;
	spr_nazi_shoot_a.sp[9][1]=gfx_pink;
	spr_nazi_shoot_a.sp[9][2]=gfx_orange;
	spr_nazi_shoot_a.sp[9][3]=gfx_orange;
	spr_nazi_shoot_a.sp[9][4]=gfx_orange;
	spr_nazi_shoot_a.sp[9][5]=gfx_black;
	spr_nazi_shoot_a.sp[9][6]=gfx_orange;
	spr_nazi_shoot_a.sp[9][7]=gfx_pink;
	spr_nazi_shoot_a.sp[9][8]=gfx_pink;
	spr_nazi_shoot_a.sp[10][0]=gfx_pink;
	spr_nazi_shoot_a.sp[10][1]=gfx_pink;
	spr_nazi_shoot_a.sp[10][2]=gfx_black;
	spr_nazi_shoot_a.sp[10][3]=gfx_black;
	spr_nazi_shoot_a.sp[10][4]=gfx_black;
	spr_nazi_shoot_a.sp[10][5]=gfx_black;
	spr_nazi_shoot_a.sp[10][6]=gfx_black;
	spr_nazi_shoot_a.sp[10][7]=gfx_pink;
	spr_nazi_shoot_a.sp[10][8]=gfx_pink;
	spr_nazi_shoot_a.sp[11][0]=gfx_pink;
	spr_nazi_shoot_a.sp[11][1]=gfx_pink;
	spr_nazi_shoot_a.sp[11][2]=gfx_orange;
	spr_nazi_shoot_a.sp[11][3]=gfx_orange;
	spr_nazi_shoot_a.sp[11][4]=gfx_orange;
	spr_nazi_shoot_a.sp[11][5]=gfx_orange;
	spr_nazi_shoot_a.sp[11][6]=gfx_orange;
	spr_nazi_shoot_a.sp[11][7]=gfx_pink;
	spr_nazi_shoot_a.sp[11][8]=gfx_pink;
	spr_nazi_shoot_a.sp[12][0]=gfx_pink;
	spr_nazi_shoot_a.sp[12][1]=gfx_pink;
	spr_nazi_shoot_a.sp[12][2]=gfx_orange;
	spr_nazi_shoot_a.sp[12][3]=gfx_orange;
	spr_nazi_shoot_a.sp[12][4]=gfx_pink;
	spr_nazi_shoot_a.sp[12][5]=gfx_orange;
	spr_nazi_shoot_a.sp[12][6]=gfx_orange;
	spr_nazi_shoot_a.sp[12][7]=gfx_pink;
	spr_nazi_shoot_a.sp[12][8]=gfx_pink;
	spr_nazi_shoot_a.sp[13][0]=gfx_pink;
	spr_nazi_shoot_a.sp[13][1]=gfx_pink;
	spr_nazi_shoot_a.sp[13][2]=gfx_blue;
	spr_nazi_shoot_a.sp[13][3]=gfx_blue;
	spr_nazi_shoot_a.sp[13][4]=gfx_pink;
	spr_nazi_shoot_a.sp[13][5]=gfx_blue;
	spr_nazi_shoot_a.sp[13][6]=gfx_blue;
	spr_nazi_shoot_a.sp[13][7]=gfx_pink;
	spr_nazi_shoot_a.sp[13][8]=gfx_pink;
	for(isc2=0;isc2<=13;isc2++){
		if(isc2<14){
			for(isc1=0;isc1<=8;isc1++){
				if(isc1<=8){
					if(isc2>=9){
						spr_nazi_die_a.sp[isc2][isc1]=gfx_red;
					}else{
							spr_nazi_die_a.sp[isc2][isc1]=gfx_pink;
					}
				}
			}
		}
	}
	spr_nazi_die_a.sp[4][4]=gfx_blue;
	spr_nazi_die_a.sp[4][5]=gfx_blue;
	spr_nazi_die_a.sp[4][6]=gfx_blue;
	spr_nazi_die_a.sp[5][3]=gfx_blue;
	spr_nazi_die_a.sp[5][4]=gfx_blue;
	spr_nazi_die_a.sp[5][5]=gfx_yellow;
	spr_nazi_die_a.sp[5][6]=gfx_blue;
	spr_nazi_die_a.sp[6][3]=gfx_blue;
	spr_nazi_die_a.sp[6][5]=gfx_blue;
	spr_nazi_die_a.sp[6][4]=gfx_yellow;
	spr_nazi_die_a.sp[6][6]=gfx_blue;
		spr_nazi_die_a.sp[7][3]=gfx_blue;
	spr_nazi_die_a.sp[7][4]=gfx_blue;
	spr_nazi_die_a.sp[7][5]=gfx_yellow;
	spr_nazi_die_a.sp[7][6]=gfx_blue;
	spr_nazi_die_a.sp[8][3]=gfx_blue;
	spr_nazi_die_a.sp[8][5]=gfx_blue;
	spr_nazi_die_a.sp[8][4]=gfx_yellow;
	spr_nazi_die_a.sp[8][6]=gfx_blue;
		spr_nazi_die_a.sp[9][3]=gfx_blue;
	spr_nazi_die_a.sp[9][4]=gfx_blue;
	spr_nazi_die_a.sp[9][5]=gfx_yellow;
	spr_nazi_die_a.sp[9][6]=gfx_blue;
	spr_nazi_die_a.sp[10][3]=gfx_blue;
	spr_nazi_die_a.sp[10][5]=gfx_blue;
	spr_nazi_die_a.sp[10][4]=gfx_blue;
	spr_nazi_die_a.sp[10][6]=gfx_blue;
}

//spriteArry[1].sp=spr_nazi_shoot_a;
int block_counter=0;
bool cantmovef = false;
bool flipped_sprites_clock=false;
bool shoot=false;
bool stab=false;
int weapon=0;
void draw(){

	
	float dist;
	float ang;
	int xscan;
	float tmpang;
	float xt;
	float yt;
	int brush_ind;
	uint8_t tmpdata_color_brush;
	uint8_t tmpdata_color_brush2;
	float y_mina;
	float y_maxa;
	float y_heia;
	int ymiia;
	int ymaia;
	int i2;
	int yheia;
	float tmp_calc_misc;
	bool hit;
	int y_s;
	int x_s;
	int sprite_ind;
	bool spr_found=false;
	bool lost=false;
	int temp_scan_x;
	float tang;
	float dist2;
	bool not_lost;
	int midx;
	float tmp_spr_w;
	int xps;
	float dist3;
	int yps;
	uint8_t cosprch;
	Point2D testpos2;
	SpriteDt tmpsprite;
	Point2D testpos;
	res_spr_x();
	
	gfx_FillScreen(gfx_white);
	cantmovef = false;
		randv2++;
	randv+=2;
	if(randv>randv2){
		randv=-5;
		
	}
	if(randv2>randv+10){
		randv2=4;
	}
	for(xscan=0; xscan<320; xscan+=40){
		dist = 0.5;
		hit=false;
		while(dist <max_dist&&hit==false){
			tmpang=plyr.ang;
			
				ang=((float)(xscan)-160)/160*3.1415/4+tmpang;
				
			xt=plyr.pos.x;
			yt=plyr.pos.y;
			testpos.x = xt+sin(ang)*dist;
			testpos.y = yt+cos(ang)*dist;
			
			for(brush_ind = 0; brush_ind < num_brushes; brush_ind += 1){
				
				if(testpos.x<brushArry[brush_ind].pointUpRt.x && testpos.x > brushArry[brush_ind].pointLoLt.x&&testpos.y<brushArry[brush_ind].pointUpRt.y&&testpos.y>brushArry[brush_ind].pointLoLt.y){
					if(dist==1&&(xscan<240 && xscan>80)){
						cantmovef = true;
					}
					hit=true;
					tmpdata_color_brush = brushArry[brush_ind].wc1;
					tmpdata_color_brush2 = brushArry[brush_ind].wc2;
					y_maxa=(float)(60)/dist+120;
					y_mina=(float)(-60)/dist+120;
					if(y_mina > y_maxa){
						tmp_calc_misc=y_maxa;
						y_maxa=y_mina;
						y_mina = y_maxa;
					}

							if(block_counter > 5){
								block_counter=0;
								gfx_SetColor(tmpdata_color_brush2);
							}
							else{
								gfx_SetColor(tmpdata_color_brush);
							}
					y_heia=y_maxa-y_mina;
					ymaia=(int)(y_mina);
					yheia=(int)(y_heia);
					for(y_s = ymaia; y_s < yheia+ymaia; y_s+=yheia/7){
						gfx_FillRectangle(xscan, y_s, 40, yheia/7);
							block_counter+=1;
							if(block_counter >5){
								block_counter=0;
								gfx_SetColor(tmpdata_color_brush2);
							}
							else{
								gfx_SetColor(tmpdata_color_brush);
							}
						
					}
					
				}		
			}
			dist+=0.5;
		}
	}
	xscan=0;
	while(xscan<320){
		dist = 0.5;
		hit=false;
		while(dist <max_dist&&hit==false){
			ang=getangscan(xscan);	
			xt=plyr.pos.x;
			yt=plyr.pos.y;
			testpos.x = xt+sin(ang)*dist;
			testpos.y = yt+cos(ang)*dist;
			
			for(brush_ind = 0; brush_ind < num_brushes; brush_ind += 1){
				if(testpos.x<brushArry[brush_ind].pointUpRt.x && testpos.x > brushArry[brush_ind].pointLoLt.x&&testpos.y<brushArry[brush_ind].pointUpRt.y&&testpos.y>brushArry[brush_ind].pointLoLt.y){
					hit=true;
				}
			}
			dist+=0.5;	
		}
		dist3=dist;
		dist=0;
		while(dist<dist3){
			dist+=0.5;
			ang=getangscan(xscan);	
			xt=plyr.pos.x;
			yt=plyr.pos.y;
			testpos.x = xt+sin(ang)*dist;
			testpos.y = yt+cos(ang)*dist;
			sprite_ind=0;
			spr_found=false;
			while(sprite_ind<num_sprites&&spr_found==false){
				bool is_nazi=false;
				for(i2  = 0; i2<num_nazis; i2++){
					
					if(naziArray[i2].sp_id==sprite_ind){
						is_nazi=true;
						if(xscan>80&&xscan<240){
							/*DAMAGE CODE*/
							if(stab==true&& dist<=1){
								naziArray[i2].hp-=16;
								
							}
							
							
							if(shoot==true &&ammo>0){
								naziArray[i2].hp-=45;
								

							}
							
					}
					}
				}
				if((testpos.x-spriteArry[sprite_ind].pos.x)*(testpos.x-spriteArry[sprite_ind].pos.x)+(testpos.y-spriteArry[sprite_ind].pos.y)*(testpos.y-spriteArry[sprite_ind].pos.y)<0.25&&spriteArry[sprite_ind].ena==true&&spriteArry[sprite_ind].rendered==false){
					tmpsprite=spriteArry[sprite_ind];
					if(tmpsprite.min_x==0||tmpsprite.min_x>xscan){
						tmpsprite.min_x=xscan;
						temp_scan_x=xscan;
						lost=false;
						not_lost=true;
						while(temp_scan_x<320&&lost==false){
							temp_scan_x+=40;
							tang=getangscan(temp_scan_x);	
							dist2=0;
							not_lost=true;
							while(dist2<dist&&lost==false){
								dist2+=0.5;
								testpos2.x = xt+sin(tang)*dist2;
								testpos2.y = yt+cos(tang)*dist2;
								if((testpos2.x-spriteArry[sprite_ind].pos.x)*(testpos2.x-spriteArry[sprite_ind].pos.x)+(testpos2.y-spriteArry[sprite_ind].pos.y)*(testpos2.y-spriteArry[sprite_ind].pos.y)<0.25 &&lost==false){
									lost=false;	
									not_lost=true;
								}else{
									not_lost=false;
								}
								//end testing brush
							}
							if(not_lost==false){
								lost=true;
							}
														
						}
						if(tmpsprite.max_x==0 ||tmpsprite.max_x<temp_scan_x){
							tmpsprite.max_x=temp_scan_x;
						}
						y_maxa=(float)(60)/dist+120;
						y_mina=(float)(-120)*tmpsprite.spheight/dist+(float)(60)/dist+120;
						tmp_spr_w=((float)(tmpsprite.max_x)-(float)(tmpsprite.min_x))/2;
						tmp_spr_w*=tmpsprite.spwidth;
						midx =(tmpsprite.min_x+tmpsprite.max_x)/2;
						tmpsprite.min_x=midx-(int)(tmp_spr_w);
						tmpsprite.max_x=midx+(int)(tmp_spr_w);
					if(y_mina > y_maxa){
						tmp_calc_misc=y_maxa;
						y_maxa=y_mina;
						y_mina = y_maxa;
					}
						gfx_SetColor(gfx_white);
					y_heia=y_maxa-y_mina;
					ymaia=(int)(y_mina);
						yheia=(int)(y_heia);
						xps = 0;
						
						for(x_s = tmpsprite.min_x; x_s<tmpsprite.max_x; x_s+=(((int)(tmp_spr_w)*2)/9)){
							
						yps=0;	
						//min and max x are render positions
							for(y_s = ymaia; y_s<ymaia+yheia; y_s+=yheia/14){
								if(xps<9&&yps<14){
									if(tmpsprite.fliptick==true){
										if(flipped_sprites_clock==true){
											cosprch=tmpsprite.sp.sp[yps][8-xps];
										}else{
										cosprch=tmpsprite.sp.sp[yps][xps];
									}
									}else{
										cosprch=tmpsprite.sp.sp[yps][xps];
									}
									
									if(cosprch!=gfx_pink){
										gfx_SetColor(cosprch);	
										gfx_FillRectangle(x_s, y_s, (((int)(tmp_spr_w)*2)/9), yheia/14);
									}
								}
								
								yps++;
						}
						xps++;
						}
						spriteArry[sprite_ind].rendered=true;
					}
				}
				sprite_ind+=1;
			}
			
		}
		 xscan+=40;
	}
	
	
	if(weapon==1){
		gfx_SetColor(gfx_black);
	gfx_FillRectangle(160-20,160,40,20);
		gfx_FillRectangle(160-30,180,60,20);
	}
	if(weapon==0){
			gfx_SetColor(gfx_black);
	gfx_FillRectangle(240,160,10,20);
		gfx_FillRectangle(240,180,20,20);
	}
	
		gfx_SetColor(gfx_blue);	
	
	gfx_FillRectangle(0,200,320,40);
	gfx_SetTextBGColor(gfx_white);
	gfx_SetTextFGColor(gfx_black);
	 gfx_SetTextScale(2,2);
	gfx_SetTextXY(20,220);
	gfx_PrintInt(ammo,2);
	gfx_SetTextXY(120,220);
	gfx_PrintInt(health,3);
	gfx_SetTextXY(180,220);
	gfx_PrintInt(lives,3);
	
	
	
}
//COPY BELOW FOR SIZE OF BRUSH ARRAY
//sizeof(brushArry)/sizeof(Brush2D)

uint8_t colorOne = 0;
void main() {
	bool in_menu=true;

	
	float ang = 0;
	bool run=true;
	int nic = 0;
	
	lives=3;
	
	delay(10);
	plyr.pos.x=0;
	plyr.pos.y=0;
	plyr.ang=0;
	
	// Main program code goes here
	// Init graphics
	gfx_Begin();
	while(run==true){
		gfx_SetDrawBuffer();
		gfx_SetColor(gfx_red);
		gfx_FillRectangle(0,0,320,240);
		gfx_SetTextFGColor(gfx_white);
		gfx_SetTextBGColor(gfx_black);
	 	gfx_SetTextScale(2,2);
		gfx_SetTextXY(20,20);
		gfx_PrintString("Wolfenstein CE");
		
		gfx_SetTextXY(20,120);
		gfx_SetTextScale(1,1);
	gfx_PrintString("2nd=Begin,Clr=Quit");
		gfx_BlitBuffer();
	in_menu=true;
	while(in_menu==true){
		/* menu graphics here */
		if((kb_Data[1]&kb_2nd)){
			in_menu=false;
			run=true;
		}
		if((kb_Data[6]&kb_Clear)){
			in_menu=false;
			run=false;
		}
	}
		lives=3;
		
		delay(75);
		kb_Scan();
	while(run==true){
			while(lives>0 &&run==true){
	d_init();
	kb_Scan();

	ang = 0;

	nic = 0;
	delay(10);
	plyr.pos.x=0;
	plyr.pos.y=0;
	plyr.ang=0;
	while(run==true&&health>0){
		float sc_x_bak=0;
		bool hit_back=false;
		int brush_ind=0;
		kb_Scan();
		if(kb_Data[6]&kb_Clear){
			run=false;
		}
	gfx_SetDrawBuffer();
		//draw
		flipped_sprites_clock=!flipped_sprites_clock;
	draw();
	//   
		gfx_BlitBuffer();
		if((kb_Data[7]&kb_Right)){
			plyr.ang+=3.14/16;
		}
		if((kb_Data[7]&kb_Left)){
			plyr.ang-=3.14/16;
		}
		if((kb_Data[7]&kb_Up)&&cantmovef==false){
			plyr.pos.x += sin(plyr.ang)/2;
			plyr.pos.y += cos(plyr.ang)/2;
			
		}
		for(brush_ind = 0; brush_ind < num_brushes; brush_ind += 1){
			for(sc_x_bak=-3.14-3.14/8; sc_x_bak<-3.14+3.14/8; sc_x_bak+=3.14/32){
			
				if(plyr.pos.x+sin(plyr.ang+sc_x_bak)<brushArry[brush_ind].pointUpRt.x && plyr.pos.x+sin(plyr.ang+sc_x_bak) > brushArry[brush_ind].pointLoLt.x&& plyr.pos.y+cos(plyr.ang+sc_x_bak)<brushArry[brush_ind].pointUpRt.y&& plyr.pos.y+cos(plyr.ang+sc_x_bak)>brushArry[brush_ind].pointLoLt.y){
					hit_back=true;
				}
			}
		}
		if((kb_Data[7]&kb_Down)&&hit_back==false){
			plyr.pos.x -= sin(plyr.ang)/2;
			plyr.pos.y -= cos(plyr.ang)/2;
			
		}
			if((kb_Data[3]&kb_1)){
			weapon=0;
			
			
			}
		if((kb_Data[4]&kb_2)){
			weapon=1;
			
			
		}
		if(stab==true||shoot==true){
			
			if(shoot==true){
				if(ammo>0){
					ammo--;
				}
				shoot=false;
			}
			stab=false;
		}
		if(weapon==0&&(kb_Data[1]&kb_2nd)){
			stab=true;
		}
		if(weapon==1&&(kb_Data[1]&kb_2nd)){
			shoot=true;
		}
		for(nic=0; nic<num_nazis;nic++){
			handleNazi(0);	
		}
	}
				lives--;
			}
	}
	}

	//End graphics 
	gfx_End();
	//end
	exit(0);
}

/* Other functions */
