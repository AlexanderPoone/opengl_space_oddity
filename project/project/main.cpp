#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/glui.h"
#include "GL/glm.hpp"
#include "GL/gtc/matrix_transform.hpp"
#include "GL/SOIL.h"

#include "irrKlang.h"
using namespace std;
using namespace irrklang;

#define FILE_PATH "Space Oddity.ogg"
#pragma comment(lib, "winmm")
#pragma comment(lib, "irrKlang.lib")

/** These are the live variables passed into GLUI ***/
int   wireframe = 0;
int   segments = 8;
int   main_window;

vector<std::string> faces
{
	"basic_files/texture/purple_nebula/Right_MauveSpaceBox.png",
	"basic_files/texture/purple_nebula/Left_MauveSpaceBox.png",
	"basic_files/texture/purple_nebula/Up_MauveSpaceBox.png",
	"basic_files/texture/purple_nebula/Down_MauveSpaceBox.png",
	"basic_files/texture/purple_nebula/Back_MauveSpaceBox.png",
	"basic_files/texture/purple_nebula/Front_MauveSpaceBox.png"
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

float lampVertices[] = {
	// positions
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f, -0.1f,  0.1f,
	0.1f, -0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,

	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,

	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,

	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f,  0.1f,
	0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f,  0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f
};

glm::vec3 lightPos(-0.05f, -0.05f, -0.5f);
glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);

GLuint lampVAO, lampVBO, lampShader;

GLuint skyboxVAO, skyboxVBO, skyboxShader, skyboxTexture;

GLuint planetCommonVAO, planetCommonVBO, planetCommonUV, planetCommonNormal, planetCommonShader;
GLuint planet0_diffuseMap, planet0_specularMap, planet0_emissionMap;
GLuint planet1_diffuseMap, planet1_specularMap, planet1_emissionMap;
GLuint planet2_diffuseMap, planet2_specularMap, planet2_emissionMap;

GLfloat planet0_rotationAngle = 0.0f;

int fogColorId;
float hor = 0.0f, ver = 0.0f;

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	GLUI_Master.auto_set_viewport();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);

	glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4();
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 0),
		glm::vec3(hor, ver, -1),
		glm::vec3(0, 1, 0)  // head is up (set to 0, -1, 0 to look upside down)
	);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)500, 0.1f, 100.0f);



	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	glUseProgram(skyboxShader);
	GLint viewUniformLocation = glGetUniformLocation(skyboxShader, "view");
	glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);
	GLint projectionUniformLocation = glGetUniformLocation(skyboxShader, "projection");
	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &projection[0][0]);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
	glUseProgram(lampShader);
	GLint modelUniformLocation = glGetUniformLocation(lampShader, "model");
	glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &model[0][0]);
	viewUniformLocation = glGetUniformLocation(lampShader, "view");
	glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);
	projectionUniformLocation = glGetUniformLocation(lampShader, "projection");
	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &projection[0][0]);
	// lamp cube
	glBindVertexArray(lampVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	/****************************************/
	/*         Planet 0 starts here         */
	/****************************************/
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(planet0_rotationAngle), glm::vec3(0,1,0));
	planet0_rotationAngle += 0.01f;
	model = glm::translate(model, glm::vec3(0.3f,0.0f,-0.7f));
	model = glm::scale(model, glm::vec3(0.1f));
	glUseProgram(planetCommonShader);

	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.position"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "viewPos"), 1, &cameraPos[0]);
	std::cout << glGetError() << std::endl; // returns 0 (no error)

	glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.ambient"), 1, &lightAmbient[0]);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.diffuse"), 1, &lightDiffuse[0]);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.specular"), 1, &lightSpecular[0]);

	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "projection"), 1, GL_FALSE, &projection[0][0]);

	glUniform1i(glGetUniformLocation(planetCommonShader, "material.emission"), 0);				//sampler
	glUniform1i(glGetUniformLocation(planetCommonShader, "material.diffuse"), 1);				//sampler
	glUniform1i(glGetUniformLocation(planetCommonShader, "material.specular"), 2);				//sampler
	GLfloat materialShininess = 64.0f;															//but not this one
	glUniform1f(glGetUniformLocation(planetCommonShader, "material.shininess"), materialShininess);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planet0_diffuseMap);
	// bind specular map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planet0_specularMap);
	// bind emission map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planet0_emissionMap);
	// render the sphere
	glBindVertexArray(planetCommonVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 98304);
	glBindVertexArray(0);
	/****************************************/
	/*          Planet 0 ends here          */
	/****************************************/

	//static float rotationX = 0.0, rotationY = 0.0;

	//glClearColor(.9f, .9f, .9f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	///*** Rotate the object ***/
	//rotationX += 3.3f;
	//rotationY += 4.7f;

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(0.0, 0.0, -1.0);
	//glRotatef(rotationY, 0.0, 1.0, 0.0);
	//glRotatef(rotationX, 1.0, 0.0, 0.0);

	///*** Now we render object, using the variables 'segments' and
	//'wireframe'.  These are _live_ variables, which are transparently
	//updated by GLUI ***/

	//if (wireframe)
	//	glutWireTorus(.2, .5, 16, segments);
	//else
	//	glutSolidTorus(.2, .5, 16, segments);

	glutSwapBuffers();
}

void myGlutKeyboard(unsigned char key, int x, int y) {
	//Please set lower case ‘a’, s’, d’ as switch between 3 different viewpoints.
	if ((key == 'a' || key == 'A'))
	{
		hor -= 0.1f;
	}
	if ((key == 'd' || key == 'D') && (hor < 1.0f))
	{
		hor += 0.1f;
	}
	if ((key == 'w' || key == 'W') && (ver < 1.0f))
	{
		ver += 0.1f;
	}
	if ((key == 's' || key == 'S') && (ver > -1.0f))
	{
		ver -= 0.1f;
	}
}

void glui_callback(int control_id) {
	switch (control_id) 
	{
		case 17:
			if (fogColorId == 0) printf("Fog colour is white\n");
			else if (fogColorId == 1) printf("Fog colour is blue\n");
			break;
	}
}

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
				   // else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}
	return true;
}


GLuint loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		int width, height;

		unsigned char* data =
			SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		//unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			SOIL_free_image_data(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

GLuint loadSphereTexture(string im)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int width, height;

	unsigned char* data =
		SOIL_load_image(im.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
	}
	else {
		std::cout << "Sphere texture failed to load at path: " << im << std::endl;
		SOIL_free_image_data(data);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return textureID;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(string fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

GLuint installShaders(string vs, string fs)
{
	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode(vs);
	adapter[0] = temp.c_str();
	glShaderSource(vsID, 1, adapter, 0);
	temp = readShaderCode(fs);
	adapter[0] = temp.c_str();
	glShaderSource(fsID, 1, adapter, 0);

	glCompileShader(vsID);
	glCompileShader(fsID);

	if (!checkShaderStatus(vsID) || !checkShaderStatus(fsID))
		return 0;

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vsID);
	glAttachShader(programID, fsID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return 0;

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return programID;
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	char pBuf[256]; size_t len = sizeof(pBuf);

	GetModuleFileName(NULL, pBuf, len);
	printf("%s\n", pBuf);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(800, 500);

	main_window = glutCreateWindow("Project");

	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// lamp VAO
	glGenVertexArrays(1, &lampVAO);
	glGenBuffers(1, &lampVBO);
	glBindVertexArray(lampVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lampVertices), &lampVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	lampShader = installShaders("lamp.vs", "lamp.fs");

	// 3 planets
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ("io.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &planetCommonVAO);

	// Planet Vertices
	glGenBuffers(1, &planetCommonVBO);
	glBindVertexArray(planetCommonVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planetCommonVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, planetCommonVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Planet UVs
	glGenBuffers(1, &planetCommonUV);
	glBindBuffer(GL_ARRAY_BUFFER, planetCommonUV);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, planetCommonUV);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Planet Normals
	glGenBuffers(1, &planetCommonNormal);
	glBindBuffer(GL_ARRAY_BUFFER, planetCommonNormal);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, planetCommonNormal);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	planet0_emissionMap = loadSphereTexture("mercator.jpg");
	planet0_diffuseMap = loadSphereTexture("mercator.jpg");
	planet0_specularMap = loadSphereTexture("mercator_specular.jpg");

	planet1_diffuseMap = loadSphereTexture("pluto.png");
	planet1_specularMap = loadSphereTexture("pluto.png");
	planet1_emissionMap = loadSphereTexture("pluto.png");

	planet2_diffuseMap = loadSphereTexture("neptune.jpg");
	planet2_specularMap = loadSphereTexture("neptune.jpg");
	planet2_emissionMap = loadSphereTexture("neptune.jpg");

	planetCommonShader = installShaders("planetCommon.vs", "planetCommon.fs");


	// skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	skyboxTexture = loadCubemap(faces);
	skyboxShader = installShaders("skybox.vs", "skybox.fs");

	glutDisplayFunc(myGlutDisplay);
	//glutReshapeFunc(myGlutReshape);

	///****************************************/
	///*       Set up OpenGL lights           */
	///****************************************/

	//GLfloat light0_ambient[] = { 0.1f, 0.1f, 0.3f, 1.0f };
	//GLfloat light0_diffuse[] = { .6f, .6f, 1.0f, 1.0f };
	//GLfloat light0_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	//glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	///****************************************/
	///*          Enable z-buffering          */
	///****************************************/

	//glEnable(GL_DEPTH_TEST);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI *glui = GLUI_Master.create_glui_subwindow(main_window,
		GLUI_SUBWINDOW_RIGHT);

	glui->set_main_gfx_window(main_window);
	glui->add_separator();
	glui->add_statictext("iNavigation@CUHK");
	glui->add_separator();

	glui->add_checkbox("Wireframe", &wireframe);
	GLUI_Spinner *segment_spinner =
		glui->add_spinner("Segments:", GLUI_SPINNER_INT, &segments);
	segment_spinner->set_int_limits(3, 60);

	GLUI_Listbox *listbox = glui->add_listbox("A listbox");
	listbox->add_item(1, "Red");
	listbox->add_item(2, "Green");
	listbox->add_item(3, "Blue");

	GLUI_Rollout *render_rollout = glui->add_rollout("Render Control");

	GLUI_Panel *obj_panel = glui->add_panel_to_panel(render_rollout, "Viewpoint");
	GLUI_RadioGroup *group1 = glui->add_radiogroup_to_panel(obj_panel);
	glui->add_radiobutton_to_group(group1, "Left");
	glui->add_radiobutton_to_group(group1, "Top");
	glui->add_radiobutton_to_group(group1, "Vehicle");

	GLUI_Panel *fog_panel = glui->add_panel_to_panel(render_rollout, "Fog On/Off");
	GLUI_Checkbox *fog_checkbox = glui->add_checkbox_to_panel(fog_panel, "Fog");

	glui->add_column_to_panel(render_rollout, 0);

	GLUI_Panel *fog_color_panel = glui->add_panel_to_panel(render_rollout, "Fog Colour");
	GLUI_RadioGroup *group2 = glui->add_radiogroup_to_panel(fog_color_panel, &fogColorId, 17, glui_callback);
	glui->add_radiobutton_to_group(group2, "Ivory");
	glui->add_radiobutton_to_group(group2, "Ocean blue");

	GLUI_Rotation *arcball = glui->add_rotation("Rotation");

	GLUI_Translation *translation_xy = glui->add_translation("Translation XY", GLUI_TRANSLATION_XY);

	GLUI_Translation *translation_x = glui->add_translation("Translation X", GLUI_TRANSLATION_X);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
	//GLUI_Master.set_glutMouseFunc();

	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);




	//wchar_t wtext[20];
	//mbstowcs(wtext, argv[1], strlen(argv[1]) + 1);
	//LPWSTR filename = wtext;

	/*--------------------------------------------------------------------------------
	*Initiate IrrKlang engine
	---------------------------------------------------------------------------------*/
	char q = 0;
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine) {
		cout << "Cannot open engine";
		return 0;
	}
	/*--------------------------------------------------------------------------------
	*play music
	---------------------------------------------------------------------------------*/
	//false for no loop
	engine->play2D(FILE_PATH, false);

	glutMainLoop();
}