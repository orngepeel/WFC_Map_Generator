#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "loadobjfile.cpp"

// title of these windows:

const char *WINDOWTITLE = { "Final Project -- Wave Function Collapse -- Devon Braner" };
const char *GLUITITLE   = { "User Interface Window" };


// milliseconds per time cycle:

const int MS_PER_CYCLE = { 5000 };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { .1 };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


const int SCROLL_WHEEL_UP = 3;
const int SCROLL_WHEEL_DOWN = 4;

// equivalent mouse movement when we click the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;

// active mouse buttons (or them together):

const int LEFT = 4;
const int MIDDLE = 2;
const int RIGHT = 1;


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};

struct Tile
{
	int tile_index;
	std::vector<int> N_neighbors, E_neighbors, S_neighbors, W_neighbors;
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
bool	Freeze;
bool	Light0On;
bool	Light1On;
bool	Light2On;
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
float	Time;				// fraction (0.-1.) of the time cycle
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
float	Xpos, Ypos, Zpos;		// camera position
float	Xtar, Ytar, Ztar;		// look at
float	Xup, Yup, Zup;

float White[3] = { 1., 1., 1. };

GLuint	buildings0_0;
GLuint	buildings0_1;
GLuint	buildings0_2;
GLuint	buildings0_3;

GLuint	buildings1;

GLuint	buildings2_0;
GLuint	buildings2_1;
GLuint	buildings2_2;
GLuint	buildings2_3;

GLuint	buildings3_0;
GLuint	buildings3_1;
GLuint	buildings3_2;
GLuint	buildings3_3;

GLuint	street_0;
GLuint	street_1;

GLuint	street_corner_NE;
GLuint	street_corner_SE;
GLuint	street_corner_SW;
GLuint	street_corner_NW;

GLuint	street_intersection;

GLuint	street_end_N;
GLuint	street_end_E;
GLuint	street_end_S;
GLuint	street_end_W;

GLuint	park0;

GLuint	park1_N;
GLuint	park1_E;
GLuint	park1_S;
GLuint	park1_W;

Tile	tbuildings0_0;
Tile	tbuildings0_1;
Tile	tbuildings0_2;
Tile	tbuildings0_3;

Tile	tbuildings1;

Tile	tbuildings2_0;
Tile	tbuildings2_1;
Tile	tbuildings2_2;
Tile	tbuildings2_3;

Tile	tbuildings3_0;
Tile	tbuildings3_1;
Tile	tbuildings3_2;
Tile	tbuildings3_3;

Tile	tstreet_0;
Tile	tstreet_1;

Tile	tstreet_corner_NE;
Tile	tstreet_corner_SE;
Tile	tstreet_corner_SW;
Tile	tstreet_corner_NW;

Tile	tstreet_intersection;

Tile	tstreet_end_N;
Tile	tstreet_end_E;
Tile	tstreet_end_S;
Tile	tstreet_end_W;

Tile	tpark0;

Tile	tpark1_N;
Tile	tpark1_E;
Tile	tpark1_S;
Tile	tpark1_W;

std::vector<GLuint> tile_list;

std::vector<Tile> all_tiles;

const int MAPWIDTH = 3;
const int MAPHEIGHT = 3;

std::vector<std::vector<std::vector<Tile>>> map;

const float STEPSIZE = .1;

glm::vec3 direction;
glm::vec3 rightVec;
glm::vec3 upVec;
glm::vec3 position;

float horizontal;
float vertical;


// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );

void	SetMaterial( float, float, float,  float );
void	SetPointLight( int, float, float, float,  float, float, float );
void	SetSpotLight( int, float, float, float,  float, float, float, float, float, float );

float * Array3( float, float, float );
float * Array4( float, float, float, float );
float * BlendArray3( float, float [3], float [3] );
float * MulArray3( float, float [3] );

void	PassiveMouseMotion(int, int);

int		GetRandNum(int);
std::tuple<int, int> GetNorth(int, int);
std::tuple<int, int> GetEast(int, int);
std::tuple<int, int> GetSouth(int, int);
std::tuple<int, int> GetWest(int, int);

void	initializeMap();
void	collapseNeighbors(int, std::tuple<int, int>, std::tuple<int, int>, std::tuple<int, int>, std::tuple<int, int>);
std::tuple<int, int, int> FindFewestNeighbors();
void	populateMap();

// main program:

int
main( int argc, char *argv[ ] )
{
	std::srand(std::time(0));
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );


	// create the display structures that will not change:

	InitLists( );


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
	int ms = glutGet( GLUT_ELAPSED_TIME );
	ms %= MS_PER_CYCLE;
	Time = (float)ms / (float)( MS_PER_CYCLE - 1 );
	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}

	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );

	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	// specify shading:

	glShadeModel( GL_SMOOTH );

	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );

	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.01, 10000. );
	else
		gluPerspective( 90., 1.,	0.01, 10000. );

	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// set the eye position, look-at position, and up-vector:

	gluLookAt( Xpos, Ypos, Zpos,    Xtar,  Ytar, Ztar,     Xup, Yup, Zup );

	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );

	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	glDisable( GL_FOG );

	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3f( 1., 1., 1. );
		glCallList( AxesList );
	}

	// since we are using glScalef( ), be sure normals get unitized:

	glEnable( GL_NORMALIZE );

	// draw the objects:

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, MulArray3( .3f, White ) );
	glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	glEnable( GL_LIGHTING );

#define RAD 7.f
	float theta = (float)(2. * M_PI) * Time;
	glPushMatrix( );
		SetPointLight( GL_LIGHT0,  MAPWIDTH / 2, 100., MAPHEIGHT / 2,  1., 1., 1. );
		SetPointLight(GL_LIGHT1, MAPWIDTH * .75, 100., MAPHEIGHT * .25, 1., 1., 1.);
		SetPointLight(GL_LIGHT2, MAPWIDTH * .25, 100., MAPHEIGHT * .75, 1., 1., 1.);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable( GL_LIGHTING );
	glPopMatrix( );


	for (int i = 0; i < MAPWIDTH; i++)
	{
		for (int j = 0; j < MAPHEIGHT; j++)
		{
			if (map[i][j].size() == 0)
			{
				glPushMatrix();
				glTranslatef(100 * i, 0, 100 * j);
				glScalef(10., 10., 10.);
				glCallList(park0);
				glPopMatrix();
			}
			else {
				glPushMatrix();
				glTranslatef(100 * i, 0, 100 * j);
				glScalef(10., 10., 10.);
				glCallList(tile_list[map[i][j][0].tile_index]);
				glPopMatrix();
			}
		}
	}


	glDisable( GL_LIGHTING );

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );
	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	//glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics()
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);

	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);

	// set the framebuffer clear values:

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(PassiveMouseMotion);
	glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(-1, NULL, 0);
	glutIdleFunc(Animate);

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInit Error\n");
	}
	else
		fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	Xpos = 20;
	Ypos = 100;
	Zpos = 20;

	Xtar = 100;
	Ytar = 80;
	Ztar = 100;

	Xup = 0;
	Yup = 1;
	Zup = 0;

	tbuildings0_0.tile_index = 0;
	tbuildings0_0.N_neighbors = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_0.E_neighbors = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_0.S_neighbors = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings0_0.W_neighbors = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings0_1.tile_index = 1;
	tbuildings0_1.N_neighbors = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_1.E_neighbors = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_1.S_neighbors = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings0_1.W_neighbors = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings0_2.tile_index = 2;
	tbuildings0_2.N_neighbors = { 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_2.E_neighbors = { 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_2.S_neighbors = { 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings0_2.W_neighbors = { 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings0_3.tile_index = 3;
	tbuildings0_3.N_neighbors = { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_3.E_neighbors = { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings0_3.S_neighbors = { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings0_3.W_neighbors = { 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings1.tile_index = 4;
	tbuildings1.N_neighbors = { 0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings1.E_neighbors = { 0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings1.S_neighbors = { 0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings1.W_neighbors = { 0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings2_0.tile_index = 5;
	tbuildings2_0.N_neighbors = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_0.E_neighbors = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_0.S_neighbors = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings2_0.W_neighbors = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings2_1.tile_index = 6;
	tbuildings2_1.N_neighbors = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_1.E_neighbors = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_1.S_neighbors = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings2_1.W_neighbors = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings2_2.tile_index = 7;
	tbuildings2_2.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_2.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_2.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings2_2.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings2_3.tile_index = 8;
	tbuildings2_3.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_3.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings2_3.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings2_3.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings3_0.tile_index = 9;
	tbuildings3_0.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_0.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_0.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings3_0.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings3_1.tile_index = 10;
	tbuildings3_1.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_1.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_1.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings3_1.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings3_2.tile_index = 11;
	tbuildings3_2.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_2.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_2.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings3_2.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tbuildings3_3.tile_index = 12;
	tbuildings3_3.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_3.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tbuildings3_3.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 16, 17, 20, 21, 23, 24, 25, 26, 27, 28 };
	tbuildings3_3.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 17, 18, 20, 21, 22, 24, 25, 26, 27, 28 };

	tstreet_0.tile_index = 13;
	tstreet_0.N_neighbors = { 13, 16, 17, 19, 20 };
	tstreet_0.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23, 24, 25, 26, 27, 28 };
	tstreet_0.S_neighbors = { 13, 15, 18, 19, 22 };
	tstreet_0.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 21, 22, 24, 25, 26, 27, 28 };

	tstreet_1.tile_index = 14;
	tstreet_1.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23, 24, 25, 26, 27, 28 };
	tstreet_1.E_neighbors = { 14, 17, 18, 19, 21 };
	tstreet_1.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 20, 21, 23, 24, 25, 26, 27, 28 };
	tstreet_1.W_neighbors = { 14, 15, 16, 19, 23 };

	tstreet_corner_NE.tile_index = 15;
	tstreet_corner_NE.N_neighbors = { 13, 17, 20 };
	tstreet_corner_NE.E_neighbors = { 14, 17, 21 };
	tstreet_corner_NE.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_corner_NE.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 18, 24, 25, 26, 27, 28 };

	tstreet_corner_SE.tile_index = 16;
	tstreet_corner_SE.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_corner_SE.E_neighbors = { 14, 18, 21 };
	tstreet_corner_SE.S_neighbors = { 13, 18, 22 };
	tstreet_corner_SE.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 18, 24, 25, 26, 27, 28 };

	tstreet_corner_SW.tile_index = 17;
	tstreet_corner_SW.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_corner_SW.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_corner_SW.S_neighbors = { 13, 15, 22 };
	tstreet_corner_SW.W_neighbors = { 14, 15, 23 };

	tstreet_corner_NW.tile_index = 18;
	tstreet_corner_NW.N_neighbors = { 13, 16, 20 };
	tstreet_corner_NW.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_corner_NW.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_corner_NW.W_neighbors = { 14, 16, 23 };

	tstreet_intersection.tile_index = 19;
	tstreet_intersection.N_neighbors = { 13 };
	tstreet_intersection.E_neighbors = { 14 };
	tstreet_intersection.S_neighbors = { 13 };
	tstreet_intersection.W_neighbors = { 14 };

	tstreet_end_N.tile_index = 20;
	tstreet_end_N.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_N.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_N.S_neighbors = { 13, 15, 18 };
	tstreet_end_N.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };

	tstreet_end_E.tile_index = 21;
	tstreet_end_E.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_E.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_E.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_E.W_neighbors = { 14, 15, 16 };

	tstreet_end_S.tile_index = 22;
	tstreet_end_S.N_neighbors = { 13, 16, 17 };
	tstreet_end_S.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_S.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_S.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };

	tstreet_end_W.tile_index = 23;
	tstreet_end_W.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_W.E_neighbors = { 14, 17, 18 };
	tstreet_end_W.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };
	tstreet_end_W.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 15, 16, 24, 25, 26, 27, 28 };

	tpark0.tile_index = 24;
	tpark0.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23 };
	tpark0.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23 };
	tpark0.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23 };
	tpark0.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22 };

	tpark1_N.tile_index = 25;
	tpark1_N.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23 };
	tpark1_N.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23 };
	tpark1_N.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23 };
	tpark1_N.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22 };

	tpark1_E.tile_index = 26;
	tpark1_E.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23 };
	tpark1_E.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23 };
	tpark1_E.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23 };
	tpark1_E.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22 };

	tpark1_S.tile_index = 27;
	tpark1_S.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23 };
	tpark1_S.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23 };
	tpark1_S.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23 };
	tpark1_S.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22 };

	tpark1_W.tile_index = 28;
	tpark1_W.N_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 18, 21, 22, 23 };
	tpark1_W.E_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 20, 22, 23 };
	tpark1_W.S_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 17, 20, 21, 23 };
	tpark1_W.W_neighbors = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 17, 18, 20, 21, 22 };

	all_tiles = { tbuildings0_0, tbuildings0_1, tbuildings0_2, tbuildings0_3, tbuildings1, tbuildings2_0, tbuildings2_1, tbuildings2_2, tbuildings2_3,
				tbuildings3_0, tbuildings3_1, tbuildings3_2, tbuildings3_3, tstreet_0, tstreet_1, tstreet_corner_NE, tstreet_corner_SE, tstreet_corner_SW,
				tstreet_corner_NW, tstreet_intersection, tstreet_end_N, tstreet_end_E, tstreet_end_S, tstreet_end_W, tpark0, tpark1_N, tpark1_E, tpark1_S, tpark1_W };

	

	populateMap();
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	buildings0_0 = glGenLists(1);
	glNewList(buildings0_0, GL_COMPILE);
		glPushMatrix();
			SetMaterial(1.f, 1.f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings0.obj");
		glPopMatrix();
	glEndList();

	buildings0_1 = glGenLists(1);
	glNewList(buildings0_1, GL_COMPILE);
		glPushMatrix();
		SetMaterial(1.f, 1.f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(90., 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings0.obj");
		glPopMatrix();
	glEndList();

	buildings0_2 = glGenLists(1);
	glNewList(buildings0_2, GL_COMPILE);
		glPushMatrix();
		SetMaterial(1.f, 1.f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(180., 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings0.obj");
		glPopMatrix();
	glEndList();

	buildings0_3 = glGenLists(1);
	glNewList(buildings0_3, GL_COMPILE);
		glPushMatrix();
		SetMaterial(1.f, 1.f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(270., 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings0.obj");
		glPopMatrix();
	glEndList();

	buildings1 = glGenLists(1);
	glNewList(buildings1, GL_COMPILE);
		glPushMatrix();
		SetMaterial(.5f, .5f, .5f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings1.obj");
		glPopMatrix();
	glEndList();

	buildings2_0 = glGenLists(1);
	glNewList(buildings2_0, GL_COMPILE);
		glPushMatrix();
		SetMaterial(1.f, .5f, .5f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings2.obj");
		glPopMatrix();
	glEndList();

	buildings2_1 = glGenLists(1);
	glNewList(buildings2_1, GL_COMPILE);
		glPushMatrix();
		SetMaterial(1.f, .5f, .5f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(90, 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings2.obj");
		glPopMatrix();
	glEndList();

	buildings2_2 = glGenLists(1);
	glNewList(buildings2_2, GL_COMPILE);
		glPushMatrix();
		SetMaterial(1.f, .5f, .5f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(180, 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings2.obj");
		glPopMatrix();
	glEndList();

	buildings2_3 = glGenLists(1);
	glNewList(buildings2_3, GL_COMPILE);
		glPushMatrix();
		SetMaterial(1.f, .5f, .5f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(270., 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings2.obj");
		glPopMatrix();
	glEndList();


	buildings3_0 = glGenLists(1);
	glNewList(buildings3_0, GL_COMPILE);
		glPushMatrix();
		SetMaterial(.5f, .5f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings3.obj");
		glPopMatrix();
	glEndList();

	buildings3_1 = glGenLists(1);
	glNewList(buildings3_1, GL_COMPILE);
		glPushMatrix();
		SetMaterial(.5f, .5f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(90, 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings3.obj");
		glPopMatrix();
	glEndList();

	buildings3_2 = glGenLists(1);
	glNewList(buildings3_2, GL_COMPILE);
		glPushMatrix();
		SetMaterial(.5f, .5f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(180, 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings3.obj");
		glPopMatrix();
	glEndList();

	buildings3_3 = glGenLists(1);
	glNewList(buildings3_3, GL_COMPILE);
		glPushMatrix();
		SetMaterial(.5f, .5f, 1.f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(270., 0., 1., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("buildings3.obj");
		glPopMatrix();
	glEndList();


	street_0 = glGenLists(1);
	glNewList(street_0, GL_COMPILE);
		glPushMatrix();
			SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("street.obj");
		glPopMatrix();
	glEndList();

	street_1 = glGenLists(1);
	glNewList(street_1, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(90, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("street.obj");
		glPopMatrix();
	glEndList();


	street_corner_NE = glGenLists(1);
	glNewList(street_corner_NE, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(270, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetCorner.obj");
		glPopMatrix();
	glEndList();

	street_corner_SE = glGenLists(1);
	glNewList(street_corner_SE, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetCorner.obj");
		glPopMatrix();
	glEndList();

	street_corner_SW = glGenLists(1);
	glNewList(street_corner_SW, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(90, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetCorner.obj");
		glPopMatrix();
	glEndList();

	street_corner_NW = glGenLists(1);
	glNewList(street_corner_NW, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(180, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetCorner.obj");
		glPopMatrix();
	glEndList();


	street_intersection = glGenLists(1);
	glNewList(street_intersection, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetIntersection.obj");
		glPopMatrix();
	glEndList();


	street_end_N = glGenLists(1);
	glNewList(street_end_N, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(90, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetEnd.obj");
		glPopMatrix();
	glEndList();

	street_end_E = glGenLists(1);
	glNewList(street_end_E, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(180, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetEnd.obj");
		glPopMatrix();
	glEndList();

	street_end_S = glGenLists(1);
	glNewList(street_end_S, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(270, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetEnd.obj");
		glPopMatrix();
	glEndList();

	street_end_W = glGenLists(1);
	glNewList(street_end_W, GL_COMPILE);
		glPushMatrix();
		SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("streetEnd.obj");
		glPopMatrix();
	glEndList();


	park0 = glGenLists(1);
	glNewList(park0, GL_COMPILE);
		glPushMatrix();
			SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("park0.obj");
		glPopMatrix();
	glEndList();


	park1_N = glGenLists(1);
	glNewList(park1_N, GL_COMPILE);
		glPushMatrix();
			SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("park1.obj");
		glPopMatrix();
	glEndList();

	park1_E = glGenLists(1);
	glNewList(park1_E, GL_COMPILE);
		glPushMatrix();
			SetMaterial(0.1f, 1.f, 0.1f, 16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(90, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("park1.obj");
		glPopMatrix();
	glEndList();

	park1_S = glGenLists(1);
	glNewList(park1_S, GL_COMPILE);
		glPushMatrix();
			SetMaterial(0.1f, 1.f, 0.1f,16.f);
			glTranslatef(0., 0., 0.);
			glRotatef(180, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("park1.obj");
		glPopMatrix();
	glEndList();

	park1_W = glGenLists(1);
	glNewList(park1_W, GL_COMPILE);
		glPushMatrix();
			SetMaterial(0.1f, 1.f, 0.1f, 16);
			glTranslatef(0., 0., 0.);
			glRotatef(270, 0, 1, 0);
			glShadeModel(GL_SMOOTH);
			LoadObjFile("park1.obj");
		glPopMatrix();
	glEndList();

	tile_list = { buildings0_0, buildings0_1, buildings0_2, buildings0_3, buildings1, buildings2_0, buildings2_1, buildings2_2, buildings2_3,
				buildings3_0, buildings3_1, buildings3_2, buildings3_3, street_0, street_1, street_corner_NE, street_corner_SE, street_corner_SW,
				street_corner_NW, street_intersection, street_end_N, street_end_E, street_end_S, street_end_W, park0, park1_N, park1_E, park1_S, park1_W };

	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );


	rightVec = { sin(horizontal = 3.14/2),
		0,
		cos(horizontal - 3.14/2) };
	
	upVec = glm::cross(rightVec, direction);

	switch( c )
	{
		case '0':
			Light0On = ! Light0On;
			break;

		case '1':
			Light1On = ! Light1On;
			break;

		case '2':
			Light2On = ! Light2On;
			break;

		case 'f':
		case 'F':
			Freeze = ! Freeze;
			if( Freeze )
				glutIdleFunc( NULL );
			else
				glutIdleFunc( Animate );
			break;

		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
			Ypos += 3;
			break;

		case 'e':
		case 'E':
			Ypos -= 3;
			break;

		case 'w':
		case 'W':
			Xpos += 3;
			break;

		case 's':
		case 'S':
			Xpos -=  3;
			break;

		case 'a':
		case 'A':
		
			Zpos -= rightVec[2] * 3;
			break;

		case 'd':
		case 'D':
			Zpos += rightVec[2] * 3;
			break;

		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

void
PassiveMouseMotion(int x, int y)
{
	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	

	if (dy <= 90 && dy >= -90)
	{
		Xrot += (ANGFACT * dy);
		Yrot += (ANGFACT * dx);

		horizontal = Xrot;
		vertical = Yrot;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthCueOn = 0;
	Freeze = false;
	Light0On = true;
	Light1On = true;
	Light2On = true;
	Time = 0.;
	Scale  = 1.0;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};

static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};

static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = (float)floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}


void
SetMaterial( float r, float g, float b,  float shininess )
{
	glMaterialfv( GL_BACK, GL_EMISSION, Array3( 0., 0., 0. ) );
	glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f, White ) );
	glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
	glMaterialfv( GL_BACK, GL_SPECULAR, Array3( 0., 0., 0. ) );
	glMaterialf ( GL_BACK, GL_SHININESS, 5.f );

	glMaterialfv( GL_FRONT, GL_EMISSION, Array3( 0., 0., 0. ) );
	glMaterialfv( GL_FRONT, GL_AMBIENT, Array3( r, g, b ) );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, Array3( r, g, b ) );
	glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .8f, White ) );
	glMaterialf ( GL_FRONT, GL_SHININESS, shininess );
}


void
SetPointLight( int ilight, float x, float y, float z,  float r, float g, float b )
{
	glLightfv( ilight, GL_POSITION,  Array3( x, y, z ) );
	glLightfv( ilight, GL_AMBIENT,   Array3( 0., 0., 0. ) );
	glLightfv( ilight, GL_DIFFUSE,   Array3( r, g, b ) );
	glLightfv( ilight, GL_SPECULAR,  Array3( r, g, b ) );
	glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
	glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
	glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}


void
SetSpotLight( int ilight, float x, float y, float z,  float xdir, float ydir, float zdir, float r, float g, float b )
{
	glLightfv( ilight, GL_POSITION,  Array3( x, y, z ) );
	glLightfv( ilight, GL_SPOT_DIRECTION,  Array3(xdir,ydir,zdir) );
	glLightf(  ilight, GL_SPOT_EXPONENT, 1. );
	glLightf(  ilight, GL_SPOT_CUTOFF, 45. );
	glLightfv( ilight, GL_AMBIENT,   Array3( 0., 0., 0. ) );
	glLightfv( ilight, GL_DIFFUSE,   Array3( r, g, b ) );
	glLightfv( ilight, GL_SPECULAR,  Array3( r, g, b ) );
	glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
	glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
	glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}

float *
Array3( float a, float b, float c )
{
        static float array[4];

        array[0] = a;
        array[1] = b;
        array[2] = c;
        array[3] = 1.;
        return array;
}

float *
Array4( float a, float b, float c, float d )
{
        static float array[4];

        array[0] = a;
        array[1] = b;
        array[2] = c;
        array[3] = d;
        return array;
}

float *
BlendArray3( float factor, float array0[3], float array1[3] )
{
        static float array[4];

        array[0] = factor * array0[0]  +  ( 1.f - factor ) * array1[0];
        array[1] = factor * array0[1]  +  ( 1.f - factor ) * array1[1];
        array[2] = factor * array0[2]  +  ( 1.f - factor ) * array1[2];
        array[3] = 1.;
        return array;
}

float *
MulArray3( float factor, float array0[3] )
{
        static float array[4];

        array[0] = factor * array0[0];
        array[1] = factor * array0[1];
        array[2] = factor * array0[2];
        array[3] = 1.;
        return array;
}

int GetRandNum(int i)
{
	int random = rand();

	return random % i;
}
		
std::tuple<int, int> GetNorth(int x, int y)
{
	if (x > 0)
		return std::make_tuple(x - 1, y);
	else
		return std::make_tuple(-1, -1);
}

std::tuple<int, int> GetEast(int x, int y)
{
	if (y < MAPHEIGHT - 1)
		return std::make_tuple(x, y + 1);
	else
		return std::make_tuple(-1, -1);
}

std::tuple<int, int> GetSouth(int x, int y)
{
	if (x < MAPWIDTH - 1)
		return std::make_tuple(x + 1, y);
	else
		return std::make_tuple(-1, -1);
}

std::tuple<int, int> GetWest(int x, int y)
{
	if (y > 0)
		return std::make_tuple(x, y - 1);
	else
		return std::make_tuple(-1, -1);
}

void	initializeMap()
{

	for (int i = 0; i < MAPWIDTH; i++)
	{
		std::vector<std::vector<Tile>> cols;
		for (int j = 0; j < MAPHEIGHT; j++)
		{
			cols.push_back(all_tiles);
		}
		map.push_back(cols);
	}
}

void	collapseNeighbors(int tile_index, std::tuple<int, int> north, std::tuple<int, int> east, std::tuple<int, int> south, std::tuple<int, int> west)
{
	std::vector<int> legal_north_neighbors = all_tiles[tile_index].N_neighbors;
	std::vector<int> legal_east_neighbors = all_tiles[tile_index].E_neighbors;
	std::vector<int> legal_south_neighbors = all_tiles[tile_index].S_neighbors;
	std::vector<int> legal_west_neighbors = all_tiles[tile_index].W_neighbors;

	std::vector<Tile> collapsed_north = {};
	std::vector<Tile> collapsed_east = {};
	std::vector<Tile> collapsed_south = {};
	std::vector<Tile> collapsed_west = {};

	if (std::get<0>(north) != -1)
	{
		std::vector<Tile> n = map[std::get<0>(north)][std::get<1>(north)];
		if (n.size() > 1)
		{
			for (int i = 0; i < n.size(); i++)
			{
				int key = n[i].tile_index;
				if (std::find(legal_north_neighbors.begin(), legal_north_neighbors.end(), key) != legal_north_neighbors.end())
				{
					collapsed_north.push_back(n[i]);
				}
			}
			map[std::get<0>(north)][std::get<1>(north)] = collapsed_north;
		}
	}

	if (std::get<0>(east) != -1)
	{
		std::vector<Tile> e = map[std::get<0>(east)][std::get<1>(east)];
		if (e.size() > 1)
		{
			for (int i = 0; i < e.size(); i++)
			{
				int key = e[i].tile_index;
				if (std::find(legal_east_neighbors.begin(), legal_east_neighbors.end(), key) != legal_east_neighbors.end())
				{
					collapsed_east.push_back(e[i]);
				}
			}
			map[std::get<0>(east)][std::get<1>(east)] = collapsed_east;
		}
	}

	if (std::get<0>(south) != -1)
	{
		std::vector<Tile> s = map[std::get<0>(south)][std::get<1>(south)];
		if (s.size() > 1)
		{
			for (int i = 0; i < s.size(); i++)
			{
				int key = s[i].tile_index;
				if (std::find(legal_south_neighbors.begin(), legal_south_neighbors.end(), key) != legal_south_neighbors.end())
				{
					collapsed_south.push_back(s[i]);
				}
			}
			map[std::get<0>(south)][std::get<1>(south)] = collapsed_south;
		}
	}

	if (std::get<0>(west) != -1)
	{
		std::vector<Tile> w = map[std::get<0>(west)][std::get<1>(west)];
		if (w.size() > 1)
		{
			for (int i = 0; i < w.size(); i++)
			{
				int key = w[i].tile_index;
				if (std::find(legal_west_neighbors.begin(), legal_west_neighbors.end(), key) != legal_west_neighbors.end())
				{
					collapsed_west.push_back(w[i]);
				}
			}
			map[std::get<0>(west)][std::get<1>(west)] = collapsed_west;
		}
	}
}

std::tuple<int, int, int> FindFewestNeighbors()
{
	int min_neighbors = 29;

	std::tuple <int, int, int> coords = std::make_tuple(-1, -1, -1);

	for (int i = 0; i < MAPWIDTH; i++)
	{
		for (int j = 0; j < MAPHEIGHT; j++)
		{
			if (map[i][j].size() < min_neighbors && map[i][j].size() > 1)
			{
				min_neighbors = map[i][j].size();
				coords = std::make_tuple(i, j, min_neighbors);
			}
		}
	}

	return coords;
}

void	populateMap()
{
	initializeMap();

	int randx = GetRandNum(MAPWIDTH);
	int randy = GetRandNum(MAPHEIGHT);

	int randTile = GetRandNum(29);

	map[randx][randy] = { all_tiles[randTile] };

	std::tuple<int, int> base_n = GetNorth(randx, randy);
	std::tuple<int, int> base_e = GetEast(randx, randy);
	std::tuple<int, int> base_s = GetSouth(randx, randy);
	std::tuple<int, int> base_w = GetWest(randx, randy);

	collapseNeighbors(randTile, base_n, base_e, base_s, base_w);

	std::tuple<int, int, int> end_condition = std::make_tuple(-1, -1, -1);

	while (FindFewestNeighbors() != end_condition)
	{
		std::tuple<int, int, int> fewest = FindFewestNeighbors();

		int fewestx = std::get<0>(fewest);
		int fewesty = std::get<1>(fewest);

		int selectedTile = map[fewestx][fewesty][GetRandNum(map[fewestx][fewesty].size())].tile_index;

		map[fewestx][fewesty] = { all_tiles[selectedTile] };

		std::tuple<int, int> n = GetNorth(fewestx, fewesty);
		std::tuple<int, int> e = GetEast(fewestx, fewesty);
		std::tuple<int, int> s = GetSouth(fewestx, fewesty);
		std::tuple<int, int> w = GetWest(fewestx, fewesty);

		collapseNeighbors(selectedTile, n, e, s, w);
	}
}
