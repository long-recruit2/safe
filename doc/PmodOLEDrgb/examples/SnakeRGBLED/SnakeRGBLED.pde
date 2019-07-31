/************************************************************************/
/*									*/
/*  SnakeRGBLED demo -- OLEDrgb Display demo                                     */
/*									*/
/************************************************************************/
/*  Author: 	Thomas Kappenman						*/
/*  Copyright 2015, Digilent Inc.					*/
/************************************************************************/

/************************************************************************/
/*  Revision History:							*/
/*									*/
/*  8/12/2015(TommyK): created						*/
/*									*/
/************************************************************************/

/************************************************************************/
/*  Board Support:							*/
/*									*/

/*  Cerebot Mx7ck:                    Header JD                         */
/************************************************************************/
//Configured for MX7ck (Chipkit Pro MX7)
//PmodOLEDrgb on JD
//PmodBTN on JF 1-6

#include <DSPI.h>
#include <OLEDrgb.h>

typedef struct snode{
  uint8_t en;
  uint8_t count;
  uint16_t * color;
  uint8_t val;
  struct snode* tailptr;
}SNODE;

#include "snake.h"
snode grid[48][32];

Oledrgb OLEDrgb;

#define UP 0   
#define LEFT 1 
#define DOWN 2 
#define RIGHT 3

// PMODBTN DEFINITIONS FOR MX7cK
#define UPPIN 43
#define DOWNPIN 40
#define LEFTPIN 42
#define RIGHTPIN 41

#define RESET_ST 0
#define TITLE_ST 1
#define RUN_GAME_ST 2
#define GAMEOVER_ST 3
#define PLUGIN 4

uint16_t head[2]={6,1};
uint16_t food[2];
uint16_t dir=RIGHT;
uint16_t lastdir=RIGHT;
uint16_t length=6;
int lastupdate;
uint16_t lastbuf[2];
static int i,j;
uint16_t difficulty=500;
uint16_t snakehue=500;
uint16_t backhue=700;
uint16_t foodhue=214;

int backupdate=0;

const uint8_t snakebrightness=255;
const uint8_t backbrightness=150;

const uint8_t refreshrate=15;

snode* snake_ptr;

uint8_t changed_flag = 0;
uint8_t update_flag = 0;
uint8_t game_state = RESET_ST;

void setup() {
  reset_grid();
  OLEDrgb.begin();
  pinMode(UPPIN, INPUT);
  pinMode(DOWNPIN, INPUT);
  pinMode(LEFTPIN, INPUT);
  pinMode(RIGHTPIN, INPUT);
  
}

void loop(){
  switch (game_state) {
      //Reset state
	  //Resets the playing grid and draws the first snake splash screen
      case RESET_ST:
        game_state = TITLE_ST;
          reset_grid();
          store_splash((uint8_t*)snake1);
          lastupdate=millis();
      break;
    //Title state
	//Draws the 5 bitmap arrays stored in "snake.h" in intervals
	case TITLE_ST:
      if(millis()-lastupdate>2000){
        if(millis()-lastupdate<2100)store_splash((uint8_t*)snake2);
        else if(millis()-lastupdate<2200)store_splash((uint8_t*)snake3);
        else if(millis()-lastupdate<2300)store_splash((uint8_t*)snake4);
        else if(millis()-lastupdate<2400)store_splash((uint8_t*)snake5);
        else if(millis()-lastupdate<2500)store_splash((uint8_t*)snake4);
        else if(millis()-lastupdate<2600)store_splash((uint8_t*)snake3);
        else if(millis()-lastupdate<2700)store_splash((uint8_t*)snake2);
        else if(millis()-lastupdate<2800){store_splash((uint8_t*)snake1);lastupdate=millis();}
      }
	//If a button is pressed
      if(digitalRead(UPPIN)||digitalRead(DOWNPIN)||digitalRead(LEFTPIN)||digitalRead(UPPIN)){
		//If all of the buttons are high, it is likely that the pmodBTN is unplugged, so move to that state
        if ((digitalRead(DOWNPIN)&&digitalRead(RIGHTPIN)&&digitalRead(LEFTPIN)&&digitalRead(UPPIN))){
          store_splash((uint8_t *)plugin);
          game_state= PLUGIN;
        }
		//Start the game
        else{
        lastupdate=millis();
          backupdate=millis();
          game_state = RUN_GAME_ST;
          lastdir=RIGHT;
        }
      }
     break;
	 //Plugin state
	 //Displays the "Plug PmodBTN into connector" screen
     case PLUGIN:
       if (!(digitalRead(DOWNPIN)||digitalRead(RIGHTPIN)||digitalRead(LEFTPIN)||digitalRead(UPPIN)))
         game_state = RESET_ST;
     break;
	 //Run game state
	 //Runs the game
     case RUN_GAME_ST:
       run_game();
     break;
	 //Game over state
	 //Displays flashing death animation
     case GAMEOVER_ST:
       death();
       reset_grid();
       game_state = RESET_ST;
     break;
  }
}

//getHSVcolor
//
//Returns the RGB value of a certain Hue, Saturation, and Value

uint16_t getHSVcolor(uint16_t hue, uint8_t sat, uint8_t val){
   uint8_t region, remain, p, q, t;
   uint8_t R,G,B;
   region = hue/256;
   remain = hue - (region * 256);
   p = (val * (255-sat))>>8;
   q = (val * (255 - ((sat * remain)>>8)))>>8;
   t = (val * (255 - ((sat * (255 - remain))>>8)))>>8;
   
   switch(region){
      case 0:
       R = val;
       G = t;
       B = p;
       break; 
      case 1:
       R = q;
       G = val;
       B = p;
       break;
      case 2:
       R = p;
       G = val;
       B = t;
       break;
       case 3:
       R = p;
       G = q;
       B = val;
       break;
       case 4:
       R = t;
       G = p;
       B = val;
       break;
       default:
       R = val;
       G = p;
       B = q;
       break;
   }
   return OLEDrgb.BuildRGB(R,G,B);
}

//run_game
//
//Checks for button presses and checks to see if it is time to update the screen or background

void run_game(){
  //Buttons connected to 40 (yellow UP) - 42 (blue LEFT) - 43 (green DOWN) - 41 (brown RIGHT)
  //check up
  if(digitalRead(UPPIN)){
	//If the PmodBTN was removed
    if (digitalRead(RIGHTPIN)&&digitalRead(LEFTPIN)&&digitalRead(UPPIN)&&digitalRead(DOWNPIN)){
      reset_grid();
      store_splash((uint8_t *)plugin);
      game_state=PLUGIN;
      return;
    }
    if(lastdir!=UP)
      dir=DOWN;
  }
  
  //check left
  else if(digitalRead(LEFTPIN)){
    if(lastdir!=RIGHT)
      dir=LEFT;
  }
  
  //check down
  else if(digitalRead(DOWNPIN)){
    if(lastdir!=DOWN)
      dir=UP;
  }

  //check right
  else if(digitalRead(RIGHTPIN)){
    if(lastdir!=LEFT)
      dir=RIGHT;
  }
  
  if (millis()-lastupdate>=difficulty){//Update every [difficulty] ms
    lastupdate=millis();
    
    lastdir=dir;
    snakehue+=5;
    if (snakehue>=1535){
      snakehue=0;
    }

    //background has changed so set flag
    update_flag=1;
    changed_flag = 1;
  }

  //update background color at a certain rate
  if (millis()-backupdate>=refreshrate){
    backupdate=millis();
    backhue+=2;
    if (backhue>=1535){
      backhue=0;
    }

    //background has changed so set flag
    changed_flag = 1;
  }
  
  //only update the array and refresh LEDs if the state has changed
  if(changed_flag == 1){
    if (update_flag){update();update_flag=0;}
    store_array();
    changed_flag = 0;
  }
}

//update
//
//Updates the game to the next frame

void update(){
  lastbuf= {head[0], head[1]};
  switch (dir){
    case UP:
      //if (head[1]==0){reset_grid(); return;}  //if the head runs into the ceiling, you die
      if (head[1]==0){ game_state = GAMEOVER_ST; return;}  //if the head runs into the ceiling, you die
      else head[1]--;
      break;
    case DOWN:
      if (head[1]==31){ game_state = GAMEOVER_ST; return;}  //if the head runs into the floor, you die
      else head[1]++;
      break;
    case RIGHT:
      if(head[0]==47){ game_state = GAMEOVER_ST; return;}  //if the head runs into the right wall, you die
      else head[0]++;
      break;
    case LEFT:
      if(head[0]==0){ game_state = GAMEOVER_ST; return;}  //if the head runs into the left wall, you die
      else head[0]--;
      break;
  }
    snake_ptr=&grid[head[0]][head[1]];
    if (snake_ptr->en==1){game_state = GAMEOVER_ST; return;}//Ran into self!
    if (snake_ptr->en==2){//Food!
      length+=2;
      if (difficulty>3)difficulty-=3;
      spawn_food();
    }
    snake_ptr->en=1;
    snake_ptr->count=1;
    snake_ptr->color= &snakehue;
    snake_ptr->val = snakebrightness;
    snake_ptr->tailptr= &grid[lastbuf[0]][lastbuf[1]];
    while(snake_ptr->tailptr){
      if (snake_ptr->tailptr->count<length){//Next segment count is less than length, it lives
        snake_ptr->tailptr->count++;//Decrement tailptr count
      }
      else{//Next segment count is length so it dies
        snake_ptr->tailptr->en=0;
        snake_ptr->tailptr->count=0;
        snake_ptr->tailptr->color= &backhue;
        snake_ptr->tailptr->val = backbrightness;
        snake_ptr->tailptr=NULL;//Remove tailptr of current segment
        break;
      }
      snake_ptr=snake_ptr->tailptr;
    }
}

//store_splash
//
//Draws the bitmap across the entire screen

void store_splash(uint8_t* bmp){
  OLEDrgb.DrawBitmap(0,0,95,63, bmp);
}

//store_array
//
//Scales the 48x32 array to fill the screen

void store_array(){
  uint16_t fullcolor;
  uint8_t temp[64][96][2];
  for (j=0; j<32; j++){
    for(i=0;i<48;i++){
      fullcolor = getHSVcolor((*grid[i][j].color+ grid[i][j].count*6)%1535, 255, grid[i][j].val);
      //Top left pixel
	  temp[2*j][2*i][0]=fullcolor>>8;
      temp[2*j][2*i][1]=fullcolor;
	  //Top Right pixel
      temp[2*j][2*i+1][0]=fullcolor>>8;
      temp[2*j][2*i+1][1]=fullcolor;
	  //Bottom Left pixel
      temp[2*j+1][2*i][0]=fullcolor>>8;
      temp[2*j+1][2*i][1]=fullcolor;
	  //Bottom right pixel
      temp[2*j+1][2*i+1][0]=fullcolor>>8;
      temp[2*j+1][2*i+1][1]=fullcolor;
    }
  }
   OLEDrgb.DrawBitmap(0, 0,95,63, **temp);
}

//store_blank
//
//Clears the screen

void store_blank(){
  OLEDrgb.DrawRectangle(0, 0, 95, 63, 0, 1, 0);
}

//death
//
//Flashes the screen on and off at decreasing intervals

void death(){
  store_blank();
  delay(50);
  store_array();
  delay(250);
  store_blank();
  delay(50);
  store_array();
  delay(200);
  store_blank();
  delay(50);
  store_array();
  delay(150);
  store_blank();
  delay(50);
  store_array();
  delay(100);
  store_blank();
  delay(50);
  store_array();
  delay(50);
  store_blank();
  delay(50);
  store_array();
  delay(30);
  store_blank();
  delay(30);
  store_array();
  delay(20);
  store_blank();
  delay(500);
}

//spawn_food
//
//Changes one of the items in the game grid into food

void spawn_food(){
  food={random(47),random(31)};
  while (grid[food[0]][food[1]].en!=0){
    food={random(47),random(31)};
  }
  grid[food[0]][food[1]].en=2;
  grid[food[0]][food[1]].count=0;
  grid[food[0]][food[1]].color = &foodhue;
  grid[food[0]][food[1]].val = snakebrightness;
}

//reset_grid
//
//Clears the grid and places the snake in the top left

void reset_grid(){
  for (i=0;i<48; i++){
    for(j=0; j<32; j++){
      grid[i][j].en=0;
      grid[i][j].count=0;
      grid[i][j].color = &backhue;
      grid[i][j].val = backbrightness;
      grid[i][j].tailptr=NULL;
    }
  }

  dir= RIGHT;
  head={6,1};
  length=6;
  for (i=6; i>=1; i--){
    snake_ptr=&grid[i][1];
    snake_ptr->en=1;
    snake_ptr->color = &snakehue;
    snake_ptr->count=7-i;
    snake_ptr->tailptr=&grid[i-1][1];
    snake_ptr->val=snakebrightness;
    snake_ptr=snake_ptr->tailptr;
  }  
  spawn_food();
  difficulty=100;
}
