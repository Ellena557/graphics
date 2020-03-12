#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")

#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "common/shader.hpp"
#include "common/shader.cpp"

#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
using namespace std;

GLFWwindow* window;

int main()
{
	if (!glfwInit())
	{
		cout << "An error occured while GLFW initializing!" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x smoothing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // in order to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // the same reason
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // new OpenGL

	// open the window and create the OpenGL context here
	window = glfwCreateWindow(1024, 768, "HW3", NULL, NULL);

	if (window == NULL) {
		cout << "Can't open the window!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cout << "An error occured while GLFW initializing!" << endl;
		glfwTerminate();
		return -1;
	}

	// button press reaction
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// background
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// create Vertex Array Object 
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// create program from shader
	GLuint programID = LoadShaders("vertex.vertexshader", "triang.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Model matrix : an identity matrix
	glm::mat4 Model = glm::mat4(1.0f);

	vec3 cameraPos = glm::vec3(3, 2, 0);
	vec3 originPos = glm::vec3(0, 0, 0);
	vec3 headPos = glm::vec3(0, 1, 0);

	glm::mat4 View = glm::lookAt(cameraPos, originPos, headPos);

	// ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model;

	// a regular icosahedron consists of 20 triangles, so there will be 3*20=60 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		0.809f, 0.5f, 0.588f, //1st triange (1-2-3)
		0.309f, -0.5f, 0.951f,
		-0.309f, 0.5f, 0.951f,

		0.809f, 0.5f, 0.588f, //2nd triange (1-10-2)
		1.0f, -0.5f, 0.0f,
		0.309f, -0.5f, 0.951f,

		0.809f, 0.5f, 0.588f, //3rd triange (1-9-10)
		0.809f, 0.5f, -0.588f,
		1.0f, -0.5f, 0.0f,

		0.809f, 0.5f, -0.588f, //4th triange (9-8-10)
		0.309f, -0.5f, -0.951f,
		1.0f, -0.5f, 0.0f,

		0.809f, 0.5f, -0.588f, //5th triange (9-7-8)
		-0.309f, 0.5f, -0.951f,
		0.309f, -0.5f, -0.951f,

		-0.309f, 0.5f, -0.951f, //6th triange (7-6-8)
		-0.809f, -0.5f, -0.588f,
		0.309f, -0.5f, -0.951f,

		-0.309f, 0.5f, -0.951f, //7th triange (7-5-6)
		-1.0f, 0.5f, 0.0f,
		-0.809f, -0.5f, -0.588f,

		-1.0f, 0.5f, 0.0f, //8th triange (5-4-6)
		-0.809f, -0.5f, 0.588f,
		-0.809f, -0.5f, -0.588f,

		-1.0f, 0.5f, 0.0f, //9th triange (5-3-4)
		-0.309f, 0.5f, 0.951f,
		-0.809f, -0.5f, 0.588f,

		-0.309f, 0.5f, 0.951f,  //10th triange (3-2-4)
		0.309f, -0.5f, 0.951f,
		-0.809f, -0.5f, 0.588f,

		0.0f, 1.118f, 0.0f,  //11th triange (11-3-1)
		-0.309f, 0.5f, 0.951f,
		0.809f, 0.5f, 0.588f,

		0.0f, 1.118f, 0.0f,  //12th triange (11-1-9)
		0.809f, 0.5f, 0.588f,
		0.809f, 0.5f, -0.588f,

		0.0f, 1.118f, 0.0f,  //13th triange (11-9-7)
		0.809f, 0.5f, -0.588f,
		-0.309f, 0.5f, -0.951f,

		0.0f, 1.118f, 0.0f,  //14th triange (11-7-5)
		-0.309f, 0.5f, -0.951f,
		-1.0f, 0.5f, 0.0f,

		0.0f, 1.118f, 0.0f,  //15th triange (11-5-3)
		-1.0f, 0.5f, 0.0f,
		-0.309f, 0.5f, 0.951f,

		0.0f, -1.118f, 0.0f,  //16th triange (12-2-10)
		0.309f, -0.5f, 0.951f,
		1.0f, -0.5f, 0.0f,

		0.0f, -1.118f, 0.0f,  //17th triange (12-10-8)
		1.0f, -0.5f, 0.0f,
		0.309f, -0.5f, -0.951f,

		0.0f, -1.118f, 0.0f,  //18th triange (12-8-6)
		0.309f, -0.5f, -0.951f,
		-0.809f, -0.5f, -0.588f,

		0.0f, -1.118f, 0.0f,  //19th triange (12-6-4)
		-0.809f, -0.5f, -0.588f,
		-0.809f, -0.5f, 0.588f,

		0.0f, -1.118f, 0.0f,  //20th triange (12-4-2)
		-0.809f, -0.5f, 0.588f,
		0.309f, -0.5f, 0.951f
	};

	// One color for each vertex. 
	static const GLfloat g_color_buffer_data[] = {
		1.0f, 0.97f, 0.0f,		//1
		0.0f, 1.0f, 0.267f,
		1.0f, 0.97f, 0.0f,

		1.0f, 0.54f, 0.0f,	    //2	
		1.0f, 0.54f, 0.0f,
		0.0f, 1.0f, 0.737f,

		1.0f, 0.97f, 0.0f,
		0.0f, 1.0f, 0.267f,
		1.0f, 0.97f, 0.0f,

		0.0f, 1.0f, 0.737f,
		0.0f, 1.0f, 0.737f,
		1.0f, 0.54f, 0.0f,

		0.0f, 1.0f, 0.267f,
		1.0f, 0.97f, 0.0f,
		0.0f, 1.0f, 0.267f,

		1.0f, 0.54f, 0.0f,		//6
		1.0f, 0.54f, 0.0f,
		0.0f, 1.0f, 0.737f,

		1.0f, 0.97f, 0.0f,
		0.0f, 1.0f, 0.267f,
		1.0f, 0.97f, 0.0f,

		0.0f, 1.0f, 0.737f,
		0.0f, 1.0f, 0.737f,
		1.0f, 0.54f, 0.0f,

		1.0f, 0.97f, 0.0f,
		1.0f, 0.97f, 0.0f,
		0.0f, 1.0f, 0.267f,

		1.0f, 0.54f, 0.0f,
		1.0f, 0.54f, 0.0f,
		0.0f, 1.0f, 0.737f,

		1.0f, 0.0f, 0.078f,		//11
		1.0f, 0.592f, 0.51f,
		1.0f, 0.592f, 0.51f,

		1.0f, 0.0f, 0.078f,		
		1.0f, 0.56f, 0.686f,
		1.0f, 0.56f, 0.686f,

		1.0f, 0.0f, 0.078f,
		1.0f, 0.4f, 1.0f,
		1.0f, 0.4f, 1.0f,

		1.0f, 0.0f, 0.078f,
		0.784f, 0.0f, 1.0f,
		0.784f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.078f,
		0.467f, 0.02f, 1.0f,
		0.467f, 0.02f, 1.0f,

		1.0f, 0.0f, 0.078f,		//16
		1.0f, 0.592f, 0.51f,
		1.0f, 0.592f, 0.51f,

		1.0f, 0.0f, 0.078f,
		1.0f, 0.56f, 0.686f,
		1.0f, 0.56f, 0.686f,

		1.0f, 0.0f, 0.078f,
		1.0f, 0.4f, 1.0f,
		1.0f, 0.4f, 1.0f,

		1.0f, 0.0f, 0.078f,
		0.784f, 0.0f, 1.0f,
		0.784f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.078f,
		0.467f, 0.02f, 1.0f,
		0.467f, 0.02f, 1.0f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// the camera will move on the circle
	double pos = 0.0;
	double pos2 = 0.0;  // to move along the 2nd axis
	double pi = 3.1415;

	do {
		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use program
		glUseProgram(programID);

		glm::mat4 View = glm::lookAt(cameraPos, originPos, headPos);

		// ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View * Model;

		pos += pi / 360 / 12; // to move rather slow
		pos2 += pi / 360 / 20; // to move even slower

		cameraPos = glm::vec3(3 * cos(pos), 2 * cos(pos2), 3 * sin(pos));

        // Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// vertices will be the 1st buffer attribute
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		
		// 3=size, GL_FLOAT=type, GL_FALSE=values are not normalized, 0=step
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// colors will be the 2nd buffer attribute
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// draw the triangles
		glDrawArrays(GL_TRIANGLES, 0, 3 * 20);   // start from 0 vertex, use 3 * 20 of them

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} // pressing Escape or window closing
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// cleanup 
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window
	glfwTerminate();

	cout << "The end." << endl;
	return 0;
}
