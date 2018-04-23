// Include GLFW

#include "Functions.hpp"
/*
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
	if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
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

	glfwSetWindowTitle("SubWay");

	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);
	glfwSetMousePos(1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

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
	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
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
void RefreshDeltaTime()
{
	lastTime = glfwGetTime();
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
}
void UniformMatrices(mat4& MVP, mat4& Model, mat4& View)
{
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
}
void LoadObject(string Object, string Texture, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer, vector<vec3>& vertices, vector<vec2>& uvs, vector<vec3>& normals)
{
	loadOBJ("PlayerObject/suzanne.obj", vertices, uvs, normals);
	TextureId = loadBMP_custom("Game/Colors.bmp");
	Buffer(verticesbuffer, uvsbuffer, normalsbuffer, vertices, uvs, normals);
}

void DisableVertex()
{
	glDisableVertexAttribArray(vertexPosition_modelspaceID);
	glDisableVertexAttribArray(vertexUVID);
	glDisableVertexAttribArray(vertexNormal_modelspaceID);
}
void ChangeLoop()
{
	DisableVertex();
	lastTime = currentTime;
	glfwSwapBuffers();
}
void Refresh()
{
	RefreshDeltaTime();
	computeMatricesFromInputs();
	Projection = getProjectionMatrix();
	View = getViewMatrix();
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Use our shader
	glUseProgram(programID);

}

void Draw(vector<vec3> vertices, int LightX, int LightY, int LightZ, GLuint& TextureId, GLuint& verticesbuffer, GLuint& uvsbuffer, GLuint& normalsbuffer)
{
	Model = TranslationMatrix*RotationMatrix * ScalingMatrix;
	MVP = Projection * View * Model;
	UniformMatrices(MVP, Model, View);
	Binding(verticesbuffer, uvsbuffer, normalsbuffer);
	BindingTextures(TextureId);
	//lightPos = glm::vec3(-1, 500, 4);
	glUniform3f(LightID, LightX, LightY, LightZ);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//change the loop
	ChangeLoop();
}

void CloseWindow()
{

	// Cleanup VBO and shader
	// it's important to do that at the end but now we will work without it
	/*glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}*/