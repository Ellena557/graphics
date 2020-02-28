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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // same reason
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // new OpenGL

	// open the window and create the OpenGL context here
	window = glfwCreateWindow(1024, 768, "HW1", NULL, NULL);

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
		return -1;
	}

	// button press reaction
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//background
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

	//create Vertex Array Object 
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID1 = LoadShaders("hw1vertex.vertexshader", "hw1triang1.fragmentshader");
	GLuint programID2 = LoadShaders("hw1vertex.vertexshader", "hw1triang2.fragmentshader");

	// 6 vertexes of the triangles
	static const GLfloat g_vertex_buffer_data[] = {
	   0.7f, 0.7f, 0.0f,
	   0.7f, -0.1f, 0.0f,
	   -0.7f, -0.7f, 0.0f,

	   -0.7f, 0.7f, 0.0f,
	   -0.7f, -0.1f, 0.0f,
	   0.7f, -0.7f, 0.0f,
	};

	// identifier of a vertex buffer
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);

	// make the buffer the current one
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// send the info about vertixes to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do {
		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// use program
		glUseProgram(programID1);

		// vertixes will be the 1st buffer attribute
		glEnableVertexAttribArray(0);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		// 3=size, GL_FLOAT=type, GL_FALSE=values are not normalized, 0=step
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// draw the first triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);   // start from 0 vertex, use 3 of them

		//draw the second triangle
		glUseProgram(programID2);
		glDrawArrays(GL_TRIANGLES, 3, 3); 

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} // pressing Escape or window closing
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	
	// cleanup 
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID1);
	glDeleteProgram(programID2);

	// Close OpenGL window
	glfwTerminate();

	cout << "End of the program" << endl;
	return 0;
}
