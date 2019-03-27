
SDL_DisplayMode displayMode;
SDL_Renderer *ren;
SDL_GLContext glContext;
SDL_Window *win;
int winWidth,winHeight;
GLUquadric *quad;

//Initialise SDL/GL
void initSDL()
{
	winWidth=640;winHeight=480;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init Error: %s\n",SDL_GetError());
		exit(-1);
	}
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
	win = SDL_CreateWindow("demo1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	if (win == NULL){
		printf("SDL_CreateWindow Error: %s\n",SDL_GetError());
		SDL_Quit();
		exit(-1);
	}
	
	glContext=SDL_GL_CreateContext(win);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
		
	glewExperimental = GL_TRUE; 
	GLenum glewError = glewInit();
	
	SDL_GetDesktopDisplayMode(0,&displayMode);
	
	quad=gluNewQuadric();
}

inline void renderSphere(float x,float y,float z,float r)
{
	glTranslatef(x,y,z);
	gluSphere(quad,r,32,32);
	glTranslatef(-x,-y,-z);
}

bool quadraticSolver(float a,float b,float c,float *posSol,float *negSol)
{
	float det=b*b-4*a*c;
	if(det<0) return false;
	*posSol=(-b+sqrt(det))/2/a;
	*negSol=(-b-sqrt(det))/2/a;
	return true;
}

inline float max(float a,float b)
{
	return a>b?a:b;
}
inline int max(int a,int b)
{
	return a>b?a:b;
}
inline float min(float a,float b)
{
	return a<b?a:b;
}
inline int min(int a,int b)
{
	return a<b?a:b;
}
