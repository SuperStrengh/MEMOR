#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>

#include "Dialog.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "ParticleSystem.hpp"
#include "Skybox.hpp"
//#include "MousePicker.hpp"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include<stb/stb_image.h>


int width = 1280;
int height = 720;

int normalWidth = 1280;
int normalHeight = 720;

int MouseposX = 0;
int MouseposY = 0;

bool fullscreen = false;

Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f), 0.1f, 500.0f);

glm::vec3 Position = glm::vec3(0.0f, 14.6f, -20.0f);

std::vector <Model> RenderedModels;
Model Human;
Model glow;

Particles DustParticle;
Particles SmokeParticle;

float speed = 0.4;

bool PressedLeft = false;
bool PressedRight = false;
bool PressedUp = false;
bool PressedDown = false;

bool PressedEkey = false;

float fps = 0;

double prevTime = 0.0;
double crntTime = 0.0;
double timeDiff;
// Keeps track of the amount of frames in timeDiff
unsigned int counter = 0;

glm::mat4 Textprojection = glm::ortho(0.0f, 1 - static_cast<float>(width), 0.0f, static_cast<float>(height));

void handleMouseMovement(int x, int y)
{
	int MouseposX = x;
	int MouseposY = y;
}

void handleKeypress(unsigned char key, int x, int y) 
{
	switch (key) {
		case 101://e key
			PressedEkey = true;

			break;
		case 45: //- key
			if(!fullscreen)
			{
				glutFullScreen();
			}else{
				const int pos_x = glutGet(GLUT_SCREEN_WIDTH);
    			const int pos_y = glutGet(GLUT_SCREEN_HEIGHT);
				
				glutReshapeWindow(normalWidth, normalHeight);
    			glutPositionWindow(50, 20);
			}

			fullscreen = !fullscreen;
		 	
			break;
		case 95: //_ key
			if(!fullscreen)
			{
				glutFullScreen();
			}else{
				const int pos_x = glutGet(GLUT_SCREEN_WIDTH);
    			const int pos_y = glutGet(GLUT_SCREEN_HEIGHT);

				glutReshapeWindow(normalWidth, normalHeight);
    			glutPositionWindow(50, 20);
			}

			fullscreen = !fullscreen;
		 	
			break;
		case 119: //W key
			PressedUp = true;

			break;
		case 97: //A key
			PressedRight = true;

			break;
		case 115: //S key
			PressedDown = true;

			break;
		case 100: //D key
			PressedLeft = true;

			break;
	}

	SetKeyStatus(PressedRight, PressedLeft, PressedUp, PressedDown);
}

void handleUpKeypress(unsigned char key, int x, int y) 
{
	switch (key) {
		case 101://e key
			PressedEkey = false;
			
			break;
		case 119: //W key
			PressedUp = false;

			break;
		case 97: //A key
			PressedRight = false;

			break;
		case 115: //S key
			PressedDown = false;

			break;
		case 100: //D key
			PressedLeft = false;

			break;
	}

	SetKeyStatus(PressedRight, PressedLeft, PressedUp, PressedDown);
}


void initRendering() 
{
	InitAudio();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	glEnable(GL_ALPHA_TEST);
	glClearColor(0.0f, 0.75f, 0.75f, 0.0);

	PlaySFX("res/dev/MEMOR_Intro.wav");

	glMatrixMode(GL_TEXTURE_MATRIX);
	glLoadIdentity();
	
	glScalef(-1.0, 1.0, 1.0);

	Human = Model("res/gfx/penguin.gltf", Position,glm::quat(-0.25f, 0.0f, 0.0f, 1.0f),glm::vec3(0.62f, 0.56f, 0.57f), "Human/FrontAnimation/penguin1.png", "Outlined");
	glow = Model("res/gfx/Ball.gltf" , glm::vec3(0.0f, 18.0f, -30.0f),glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), " ", "Glow");

	DustParticle = Particles(1.2f, 0.4f, "res/gfx/Dust.png", glm::vec3(0.0f, 16.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 0.9f), glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec2(1.5f, 1.5f), glm::vec2(1.0f, 1.0f));

	SmokeParticle = Particles(14.0f, 3.0f, "res/gfx/Dust.png", glm::vec3(0.0f, 18.0f, 0.0f), glm::vec3(0.0f, 15.0f, 0.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 0.8f), glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec2(1.25f, 1.25f), glm::vec2(3.25f, 2.75f));

	SetupModels();

	SetupDialog("res/dialog/dialog.json");

	FontRenderSetup();
	
	SetupSkyboxTextures();
}

void handleResize(int w, int h) 
{
	if(w != 0 && h != 0)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.0, (float)w / (float)h, 1.0f, 500.0f);

		width = w;
		height = h;
			
		if(!fullscreen)
		{
			normalWidth = w;
			normalHeight = h;
		}
	}
}

void drawScene() 
{
	if(fps != 0)
	{
		speed = 16 / fps;
		
		SetSprite(fps, PlayerInputs(speed, Position), Position);

		if(PlayerInputs(speed, Position) != Position)
		{
			DustParticle.Enabled = true;
			DustParticle.ParcStartPosition = Position + glm::vec3(0.0f, 1.5f, -1.25f);


			Position = PlayerInputs(speed, Position);
			Human.Move(Position);
		}else{
			DustParticle.Enabled = false;
		}
	}

	std::map<int, Model> Models = GetModels();
	
	camera.updateMatrix(45.0f, width, height, Position);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.75f, 0.75f, 0.0);
	
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();

	DrawSkybox(camera.cameraMatrix * glm::translate(camera.Position));

	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = {0.0f, 1.0f, 1.0f, 0.0f};
	GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 0.0f};

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	//Draw Normal Models

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	for(int i = Models.size() - 1; i >= 0; i--)
	{
		if(Models[i].DrawType == "Default" && Models[i].IsNPC != true)
		{
			Models[i].Draw("Default", camera);
		}
	}

	//Draw Outlined Models
	
	glDisable(GL_DEPTH_TEST);


	for(int i = Models.size() - 1; i >= 0; i--)
	{
		if(Models[i].DrawType == "Outlined" || Models[i].IsNPC == true)
		{
			Models[i].Draw("Outlined", camera);
		}
	}

	Human.Draw("Outlined", camera);

	//Draw Main Models

	glEnable(GL_DEPTH_TEST);


	for(int i = Models.size() - 1; i >= 0; i--)
	{
		if(Models[i].DrawType == "Outlined" || Models[i].IsNPC == true)
		{
			Models[i].Draw("Default", camera);
		}
	}

	Human.Draw("Default", camera);

	//Draw Glowing Models

	glDisable(GL_LIGHTING);


	ClearLightBuffer();

	glow.Draw("Glow", camera);

	//Particles

	DustParticle.DrawParticles((camera.cameraMatrix), camera.LookDirection);

	SmokeParticle.DrawParticles((camera.cameraMatrix), camera.LookDirection);

	//Draw Text

	GetDialogInput(PressedEkey, Position);

	RenderDialog(fps, Textprojection, camera, Position);


	glEnable(GL_LIGHTING);

	glutSwapBuffers();
}

//Called every frame

void update() 
{
	crntTime = glutGet(GLUT_ELAPSED_TIME);
	timeDiff = crntTime - prevTime;
	counter++;

	if (timeDiff > 0)
	{
		fps = counter * 1000.0f / timeDiff;
		std::string FPS = std::to_string(fps);
		std::string textspeed = std::to_string(counter);
		std::string newTitle = "MEMOR - " + FPS + " FPS / " + textspeed + " counter";
		glutSetWindowTitle(newTitle.c_str());

		// Resets times and counter
		prevTime = crntTime;
		counter = 0;
	}

	Human.ChangeTexture(GetSprite());

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	const int pos_x = glutGet(GLUT_SCREEN_WIDTH);
	const int pos_y = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 20);

	glutCreateWindow("MEMOR");

	initRendering();
	
	glutDisplayFunc( drawScene);

	glutKeyboardFunc( handleKeypress);
	glutKeyboardUpFunc( handleUpKeypress);
	glutIdleFunc( update);
	glutReshapeFunc( handleResize);

	glutPassiveMotionFunc( handleMouseMovement );

	glutMainLoop();
	return 0;
}