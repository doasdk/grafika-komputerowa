#pragma once
#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"

#include "Shader_Loader.h"
#include "Render_Utils.h"

// funkcja renderujaca scene    
void renderScene(GLFWwindow* window)
{
    float time = glfwGetTime();

    // ZADANIE: Przesledz kod i komentarze
    // ZADANIE: Zmien kolor tla sceny, przyjmujac zmiennoprzecinkowy standard RGBA
    float color1 = 12 * sin(20*time);
    float color2 = 16.8f * sin(13*time);
    float color3 = 12.1f * sin(9*time);
    float color4 = 15.2f * sin(3*time);

    glClearColor(color1, color2, color3, color4);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Powinno byc wywolane po kazdej klatce
    glfwSwapBuffers(window);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void init(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void shutdown(GLFWwindow* window)
{
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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