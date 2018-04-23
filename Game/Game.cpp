// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/Functions.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <math.h>
#include <Windows.h>
#include <mmsystem.h>
using namespace glm;
using namespace std;

//Variables
//Ids from shaders
GLuint programID; //basic id for shaders
GLuint programID2; //basic id2 for shaders
GLuint vertexPosition_modelspaceID; //basic id for shaders
GLuint vertexUVID; //basic id for shaders
GLuint vertexNormal_modelspaceID; //basic id for shaders
GLuint MatrixID; //basic id for shaders
GLuint ViewMatrixID; //basic id for shaders
GLuint ModelMatrixID; //basic id for shaders
GLuint TextureID; //basic id for shaders
GLuint LightID; //basic id for shaders
GLuint LightID2; //basic id2 for shaders
GLuint TextureTimer;//Texture for timer

//matrices used to calcutlate rotation,translation,scaling,projection,view and vector to put light position
mat4 Projection = getProjectionMatrix(); //get the projection of the camera
mat4 View = getViewMatrix(); //get the view of the camera
mat4 RotationMatrix; //this is the rotation matrix that is used for every object to rotate it with amount of value
mat4 RotationMatrix2; //this is a second rotation matrix that is used for every object to rotate it with amount of value
mat4 TranslationMatrix; // this is the translation matrix that is used for every object to translate it with amount of value
mat4 ScalingMatrix; //this is the scaling matrix that is used for every object to scale it with amount of value
mat4 Model; // translation matrix * rotation matrix * scaling matrix for every object
mat4 MVP; // Projection * View * Model for every object
vec3 lightPos; //the postion of the light

//Player
float speed; //speed of the inputs when you click on it
float positionx; //postion of the player in the x coordinates 
float positiony; //postion of the player in the y coordinates 
float positionz; //postion of the player in the z coordinates 
float MoveAsteroid; // the Asteroid translation in z axis
bool MovementRightx; //check on the right x axis movement for the player
bool MovementRighty; //check on the right y axis movement for the player
bool MovementRightz; //check on the right z axis movement for the player
bool MovementLeftx; //check on the left x axis movement for the player
bool MovementLefty; //check on the left y axis movement for the player
bool MovementLeftz; //check on the left z axis movement for the player
float SwingInitial; // swing the plane intial value
float Swing; // the current swing of the plane 
float RotationSwingz; // the value of rotation in forward and backward
float SwingUpDownInitial;
float SwingUpDown;
float RotationSwingx; // the value of rotation in right and left of the plane
bool drawPlayer; //check if we should draw the player or not

//Asteroid
float effectiveyup; //the Asteroid postion in the y axis 
float effectiveydown; //the Asteroid postion in the y axis 
float MovingSpeed; //the scene moving speed
float UpDownAsteroidSpeed; //the speed of the Asteroid up and down on the y axis
float AsteroidRotationx; // Asteroid rotation on the x axis
float AsteroidRotationy; // Asteroid rotation on the y axis
float AsteroidRotationz; // Asteroid rotation on the z axis
float AsteroidScale; // Asteroid Scale uniformally
bool reached; // if the Asteroid reached to max or min hight in y axis
float orientation; //a value that change every time to rotate different angles every time
bool drawAsteroids; //check if we should draw asteroids or not
float xright; //position of the right asteroids on x axis
float xleft;//position of the left asteroids on x axis

//background and street
float MoveStreet = 0; // the street translation in z axis
float resizex = 25; // the scale size of the background in x axis
float resizey = 18.5f; // the scale size of the background in y axis
GLuint Texture3; // for background texture
GLuint Texture4; // for the road texture
GLuint Texture1; // road texture in intial
GLuint Texture; // texture used in the change state
GLuint vertexbuffer3; //the buffer value of the background vertices
GLuint uvbuffer3; //the buffer value of the background uvs
GLuint normalbuffer3; //the buffer value of the background normal
vector<vec3> vertices[5]; //holds the vertuces of all objects
vector<vec2> uvs[5]; //holds the uvs of all objects
vector<vec3> normals[5]; //holds the normals of all objects
bool drawastreet;  //if the street should be drawn or not

//levels,time and pause
bool level1; //level 1 in the game
bool level2; //level 2 in the game
bool level3; //level 3 in the game
bool level4; //level 4 in the game
bool level5; //level 5 in the game
bool Pause; //check if the game is paused or not
float PauseTimeLimit; //this is a value of timer before the timer paused
float PauseSpeed; //used in the pause to store the moving speed
float PauseUpDownAsteroidSpeed; // used in the pause to store the speed of up and down Asteroids
float timer; //timer of the game
static double lastTime; //used to calculate the last frame
double currentTime; // used to get the current frame
float deltaTime; // used to get the time between them
vector<bool> State; //holds all the states in the game
					//0 : lost
					//1 : background is the stars photo
					//2 : background is the avengers trailer
					//5 : street texture's is the avengers trailer
					//9 : won 
bool End;
bool rightx; // determine if the right asteroids should move right or not
bool leftx; // determine if the left asteroids should move right or not
bool hardright; //determine if the right asteroids should move in x axis or not
bool hardleft; //determine if the left asteroids should move in x axis or not

//frames
string VideoString; //the place name for the frame
int LosingCounter; //counter to the frames of the losing video
int WinningCounter; //counter to the frames of the winning video
int VideoMode1Counter; //counter to the frames of the avenger video
int VideoMode2Counter; //counter to the frames of the illiosion video
int VideoMode3Counter; //counter to the frames of the space video


bool one;
float timeflicker;
bool DrawTime;
int timercounter;
float timetimer;
//int numberofflickersperloop = 10;
//Functions

//load object and bind it in  the buffer
void LoadObject(char* Object, char* Texture, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer, vector<vec3>& vertices, vector<vec2>& uvs, vector<vec3>& normals);
//bind and generate buffers for the object
void Buffer(GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer, vector<vec3> vertices, vector<vec2> uvs, vector<vec3> normals);
//active the current buffer
void Binding(GLuint verticesbuffer, GLuint uvsbuffer, GLuint normalsbuffer);
//refresh the time between current and last frame
void RefreshDeltaTime();
//uniform matrices with the id from shaders
void UniformMatrices(mat4& MVP, mat4& Model, mat4& View);
//active texture and bind it
void BindingTextures(GLuint Texture);
//disable ids from shaders
void DisableVertex();
//change the current frame and reset some changes
void ChangeLoop();
//refresh the the current frame
void Refresh();
//handle all the initial opening in the game
int OpenWindow();
//handle the close of the program
void CloseWindow();
//handle the buffers at the end of the game
void CleanBuffer(GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
//draw the current object on the screen
void Draw(GLuint ProgramId, GLuint lightId,vector<vec3> vertices, int LightX, int LightY, int LightZ, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
//take inputs from the user
void Inputs();
//draw the street
void DrawStreet(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
//draw the Asteroids
void DrawAsteroids(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
//draw the background
void DrawBackGround(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
//draw the player
void DrawPlayer(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
//draw the Timer
void DrawTimer(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
//detect collision
bool Collision(float x, float y, float z);
//change levels in the game
void Levels();
//check the x,y and z axis boarders
void CheckBoarders();
//check if the game is paused and reset the values of the game
void CheckPause();
//handle the rotation of the plane
void Wings(float& value);
//handle the animation of the plane
void PlayerAnimation(float& Initial, float& Current, float max);
//handle more than input on hte plane
void WingsAnimation();
//play a music
void MainSound();
//active specific state
void ActiveState(int j,int min,int max);
//change the state
void ChangeState();
//used in background videos
void VideoImport(string directory, GLuint& TextureId, int& value);
//Initialize the variables
void Initialization();
//Move The Object right and left
void MoveRightLeft(float& xright, float& xleft);
//screenflicker
GLuint flicker(GLuint ProgramId, GLuint lightId);

void main()
{
	
	OpenWindow();


	// Asteroids
	GLuint Texture;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	LoadObject("AsteroidObject/A2.obj","AsteroidObject/Main_Defuse.bmp", Texture, vertexbuffer, uvbuffer, normalbuffer,vertices[0],uvs[0],normals[0]);

	//Road
	
	GLuint vertexbuffer1;
	GLuint uvbuffer1;
	GLuint normalbuffer1;
	LoadObject("BackGroundObject/Windowobj.obj", "StreetObject/king.bmp", Texture1, vertexbuffer1, uvbuffer1, normalbuffer1, vertices[1], uvs[1], normals[1]);

	//Player
	GLuint Texture2;
	GLuint vertexbuffer2;
	GLuint uvbuffer2;
	GLuint normalbuffer2;
	LoadObject("PlayerObject/PlayerObject.obj", "PlayerObject/PlayerTexture.bmp", Texture2, vertexbuffer2, uvbuffer2,normalbuffer2, vertices[2], uvs[2], normals[2]);

	//BackGround
	LoadObject("BackGroundObject/Windowobj.obj", "BackGroundObject/BackGround1.bmp", Texture3,
		vertexbuffer3, uvbuffer3, normalbuffer3, vertices[3], uvs[3], normals[3]);



	do{
		Refresh();
		ChangeState();
		DrawPlayer(vertices[2], Texture2, vertexbuffer2, uvbuffer2, normalbuffer2);
		DrawAsteroids(vertices[0], Texture, vertexbuffer, uvbuffer, normalbuffer);				
		DrawStreet(vertices[1], Texture4, vertexbuffer1, uvbuffer1, normalbuffer1);
		DrawTimer(vertices[3], TextureTimer, vertexbuffer3, uvbuffer3, normalbuffer3);
		ChangeLoop();
		
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) && !End );
	//clean the buffer
	CleanBuffer(Texture1, vertexbuffer1, uvbuffer1, normalbuffer1);
	CleanBuffer(Texture2, vertexbuffer2, uvbuffer2, normalbuffer2);
	CleanBuffer(Texture3, vertexbuffer3, uvbuffer3, normalbuffer3);


	CloseWindow();
}

void DrawPlayer(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{
	if (!drawPlayer)
		return;
	//rotate the plane according to the plane animation
	RotationMatrix = eulerAngleYXZ(0.0f, RotationSwingx, RotationSwingz);//yaw, pitch and roll. Measured in radians
	//translate the plane from inputs
	TranslationMatrix = translate(mat4(), vec3(positionx, positiony, positionz));
	//scale the plane
	ScalingMatrix = scale(mat4(), vec3(0.0005f, 0.0005f, 0.0005f));
	//draw the plane
	Draw(programID,LightID,vertices, 4, 7, 0, TextureId, verticesbuffer, uvsbuffer, normalsbuffer);
}
void DrawBackGround(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{
	//rotation of the background
	RotationMatrix = eulerAngleYXZ(3.14f, 11.0f, 0.0f);
	//the position of the background
	TranslationMatrix = translate(mat4(), vec3(0, 0, -41));
	//the scaling of the background
	ScalingMatrix = scale(mat4(), vec3(28, 0.0f, 19.0f));
	//draw the background
	Draw(programID,LightID,vertices, 4, 4, -70, TextureId, verticesbuffer, uvsbuffer, normalsbuffer);
}
void DrawAsteroids(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{
	if (State[9] || !drawAsteroids)
		return;
	float Initial = MoveAsteroid;

	//Maximum Move In Height
	float YMAX = 4;

	MoveRightLeft(xright,xleft);
	float effectivex;
	//Initial Position In Y
	float starty;
	//Translation In Y
	float moviny;
	bool first = true;
	//counter to determine left and right Asteroid
	int counter = 1;
	
	if (!Pause)
	{
		orientation += 3.14159f / 2.0f * deltaTime;
		AsteroidRotationz = orientation;
		AsteroidRotationx = 2 * orientation;
	}
	if (!level2&&!Pause)
	{
		orientation += 3.14159f / 4.0f * deltaTime;
		AsteroidRotationz = orientation;
	}
	
	for (int i = 0; i < 7; i++)
	{
		//if the Asteroid acrosses the view position then don't draw it
		if (MoveAsteroid < 12)
		{
			//determine the right and left Asteroid
			if (counter % 2 == 0)
			{
				//transleat the right asroid to the right
				effectivex = xright;
				//if the Asteroid reached to the max or min height then reverse the movement
				if (reached)
				{
					starty = 2;
					moviny = effectiveydown + starty;
				}
				else
				{
					starty = -2;
					moviny = effectiveyup + starty;
				}

			}
			else
			{
				//translate the left Asteroid to the left
				effectivex = xleft;
				//if the Asteroid reached to the max or min height then reverse the movement
				if (reached)
				{
					starty = -2;
					moviny = effectiveyup + starty;
				}
				else
				{
					starty = 2;
					moviny = effectiveydown + starty;
				}
			}
			//check if the Asteroid reached to the maximum height then make the the check reach to true
			if (abs(effectiveyup) >= YMAX)
			{
				effectiveyup = 0;
				effectiveydown = 0;
				if (reached)
					reached = false;
				else
					reached = true;
			}

			//rotate the Asteroid around itself
			RotationMatrix = eulerAngleYXZ(AsteroidRotationy, AsteroidRotationx, AsteroidRotationz);//yaw, pitch and roll. Measured in radians
			//translate the Asteroid 
			TranslationMatrix = translate(mat4(), vec3(effectivex, moviny, MoveAsteroid));			
			//scale the Asteroid
			ScalingMatrix = scale(mat4(), vec3(AsteroidScale, AsteroidScale, AsteroidScale));
			//check collision
			if (first)
				first = Collision(effectivex, moviny, MoveAsteroid);
			//draw the Asteroid
				Draw(programID2,LightID2,vertices, 2, 4, -1, TextureId, verticesbuffer, uvsbuffer, normalsbuffer);
		}
		else
		{
			//minus the i to make an infinite loop
			i--;
		}
		//put the next Asteroid with distance on z axis equal to 
		MoveAsteroid -= 10.0f;
		//increment the counter
		counter++;
	}
	//reset the Asteroid movement for this frame
	MoveAsteroid = Initial;
	//translate it in z axis with an amount of value
	MoveAsteroid += MovingSpeed * deltaTime;
	//translate it in y axis with an amount of value
	effectiveydown -= UpDownAsteroidSpeed * deltaTime;
	effectiveyup += UpDownAsteroidSpeed *deltaTime;
}
void DrawStreet(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{
	if (!drawastreet)
		return;
	//get the initial position of the street in the z axis
	float Initial = MoveStreet;
	for (int i = 0; i < 7; i++)
	{
		//if the street acrosses the view position then don't draw it
		if (MoveStreet < 12)
		{
			RotationMatrix = eulerAngleYXZ(0.0f, 0.0f, 0.0f);//yaw, pitch and roll. Measured in radians
			TranslationMatrix = translate(mat4(), vec3(0, -3.5f, MoveStreet));
			ScalingMatrix = scale(mat4(), vec3(4.5f, 0.0f, 4.4f));
			Draw(programID,LightID,vertices, 4, 4, MoveStreet, TextureId, verticesbuffer, uvsbuffer, normalsbuffer);
		}
		else
			//to make it infinite
			i--;
		//move the position to draw the next street
		MoveStreet -= 8.7f;
	}
	//reset the position
	MoveStreet = Initial;
	//move the position forward a little bit
	MoveStreet += MovingSpeed * deltaTime;
}
void DrawTimer(vector<vec3> vertices, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{

	if (int(timetimer - timer) != 0)
	{
		timetimer = timer;
		VideoImport("BackGroundObject/Timer/1 (", TextureTimer, timercounter);
	}

	if (!DrawTime)
		return;

	//rotation of the Timer
	RotationMatrix = eulerAngleYXZ(3.14f, 11.0f, 0.0f);
	//the position of the Timer
	TranslationMatrix = translate(mat4(), vec3(0, 15, -40));
	//the scaling of the Timer
	ScalingMatrix = scale(mat4(), vec3(3, 0.0f, 3.0f));
	//draw the Timer
	Draw(programID, LightID, vertices, 4, 4, -70, TextureTimer, verticesbuffer, uvsbuffer, normalsbuffer);
}

void MoveRightLeft(float& xright, float& xleft)
{
	if (hardright)
	{
		if (xright >= 2)
			rightx = false;
		else if (xright <= 0.1f)
			rightx = true;

		//Move The Object Right Side
		if (rightx)
			xright += 5 * deltaTime;
		else
			xright -= 5 * deltaTime;
	}
	else
		xright = 1 * (AsteroidScale / 0.5f);
	if (hardleft)
	{
		if (xleft <= -2)
			leftx = false;
		else if (xleft >= -0.1f)
			leftx = true;

		//Move The Object Left Side
		if (leftx)
			xleft -= 5 * deltaTime;
		else
			xleft += 5 * deltaTime;
		cout << xleft << endl;
	}
	else
		xleft = -1 * (AsteroidScale / 0.5f);
	

}
GLuint flicker(GLuint ProgramId, GLuint lightId)
{
	if (timeflicker - timer > 0 )
	{
		//numberofflickersperloop--;
		if (timeflicker - timer > 3)
		{
			timeflicker = timer - 10;
		}
		else
		{
			if (one)
			{
				glUseProgram(programID2);
				one = false;
				return LightID2;
			}
			else
			{
				glUseProgram(programID);
				one = true;
				return LightID;
			}
		}
	}
	else
	{
		glUseProgram(ProgramId);
		return lightId;
	}
}
bool Collision(float x, float y, float z)
{
	bool changed = false;
	MovementRightx = true;
	MovementRighty = true;
	MovementRightz = true;
	MovementLeftx = true;
	MovementLefty = true;
	MovementLeftz = true;
	if (positionx - x > 0 && positionx - x < 0.7 && abs(positionz - z) < 0.7 && abs(positiony - y) < 0.4)
	{
		MovementLeftz = false;
		changed = true;
	}
	else if (positionx - x < 0 && positionx - x > -0.7 && abs(positionz - z) < 0.7 && abs(positiony - y) < 0.4)
	{
		MovementRightx = false;
		changed = true;
	}

	if (positiony - y > 0 && positiony - y < 0.4 && abs(positionz - z) < 0.7 && abs(positionx - x) < 0.7)
	{
		MovementLefty = false;
		changed = true;
	}
	else if (positiony - y < 0 && positiony - y > -0.4 && abs(positionz - z) < 0.7 && abs(positionx - x) < 0.7)
	{
		MovementRighty = false;
		changed = true;
	}

	if (positionz - z >= -0.2 && positionz - z < 0.7 && abs(positionx - x) < 0.7 && abs(positiony - y) < 0.4)
	{
		positionz += MovingSpeed * deltaTime;
		MovementLeftz = false;
		changed = true;
	}
	else if (positionz - z < 0 && positionz - z > -0.7 && abs(positionx - x) < 0.7 && abs(positiony - y) < 0.4)
	{
		MovementRightz = false;
		changed = true;
	}

	if (changed)
		return false;
	else
		return true;
}
void Inputs()
{
	//if the game not paused so take the up,down,right,left buttons from the player
	if(!Pause && !State[0])
	{
	// Move up
		Swing = 0; //reset the right and the left swing movement
		SwingUpDown = 0; // reset the forward and backward swing movement
	if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS&& MovementRighty) {
		positiony += deltaTime * speed;
	}
	// Move down
	if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS&&MovementLefty) {
		positiony -= deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS && MovementRightx) {
		positionx += deltaTime * speed;
		//play some animation
		PlayerAnimation(SwingInitial, Swing, -1);

	}
	// Strafe left
	if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS&&MovementLeftx) {
		positionx -= deltaTime * speed;
		//play some animation
		PlayerAnimation(SwingInitial, Swing, 1);

	}

	// Move backward
	if (glfwGetKey('S') == GLFW_PRESS&& MovementRightz) {
		positionz += deltaTime * speed;
		//play some animation
		PlayerAnimation(SwingUpDownInitial, SwingUpDown, 0.5f);

	}
	// Move forward
	if (glfwGetKey('W') == GLFW_PRESS&&MovementLeftz) {
		positionz -= deltaTime * speed;
		//play some animation
		PlayerAnimation(SwingUpDownInitial, SwingUpDown, -0.2f);
		
	}
	//play the wings animations
	WingsAnimation();
}
	//if the player won or lost don't take the pause button or change the background
	//if (State[0] || State[9])
		//return;

	//pause the game 
	if (glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS&& (PauseTimeLimit - timer) >0.1f &&!State[0]&&!State[9])
	{
		//if the game is pause so resume the game
		if (Pause)
		{
			Pause = false;
			timer += PauseTimeLimit - timer;
		}
		else
			Pause = true;

		//get the current timer to determine the difference between clicks
		PauseTimeLimit = timer;
		CheckPause();
	}
	if (glfwGetKey(GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		Initialization();
	}

	//activate the normal background
	if (glfwGetKey(GLFW_KEY_KP_1) == GLFW_PRESS)
	{
		ActiveState(1,2,4);
	}

	//activate the marvel video background
	if (glfwGetKey(GLFW_KEY_KP_2) == GLFW_PRESS)
	{
		ActiveState(2,1,4);
	}
	//activate illiosion video background
	if (glfwGetKey(GLFW_KEY_KP_3) == GLFW_PRESS)
	{
		ActiveState(3, 1, 4);
	}

	//activate Space video background
	if (glfwGetKey(GLFW_KEY_KP_4) == GLFW_PRESS)
	{
		ActiveState(4, 1, 4);
		PlaySound(TEXT("Sound/5.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//	PlaySound(TEXT("Sound/Entering The Stronghold - Audio Visual Animation HD! - YouTube.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	//activate and deactivate the video on the street
	if (glfwGetKey(GLFW_KEY_KP_5) == GLFW_PRESS&&(PauseTimeLimit - timer) >0.1f)
	{
		ActiveState(5, 5, 8);
		PauseTimeLimit = timer;
	}

	//activate and deactivate texture 5 on the street
	if (glfwGetKey(GLFW_KEY_KP_6) == GLFW_PRESS && (PauseTimeLimit - timer) >0.1f)
	{
		ActiveState(6, 5, 8);
		PauseTimeLimit = timer;
	}

	if (glfwGetKey(GLFW_KEY_KP_7) == GLFW_PRESS && (PauseTimeLimit - timer) >0.1f)
	{
		if (drawastreet)
			drawastreet = false;
		else
			drawastreet = true;
		ActiveState(7, 5, 8);
		PauseTimeLimit = timer;
	}

	if (glfwGetKey(GLFW_KEY_KP_8) == GLFW_PRESS && (PauseTimeLimit - timer) >0.1f)
	{
		if (DrawTime)
			DrawTime = false;
		else
			DrawTime = true;
		ActiveState(7, 5, 8);
		PauseTimeLimit = timer;
	}

	if (glfwGetKey('F') == GLFW_PRESS)
	{
		PlaySound(TEXT("Sound/02. APETITAN.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (glfwGetKey('G') == GLFW_PRESS)
	{
		PlaySound(TEXT("Sound/EDM - Hallo - Omfg - YouTube.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (glfwGetKey('H') == GLFW_PRESS)
	{
		PlaySound(TEXT("Sound/13-Cheap Thrills.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (glfwGetKey('J') == GLFW_PRESS)
	{
		PlaySound(TEXT("Sound/EDM - Alan Walker - Gespenst - YouTube.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (glfwGetKey('K') == GLFW_PRESS)
	{
		PlaySound(TEXT("Sound/EDM - Entstellen - Leer - YouTube.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (glfwGetKey('L') == GLFW_PRESS)
	{
		PlaySound(TEXT("Sound/EDM - Warrio - Sterbliche (Feat. Laura Brehm) - YouTube.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (glfwGetKey('Z') == GLFW_PRESS)
	{
		PlaySound(TEXT("Sound/Titanic Theme Song • My Heart Will Go On • Celine Dion - YouTube.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	
}
void WingsAnimation()
{
	//if the player didn't put right or left button the reset the plane to the idle animation
	if (Swing == 0)
		Wings(SwingInitial);
	// else rotate the wings
	RotationSwingz = SwingInitial;
	//if the player didn't put forward or backward button the reset the plane to the idle animation
	if (SwingUpDown == 0)
		Wings(SwingUpDownInitial);
	// else rotate the plane
	RotationSwingx = SwingUpDownInitial;
 }
void Wings(float& value)
{
	if (value < 0)
	{
		value += 0.08f;
	}
	if (value > 0)
		value -= 0.08f;
}
void PlayerAnimation(float& Initial, float& Current, float max)
{
	//determine if right,left,forward,backward animation
	if (max > 0)
	{
		Current = Initial;

		if (Current < 0)
		{
			Initial += 0.1f;
		}
		else
		{
			Current += 0.05f;
			if (Current > max)
				Current = max;
			Initial = Current;
		}
	}
	else
	{
		Current = Initial;

		if (Current > 0)
		{
			Initial -= 0.1f;
		}
		else
		{
			Current -= 0.05f;
			if (Current < max)
				Current = max;
			Initial = Current;
		}

	}
	

}

void Buffer(GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer, vector<vec3> vertices, vector<vec2> uvs, vector<vec3> normals)
{
	glGenBuffers(1, &verticesbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticesbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvsbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

}
void Binding(GLuint verticesbuffer, GLuint uvsbuffer, GLuint normalsbuffer)
{
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, verticesbuffer);
	glVertexAttribPointer(
		vertexPosition_modelspaceID,  // The attribute we want to configure
		3,                            // size
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, uvsbuffer);
	glVertexAttribPointer(
		vertexUVID,                   // The attribute we want to configure
		2,                            // size : U+V => 2
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(vertexNormal_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
	glVertexAttribPointer(
		vertexNormal_modelspaceID,    // The attribute we want to configure
		3,                            // size
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

}
void BindingTextures(GLuint Texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);
}
void UniformMatrices(mat4& MVP, mat4& Model, mat4& View)
{
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
}
void LoadObject(char* Object, char* Texture,GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer, vector<vec3>& vertices, vector<vec2>& uvs, vector<vec3>& normals)
{
	loadOBJ(Object, vertices, uvs, normals);
	TextureId = loadBMP_custom(Texture);
	Buffer(verticesbuffer, uvsbuffer, normalsbuffer, vertices, uvs, normals);
}
void Draw(GLuint ProgramId, GLuint lightId, vector<vec3> vertices, int LightX, int LightY, int LightZ, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{
	glUseProgram(ProgramId);
	GLuint lightsId = flicker(ProgramId, lightId);
	Model = TranslationMatrix*RotationMatrix * ScalingMatrix;
	MVP = Projection * View * Model;
	//uniform the matrices to the ids shader
	UniformMatrices(MVP, Model, View);
	//active the vertices buffer 
	Binding(verticesbuffer, uvsbuffer, normalsbuffer);
	//active the texture according to its id in the buffer
	BindingTextures(TextureId);
	//uniform the light according to the ids shader
	glUniform3f(lightsId, LightX, LightY, LightZ);
	//draw the object
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void ChangeLoop()
{
	//delete the texture if the background is video
	if (Texture != Texture3)
	{
		glDeleteTextures(1, &Texture);
	}
	DisableVertex();
	lastTime = currentTime;
	glfwSwapBuffers();
}
void Refresh()
{
	
	RefreshDeltaTime();
	//numberofflickersperloop = 10;
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	//glUseProgram(programID);
	if(!State[0])
	//adjust the timer
	timer -= deltaTime;
	//check the level
	Levels();
	//check the boarders
	CheckBoarders();
	//take inputs from the player
	Inputs();
}
void RefreshDeltaTime()
{
	//get the current time
	currentTime = glfwGetTime();
	//change the delta time
	deltaTime = float(currentTime - lastTime);
}

void CleanBuffer(GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{
	// Cleanup VBO and shader
	// it's important to do that at the end but now we will work without it
	    glDeleteBuffers(1, &verticesbuffer);
		glDeleteBuffers(1, &uvsbuffer);
		glDeleteBuffers(1, &normalsbuffer);
		glDeleteTextures(1, &TextureId);
}
void DisableVertex()
{
	glDisableVertexAttribArray(vertexPosition_modelspaceID);
	glDisableVertexAttribArray(vertexUVID);
	glDisableVertexAttribArray(vertexNormal_modelspaceID);
}

void CheckBoarders()
{
	//check the boarders of each axis and deactivate if the player reached to the boarder
	if (positionx >= 2)
		MovementRightx = false;
	else
		MovementRightx = true;

	if (positiony >= 1.5f)
		MovementRighty = false;
	else
		MovementRighty = true;
	if (positionz >= 2)
		MovementRightz = false;
	else
		MovementRightz = true;

	if (positionx <= -2)
		MovementLeftx = false;
	else
		MovementLeftx = true;

	if (positiony <= -2)
		MovementLefty = false;
	else
		MovementLefty = true;

	if (positionz <= -2)
		MovementLeftz = false;
	else
		MovementLeftz = true;
}
void Levels()
{
	//check if the game is pause
	if (Pause)
		return;
	
	//determine the current state depending on the timer
	if (timer > 40 && timer < 50 && level1)
	{
		AsteroidRotationz = 1;
		MovingSpeed += 0.2f;
		UpDownAsteroidSpeed *= 2;
		level1 = false;
	}

	if (timer > 30 && timer < 40 && level2)
	{
		level1 = true;
		AsteroidRotationz = 0;
		MovingSpeed *= 1.5f;
		UpDownAsteroidSpeed *= 1.5f;
		//AsteroidScale *= 1.5f;
		level2 = false;
	}

	if (timer > 20 && timer < 30 && level3)
	{
		level2 = true;
		MovingSpeed *= 0.5f;
		UpDownAsteroidSpeed *= 3;
		hardleft = false;
		hardright = true;
		level3 = false;
	}

	if (timer > 10 && timer < 20 && level4)
	{
		level3 = true;
		MovingSpeed *= 3;
		UpDownAsteroidSpeed /= 3;
		hardleft = true;
		hardright = false;
		level4 = false;
	}

	if (timer > 0 && timer < 10 && level5)
	{
		level4 = true;
		MovingSpeed *= 1.5f;
		UpDownAsteroidSpeed *= 1.5f;
		hardleft = true;
		hardright = true;
		level5 = false;
	}
	//if the timer ended so you won
	if (timer <= 0 && !State[9])
	{
		timer = 0;
		PlaySound(NULL, 0, 0);
		ActiveState(9,0,4);
		PlaySound(TEXT("Sound/Winning.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	//if the position of the player on the z axis more than 3 so the player lost
	if (positionz > 3 && !State[0])
	{
		PlaySound(NULL, 0, 0);
		ActiveState(0,0,4);
		PlaySound(TEXT("Sound/Losing.wav"), NULL, SND_FILENAME | SND_ASYNC);
		drawastreet = false;
		drawAsteroids = false;
		drawPlayer = false;
	}

}
void CheckPause()
{
	if (!Pause)
	{
		MovingSpeed = PauseSpeed;
		UpDownAsteroidSpeed = PauseUpDownAsteroidSpeed;
		return;
	}

	PauseSpeed = MovingSpeed;
	PauseUpDownAsteroidSpeed = UpDownAsteroidSpeed;
	MovingSpeed = 0;
	UpDownAsteroidSpeed = 0;
}
void MainSound()
{
	//play the music looped
	PlaySound(TEXT("Sound/02. APETITAN.wav"), NULL, SND_FILENAME | SND_ASYNC);

}
void ActiveState(int j, int min, int max)
{
	//deactivate all the states
	for (; min <= max; min++)
	{
		State[min] = false;
	}
	//activate specific state
	State[j] = true;
}
void ChangeState()
{
	//check the active state and change to it
	if (State[0])
	{
		//check if the counting reached to the last frame or not
		if (LosingCounter == 554)
		{
			End = true;
		}
		VideoImport("BackGroundObject/Losing/1 (", Texture, LosingCounter);
		if (State[5])
		{
			Texture4 = Texture;
		}
	}
	else if (State[9])
	{
		//check if the counting reached to the last frame or not
		if (WinningCounter == 3466)
		{
			End = true;
		}
		VideoImport("BackGroundObject/Winning/1 (", Texture, WinningCounter);
		if (State[5])
		{
			Texture4 = Texture;
		}
	}
	else if (State[1])
	{
		Texture = Texture3;
		if (State[5])
		{
			Texture4 = Texture;
		}
	}
	else if (State[2])
	{
		//check if the counting reached to the last frame or not
		if (VideoMode1Counter == 3597)
			VideoMode1Counter = 873;
		VideoImport("BackGroundObject/VideoMode1/1 (", Texture, VideoMode1Counter);
		if (State[5])
		{
			Texture4 = Texture;
		}
	}
	else if (State[3])
	{
		if (VideoMode2Counter == 1524)
			VideoMode2Counter = 1;
		VideoImport("BackGroundObject/VideoMode4/1 (", Texture, VideoMode2Counter);
		if (State[5])
		{
			Texture4 = Texture;
		}
	}

	else if (State[4])
	{
		if (VideoMode3Counter == 1550)
			VideoMode3Counter = 1;
		
		VideoImport("BackGroundObject/VideoMode5/1 (", Texture, VideoMode3Counter);
		if (State[5])
		{
			Texture4 = Texture;
		}
	}
	if (State[6])
	{
		Texture4 = Texture1;
	}
	//draw the background according to the current state
	DrawBackGround(vertices[3], Texture, vertexbuffer3, uvbuffer3, normalbuffer3);
}
void VideoImport(string directory, GLuint& TextureId, int& value)
{
	if (!Pause)
	{
		VideoString = directory + to_string(value) + ").bmp";
		value ++;
	}

	char* name = new char[VideoString.size() + 1];
	strcpy(name, VideoString.c_str());
	TextureId = loadBMP_custom(name);
}



int OpenWindow()
{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	if (!glfwOpenWindow(1920, 1080, 0, 0, 0, 0, 64, 0, GLFW_FULLSCREEN))
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle("Asteroids War");

	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);
	glfwSetMousePos(1920 / 2, 1080 / 2);
	// Dark black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	vertexUVID = glGetAttribLocation(programID, "vertexUV");
	vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");
	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	programID2 = LoadShaders("StandardShading2.vertexshader", "StandardShading2.fragmentshader");
	LightID2 = glGetUniformLocation(programID2, "LightPosition_worldspace");

	glUseProgram(programID);
	//get the last frame as the current frame
	lastTime = glfwGetTime();
	//change the projection of the camera
	computeMatricesFromInputs();
	//get the projection of the camera
	Projection = getProjectionMatrix();
	//get the view of the camera
	View = getViewMatrix();
	//Initialize the variables
	Initialization();
}
void CloseWindow()
{


	glDeleteTextures(1, &Texture4);
	// Close OpenGL window and terminate GLFW
	glDeleteProgram(programID);
	glfwTerminate();
}
void Initialization()
{
	//Player
	speed = 2;
	positionx = 0;
	positiony = 0;
	positionz = 0;
	MoveAsteroid = 0;
	MovementRightx = true;
	MovementRighty = true;
	MovementRightz = true;
	MovementLeftx = true;
	MovementLefty = true;
	MovementLeftz = true;
	SwingInitial = 0;
	RotationSwingz = 0;
	SwingUpDownInitial = 0;
	RotationSwingx = 0;
	drawPlayer = true;

	//Asteroid
	effectiveyup = 0;
	effectiveydown = 0;
	MovingSpeed = 20;
	UpDownAsteroidSpeed = 1;
	AsteroidRotationx = 0;
	AsteroidRotationy = 0;
	AsteroidRotationz = 0;
	AsteroidScale = 0.5;
	reached = true;
	orientation = 0;
	drawAsteroids = true;
	xright = 1;
	xleft = -1;
	//background and street
	MoveStreet = 0;
	resizex = 25;
	resizey = 18.5f;
	drawastreet = false;
	one = true;
	timeflicker = 50;
	DrawTime = false;
	timercounter = 1;
	timetimer = 60;
	//levels,time and pause
	level1 = true;
	level2 = true;
	level3 = true;
	level4 = true;
	level5 = true;
	Pause = false;
	PauseTimeLimit = 63;
	timer = 60;
	//0 : lost
	//1 : background is the stars photo
	//2 : background is the avengers trailer
	//3 : background is Space
	//5 : street texture's is the avengers trailer
	//9 : won 
	End = false;
	rightx = true;
	leftx = true;
	hardright = false;
	hardleft = false;

	//frames
	LosingCounter = 1;
	WinningCounter = 1;
	VideoMode1Counter = 873;
	VideoMode2Counter = 1;
	VideoMode3Counter = 1;
	//to check if its the first time or not
	if (State.size() == 0)
	{
		//put to the vector all states
		for (int i = 0; i < 10; i++)
		{
			State.push_back(false);
		}
	}
	//active the initial state the normal background
	ActiveState(3, 0, 9);
	//active the initial state to the street texture
	ActiveState(6, 5, 8);
	//play music on the game and repeated
	MainSound();

}

