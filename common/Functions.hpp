#ifndef Functions_HPP
#define Functions_HPP
// Include standard headers
/*
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
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
#include <glm/gtx/euler_angles.hpp>//rotation
#include <glm/gtx/norm.hpp>//rotation
using namespace glm;
using namespace std;

//Variables
GLuint programID;
GLuint vertexPosition_modelspaceID;
GLuint vertexUVID;
GLuint vertexNormal_modelspaceID;
GLuint MatrixID;
GLuint ViewMatrixID;
GLuint ModelMatrixID;
GLuint TextureID;
GLuint LightID;

static double lastTime;
double currentTime;
float deltaTime;
mat4 Projection = getProjectionMatrix();
mat4 View = getViewMatrix();
mat4 RotationMatrix;
mat4 TranslationMatrix;
mat4 ScalingMatrix;
mat4 Model;
mat4 MVP;
vec3 lightPos;

//Functions
void LoadObject(string Object, string Texture, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer, vector<vec3>& vertices, vector<vec2>& uvs, vector<vec3>& normals);
void Buffer(GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer, vector<vec3> vertices, vector<vec2> uvs, vector<vec3> normals);
void Binding(GLuint verticesbuffer, GLuint uvsbuffer, GLuint normalsbuffer);
void RefreshDeltaTime();
void UniformMatrices(mat4& MVP, mat4& Model, mat4& View);
void BindingTextures(GLuint Texture);
void DisableVertex();
void ChangeLoop();
void Refresh();
int OpenWindow();
void CloseWindow();
void Draw(vector<vec3> vertices, int LightX, int LightY, int LightZ, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer);
*/
#endif