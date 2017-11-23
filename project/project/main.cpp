#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <deque>
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
	-0.1f,  0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	-0.1f,  0.1f, -0.1f,

	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,

	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,

	-0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,

	-0.1f,  0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,

	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f,  0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f,  0.1f,
	0.1f, -0.1f,  0.1f
};

const int WIDTH = 1200;
const int HEIGHT = 800;
const float ASPECT = float(WIDTH) / HEIGHT;   // desired aspect ratio

glm::vec3 lightPos(-0.05f, -0.05f, -0.5f);
glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);

GLuint lampVAO, lampVBO, lampShader;

GLuint skyboxVAO, skyboxVBO, skyboxShader, skyboxTexture;

GLuint planetCommonVAO, planetCommonVBO, planetCommonUV, planetCommonNormal, planetCommonShader;
GLuint planet0_diffuseMap, planet0_specularMap, planet0_emissionMap;
GLuint planet1_diffuseMap, planet1_specularMap, planet1_emissionMap;
GLuint planet2_diffuseMap, planet2_specularMap, planet2_emissionMap;

GLuint vehicleVAO, vehicleVBO, vehicleUV, vehicleNormal, vehicleShader, vehicle_diffuseMap, vehicle_specularMap, vehicle_emissionMap;

GLuint amount = 100, asteroidCommonVBO, asteroidCommonUV, asteroidCommonNormal, asteroidCommonInstanced, asteroidCommonDrawSize, asteroidCommonTexture, asteroidCommonShader;
std::vector<GLuint> asteroidVAO;

GLuint starCommonVAO, starVBO, starCommonUV, starCommonNormal, star_diffuseMap, star_specularMap, star_emissionMap, starCommonShader;

GLfloat diffuse = 0.5f;

glm::vec4 FogRealColor(.8f, .8f, .8f, 1.0f); // vec4 FogRealColor = vec4(0.0f, 0.467f, 0.745f, 1.0f);
glm::mat4 view=glm::lookAt(
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, -1),
	glm::vec3(0, 1, 0)  // head is up (set to 0, -1, 0 to look upside down)
);
GLfloat vehiclePosX=0.0f, vehiclePosY=0.0f, vehiclePosZ=0.0f;

int FogFlag=0;

std::deque<glm::mat4> vehicleHistory;

GLfloat lightPos_translate=0.0f, lightPos_step=0.005f;
GLfloat planet0_rotationAngle = 0.0f, vehicle_rotationAngle = 0.0f, vehicle_s=0.5f;

int viewpointId=0, fogColorId;
GLUI_RadioGroup *group1;
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

void myGlutReshape(int width, int height)
{
	GLUI_Master.auto_set_viewport();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int w = height * ASPECT;           // w is width adjusted for aspect ratio
	int left = (width - w) / 2;
	glViewport(left/2, 0, w, height);       // fix up the viewport to maintain aspect ratio
	gluOrtho2D(0, WIDTH, HEIGHT, 0);   // only the window is changing, not the camera
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.5f, .5f, .5f, 1.0f);
	glClearDepth(1);

	glm::mat4 model = glm::mat4();
	glm::mat4 view0 = glm::lookAt(
		glm::vec3(0, 0, 0),
		glm::vec3(hor, ver, -1),
		glm::vec3(0, 1, 0)  // head is up (set to 0, -1, 0 to look upside down)
	);
	switch (viewpointId) {
		case 0:
			view = glm::lookAt(
				glm::vec3(0, 0, 0),
				glm::vec3(hor, ver, -1),
				glm::vec3(0, 1, 0)  // head is up (set to 0, -1, 0 to look upside down)
			);
			break;
		case 1:
			view = glm::lookAt(
				glm::vec3(0, 0.9f, -0.7),
				glm::vec3(hor, ver, -0.7),
				glm::vec3(0, 0, -0.7)  // head is up (set to 0, -1, 0 to look upside down)
			);
			break;
		case 2:
			view = glm::lookAt(
				//cos sin, z is same with vehicle
				glm::vec3(vehiclePosX, vehiclePosY, vehiclePosZ),
				glm::vec3(0.3f+0.1f*glm::cos(glm::radians(vehicle_rotationAngle)), 0.1f*glm::sin(glm::radians(vehicle_rotationAngle)), vehiclePosZ),
				glm::vec3(0, 1, 0)  // head is up (set to 0, -1, 0 to look upside down)
			);
			cout << "Angle: " << vehicle_rotationAngle;
			cout << 0.5f*glm::cos(glm::radians(vehicle_rotationAngle));
			cout << 0.5f*glm::sin(glm::radians(vehicle_rotationAngle));
			break;
	}
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)500, 0.1f, 100.0f);

	lightPos_translate += lightPos_step;

	if (lightPos_translate >= 0.25f) lightPos_step = -0.005f;
	else if (lightPos_translate <= -0.25f) lightPos_step = 0.005f;

	lightPos = glm::vec3(-0.05f + lightPos_translate, -0.05f, -0.4f);

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	glUseProgram(skyboxShader);
	GLint viewUniformLocation = glGetUniformLocation(skyboxShader, "view");
	glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, &view[0][0]);
	GLint view0UniformLocation = glGetUniformLocation(skyboxShader, "view0");
	glUniformMatrix4fv(view0UniformLocation, 1, GL_FALSE, &view0[0][0]);
	GLint projectionUniformLocation = glGetUniformLocation(skyboxShader, "projection");
	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &projection[0][0]);
	glUniform1i(glGetUniformLocation(skyboxShader, "FogFlag"), FogFlag);
	glUniform4fv(glGetUniformLocation(skyboxShader, "FogRealColor"), 1, &FogRealColor[0]);
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
	view0UniformLocation = glGetUniformLocation(lampShader, "view0");
	glUniformMatrix4fv(view0UniformLocation, 1, GL_FALSE, &view0[0][0]);
	projectionUniformLocation = glGetUniformLocation(lampShader, "projection");
	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, &projection[0][0]);
	glUniform1i(glGetUniformLocation(lampShader, "FogFlag"), FogFlag);
	glUniform4fv(glGetUniformLocation(lampShader, "FogRealColor"), 1, &FogRealColor[0]);
	// lamp cube
	glBindVertexArray(lampVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	/****************************************/
	/*         Planet 0 starts here         */
	/****************************************/
	model = glm::translate(glm::mat4(), glm::vec3(0.3f, 0.0f, -0.9f))
		* glm::scale(glm::mat4(), glm::vec3(0.1f))
		* glm::rotate(glm::mat4(), glm::radians(planet0_rotationAngle), glm::vec3(0, 1, 0))
		* glm::mat4(1.0f);
	planet0_rotationAngle -= 0.5f;
	glUseProgram(planetCommonShader);

	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.position"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "viewPos"), 1, &cameraPos[0]);
	//std::cout << glGetError() << std::endl; // returns 0 (no error)

	glm::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
	glm::vec3 lightDiffuse(diffuse, diffuse, diffuse);
	glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.ambient"), 1, &lightAmbient[0]);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.diffuse"), 1, &lightDiffuse[0]);
	glUniform3fv(glGetUniformLocation(planetCommonShader, "light.specular"), 1, &lightSpecular[0]);

	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "view0"), 1, GL_FALSE, &view0[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "projection"), 1, GL_FALSE, &projection[0][0]);

	glUniform1i(glGetUniformLocation(planetCommonShader, "material.emission"), 0);				//sampler
	glUniform1i(glGetUniformLocation(planetCommonShader, "material.diffuse"), 1);				//sampler
	glUniform1i(glGetUniformLocation(planetCommonShader, "material.specular"), 2);				//sampler
	GLfloat materialShininess = 64.0f;															//but not this one
	glUniform1f(glGetUniformLocation(planetCommonShader, "material.shininess"), materialShininess);
	glUniform1i(glGetUniformLocation(planetCommonShader, "FogFlag"), FogFlag);
	glUniform4fv(glGetUniformLocation(planetCommonShader, "FogRealColor"), 1, &FogRealColor[0]);

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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 153600); //98304
	glBindVertexArray(0);
	/****************************************/
	/*          Planet 0 ends here          */
	/****************************************/
	model = glm::translate(glm::mat4(), glm::vec3(0.8f, 0.3f, -1.0f))
		* glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.05f))
		* glm::mat4(1.0f);
	// rebind model matrix
	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "model"), 1, GL_FALSE, &model[0][0]);
	materialShininess = 100.0f;
	glUniform1f(glGetUniformLocation(planetCommonShader, "material.shininess"), materialShininess);
	// bind diffuse map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planet1_diffuseMap);
	// bind specular map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planet1_specularMap);
	// bind emission map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planet1_emissionMap);
	// render the sphere
	glBindVertexArray(planetCommonVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 153600); //98304
	glBindVertexArray(0);
	/****************************************/
	/*          Planet 1 ends here          */
	/****************************************/
	model = glm::translate(glm::mat4(), glm::vec3(0.15f, -0.05f, -0.5f))
		* glm::scale(glm::mat4(), glm::vec3(0.01f, 0.01f, 0.01f))
		* glm::mat4(1.0f);
	// rebind model matrix
	glUniformMatrix4fv(glGetUniformLocation(planetCommonShader, "model"), 1, GL_FALSE, &model[0][0]);
	materialShininess = 10.0f;
	glUniform1f(glGetUniformLocation(planetCommonShader, "material.shininess"), materialShininess);
	// bind diffuse map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, planet2_diffuseMap);
	// bind specular map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, planet2_specularMap);
	// bind emission map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, planet2_emissionMap);
	// render the sphere
	glBindVertexArray(planetCommonVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 153600); //98304
	glBindVertexArray(0);
	/****************************************/
	/*          Planet 2 ends here          */
	/****************************************/

	/****************************************/
	/*        Asteroids starts here         */
	/****************************************/
	glUseProgram(asteroidCommonShader);
	glUniform3fv(glGetUniformLocation(asteroidCommonShader, "light.ambient"), 1, &lightAmbient[0]);
	glUniform3fv(glGetUniformLocation(asteroidCommonShader, "light.diffuse"), 1, &lightDiffuse[0]);
	glUniform3fv(glGetUniformLocation(asteroidCommonShader, "light.specular"), 1, &lightSpecular[0]);

	glUniformMatrix4fv(glGetUniformLocation(asteroidCommonShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(asteroidCommonShader, "view0"), 1, GL_FALSE, &view0[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(asteroidCommonShader, "projection"), 1, GL_FALSE, &projection[0][0]);

	glUniform1i(glGetUniformLocation(asteroidCommonShader, "material.diffuse"), 0);				//sampler
	glUniform1i(glGetUniformLocation(asteroidCommonShader, "material.specular"), 1);				//sampler
	materialShininess = 64.0f;															//but not this one
	glUniform1f(glGetUniformLocation(asteroidCommonShader, "material.shininess"), materialShininess);
	glUniform1i(glGetUniformLocation(asteroidCommonShader, "FogFlag"), FogFlag);
	glUniform4fv(glGetUniformLocation(asteroidCommonShader, "FogRealColor"), 1, &FogRealColor[0]);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, asteroidCommonTexture);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, asteroidCommonTexture);
	for (unsigned int x = 0; x < asteroidVAO.size(); x++) {
		glBindVertexArray(asteroidVAO[x]);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 576, amount);	// primcount is amount
	}
	glBindVertexArray(0);
	/****************************************/
	/*         Asteroids ends here          */
	/****************************************/

	/****************************************/
	/*      Space Vehicle starts here       */
	/****************************************/
	vehicle_rotationAngle += vehicle_s;
	model = glm::translate(glm::mat4(), glm::vec3(0.3f, 0.0f, -0.9f))
		* glm::rotate(glm::mat4(), glm::radians(vehicle_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::translate(glm::mat4(), glm::vec3(0.2f, 0.0f, 0.0f))
		* glm::scale(glm::mat4(), glm::vec3(0.02f))
		* glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::mat4(1.0f);
	vehiclePosX = model[3][0];
	vehiclePosY = model[3][1];
	vehiclePosZ = model[3][2];

	glUseProgram(vehicleShader);
	//Store past model matrices in buffer!!!
	vehicleHistory.push_back(model);

	glUniform3fv(glGetUniformLocation(vehicleShader, "light.position"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(vehicleShader, "viewPos"), 1, &cameraPos[0]);
	std::cout << glGetError() << std::endl; // returns 0 (no error)

	glUniform3fv(glGetUniformLocation(vehicleShader, "light.ambient"), 1, &lightAmbient[0]);
	glUniform3fv(glGetUniformLocation(vehicleShader, "light.diffuse"), 1, &lightDiffuse[0]);
	glUniform3fv(glGetUniformLocation(vehicleShader, "light.specular"), 1, &lightSpecular[0]);

	glUniformMatrix4fv(glGetUniformLocation(vehicleShader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(vehicleShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(vehicleShader, "view0"), 1, GL_FALSE, &view0[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(vehicleShader, "projection"), 1, GL_FALSE, &projection[0][0]);

	glUniform1i(glGetUniformLocation(vehicleShader, "material.emission"), 0);				//sampler
	glUniform1i(glGetUniformLocation(vehicleShader, "material.diffuse"), 1);				//sampler
	glUniform1i(glGetUniformLocation(vehicleShader, "material.specular"), 2);				//sampler
	materialShininess = 64.0f;															//but not this one
	glUniform1f(glGetUniformLocation(vehicleShader, "material.shininess"), materialShininess);
	glUniform1i(glGetUniformLocation(vehicleShader, "FogFlag"), FogFlag);
	glUniform4fv(glGetUniformLocation(vehicleShader, "FogRealColor"), 1, &FogRealColor[0]);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, vehicle_diffuseMap);
	// bind specular map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, vehicle_specularMap);
	// bind emission map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, vehicle_emissionMap);
	// render the vehicle
	glBindVertexArray(vehicleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 154959);
	glBindVertexArray(0);
	/****************************************/
	/*       Space Vehicle ends here        */
	/****************************************/

	/****************************************/
	/*        Star Trail starts here        */
	/****************************************/
	if (vehicleHistory.size() == 120) {

		model = vehicleHistory.front()
			* glm::scale(glm::mat4(), glm::vec3(.1f, .1f, .1f))
			* glm::mat4();
		glUseProgram(starCommonShader);

		glUniform3fv(glGetUniformLocation(starCommonShader, "light.position"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(starCommonShader, "viewPos"), 1, &cameraPos[0]);

		glUniform3fv(glGetUniformLocation(starCommonShader, "light.ambient"), 1, &lightAmbient[0]);
		glUniform3fv(glGetUniformLocation(starCommonShader, "light.diffuse"), 1, &lightDiffuse[0]);
		glUniform3fv(glGetUniformLocation(starCommonShader, "light.specular"), 1, &lightSpecular[0]);

		glUniformMatrix4fv(glGetUniformLocation(starCommonShader, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(starCommonShader, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(starCommonShader, "view0"), 1, GL_FALSE, &view0[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(starCommonShader, "projection"), 1, GL_FALSE, &projection[0][0]);

		glUniform1i(glGetUniformLocation(starCommonShader, "material.emission"), 0);				//sampler
		glUniform1i(glGetUniformLocation(starCommonShader, "material.diffuse"), 1);				//sampler
		glUniform1i(glGetUniformLocation(starCommonShader, "material.specular"), 2);				//sampler
		materialShininess = 64.0f;															//but not this one
		glUniform1f(glGetUniformLocation(starCommonShader, "material.shininess"), materialShininess);
		glUniform1i(glGetUniformLocation(starCommonShader, "FogFlag"), FogFlag);
		glUniform4fv(glGetUniformLocation(starCommonShader, "FogRealColor"), 1, &FogRealColor[0]);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, star_diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, star_specularMap);
		// bind emission map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, star_emissionMap);
		// render the last star (smallest)
		glBindVertexArray(starCommonVAO);
		glDrawArrays(GL_TRIANGLES, 0, 8487);
		vehicleHistory.pop_front();
		for (int x = 1; x < 9; x++) {
			// render eight stars from second last to first (bigger and bigger)
			model = vehicleHistory[x*11]
				* glm::scale(glm::mat4(), glm::vec3(.1f+.01f*x, .1f+.01f*x, .1f+.01f*x))
				* glm::mat4();
			// Rebind the model matrix
			glUniformMatrix4fv(glGetUniformLocation(starCommonShader, "model"), 1, GL_FALSE, &model[0][0]);
			glBindVertexArray(starCommonVAO);
			glDrawArrays(GL_TRIANGLES, 0, 8487);
		}
		glBindVertexArray(0);
	}
	/****************************************/
	/*         Star Trail ends here         */
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
		group1->set_selected(0);
		viewpointId = 0;
	}
	if ((key == 's' || key == 'S'))
	{
		group1->set_selected(1);
		viewpointId = 1;
	}
	if ((key == 'd' || key == 'D'))
	{
		group1->set_selected(2);
		viewpointId = 2;
	}
	if ((key == 'f' || key == 'F'))
	{
		hor -= 0.1f;
	}
	if ((key == 'h' || key == 'H') && (hor < 1.0f))
	{
		hor += 0.1f;
	}
	if ((key == 't' || key == 'T') && (ver < 1.0f))
	{
		ver += 0.1f;
	}
	if ((key == 'g' || key == 'G') && (ver > -1.0f))
	{
		ver -= 0.1f;
	}
	if (key == 'q' || key == 'Q')
	{
		diffuse += 0.1f;
	}
	if (key == 'e' || key == 'E')
	{
		diffuse -= 0.1f;
	}
	if (key == '\033')
	{
		exit(0);
	}
}

void myGlutMouse(int button, int state, int x, int y) {
	////TODO: 
	//moveX = (256.0 - x) / 128.0;
	//moveY = (y - 256.0) / 128.0;
}

void glui_callback(int control_id) {
	switch (control_id) 
	{
		case 17:	// fog colour
			if (fogColorId == 0) {
				FogRealColor = glm::vec4(.8f, .8f, .8f, 1.0f);
				printf("Fog colour is white\n");
			} else if (fogColorId == 1) {
				FogRealColor = glm::vec4(0.0f, 0.467f, 0.745f, 1.0f);
				printf("Fog colour is blue\n");
			}
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

GLuint loadAsteroidTexture(string im)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

GLuint loadStarTexture(string im)
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
		std::cout << "Star texture failed to load at path: " << im << std::endl;
		SOIL_free_image_data(data);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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
	glutInitWindowSize(1400, 600);

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
	loadOBJ("io2.obj", vertices, uvs, normals);
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

	planet0_emissionMap = loadSphereTexture("myearth.jpg");
	planet0_diffuseMap = loadSphereTexture("myearth.jpg");
	planet0_specularMap = loadSphereTexture("myearth_specular.jpg");

	planet1_diffuseMap = loadSphereTexture("io.jpg");
	planet1_specularMap = loadSphereTexture("io.jpg");
	planet1_emissionMap = loadSphereTexture("io_specular.jpg");

	planet2_diffuseMap = loadSphereTexture("neptune.jpg");
	planet2_specularMap = loadSphereTexture("neptune.jpg");
	planet2_emissionMap = loadSphereTexture("neptune.jpg");

	planetCommonShader = installShaders("planetCommon.vs", "planetCommon.fs");

	// Asteroids
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glutGet(GLUT_ELAPSED_TIME)); // initialize random seed	
	float radius = 3.0f;
	float offset = 0.2f;

	vertices.clear();
	uvs.clear();
	normals.clear();
	loadOBJ("rock.obj", vertices, uvs, normals);

	// Asteroids Vertices
	glGenBuffers(1, &asteroidCommonVBO);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Asteroids UVs
	glGenBuffers(1, &asteroidCommonUV);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonUV);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	// Asteroids Normals
	glGenBuffers(1, &asteroidCommonNormal);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonNormal);				// VBO
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < amount; i++) {
		glm::mat4 model=
			glm::translate(glm::mat4(), glm::vec3(0.8f, 0.3f, -1.0f))
			* glm::scale(glm::mat4(), glm::vec3(0.05f))
			* glm::mat4(); //			glm::translate(glm::mat4(), glm::vec3(0.3f, 0.0f, -0.9f))
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	// configure instanced array
	// -------------------------
	glGenBuffers(1, &asteroidCommonInstanced);
	glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonInstanced);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int x = 0; x < vertices.size(); x++) {
		GLuint tmp;
		glGenVertexArrays(1, &tmp);
		asteroidVAO.push_back(tmp);
		glBindVertexArray(tmp);

		glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonUV);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonNormal);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// set attribute pointers for matrix (4 times vec4)
		glBindBuffer(GL_ARRAY_BUFFER, asteroidCommonInstanced);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
	asteroidCommonTexture = loadAsteroidTexture("asteroid.jpg");
	asteroidCommonShader = installShaders("asteroidCommon.vs", "asteroidCommon.fs");

	// space vehicle 154959
	vertices.clear();
	uvs.clear();
	normals.clear();
	loadOBJ("Arc170.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &vehicleVAO);

	// Planet Vertices
	glGenBuffers(1, &vehicleVBO);
	glBindVertexArray(vehicleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vehicleVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vehicleVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Planet UVs
	glGenBuffers(1, &vehicleUV);
	glBindBuffer(GL_ARRAY_BUFFER, vehicleUV);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vehicleUV);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Planet Normals
	glGenBuffers(1, &vehicleNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vehicleNormal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vehicleNormal);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	vehicle_diffuseMap = loadSphereTexture("shiny-metal-background.jpg");
	vehicle_specularMap = loadSphereTexture("shiny-metal-background-specular.jpg");
	vehicle_emissionMap = loadSphereTexture("shiny-metal-background.jpg");
	vehicleShader = installShaders("planetCommon.vs", "planetCommon.fs");

	// starfy trail 8478
	vertices.clear();
	uvs.clear();
	normals.clear();
	loadOBJ("starfy.obj", vertices, uvs, normals);
	// Star Vertices
	glGenBuffers(1, &starVBO);
	glBindVertexArray(starCommonVAO);
	glBindBuffer(GL_ARRAY_BUFFER, starVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, starVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Star UVs
	glGenBuffers(1, &starCommonUV);
	glBindBuffer(GL_ARRAY_BUFFER, starCommonUV);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, starCommonUV);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Star Normals
	glGenBuffers(1, &starCommonNormal);
	glBindBuffer(GL_ARRAY_BUFFER, starCommonNormal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, starCommonNormal);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	star_diffuseMap = loadStarTexture("starfy.bmp");
	star_specularMap = loadStarTexture("starfy.bmp");
	star_emissionMap = loadStarTexture("starfy.bmp");
	starCommonShader = installShaders("planetCommon.vs", "planetCommon.fs");

	vertices.clear();
	uvs.clear();
	normals.clear();

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
	glutReshapeFunc(myGlutReshape);

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
	GLUI_StaticText *heading = glui->add_statictext("iNavigation@CUHK");
	heading->set_alignment(GLUI_ALIGN_CENTER);
	glui->add_separator();

	GLUI_Spinner *segment_spinner =
		glui->add_spinner("Vehicle_S:", GLUI_SPINNER_FLOAT, &vehicle_s);
	segment_spinner->set_float_limits(0.0f,10.0f,GLUI_LIMIT_CLAMP);
	//segment_spinner->set_float_val(1.5f);

	//GLUI_Listbox *listbox = glui->add_listbox("A listbox");
	//listbox->add_item(1, "Red");
	//listbox->add_item(2, "Green");
	//listbox->add_item(3, "Blue");

	GLUI_Rollout *render_rollout = glui->add_rollout("Render Control");

	GLUI_Panel *obj_panel = glui->add_panel_to_panel(render_rollout, "Viewpoint");
	group1 = glui->add_radiogroup_to_panel(obj_panel, &viewpointId);
	glui->add_radiobutton_to_group(group1, "Left");
	glui->add_radiobutton_to_group(group1, "Top");
	glui->add_radiobutton_to_group(group1, "Vehicle");

	GLUI_Panel *fog_panel = glui->add_panel_to_panel(render_rollout, "Fog On/Off");
	GLUI_Checkbox *fog_checkbox = glui->add_checkbox_to_panel(fog_panel, "Fog", &FogFlag);

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
	GLUI_Master.set_glutMouseFunc(myGlutMouse);

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