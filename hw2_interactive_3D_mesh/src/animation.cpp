#include <sys/time.h>

#include "../include/Angel.h"
#include "../include/glmath.h"
#include "../include/Trajectory.h"
#define C30  0.433012702f // const number for the model
#define SCAL 1.0f // scal in homogenous
#define SIZE 2.0f // scal of the model

#define COLOR_LINE 0 // color of line
#define COLOR_FACE 2 // color of face


typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
const int NumVertices = 66; //(6 faces)*(2 triangles/face)+(3 vertices/triangle)
point4 points[NumVertices+38+12];
point4 colors[NumVertices+38+12]; // Vertices's color 
vec3 faces_normals[13]; // each faces normal
vec4 faces_referencpoints[39]; // 3 points on each faces
vec4 *traj_draw; // pointer to the traj to draw

point4 startendcube[48];
point4 startendcubecolor[48];



int count=1;
//-----------------------------------------
//vetex list
//-----------------------------------------
point4 vertices[13] = {
	point4(  0.5f*SIZE ,  0.0f*SIZE,  0.5f, SCAL ),
	point4(  0.25f*SIZE,  C30*SIZE,  0.5f, SCAL ),
	point4( -0.25f*SIZE,  C30*SIZE,  0.5f, SCAL ),
	point4( -0.5f*SIZE , 0.0f*SIZE,  0.5f, SCAL ),
	point4( -0.25f*SIZE, -C30*SIZE,  0.5f, SCAL ),
	point4(  0.25f*SIZE, -C30*SIZE,  0.5f, SCAL ),
	point4(  0.5f*SIZE ,  0.0f*SIZE, -0.5f, SCAL ),
	point4(  0.25f*SIZE,  C30*SIZE, -0.5f, SCAL ),
	point4( -0.25f*SIZE,  C30*SIZE, -0.5f, SCAL ),
	point4( -0.5f*SIZE ,  0.0f*SIZE, -0.5f, SCAL ),
	point4( -0.25f*SIZE, -C30*SIZE, -0.5f, SCAL ),
	point4(  0.25f*SIZE, -C30*SIZE, -0.5f, SCAL ),
	point4(  0.0f, 0.0f, 1.5f, SCAL)};

//-----------------------------------------
// RGBA olors
//-----------------------------------------
color4 vertex_colors[8] = {
	color4( 0.0, 0.0, 0.0, 1.0 ),  // black
	color4( 1.0, 0.0, 0.0, 1.0 ),  // red
	color4( 1.0, 1.0, 0.0, 0.2 ),  // yellow
	color4( 0.0, 1.0, 0.0, 1.0 ),  // green
	color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
	color4( 0.0, 1.0, 1.0, 0.1 ),  // magenta
	color4( 1.0, 1.0, 1.0, 0.1 ),  // white
	color4( 0.5, 0.5, 0.5, 0.1 )   // cyan
};
//
// cube
//
void cubemaker(const vec3 &center, const int& shift)
{
	int k=shift;
	GLfloat w=0.02;
	startendcube[k]=vec4( center[0]+w, center[1]+w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]+w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++; 
	startendcube[k]=vec4( center[0]-w, center[1]-w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]+w, center[1]-w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	
	startendcube[k]=vec4( center[0]+w, center[1]+w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]+w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]-w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]+w, center[1]-w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;

	startendcube[k]=vec4( center[0]-w, center[1]+w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]-w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]-w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]+w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;

	startendcube[k]=vec4( center[0]+w, center[1]+w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]+w, center[1]-w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]+w, center[1]-w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]+w, center[1]+w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;

	startendcube[k]=vec4( center[0]+w, center[1]+w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]+w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]+w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]+w, center[1]+w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;

	startendcube[k]=vec4( center[0]+w, center[1]-w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]-w, center[2]+w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]-w, center[1]-w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
	startendcube[k]=vec4( center[0]+w, center[1]-w, center[2]-w, 1.0f); startendcubecolor[k]=vertex_colors[4]; k++;
}


//-------------------------------------------
// Trajectory list
//-------------------------------------------

	inline 
void compute_faces_normals(vec4  i_points[39], vec3 (&i_normals)[13])
{
	for(int i=0;i<39;i+=3)
	{
		vec3 normal = normalize(trig_normal(i_points[i],i_points[i+1],i_points[i+2]));
		i_normals[i/3] = normal;
	}

}

	inline
void ref_point_add(vec4 (&i_points)[39])
{
	int i=0;

	i_points[i]=vertices[5]; i++;
	i_points[i]=vertices[12]; i++;
	i_points[i]=vertices[4]; i++;
	i_points[i]=vertices[4]; i++;
	i_points[i]=vertices[12]; i++;
	i_points[i]=vertices[3]; i++;
	i_points[i]=vertices[3]; i++;
	i_points[i]=vertices[12]; i++;
	i_points[i]=vertices[2]; i++;
	i_points[i]=vertices[2]; i++;
	i_points[i]=vertices[12]; i++;
	i_points[i]=vertices[1]; i++;
	i_points[i]=vertices[1]; i++;
	i_points[i]=vertices[12]; i++;
	i_points[i]=vertices[0]; i++;
	i_points[i]=vertices[0]; i++;
	i_points[i]=vertices[12]; i++;
	i_points[i]=vertices[5]; i++;

	//poly6
	i_points[i]=vertices[11]; i++;
	i_points[i]=vertices[10]; i++;
	i_points[i]=vertices[9]; i++;

	//rec4
	i_points[i]=vertices[0]; i++;
	i_points[i]=vertices[5]; i++;
	i_points[i]=vertices[6]; i++;	
	i_points[i]=vertices[5]; i++;
	i_points[i]=vertices[4]; i++;
	i_points[i]=vertices[11]; i++;
	i_points[i]=vertices[4]; i++;
	i_points[i]=vertices[3]; i++;
	i_points[i]=vertices[10]; i++;
	i_points[i]=vertices[3]; i++;
	i_points[i]=vertices[2]; i++;
	i_points[i]=vertices[9]; i++;
	i_points[i]=vertices[2]; i++;
	i_points[i]=vertices[1]; i++;
	i_points[i]=vertices[8]; i++;
	i_points[i]=vertices[1]; i++;
	i_points[i]=vertices[0]; i++;
	i_points[i]=vertices[7]; i++;
}
int Index = 0; // Index of VAOs
//-----------------------------------------
// Pointer to line
//-----------------------------------------
void line(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[d]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[a]; Index++;
}

void lineroof(int a, int b)
{
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[COLOR_LINE]; points[Index] = vertices[b]; Index++;
}

//-----------------------------------------
// Pointer to poly4 face
//-----------------------------------------
void quad( int a, int b, int c, int d , int color)
{
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[d]; Index++;
}
//-----------------------------------------
//Pointer to poly5 face
//-----------------------------------------
void poly6( int a, int b, int c, int d, int e, int f, int color)
{

	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[d]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[e]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[e]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[f]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[e]; Index++;
}

//-----------------------------------------
//Pointer to Trangle face
//-----------------------------------------
void trangle( int a, int b, int h, int color)
{

	colors[Index] = vertex_colors[color]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[color]; points[Index] = vertices[h]; Index++;
}

//----------------------------------------------------------------------------
// generate 12 triangles: 36 vertices and 36 colors
//----------------------------------------------------------------------------
void colorcube()
{
	//----------------LINES---------------	
	line(0,6,11,5);
	line(5,11,10,4);
	line(4,10,9,3);
	line(3,9,8,2);
	line(2,8,7,1);
	line(1,7,6,0);
	lineroof(1,12);	
	lineroof(0,12);	
	lineroof(5,12);		
	lineroof(4,12);	
	lineroof(3,12);	
	lineroof(2,12);	
	//----------------FACES----------------
	trangle(5,4,12,COLOR_FACE);
	trangle(4,3,12,COLOR_FACE);
	trangle(3,2,12,COLOR_FACE);
	trangle(2,1,12,COLOR_FACE);
	trangle(1,0,12,COLOR_FACE);
	trangle(0,5,12,COLOR_FACE);
	poly6(11,10,9,8,7,6,COLOR_FACE);
	quad( 0, 5, 11, 6 ,COLOR_FACE);
	quad( 10, 4, 5, 11 ,COLOR_FACE);
	quad( 4, 3, 9, 10 ,COLOR_FACE);
	quad( 9, 8, 2, 3 ,COLOR_FACE);
	quad( 2, 8, 7, 1 ,COLOR_FACE);
	quad( 1, 7, 6, 0 ,COLOR_FACE);
	// for the reference line
	points[Index] = point4(0.0f,0.0f,0.0f,1.0f); colors[Index] = vertex_colors[1];  Index++;
	points[Index] = point4(0.0f,0.0f,0.0f,1.0f); colors[Index] = vertex_colors[1];
}

//---------------------------------------------------------------------------
// Draw the reference Aixses
//----------------------------------------------------------------------------

void draw_aixs()
{
	points[Index] = point4(1.0f,0.0f,0.0f,1.0f);  colors[Index] = vertex_colors[1];  Index++;
	points[Index] = point4(-1.0f,0.0f,0.0f,1.0f);  colors[Index] = vertex_colors[1];  Index++;
	points[Index] = point4(0.0f,1.0f,0.0f,0.001f);  colors[Index] = vertex_colors[3];  Index++;
	points[Index] = point4(0.0f,-1.0f,0.0f,0.001f);  colors[Index] = vertex_colors[3];  Index++;
	points[Index] = point4(0.0f,0.0f,-1.0f,0.001f);  colors[Index] = vertex_colors[4];  Index++;
	points[Index] = point4(0.0f,0.0f,1.0f,0.001f);  colors[Index] = vertex_colors[4];  Index++;


}

//----------------------------------------------------------------------------
// OpenGL initializationi
//----------------------------------------------------------------------------
GLuint buffers[2];
GLuint model_view_loc;
GLuint projection_loc;
GLuint vPosition,vColor;
void init()
{
	// list init

	time_t timer;
	srand((unsigned)time(&timer));
	ref_point_add(faces_referencpoints);
	compute_faces_normals(faces_referencpoints,faces_normals);
	randomstart(end,faces_normals,faces_referencpoints,0.3);
	cubemaker(end->point,0);
	
	//std::cout<< startendcubecolor[0];
	// model init
	draw_aixs();
	colorcube();
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays( 1, &vao);
	glBindVertexArray( vao);

	// Create and initialize a buffer object
	glGenBuffers( 2, buffers);

	// Load shaders and use the resulting shader program
	//-----------------------------------------------------------
	// Create Buffer 1
	//-----------------------------------------------------------

	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors)+2*sizeof(startendcubecolor), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points) , sizeof(colors), colors );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), sizeof(startendcube), startendcube );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors)+sizeof(startendcube), sizeof(startendcube), startendcubecolor );
	// set up vertex arrays
	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*(count+1), NULL, GL_STATIC_DRAW );

	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*count, traj_draw );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*count, sizeof(point4), vec4(0,1.0,0,1.0)  );

	GLuint program = InitShader( "./glsl/vshader32.glsl", "./glsl/fshader32.glsl" );
	glUseProgram( program );

	vPosition = glGetAttribLocation( program, "vPosition" );

	glEnableVertexAttribArray( vPosition );

	vColor = glGetAttribLocation( program, "vColor" ); 
	glEnableVertexAttribArray( vColor );

	model_view_loc = glGetUniformLocation( program, "model_view");
	projection_loc = glGetUniformLocation( program, "projection");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int startx=512,moving,starty=384;
// 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------Transformation matrix declare----------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

GLfloat radius=3.0f;
point4 click_point3d;


//----------------------------------------------------------------------------
//-----------------------------------------
// Array of rotation angles (in degrees) for each coordinate axis
//-----------------------------------------
//----------------------------------------------------------------------------
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
mat4 T(1.0f);
point4 d_moving(0.0f), mouse_last(0.0f);
int first_click = 0;
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
int global_n=-1;
void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	//Rotation Matrix
	mat4  rote= (   RotateY( Theta[Xaxis]) *
			RotateZ( Theta[Zaxis]) );
	point4  eye( radius, 0.0f, 0.0f, 1.0f );

	T=rote*T;
	//eye=T*eye;	
	point4  at( 0.0f,  0.0f, 0.0f, 1.0f );
	vec4    up( 0.0f, 0.0f, 1.0f, 0.0f );

	mat4  mc = Frustum(-0.1024f, 0.1024f, -0.0768f, 0.0768f, 0.1f, 5.0f);	  
	mat4  mv = LookAt( eye, at, up );

	Theta[0]=0.0f;
	Theta[1]=0.0f;
	Theta[2]=0.0f;
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, mv*T);	
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, mc );

	click_point3d=sol_inter_sphereandline(reprojection(mc*mv,pixels2homo( startx , starty , HAFW_WIDTH, HAFW_HEIGHT, 5.0f, 0.1f, 2.0)),3.0f);	
	// Bind to buffer1, prepare to draw the 3D model
	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), sizeof(startendcube), startendcube );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors)+sizeof(startendcube), sizeof(startendcube), startendcubecolor );
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
	if(moving)
	{
		d_moving= click_point3d - mouse_last;
		if(first_click<3)
			d_moving=0.0f;

		mouse_last = click_point3d;

		points[Index] = gluInvertMatrix(T)*click_point3d;
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );

	}

	if(moving)
	{
		//glDrawArrays( GL_LINES,  36+NumVertices+12 , 2);
		//glDrawArrays( GL_LINES,  0 , 6);
	}

	glDrawArrays( GL_TRIANGLES, 48, NumVertices);	
	glDisable(GL_BLEND);  // 关闭混合

	glEnable(GL_DEPTH_TEST); // 打开深度测试

	//std::cout<<global_n<<std::endl;
	if(global_n<6)
	{
		glDrawArrays( GL_TRIANGLES, 48+global_n*3, 3);
	}


	if( global_n > 6)
	{
		glDrawArrays( GL_TRIANGLES, 78+6*(global_n-7), 6);

	}

	if(global_n == 6)
	{
		glDrawArrays(GL_TRIANGLES, 66,12);
	}

	vec4 *color_temp=new vec4[count];
	for(int i=0;i<count;++i)
	{
		color_temp[i]=vec4(0,1,0,1);

	}
	
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)+sizeof(startendcube)) );
	glDrawArrays( GL_QUADS, 50 + NumVertices, 48);		
	// bind buffer2 prepare to draw the traj
	glBindBuffer( GL_ARRAY_BUFFER, buffers[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*count*2, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*count, traj_draw );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*(count),  sizeof(point4)*count, color_temp);

	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*(count)));
	
	glDrawArrays(GL_LINE_STRIP,0,count);	

	delete [] color_temp;	
	// draw lines of the model
	glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
	glDrawArrays( GL_LINE_STRIP, 6 , 42);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------

bool Begin = false;
inline void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033: // Escape Key
			exit( EXIT_SUCCESS );
			break;
		case 's':
			Begin=true;
			break;
		case 'i':
			Begin=!Begin;
			break;
			break;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
inline static void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
			d_moving = point4(0,0,0,0);
			first_click=0;
			glutPostRedisplay();
		}
	}
}
inline static void mouseMotion(int x, int y){
	if (moving) {
		startx = x;
		starty = y;
		first_click++;	
		Theta[2]=180*d_moving[1]/M_PI;
		Theta[0]=-180*d_moving[2]/M_PI;
		glutPostRedisplay();
	}
}
int trig_tra=0;
bool stop = false;
//----------------------------------------------------------------------------
void idle( void )
{	
	if(!stop && Begin)
	{
		randomdiplace(0.02);
		count++;
		// for the normals computation
	int n=checkbound(end->point,faces_normals,faces_referencpoints);
		if(n>-1 && trig_tra==0)
		{
			pointertofaces(n,colors);
			cubemaker(end->point,24);
			trig_tra=1;
	//		std::cout<<n;
			stop=1;
			global_n= n;
		}
		traj_draw=copytoarray(count);
		//std::cout<< traj_draw[count-1] <<std::endl;		
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 1024, 768 );
	//    glutInitContextVersion( 3, 2 );
	//    glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "Color Cube" );

	glewInit();

	init();

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouseButton );
	glutMotionFunc( mouseMotion );
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
