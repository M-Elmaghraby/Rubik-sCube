#include "Angel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <ctime>

//functions
void setup_slice_zero(int setup_mode);
void setup_slice_one(int setup_mode);
void setup_slice_two(int setup_mode);
void setup_slice_three(int setup_mode);
void setup_slice_four(int setup_mode);
void setup_slice_five(int setup_mode);
void setup_slice_six(int setup_mode);
void setup_slice_seven(int setup_mode);
void setup_slice_eight(int setup_mode);

void rotate_slice_zero_cw(int value);
void rotate_slice_zero_ccw(int value);

void rotate_slice_one_cw(int value);
void rotate_slice_one_ccw(int value);

void rotate_slice_two_cw(int value);
void rotate_slice_two_ccw(int value);

void rotate_slice_three_cw(int value);
void rotate_slice_three_ccw(int value);

void rotate_slice_four_cw(int value);
void rotate_slice_four_ccw(int value);

void rotate_slice_five_cw(int value);
void rotate_slice_five_ccw(int value);

void rotate_slice_six_cw(int value);
void rotate_slice_six_ccw(int value);

void rotate_slice_seven_cw(int value);
void rotate_slice_seven_ccw(int value);

void rotate_slice_eight_cw(int value);
void rotate_slice_eight_ccw(int value);

void random_animate();
void color_cube (int value);
void highlight_slice(vec3 highlight_color);

bool check_face(GLuint face [3][3]);
bool check_winner();

// Chip Colors
const vec3 RED_COLOR = vec3(1.0, 0.0, 0.0), GREEN_COLOR = vec3(0.5, 1.0, 0.0), WHITE_COLOR = vec3(1.0, 1.0, 1.0), BLUE_COLOR = vec3(0.0, 0.0, 0.5)
	, ORANGE_COLOR = vec3(1.0,0.64,0.0) , YELLOW_COLOR = vec3(1.0,1.0,0.0) , BLACK_COLOR = vec3(0.0,0.0,0.0), HIGHLIGHT_COLOR = vec3(0.0f/255.0f,132.0f/255.0f,240.0f/255.0f) ;

//vertices positions (should be parameterized to represent all the small cubes)
const vec3 ZERO =  vec3(0.75f, 0.75f, 0.75f), ONE = vec3(-0.75f, 0.75f, 0.75f), TWO = vec3(-0.75f, -0.75f, 0.75f), THREE = vec3(0.75f, -0.75f, 0.75f),
	FOUR = vec3(0.75f, -0.75f, -0.75f), FIVE = vec3(0.75f,  0.75f, -0.75f), SIX = vec3(-0.75f, 0.75f, -0.75f), SEVEN = vec3(-0.75f, -0.75f, -0.75f);

//Window Dimensions
const GLfloat WindowWidth = 600;
const GLfloat WindowHeight = 600;

const GLint RC_NumPoints = 108*6 ; 
const GLint RC_NumIndices = 180*6 ; 

const int MODE_SET = 1;
const int MODE_RESET = 0;

// Rubiks Cube Buffer Arrays
glm::vec3 RC_points[RC_NumPoints];
vec3 RC_colors[RC_NumPoints];
GLuint RC_indices[RC_NumIndices];
GLfloat RC_animation_map[RC_NumPoints];


// Rubiks Cube Unit Variables
GLfloat m = 0.03f;
GLfloat p = 0.08f;

GLuint vertices_buffer, indices_buffer, colors_buffer, animation_map;

// Shader Program
GLuint program ;

// Transformation Matrices
GLint uniModel;
glm::mat4  model;

GLint uniView;
glm::mat4 view ;

GLint uniAnimationMatrix ;
glm::mat4 animation_matrix ;

//camera co-ordinates
glm::vec3 cam_pos, cam_up; 

//co-ordinates of previous mouse location
GLint px, py ;

//Faces arrays
GLuint red_face[3][3];
GLuint yellow_face[3][3];
GLuint orange_face[3][3];
GLuint white_face[3][3];
GLuint blue_face[3][3];
GLuint green_face[3][3];

// Animation variables
GLboolean locked = true;
GLuint num_of_animations = 1 ;
const GLuint ANIMATION_SPEED = 3;
GLfloat rot_angle = 0 ;
int next_color = 0 ;

// Higlighting variables
const int SLICE_ZERO  = 0;
const int SLICE_ONE   = 1;
const int SLICE_TWO   = 2;
const int SLICE_THREE = 3;
const int SLICE_FOUR  = 4;
const int SLICE_FIVE  = 5;
const int SLICE_SIX   = 6;
const int SLICE_SEVEN = 7;
const int SLICE_EIGHT = 8;

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

int selected_slice = 0 ;

void init( void )
{
	/*
	Initialize Rubiks Cube Points Array
	*/

	// Initialize Face Unit Points
	RC_points[0]  = glm::vec3(-0.25f, -0.25f, 0.75f); 
	RC_points[1]  = glm::vec3(-0.75f, -0.25f, 0.75f); 
	RC_points[2]  = glm::vec3(-0.75f, -0.75f, 0.75f); 
	RC_points[3]  = glm::vec3(-0.25f, -0.75f, 0.75f); 
	RC_points[4]  = glm::vec3(-0.25f-m, -0.75f+m, 0.75f+p); 
	RC_points[5]  = glm::vec3(-0.25f-m, -0.25f-m, 0.75f+p); 
	RC_points[6]  = glm::vec3(-0.75f+m, -0.25f-m, 0.75f+p); 
	RC_points[7]  = glm::vec3(-0.75f+m, -0.75f+m, 0.75f+p); 
	RC_points[8]  = glm::vec3(-0.25f-m, -0.75f+m, 0.75f+p); 
	RC_points[9]  = glm::vec3(-0.25f-m, -0.25f-m, 0.75f+p); 
	RC_points[10] = glm::vec3(-0.75f+m, -0.25f-m, 0.75f+p); 
	RC_points[11] = glm::vec3(-0.75f+m, -0.75f+m, 0.75f+p); 

	// Initialize Face Unit Colors
	for (int i = 0 ; i < 8 ; i++)
	{
		RC_colors[i] = BLACK_COLOR ;
	}

	for (int i = 8 ; i < 12 ; i++)
	{
		RC_colors[i] = RED_COLOR ;
	}

	// Initialize Face Unit Indices
	RC_indices[0] = 0 ;
	RC_indices[1] = 1 ;
	RC_indices[2] = 6 ;
	RC_indices[3] = 5 ;

	RC_indices[4] = 0 ;
	RC_indices[5] = 5 ;
	RC_indices[6] = 4 ;
	RC_indices[7] = 3 ;

	RC_indices[8]  = 4 ;
	RC_indices[9]  = 7 ;
	RC_indices[10] = 2 ;
	RC_indices[11] = 3 ;

	RC_indices[12] = 6 ;
	RC_indices[13] = 1 ;
	RC_indices[14] = 2 ;
	RC_indices[15] = 7 ;

	RC_indices[16] = 9  ;
	RC_indices[17] = 10 ;
	RC_indices[18] = 11 ;
	RC_indices[19] = 8  ;

	// Initialize Cube Face Points, Colors and Indices

	// RED FACE
	int points_cursor = 12 ;
	int indices_cursor= 20 ;
	int faces_cursor= 0 ;
	red_face[0][0] = 0;

	for(int i = 0 ; i < 3 ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			if(i!=0 || j!=0)
			{
				red_face[i][j] = points_cursor;
				glm::mat4 translation_matrix = glm::translate(glm::mat4(),glm::vec3(i*0.5f,j*0.5f,0.0f));
				for (int k = 0 ; k < 12 ; k++)
				{
					if(k < 8)
						RC_colors[points_cursor] = BLACK_COLOR ;
					else
						RC_colors[points_cursor] = RED_COLOR ;

					RC_points[points_cursor++] = glm::vec3(translation_matrix * glm::vec4(RC_points[k], 1.0f));
				}

				for (int k = 0 ; k < 20 ; k++)
				{
					RC_indices[indices_cursor++] = RC_indices[indices_cursor-20]+12;
				}
			}
		}
	}

	// Initialize Cube the remaining faces Points, Colors and Indices

	// YELLOW FACE

	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(),90.0f,glm::vec3(0.0f,1.0f,0.0f));
	for(int i = 0 ; i < 108 ; i++)
	{
		if(i % 12 < 8){
			RC_colors[points_cursor] = BLACK_COLOR ;
		}else{
			RC_colors[points_cursor] = YELLOW_COLOR ;
		}
		RC_points[points_cursor++] = glm::vec3(rotation_matrix * glm::vec4(RC_points[i], 1.0f));
	}

	for (int k = 0 ; k < 180 ; k++)
	{
		RC_indices[indices_cursor++] = RC_indices[indices_cursor-180]+108;
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			yellow_face[i][j] = (i * 3 + j) * 12 + 108 * 1;
		}
	}

	// ORANGE FACE
	rotation_matrix = glm::rotate(glm::mat4(),180.0f,glm::vec3(0.0f,1.0f,0.0f));
	for(int i = 0 ; i < 108 ; i++)
	{
		if(i % 12 < 8){
			RC_colors[points_cursor] = BLACK_COLOR ;
		}else{
			RC_colors[points_cursor] = ORANGE_COLOR;
		}
		RC_points[points_cursor++] = glm::vec3(rotation_matrix * glm::vec4(RC_points[i], 1.0f));
	}

	for (int k = 0 ; k < 180 ; k++)
	{
		RC_indices[indices_cursor++] = RC_indices[indices_cursor-180]+108;
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			orange_face[i][j] = (i * 3 + j) * 12 + 108 * 2;
		}
	}

	// WHITE FACE
	rotation_matrix = glm::rotate(glm::mat4(),270.0f,glm::vec3(0.0f,1.0f,0.0f));
	for(int i = 0 ; i < 108 ; i++)
	{
		if(i % 12 < 8){
			RC_colors[points_cursor] = BLACK_COLOR ;
		}else{
			RC_colors[points_cursor] = WHITE_COLOR;
		}
		RC_points[points_cursor++] = glm::vec3(rotation_matrix * glm::vec4(RC_points[i], 1.0f));
	}

	for (int k = 0 ; k < 180 ; k++)
	{
		RC_indices[indices_cursor++] = RC_indices[indices_cursor-180]+108;
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			white_face[i][j] = (i * 3 + j) * 12 + 108 * 3;
		}
	}

	// BLUE FACE
	rotation_matrix = glm::rotate(glm::mat4(),90.0f,glm::vec3(1.0f,0.0f,0.0f));
	for(int i = 0 ; i < 108 ; i++)
	{
		if(i % 12 < 8){
			RC_colors[points_cursor] = BLACK_COLOR ;
		}else{
			RC_colors[points_cursor] = BLUE_COLOR;
		}
		RC_points[points_cursor++] = glm::vec3(rotation_matrix * glm::vec4(RC_points[i], 1.0f));
	}

	for (int k = 0 ; k < 180 ; k++)
	{
		RC_indices[indices_cursor++] = RC_indices[indices_cursor-180]+108;
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			blue_face[i][j] = (i * 3 + j) * 12 + 108 * 4;
		}
	}

	// GREEN FACE
	rotation_matrix = glm::rotate(glm::mat4(),270.0f,glm::vec3(1.0f,0.0f,0.0f));
	for(int i = 0 ; i < 108 ; i++)
	{
		if(i % 12 < 8){
			RC_colors[points_cursor] = BLACK_COLOR ;
		}else{
			RC_colors[points_cursor] = GREEN_COLOR;
		}
		RC_points[points_cursor++] = glm::vec3(rotation_matrix * glm::vec4(RC_points[i], 1.0f));
	}

	for (int k = 0 ; k < 180 ; k++)
	{
		RC_indices[indices_cursor++] = RC_indices[indices_cursor-180]+108;
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			green_face[i][j] = (i * 3 + j) * 12 + 108 * 5;
		}
	}

	// Initialize animation map
	for(int i = 0 ; i < RC_NumPoints ; i++)
	{
		RC_animation_map[i]=FALSE;
	}

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Load shaders and use the resulting shader program
	program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );

	// Create and initialize a buffer object for vertices
	glGenBuffers( 1, &vertices_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

	// Initialize the vertex position attribute in the vertex shader
	GLuint position_loc = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( position_loc );
	glVertexAttribPointer( position_loc, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );

	// Create and initialize a buffer object for colors
	glGenBuffers( 1, &colors_buffer);
	glBindBuffer( GL_ARRAY_BUFFER, colors_buffer);
	glBufferData( GL_ARRAY_BUFFER, sizeof(RC_colors), RC_colors, GL_STATIC_DRAW );

	// Initialize the color attribute in the vertex shader
	GLuint color_loc;
	color_loc = glGetAttribLocation( program, "inVColor" );
	glEnableVertexAttribArray( color_loc );
	glVertexAttribPointer( color_loc, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );

	// Create and initialize a buffer object for animation map
	glGenBuffers( 1, &animation_map);
	glBindBuffer( GL_ARRAY_BUFFER, animation_map);
	glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );

	// Initialize the apply_animation attribute in the vertex shader
	GLuint animation_map_loc;
	animation_map_loc = glGetAttribLocation( program, "applyAnimation" );
	glEnableVertexAttribArray( animation_map_loc );
	glVertexAttribPointer( animation_map_loc, 1, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0) );

	// Create and initialize a buffer object for indices
	glGenBuffers( 1, &indices_buffer);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(RC_indices), RC_indices, GL_STATIC_DRAW );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set up model
	model = glm::mat4();
	uniModel = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	// Set up Camera/View

	cam_pos = glm::vec3(0.0001f, 0.0001f, 5.0f); 
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 aux_mat = glm::rotate(
		glm::mat4(),
		-30.0f,
		glm::vec3(1.0f, 0.0f, 0.0f)
		);

	// Calculate transformation
	cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
	cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

	aux_mat = glm::rotate(
		glm::mat4(),
		45.0f,
		glm::vec3(0.0f, 1.0f, 0.0f)
		);

	// Calculate transformation
	cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
	cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

	view = glm::lookAt(
		cam_pos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		cam_up
		);
	uniView = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	// Setup Projection
	glm::mat4 proj = glm::perspective(45.0f, 600.0f / 600.0f, 1.0f, 7.0f);
	GLint uniProj = glGetUniformLocation(program, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	// initialize animation matrix
	animation_matrix = glm::mat4();
	uniAnimationMatrix = glGetUniformLocation(program, "animation_matrix");
	glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

	glClearColor(0.1,0.1,0.1,0.5); // background

	// randomly rotate the cube faces at the beginning	
	srand((unsigned)time(0)); 
	random_animate();

}

void random_animate()
{
	if(num_of_animations > 0)
	{
		num_of_animations--;
		GLuint random = rand()%18;
		switch(random)
		{

		case 0:
			rotate_slice_zero_cw(90);
			break;
		case 1:
			rotate_slice_zero_ccw(90);
			break;
		case 2:
			rotate_slice_one_cw(90);
			break;
		case 3:
			rotate_slice_one_ccw(90);
			break;
		case 4:
			rotate_slice_two_cw(90);
			break;
		case 5:
			rotate_slice_two_ccw(90);
			break;
		case 6:
			rotate_slice_three_cw(90);
			break;
		case 7:
			rotate_slice_three_ccw(90);
			break;
		case 8:
			rotate_slice_four_cw(90);
			break;
		case 9:
			rotate_slice_four_ccw(90);
			break;
		case 10:
			rotate_slice_five_cw(90);
			break;
		case 11:
			rotate_slice_five_ccw(90);
			break;
		case 12:
			rotate_slice_six_cw(90);
			break;
		case 13:
			rotate_slice_six_ccw(90);
			break;
		case 14:
			rotate_slice_seven_cw(90);
			break;
		case 15:
			rotate_slice_seven_ccw(90);
			break;
		case 16:
			rotate_slice_eight_cw(90);
			break;
		case 17:
			rotate_slice_eight_ccw(90);
			break;
		}


	}
	else
	{
		locked = false;
		if(check_winner())
		{
			color_cube(4000);
			locked = true;
			highlight_slice(BLACK_COLOR);
		}
		else
		{
			highlight_slice(HIGHLIGHT_COLOR);
		}
	}
}

void highlight_slice(vec3 highlight_color)
{
	switch(selected_slice)
	{
	case SLICE_ZERO:
		for(int j = 0 ; j < 8 ; j++)
		{

			for(int i = 0; i < 3; i++){
				RC_colors[red_face[i][2]+j] = highlight_color;
				RC_colors[yellow_face[i][2]+j] = highlight_color;
				RC_colors[orange_face[i][2]+j] = highlight_color;
				RC_colors[white_face[i][2]+j] = highlight_color;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 8; k++)
				{
					RC_colors[green_face[i][j]+k] = highlight_color;
				}
			}
		}
		break;

	case SLICE_ONE:
		for(int j = 0 ; j < 8 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_colors[red_face[i][1]+j] = highlight_color;
				RC_colors[yellow_face[i][1]+j] = highlight_color;
				RC_colors[orange_face[i][1]+j] = highlight_color;
				RC_colors[white_face[i][1]+j] = highlight_color;
			}
		}
		break;
	case SLICE_TWO:
		for(int j = 0 ; j < 8 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_colors[red_face[i][0]+j] = highlight_color;
				RC_colors[yellow_face[i][0]+j] = highlight_color;
				RC_colors[orange_face[i][0]+j] = highlight_color;
				RC_colors[white_face[i][0]+j] = highlight_color;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 8; k++)
				{
					RC_colors[blue_face[i][j]+k] = highlight_color;
				}
			}
		}
		break;
	case SLICE_THREE:
		for(int j = 0 ; j < 8 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_colors[green_face[i][0]+j] = highlight_color;
				RC_colors[yellow_face[0][i]+j] = highlight_color;
				RC_colors[blue_face[i][2]+j] =highlight_color;
				RC_colors[white_face[2][i]+j] = highlight_color;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 8; k++)
				{
					RC_colors[red_face[i][j]+k] = highlight_color;
				}
			}
		}
		break;
		case SLICE_FOUR:
			for(int j = 0 ; j < 8 ; j++)
			{
				for(int i = 0; i < 3; i++){
					RC_colors[green_face[i][1]+j] = highlight_color;
					RC_colors[yellow_face[1][i]+j] = highlight_color;
					RC_colors[blue_face[i][1]+j] = highlight_color;
					RC_colors[white_face[1][i]+j] =highlight_color;
				}
			}
			break;
		case SLICE_FIVE:
				for(int j = 0 ; j < 8 ; j++)
				{
					for(int i = 0; i < 3; i++){
						RC_colors[green_face[i][2]+j] = highlight_color;
						RC_colors[yellow_face[2][i]+j] = highlight_color;
						RC_colors[blue_face[i][0]+j] = highlight_color;
						RC_colors[white_face[0][i]+j] = highlight_color;
					}
				}

				for(int i = 0; i <3; i++)
				{
					for(int j = 0; j < 3; j++)
					{
						for(int k = 0; k < 8; k++)
						{
							RC_colors[orange_face[i][j]+k] = highlight_color;
						}
					}
				}
			break;
		case SLICE_SIX:
			for(int j = 0 ; j < 8 ; j++)
			{
				for(int i = 0; i < 3; i++){
					RC_colors[red_face[2][i]+j] = highlight_color;
					RC_colors[green_face[2][i]+j] = highlight_color;
					RC_colors[orange_face[0][i]+j] = highlight_color;
					RC_colors[blue_face[2][i]+j] = highlight_color;
				}
			}

			for(int i = 0; i <3; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					for(int k = 0; k < 8; k++)
					{
						RC_colors[yellow_face[i][j]+k] = highlight_color;
					}
				}
			}
			break;
		case SLICE_SEVEN:
			for(int j = 0 ; j < 8 ; j++)
			{
				for(int i = 0; i < 3; i++){
					RC_colors[red_face[1][i]+j] = highlight_color;
					RC_colors[green_face[1][i]+j] = highlight_color;
					RC_colors[orange_face[1][i]+j] = highlight_color;
					RC_colors[blue_face[1][i]+j] = highlight_color;
				}
			}
			break;
		case SLICE_EIGHT:
			for(int j = 0 ; j < 8 ; j++)
			{
				for(int i = 0; i < 3; i++){
					RC_colors[red_face[0][i]+j] = highlight_color;
					RC_colors[green_face[0][i]+j] = highlight_color;
					RC_colors[orange_face[2][i]+j] = highlight_color;
					RC_colors[blue_face[0][i]+j] = highlight_color;
				}
			}

			for(int i = 0; i <3; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					for(int k = 0; k < 8 ; k++)
					{
						RC_colors[white_face[i][j]+k] = highlight_color;
					}
				}
			}
			break;
	}

	glBindBuffer( GL_ARRAY_BUFFER, colors_buffer);
	glBufferData( GL_ARRAY_BUFFER, sizeof(RC_colors), RC_colors, GL_STATIC_DRAW );

	glutPostRedisplay();
}

void color_cube (int value)
{
	if(value > 0)
	{
		if(value % 30 == 0 )
		{
			int color_key = (++next_color)%6;
			vec3 color ;
			switch(color_key)
			{
			case 0 :
				color = RED_COLOR;
				break;

			case 1 :
				color = GREEN_COLOR;
				break;

			case 2 :
				color = BLUE_COLOR;
				break;

			case 3 :
				color = YELLOW_COLOR;
				break;

			case 4 :
				color = ORANGE_COLOR;
				break;

			case 5 :
				color = WHITE_COLOR;
				break;
			}
			for(int i = 0  ; i < RC_NumPoints ; i++)
			{
				if(i%12 >= 8)
					RC_colors[i] = color ;
			}
		}

		animation_matrix = glm::rotate(
			glm::mat4(),
			-1.0f,
			glm::vec3(0.0f, 1.0f, 0.0f)
			);

		for(int i = 0  ; i < RC_NumPoints ; i++)
		{
			RC_points[i] = glm::vec3(animation_matrix * glm::vec4(RC_points[i], 1.0f));
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer);
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		glBindBuffer( GL_ARRAY_BUFFER, colors_buffer);
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_colors), RC_colors, GL_STATIC_DRAW );

		glutPostRedisplay();
		glutTimerFunc(5, color_cube, value-1);

	}
	else
	{
		exit(0);
	}
}

bool check_face(GLuint face [3][3])
{
	bool is_clear = true;
	vec3 face_color = RC_colors[face[0][0]+8];

	for(int i = 0 ; (i < 3 && is_clear) ; i++)
	{
		for(int j = 0 ; j < 3 ; j++)
		{
			vec3 current_color = RC_colors[face[i][j]+8];
			if(!(current_color[0] == face_color[0] && current_color[1] == face_color[1] && current_color[2] == face_color[2]))
			{
				is_clear =false;
				break;
			}
		}
	}

	return is_clear;
}

bool check_winner()
{

	return (check_face(red_face) && check_face(green_face) && check_face(yellow_face) && check_face(orange_face) && check_face(white_face) && check_face(blue_face) );
}

void display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear the window

	// Draw cube
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glDrawElements(GL_QUADS, RC_NumIndices, GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
}

/**
Mouse Clicks Callback Functions
*/
void mouse( int button, int state, int x, int y )
{	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		px = py = 0;
	}
}

/*
Special Keyboard Key callback method
*/
void specialKey( int key, int x, int y )
{ 
	if(!locked){
		switch ( key ) {
		case GLUT_KEY_ALT_L: 
			switch(selected_slice)
			{
			case SLICE_ZERO :
				rotate_slice_zero_cw(90);
				break;
			case SLICE_ONE :
				rotate_slice_one_cw(90);
				break;
			case SLICE_TWO :
				rotate_slice_two_cw(90);
				break;
			case SLICE_THREE :
				rotate_slice_three_cw(90);
				break;
			case SLICE_FOUR :
				rotate_slice_four_cw(90);
				break;
			case SLICE_FIVE :
				rotate_slice_five_cw(90);
				break;
			case SLICE_SIX :
				rotate_slice_six_cw(90);
				break;
			case SLICE_SEVEN :
				rotate_slice_seven_cw(90);
				break;
			case SLICE_EIGHT :
				rotate_slice_eight_cw(90);
				break;
			}
			
			locked = true;
			break;
		case GLUT_KEY_CTRL_L: 
			switch(selected_slice)
			{
			case SLICE_ZERO :
				rotate_slice_zero_ccw(90);
				break;
			case SLICE_ONE :
				rotate_slice_one_ccw(90);
				break;
			case SLICE_TWO :
				rotate_slice_two_ccw(90);
				break;
			case SLICE_THREE :
				rotate_slice_three_ccw(90);
				break;
			case SLICE_FOUR :
				rotate_slice_four_ccw(90);
				break;
			case SLICE_FIVE :
				rotate_slice_five_ccw(90);
				break;
			case SLICE_SIX :
				rotate_slice_six_ccw(90);
				break;
			case SLICE_SEVEN :
				rotate_slice_seven_ccw(90);
				break;
			case SLICE_EIGHT :
				rotate_slice_eight_ccw(90);
				break;
			}
			locked = true;
			break;
		case GLUT_KEY_UP: 
			highlight_slice(BLACK_COLOR);
			switch(selected_slice)
			{
			case SLICE_ZERO :
				selected_slice = SLICE_TWO;
				break;
			case SLICE_ONE :
				selected_slice = SLICE_ZERO;
				break;
			case SLICE_TWO :
				selected_slice = SLICE_ONE;
				break;
			case SLICE_THREE :
				selected_slice = SLICE_FOUR;
				break;
			case SLICE_FOUR :
				selected_slice = SLICE_FIVE;
				break;
			case SLICE_FIVE :
				selected_slice = SLICE_THREE;
				break;
			case SLICE_SIX :
				selected_slice = SLICE_SEVEN;
				break;
			case SLICE_SEVEN :
				selected_slice = SLICE_EIGHT;
				break;
			case SLICE_EIGHT :
				selected_slice = SLICE_SIX;
				break;
			}
			highlight_slice(HIGHLIGHT_COLOR);
			break;
		case GLUT_KEY_DOWN: 
			highlight_slice(BLACK_COLOR);
			switch(selected_slice)
			{
			case SLICE_ZERO :
				selected_slice = SLICE_ONE;
				break;
			case SLICE_ONE :
				selected_slice = SLICE_TWO;
				break;
			case SLICE_TWO :
				selected_slice = SLICE_ZERO;
				break;
			case SLICE_THREE :
				selected_slice = SLICE_FIVE;
				break;
			case SLICE_FOUR :
				selected_slice = SLICE_THREE;
				break;
			case SLICE_FIVE :
				selected_slice = SLICE_FOUR;
				break;
			case SLICE_SIX :
				selected_slice = SLICE_EIGHT;
				break;
			case SLICE_SEVEN :
				selected_slice = SLICE_SIX;
				break;
			case SLICE_EIGHT :
				selected_slice = SLICE_SEVEN;
				break;
			}
			highlight_slice(HIGHLIGHT_COLOR);
			break;
		case GLUT_KEY_RIGHT:
			highlight_slice(BLACK_COLOR);
			switch(selected_slice)
			{
			case SLICE_ZERO :
				selected_slice = SLICE_THREE;
				break;
			case SLICE_ONE :
				selected_slice = SLICE_FOUR;
				break;
			case SLICE_TWO :
				selected_slice = SLICE_FIVE;
				break;
			case SLICE_THREE :
				selected_slice = SLICE_EIGHT;
				break;
			case SLICE_FOUR :
				selected_slice = SLICE_SEVEN;
				break;
			case SLICE_FIVE :
				selected_slice = SLICE_SIX;
				break;
			case SLICE_SIX :
				selected_slice = SLICE_TWO;
				break;
			case SLICE_SEVEN :
				selected_slice = SLICE_ONE;
				break;
			case SLICE_EIGHT :
				selected_slice = SLICE_ZERO;
				break;
			}
			highlight_slice(HIGHLIGHT_COLOR);
			break;
		case GLUT_KEY_LEFT:
			highlight_slice(BLACK_COLOR);
			switch(selected_slice)
			{
			case SLICE_ZERO :
				selected_slice = SLICE_EIGHT;
				break;
			case SLICE_ONE :
				selected_slice = SLICE_SEVEN;
				break;
			case SLICE_TWO :
				selected_slice = SLICE_SIX;
				break;
			case SLICE_THREE :
				selected_slice = SLICE_ZERO;
				break;
			case SLICE_FOUR :
				selected_slice = SLICE_ONE;
				break;
			case SLICE_FIVE :
				selected_slice = SLICE_TWO;
				break;
			case SLICE_SIX :
				selected_slice = SLICE_FIVE;
				break;
			case SLICE_SEVEN :
				selected_slice = SLICE_FOUR;
				break;
			case SLICE_EIGHT :
				selected_slice = SLICE_THREE;
				break;
			}
			highlight_slice(HIGHLIGHT_COLOR);
			break;
		}
	}
	glutPostRedisplay();
}

/*
Mouse Motion Callback Method
*/
void mouseMotion(int x, int y )
{
	if(px != 0)
	{
		glm::mat4 aux_mat = glm::rotate(
			glm::mat4(),
			((px - x) / 600.0f) * 180.0f,
			cam_up
			);

		// Calculate transformation
		cam_pos = glm::vec3(aux_mat * glm::vec4(cam_pos, 1.0));
		cam_up  = glm::vec3(aux_mat * glm::vec4(cam_up, 1.0));

		view = glm::lookAt(
			cam_pos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			cam_up
			);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	}

	px = x;
	if(py != 0)
	{
		glm::vec3 rot_axis = glm::cross(cam_up, cam_pos);
		glm::mat4 aux_mat = glm::rotate(
			glm::mat4(),
			((py - y) / 600.0f) * 180.0f,
			rot_axis
			);

		// Calculate transformation
		cam_pos = glm::vec3(aux_mat*glm::vec4(cam_pos, 1.0));
		cam_up  = glm::vec3(aux_mat*glm::vec4(cam_up, 1.0));

		view = glm::lookAt(
			cam_pos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			cam_up
			);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	}
	py = y;
	glutPostRedisplay();	
}

//----------------------------------------------------------------------------
void setup_slice_zero(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[i][2]+j] = TRUE;
				RC_animation_map[yellow_face[i][2]+j] = TRUE;
				RC_animation_map[orange_face[i][2]+j] = TRUE;
				RC_animation_map[white_face[i][2]+j] = TRUE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[green_face[i][j]+k] = TRUE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[red_face[i][2]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[red_face[i][2]+j], 1.0f));
				RC_points[yellow_face[i][2]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[yellow_face[i][2]+j], 1.0f));
				RC_points[orange_face[i][2]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[orange_face[i][2]+j], 1.0f));
				RC_points[white_face[i][2]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[white_face[i][2]+j], 1.0f));
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_points[green_face[i][j]+k] = glm::vec3(animation_matrix * glm::vec4(RC_points[green_face[i][j]+k], 1.0f));
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[i][2]+j] = FALSE;
				RC_animation_map[yellow_face[i][2]+j] = FALSE;
				RC_animation_map[orange_face[i][2]+j] = FALSE;
				RC_animation_map[white_face[i][2]+j] = FALSE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[green_face[i][j]+k] = FALSE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_zero_cw(int value)
{
	// initialty set points to be animated
	if(value == 90)
	{
		setup_slice_zero(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		// Calculate transformation
		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_zero_cw, value - 1);
	}
	else
	{
		setup_slice_zero(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[i][2];
			red_face[i][2] = yellow_face[i][2];
			yellow_face[i][2] = orange_face[i][2];
			orange_face[i][2] = white_face[i][2];
			white_face[i][2] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = green_face[i][j];
				green_face[i][j] = green_face[j][i];
				green_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = green_face[i][0];
			green_face[i][0] = green_face[i][2];
			green_face[i][2] = temp;
		}
		random_animate();
	}
}

void rotate_slice_zero_ccw(int value)
{
	// initialty set points to be animated
	if(value == 90)
	{
		setup_slice_zero(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_zero_ccw, value - 1);
	}
	else
	{
		setup_slice_zero(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[i][2];
			red_face[i][2] = white_face[i][2];
			white_face[i][2] = orange_face[i][2];
			orange_face[i][2] = yellow_face[i][2];
			yellow_face[i][2] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = green_face[i][j];
				green_face[i][j] = green_face[j][i];
				green_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = green_face[0][i];
			green_face[0][i] = green_face[2][i];
			green_face[2][i] = temp;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_one(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[i][1]+j] = TRUE;
				RC_animation_map[yellow_face[i][1]+j] = TRUE;
				RC_animation_map[orange_face[i][1]+j] = TRUE;
				RC_animation_map[white_face[i][1]+j] = TRUE;
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[red_face[i][1]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[red_face[i][1]+j], 1.0f));
				RC_points[yellow_face[i][1]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[yellow_face[i][1]+j], 1.0f));
				RC_points[orange_face[i][1]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[orange_face[i][1]+j], 1.0f));
				RC_points[white_face[i][1]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[white_face[i][1]+j], 1.0f));
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[i][1]+j] = FALSE;
				RC_animation_map[yellow_face[i][1]+j] = FALSE;
				RC_animation_map[orange_face[i][1]+j] = FALSE;
				RC_animation_map[white_face[i][1]+j] = FALSE;
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_one_cw(int value)
{
	if(value == 90)
	{
		setup_slice_one(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));
		
		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_one_cw, value - 1);
	}
	else
	{
		setup_slice_one(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[i][1];
			red_face[i][1] = yellow_face[i][1];
			yellow_face[i][1] = orange_face[i][1];
			orange_face[i][1] = white_face[i][1];
			white_face[i][1] = temp;
		}
		random_animate();
	}
}

void rotate_slice_one_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_one(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));
		
		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_one_ccw, value - 1);
	}
	else
	{
		setup_slice_one(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[i][1];
			red_face[i][1] = white_face[i][1];
			white_face[i][1] = orange_face[i][1];
			orange_face[i][1] = yellow_face[i][1];
			yellow_face[i][1] = temp;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_two(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[i][0]+j] = TRUE;
				RC_animation_map[yellow_face[i][0]+j] = TRUE;
				RC_animation_map[orange_face[i][0]+j] = TRUE;
				RC_animation_map[white_face[i][0]+j] = TRUE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[blue_face[i][j]+k] = TRUE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[red_face[i][0]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[red_face[i][0]+j], 1.0f));
				RC_points[yellow_face[i][0]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[yellow_face[i][0]+j], 1.0f));
				RC_points[orange_face[i][0]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[orange_face[i][0]+j], 1.0f));
				RC_points[white_face[i][0]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[white_face[i][0]+j], 1.0f));
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_points[blue_face[i][j]+k] = glm::vec3(animation_matrix * glm::vec4(RC_points[blue_face[i][j]+k], 1.0f));
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[i][0]+j] = FALSE;
				RC_animation_map[yellow_face[i][0]+j] = FALSE;
				RC_animation_map[orange_face[i][0]+j] = FALSE;
				RC_animation_map[white_face[i][0]+j] = FALSE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[blue_face[i][j]+k] = FALSE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_two_cw(int value)
{
	if(value == 90)
	{
		setup_slice_two(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));
		
		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_two_cw, value - 1);
	}
	else
	{
		setup_slice_two(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[i][0];
			red_face[i][0] = yellow_face[i][0];
			yellow_face[i][0] = orange_face[i][0];
			orange_face[i][0] = white_face[i][0];
			white_face[i][0] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = blue_face[i][j];
				blue_face[i][j] = blue_face[j][i];
				blue_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = blue_face[0][i];
			blue_face[0][i] = blue_face[2][i];
			blue_face[2][i] = temp;
		}
		random_animate();
	}
}

void rotate_slice_two_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_two(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));
		
		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_two_ccw, value - 1);
	}
	else
	{
		setup_slice_two(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[i][0];
			red_face[i][0] = white_face[i][0];
			white_face[i][0] = orange_face[i][0];
			orange_face[i][0] = yellow_face[i][0];
			yellow_face[i][0] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = blue_face[i][j];
				blue_face[i][j] = blue_face[j][i];
				blue_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = blue_face[i][0];
			blue_face[i][0] = blue_face[i][2];
			blue_face[i][2] = temp;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_three(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[green_face[i][0]+j] = TRUE;
				RC_animation_map[yellow_face[0][i]+j] = TRUE;
				RC_animation_map[blue_face[i][2]+j] =TRUE;
				RC_animation_map[white_face[2][i]+j] = TRUE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[red_face[i][j]+k] = TRUE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[green_face[i][0]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[green_face[i][0]+j], 1.0f));
				RC_points[yellow_face[0][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[yellow_face[0][i]+j], 1.0f));
				RC_points[blue_face[i][2]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[blue_face[i][2]+j], 1.0f));
				RC_points[white_face[2][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[white_face[2][i]+j], 1.0f));
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_points[red_face[i][j]+k] = glm::vec3(animation_matrix * glm::vec4(RC_points[red_face[i][j]+k], 1.0f));
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[green_face[i][0]+j] = FALSE;
				RC_animation_map[yellow_face[0][i]+j] = FALSE;
				RC_animation_map[blue_face[i][2]+j] =FALSE;
				RC_animation_map[white_face[2][i]+j] = FALSE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[red_face[i][j]+k] = FALSE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_three_cw(int value)
{
	if(value == 90)
	{
		setup_slice_three(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_three_cw, value - 1);
	}
	else
	{		
		setup_slice_three(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = green_face[i][0];
			green_face[i][0] = white_face[2][i];
			white_face[2][i] = blue_face[2 - i][2];
			blue_face[2 - i][2] = yellow_face[0][2 - i];
			yellow_face[0][2 - i] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = red_face[i][j];
				red_face[i][j] = red_face[j][i];
				red_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = red_face[i][0];
			red_face[i][0] = red_face[i][2];
			red_face[i][2] = temp;
		}
		random_animate();
	}
}

void rotate_slice_three_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_three(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_three_ccw, value - 1);
	}
	else
	{
		setup_slice_three(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = yellow_face[0][2 - i];
			yellow_face[0][2 - i] = blue_face[2 - i][2];
			blue_face[2 - i][2] = white_face[2][i];
			white_face[2][i] = green_face[i][0];
			green_face[i][0] =  temp ;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = red_face[i][j];
				red_face[i][j] = red_face[j][i];
				red_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = red_face[0][i];
			red_face[0][i] = red_face[2][i];
			red_face[2][i] = temp;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_four(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[green_face[i][1]+j] = TRUE;
				RC_animation_map[yellow_face[1][i]+j] = TRUE;
				RC_animation_map[blue_face[i][1]+j] = TRUE;
				RC_animation_map[white_face[1][i]+j] =TRUE;
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[green_face[i][1]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[green_face[i][1]+j], 1.0f));
				RC_points[yellow_face[1][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[yellow_face[1][i]+j], 1.0f));
				RC_points[blue_face[i][1]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[blue_face[i][1]+j], 1.0f));
				RC_points[white_face[1][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[white_face[1][i]+j], 1.0f));
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[green_face[i][1]+j] = FALSE;
				RC_animation_map[yellow_face[1][i]+j] = FALSE;
				RC_animation_map[blue_face[i][1]+j] = FALSE;
				RC_animation_map[white_face[1][i]+j] =FALSE;
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_four_cw(int value)
{
	if(value == 90)
	{
		setup_slice_four(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_four_cw, value - 1);
	}
	else
	{
		setup_slice_four(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = green_face[i][1];
			green_face[i][1] = white_face[1][i];
			white_face[1][i] = blue_face[2 - i][1];
			blue_face[2 - i][1] = yellow_face[1][2 - i];
			yellow_face[1][2 - i] = temp;
		}
		random_animate();
	}
}

void rotate_slice_four_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_four(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_four_ccw, value - 1);
	}
	else
	{
		setup_slice_four(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = yellow_face[1][2 - i];
			yellow_face[1][2 - i] = blue_face[2 - i][1];
			blue_face[2 - i][1] = white_face[1][i];
			white_face[1][i] = green_face[i][1];
			green_face[i][1] =  temp ;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_five(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[green_face[i][2]+j] = TRUE;
				RC_animation_map[yellow_face[2][i]+j] = TRUE;
				RC_animation_map[blue_face[i][0]+j] = TRUE;
				RC_animation_map[white_face[0][i]+j] = TRUE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[orange_face[i][j]+k] = TRUE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[green_face[i][2]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[green_face[i][2]+j], 1.0f));
				RC_points[yellow_face[2][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[yellow_face[2][i]+j], 1.0f));
				RC_points[blue_face[i][0]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[blue_face[i][0]+j], 1.0f));
				RC_points[white_face[0][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[white_face[0][i]+j], 1.0f));
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_points[orange_face[i][j]+k] = glm::vec3(animation_matrix * glm::vec4(RC_points[orange_face[i][j]+k], 1.0f));
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[green_face[i][2]+j] = FALSE;
				RC_animation_map[yellow_face[2][i]+j] = FALSE;
				RC_animation_map[blue_face[i][0]+j] = FALSE;
				RC_animation_map[white_face[0][i]+j] = FALSE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[orange_face[i][j]+k] = FALSE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_five_cw(int value)
{
	if(value == 90)
	{
		setup_slice_five(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_five_cw, value - 1);
	}
	else
	{
		setup_slice_five(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = green_face[i][2];
			green_face[i][2] = white_face[0][i];
			white_face[0][i] = blue_face[2 - i][0];
			blue_face[2 - i][0] = yellow_face[2][2 - i];
			yellow_face[2][2 - i] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = orange_face[i][j];
				orange_face[i][j] = orange_face[j][i];
				orange_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = orange_face[0][i];
			orange_face[0][i] = orange_face[2][i];
			orange_face[2][i] = temp;
		}
		random_animate();
	}
}

void rotate_slice_five_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_five(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_five_ccw, value - 1);
	}
	else
	{
		setup_slice_five(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = yellow_face[2][2 - i];
			yellow_face[2][2 - i] = blue_face[2 - i][0];
			blue_face[2 - i][0] = white_face[0][i];
			white_face[0][i] = green_face[i][2];
			green_face[i][2] =  temp ;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = orange_face[i][j];
				orange_face[i][j] = orange_face[j][i];
				orange_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = orange_face[i][0];
			orange_face[i][0] = orange_face[i][2];
			orange_face[i][2] = temp;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_six(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[2][i]+j] = TRUE;
				RC_animation_map[green_face[2][i]+j] = TRUE;
				RC_animation_map[orange_face[0][i]+j] = TRUE;
				RC_animation_map[blue_face[2][i]+j] = TRUE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[yellow_face[i][j]+k] = TRUE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[red_face[2][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[red_face[2][i]+j], 1.0f));
				RC_points[green_face[2][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[green_face[2][i]+j], 1.0f));
				RC_points[orange_face[0][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[orange_face[0][i]+j], 1.0f));
				RC_points[blue_face[2][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[blue_face[2][i]+j], 1.0f));
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_points[yellow_face[i][j]+k] = glm::vec3(animation_matrix * glm::vec4(RC_points[yellow_face[i][j]+k], 1.0f));
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[2][i]+j] = FALSE;
				RC_animation_map[green_face[2][i]+j] = FALSE;
				RC_animation_map[orange_face[0][i]+j] = FALSE;
				RC_animation_map[blue_face[2][i]+j] = FALSE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[yellow_face[i][j]+k] = FALSE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_six_cw(int value)
{
	if(value == 90)
	{
		setup_slice_six(MODE_SET);
	}
	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(1.0f, 0.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_six_cw, value - 1);
	}
	else
	{
		setup_slice_six(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[2][i];
			red_face[2][i] = blue_face[2][i];
			blue_face[2][i] = orange_face[0][2-i];
			orange_face[0][2-i] = green_face[2][i];
			green_face[2][i] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = yellow_face[i][j];
				yellow_face[i][j] = yellow_face[j][i];
				yellow_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = yellow_face[i][0];
			yellow_face[i][0] = yellow_face[i][2];
			yellow_face[i][2] = temp;
		}
		random_animate();
	}
}

void rotate_slice_six_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_six(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(1.0f, 0.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_six_ccw, value - 1);
	}
	else
	{
		setup_slice_six(MODE_RESET);

		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = green_face[2][i] ;
			green_face[2][i] = orange_face[0][2 - i];
			orange_face[0][2 - i] = blue_face[2][i];
			blue_face[2][i] = red_face[2][i];
			red_face[2][i] = temp ;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = yellow_face[i][j];
				yellow_face[i][j] = yellow_face[j][i];
				yellow_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = yellow_face[0][i];
			yellow_face[0][i] = yellow_face[2][i];
			yellow_face[2][i] = temp;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_seven(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[1][i]+j] = TRUE;
				RC_animation_map[green_face[1][i]+j] = TRUE;
				RC_animation_map[orange_face[1][i]+j] = TRUE;
				RC_animation_map[blue_face[1][i]+j] = TRUE;
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[red_face[1][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[red_face[1][i]+j], 1.0f));
				RC_points[green_face[1][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[green_face[1][i]+j], 1.0f));
				RC_points[orange_face[1][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[orange_face[1][i]+j], 1.0f));
				RC_points[blue_face[1][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[blue_face[1][i]+j], 1.0f));
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[1][i]+j] = FALSE;
				RC_animation_map[green_face[1][i]+j] = FALSE;
				RC_animation_map[orange_face[1][i]+j] = FALSE;
				RC_animation_map[blue_face[1][i]+j] = FALSE;
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_seven_cw(int value)
{
	if(value == 90)
	{
		setup_slice_seven(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(1.0f, 0.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_seven_cw, value - 1);
	}
	else
	{
		setup_slice_seven(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[1][i];
			red_face[1][i] = blue_face[1][i];
			blue_face[1][i] = orange_face[1][2-i];
			orange_face[1][2-i] = green_face[1][i];
			green_face[1][i] = temp;
		}
		random_animate();
	}
}

void rotate_slice_seven_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_seven(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(1.0f, 0.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_seven_ccw, value - 1);
	}
	else
	{
		setup_slice_seven(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = green_face[1][i] ;
			green_face[1][i] = orange_face[1][2 - i];
			orange_face[1][2 - i] = blue_face[1][i];
			blue_face[1][i] = red_face[1][i];
			red_face[1][i] = temp ;
		}
		random_animate();
	}
}

//----------------------------------------------------------------------------
void setup_slice_eight(int setup_mode)
{
	if(setup_mode == MODE_SET)
	{
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[0][i]+j] = TRUE;
				RC_animation_map[green_face[0][i]+j] = TRUE;
				RC_animation_map[orange_face[2][i]+j] = TRUE;
				RC_animation_map[blue_face[0][i]+j] = TRUE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[white_face[i][j]+k] = TRUE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
	else if (setup_mode == MODE_RESET)
	{
		// Update points positions after animation
		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_points[red_face[0][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[red_face[0][i]+j], 1.0f));
				RC_points[green_face[0][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[green_face[0][i]+j], 1.0f));
				RC_points[orange_face[2][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[orange_face[2][i]+j], 1.0f));
				RC_points[blue_face[0][i]+j] = glm::vec3(animation_matrix * glm::vec4(RC_points[blue_face[0][i]+j], 1.0f));
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_points[white_face[i][j]+k] = glm::vec3(animation_matrix * glm::vec4(RC_points[white_face[i][j]+k], 1.0f));
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, vertices_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_points), RC_points, GL_STATIC_DRAW );

		// Reset angle and animation map
		rot_angle = 0.0;

		for(int j = 0 ; j < 12 ; j++)
		{
			for(int i = 0; i < 3; i++){
				RC_animation_map[red_face[0][i]+j] = FALSE;
				RC_animation_map[green_face[0][i]+j] = FALSE;
				RC_animation_map[orange_face[2][i]+j] = FALSE;
				RC_animation_map[blue_face[0][i]+j] = FALSE;
			}
		}

		for(int i = 0; i <3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				for(int k = 0; k < 12; k++)
				{
					RC_animation_map[white_face[i][j]+k] = FALSE;
				}
			}
		}

		glBindBuffer( GL_ARRAY_BUFFER, animation_map );
		glBufferData( GL_ARRAY_BUFFER, sizeof(RC_animation_map), RC_animation_map, GL_STATIC_DRAW );
	}
}

void rotate_slice_eight_cw(int value)
{
	if(value == 90)
	{
		setup_slice_eight(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			-(++rot_angle),
			glm::vec3(1.0f, 0.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_eight_cw, value - 1);
	}
	else
	{
		setup_slice_eight(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = red_face[0][i];
			red_face[0][i] = blue_face[0][i];
			blue_face[0][i] = orange_face[2][2-i];
			orange_face[2][2-i] = green_face[0][i];
			green_face[0][i] = temp;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = white_face[i][j];
				white_face[i][j] = white_face[j][i];
				white_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = white_face[0][i];
			white_face[0][i] = white_face[2][i];
			white_face[2][i] = temp;
		}
		random_animate();
	}
}

void rotate_slice_eight_ccw(int value)
{
	if(value == 90)
	{
		setup_slice_eight(MODE_SET);
	}

	if(value >0)
	{
		animation_matrix = glm::rotate(
			glm::mat4(),
			(++rot_angle),
			glm::vec3(1.0f, 0.0f, 0.0f)
			);
		glUniformMatrix4fv(uniAnimationMatrix, 1, GL_FALSE, glm::value_ptr(animation_matrix));

		glutPostRedisplay();
		glutTimerFunc(ANIMATION_SPEED, rotate_slice_eight_ccw, value - 1);
	}
	else
	{
		setup_slice_eight(MODE_RESET);
		// Swap Points & Colors Arrays
		GLuint temp ;

		for (int i = 0 ; i < 3 ; i++)
		{
			temp = green_face[0][i] ;
			green_face[0][i] = orange_face[2][2 - i];
			orange_face[2][2 - i] = blue_face[0][i];
			blue_face[0][i] = red_face[0][i];
			red_face[0][i] = temp ;
		}

		// Transpose Face
		for(int i = 0; i < 2; i++)
		{
			for(int j=i+1 ; j < 3 ; j++)
			{
				temp = white_face[i][j];
				white_face[i][j] = white_face[j][i];
				white_face[j][i] = temp;
			}
		}

		for(int i = 0 ; i < 3;i++)
		{
			temp = white_face[i][0];
			white_face[i][0] = white_face[i][2];
			white_face[i][2] = temp;
		}
		random_animate();
	}
}

void reshape(int w, int h) 
{
	glutPostRedisplay();
}

int	main( int argc, char **argv )
{
	// Init OpenGl
	glutInit( &argc, argv );


	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH| GLUT_MULTISAMPLE);
	glutInitWindowSize( WindowWidth, WindowHeight);

	// Create Drawing Window
	glutCreateWindow( "Rubiks Cube" );
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Init Game
	init();

	// Set Display Main Function
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );

	// Set Mouse Callback functions
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);
	glutSpecialFunc( specialKey );
	
	//set timer callback function
	glutMainLoop();
	return 0;
}