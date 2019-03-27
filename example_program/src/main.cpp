#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include <SDL.h>
//include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
//#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "header.cpp"
#include "../../simple_kinematics.h"
#include "hand_model.h"

HandModel rhand;
float state[26];

Vec3f camPos;
float azi,alt;

bool movingForward=false,movingBackward=false,movingLeft=false,movingRight=false,movingUp=false,movingDown=false,
	turningLeft=false,turningRight=false,turningUp=false,turningDown=false,showingModel=true,running=true;

void updateCamera();
void handleEvents();
void render();

int main(int argc,char** args)
{
	srand(time(NULL));
	
	initSDL();
	
	camPos={0.f,0.f,0.f};
	azi=alt=0.f;
	
	rhand.makeRight();
	
	rhand.getStateFrustrum(state);
	state[0]=0.f;
	state[1]=.25f;
	state[2]=-5.f;
	state[3]=-M_PI/2;
	state[4]=0.f;
	state[5]=0.f;
	
	unsigned int timeInFrames=0;
	while(running)
	{
		handleEvents();
		updateCamera();
		
		state[6]=rhand.minState[6]+(rhand.maxState[6]-rhand.minState[6])*(1.f+sin(2*M_PI*(timeInFrames%256)/256.f))/2;
		state[8]=rhand.minState[8]+(rhand.maxState[8]-rhand.minState[8])*(1.f+sin(2*M_PI*(timeInFrames%256)/256.f))/2;
		state[9]=rhand.minState[9]+(rhand.maxState[9]-rhand.minState[9])*(1.f+sin(2*M_PI*(timeInFrames%256)/256.f))/2;
		state[10]=rhand.minState[10]+(rhand.maxState[10]-rhand.minState[10])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.2f)))/2;
		state[12]=rhand.minState[12]+(rhand.maxState[12]-rhand.minState[12])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.2f)))/2;
		state[13]=rhand.minState[13]+(rhand.maxState[13]-rhand.minState[13])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.2f)))/2;
		state[14]=rhand.minState[14]+(rhand.maxState[14]-rhand.minState[14])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.4f)))/2;
		state[16]=rhand.minState[16]+(rhand.maxState[16]-rhand.minState[16])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.4f)))/2;
		state[17]=rhand.minState[17]+(rhand.maxState[17]-rhand.minState[17])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.4f)))/2;
		state[18]=rhand.minState[18]+(rhand.maxState[18]-rhand.minState[18])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.6f)))/2;
		state[20]=rhand.minState[20]+(rhand.maxState[20]-rhand.minState[20])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.6f)))/2;
		state[21]=rhand.minState[21]+(rhand.maxState[21]-rhand.minState[21])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.6f)))/2;
		state[22]=rhand.minState[22]+(rhand.maxState[22]-rhand.minState[22])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.8f)))/2;
		state[23]=rhand.minState[23]+(rhand.maxState[23]-rhand.minState[23])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.8f)))/2;
		state[24]=rhand.minState[24]+(rhand.maxState[24]-rhand.minState[24])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.8f)))/2;
		state[25]=rhand.minState[25]+(rhand.maxState[25]-rhand.minState[25])*(1.f+sin(2*M_PI*((timeInFrames%256)/256.f+0.8f)))/2;
		rhand.changeStateFrustrum(state);
		
		render();
		
		SDL_Delay(20);
		timeInFrames++;
	}

	return 0;
}

void updateCamera()
{
	float speed=.25f,turn=2.f;
	if(movingUp) camPos.y+=speed;
	if(movingDown) camPos.y-=speed;
	if(movingLeft)
	{
		camPos.x+=speed*cos(azi*M_PI/180);
		camPos.z+=speed*sin(azi*M_PI/180);
	}
	if(movingRight)
	{
		camPos.x-=speed*cos(azi*M_PI/180);
		camPos.z-=speed*sin(azi*M_PI/180);
	}
	if(movingForward)
	{
		camPos.z+=speed*cos(azi*M_PI/180);
		camPos.x-=speed*sin(azi*M_PI/180);
	}
	if(movingBackward)
	{
		camPos.z-=speed*cos(azi*M_PI/180);
		camPos.x+=speed*sin(azi*M_PI/180);
	}
	if(turningUp) alt+=turn;
	if(turningDown) alt-=turn;
	if(turningRight) azi+=turn;
	if(turningLeft) azi-=turn;
}

void handleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if(event.type==SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) running=false;
		else if(event.type==SDL_KEYDOWN && !event.key.repeat)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
				{
					running=false;
				} break;
				
				case SDLK_UP:
				{
					movingForward=true;
				} break;
				case SDLK_DOWN:
				{
					movingBackward=true;
				} break;
				case SDLK_RIGHT:
				{
					movingRight=true;
				} break;
				case SDLK_LEFT:
				{
					movingLeft=true;
				} break;
				case 'w':
				{
					turningUp=true;
				} break;
				case 's':
				{
					turningDown=true;
				} break;
				case 'a':
				{
					turningLeft=true;
				} break;
				case 'd':
				{
					turningRight=true;
				} break;
				case 'q':
				{
					movingUp=true;
				} break;
				case 'e':
				{
					movingDown=true;
				} break;
				case 'm':
				{
					showingModel=!showingModel;
				} break;
			}
		}
		else if(event.type==SDL_KEYUP && !event.key.repeat)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_UP:
				{
					movingForward=false;
				} break;
				case SDLK_DOWN:
				{
					movingBackward=false;
				} break;
				case SDLK_RIGHT:
				{
					movingRight=false;
				} break;
				case SDLK_LEFT:
				{
					movingLeft=false;
				} break;
				case 'w':
				{
					turningUp=false;
				} break;
				case 's':
				{
					turningDown=false;
				} break;
				case 'a':
				{
					turningLeft=false;
				} break;
				case 'd':
				{
					turningRight=false;
				} break;
				case 'q':
				{
					movingUp=false;
				} break;
				case 'e':
				{
					movingDown=false;
				} break;
			}
		}
	}
}

void render()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	glViewport(0,0,winWidth,winHeight);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluPerspective(45.0f, (float)winWidth/(float)winHeight, 1.f, 1000.0f);
	
	glRotatef(alt,1.0,0.0,0.0);
	glRotatef(azi,0.0,1.0,0.0);
	glTranslatef(camPos.x,camPos.y,camPos.z);
	 
	if(showingModel) rhand.renderMesh();
	else rhand.renderKeypoints();

	SDL_GL_SwapWindow(win);
}