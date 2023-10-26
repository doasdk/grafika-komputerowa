#pragma once
#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <vector>

#include "Shader_Loader.h"
#include "Render_Utils.h"

GLuint program; // Shader ID

GLuint quadVAO;
bool key_O = false;
bool key_P = false;
float currentRotationAngle = 0.0f;


Core::Shader_Loader shaderLoader;
std::vector<glm::vec3> quadsPositions;


glm::vec3 squarePos(0.0f, 0.0f, 0.0f);

void renderScene(GLFWwindow* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	float time = glfwGetTime();

    glUseProgram(program);
    //glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 transformation = glm::mat4(1.0f);

    for (int i = 0; i < quadsPositions.size(); i++) {
        glm::mat4 translateNewSquare = glm::translate(transformation, quadsPositions[i]);
        glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&translateNewSquare);
        Core::drawVAOIndexed(quadVAO, 6);
    }


    /*transformation = glm::translate(transformation, squarepos);
    if (key_o) {
        currentrotationangle += 0.1f; 
    }
    if (key_p) {
        currentrotationangle -= 0.1f;
    }
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(currentrotationangle), glm::vec3(0.0f, 0.0f, 1.0f));
    transformation = transformation * rotation;
    */

    //glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
    //Core::drawVAOIndexed(quadVAO, 6);
    
    glUseProgram(0);

    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    /*int width, height;
    glfwGetWindowSize(window, &width, &height);
    xpos = ((2 * xpos) / (width)) - 1;
    ypos = -(((2 * ypos) / (height)) - 1);
    squarePos = glm::vec3(xpos, ypos, 0.0f);
    */
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float x = (2 * xpos / width) - 1;
    float y = -((2 * ypos / height) - 1);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
      
        quadsPositions.push_back(glm::vec3(x, y, 0.0f));
    }

    printf("%f,%f\n", xpos, ypos);
}


void init(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    program = shaderLoader.CreateProgram("shaders/shader_1_2.vert", "shaders/shader_1_2.frag");

    float points[] = {
        -0.1,-0.1,0.0,1.0,
         0.1,-0.1,0.0,1.0,
         0.1, 0.1,0.0,1.0,
        -0.1, 0.1,0.0,1.0,
    };
    unsigned int indices[] = {
        0,1,3,
        1,2,3,
    };
    quadVAO = Core::initVAOIndexed(points, indices, 4, 4, 6);

    //Przekopiuj kod do ladowania z poprzedniego zadania
}

void shutdown(GLFWwindow* window)
{
    shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        squarePos += glm::vec3(0.0f, 0.001f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        squarePos += glm::vec3(0.0f, -0.001f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        squarePos += glm::vec3(-0.001f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        squarePos += glm::vec3(0.001f, 0.00f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            key_O = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_O) != GLFW_PRESS) {
        key_O = false;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        key_P = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_P) != GLFW_PRESS) {
        key_P = false;
    }

}

// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderScene(window);
        glfwPollEvents();
    }
}
//}