#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>


GLuint program, program_box;
Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;

glm::vec3 cameraPos = glm::vec3(-1.f, 0, 0);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
GLuint VAO, VBO;

float aspectRatio = 1.f;

std::vector<glm::vec3> quadsPositions = {
	glm::vec3(0.0, 0.0, 0.0),
	glm::vec3(0.0, 0.7, 0.0),
	glm::vec3(0.7, 0.7, 0.7),
};

glm::vec3 p = glm::vec3(0, 0, 0);


glm::mat4 createCameraMatrix() {

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// ! Macierz translation jest definiowana wierszowo dla poprawy czytelnosci. OpenGL i GLM domyslnie stosuje macierze kolumnowe, dlatego musimy ja transponowac !
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0, 1, 0)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, cameraDir));
	glm::mat4 cameraRotationMatrix = glm::mat4({
		cameraSide.x, cameraSide.y, cameraSide.z, 0.,
		cameraUp.x, cameraUp.y, cameraUp.z, 0.,
		-cameraDir.x, -cameraDir.y, -cameraDir.z, 0.,
		0.,0.,0.,1.,
		});
	cameraRotationMatrix = glm::transpose(cameraRotationMatrix);

	//Macierz translacji M_VT otrzymujemy przez translacje o `-cameraPos`
	glm::mat4 cameraMatrix = cameraRotationMatrix * glm::translate(glm::mat4(1.0), -cameraPos);;

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix(float fov, float aspectRatio) {


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// ! Macierz translation jest definiowana wierszowo dla poprawy czytelnosci. OpenGL i GLM domyslnie stosuje macierze kolumnowe, dlatego musimy ja transponowac !
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	float n = 0.1f;
	//float f = 0.5f;//żeby tylna ściana sześcianu zniknęła
	float f = 100.f;

	float s = 1 / (glm::tan(fov * glm::pi<float>() / 360));

	//zad 4
	/*glm::mat4 perspectiveMatrix = glm::mat4({
		s, 0., 0., 0.,
		0., s * aspectRatio, 0., 0.,
		0., 0., (n+f)/(n-f),2*n*f/(n-f),
		0., 0., -1., 0,
	});*/
	//zad 4*
	float xMult = 1;
	float yMult = 1;
	if (aspectRatio > 1) {
		xMult = 1 / aspectRatio;
	}
	else {
		yMult = aspectRatio;
	}

	glm::mat4 perspectiveMatrix = glm::mat4({
		s * xMult, 0., 0., 0.,
		0., s * yMult, 0., 0.,
		0., 0., (n + f) / (n - f),2 * n * f / (n - f),
		0., 0., -1., 0,
		});


	perspectiveMatrix = glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}
void renderScene(GLFWwindow* window)
{
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 transformation;

	glUseProgram(program_box);


	glBindVertexArray(VAO);
	float fov = 90;
	transformation = createPerspectiveMatrix(fov, aspectRatio);
	//zad 5
	transformation *= createCameraMatrix();

	//Zad 6
	for (const glm::vec3& pos : quadsPositions) {
		glBindVertexArray(VAO);

		glm::mat4 currTransform = glm::mat4(1.0f);
		currTransform *= createPerspectiveMatrix(fov, aspectRatio);
		currTransform *= createCameraMatrix();

		currTransform *= glm::translate(glm::mat4(1.0f), pos);

		glUniformMatrix4fv(glGetUniformLocation(program_box, "transformation"), 1, GL_FALSE, (float*)&currTransform);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//нафига вообще это нужно?
		glBindVertexArray(0);
	}

	//glUniformMatrix4fv(glGetUniformLocation(program_box, "transformation"), 1, GL_FALSE, (float*)&transformation);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);
	glUseProgram(0);
	glfwSwapBuffers(window);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / float(height);
	glViewport(0, 0, width, height);
}
void loadModelToContext(std::string path, Core::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_3_1.vert", "shaders/shader_3_1.frag");
	program_box = shaderLoader.CreateProgram("shaders/shader_2_1.vert", "shaders/shader_2_1.frag");

	glGenVertexArrays(1, &VAO);
	// 2. Powiąż stworzone VAO za pomocą funkcji glBindVertexArray
	glBindVertexArray(VAO);
	// II Zainicjalizuj VBO
	// 3. Stwórz VertexBufferObject do przechowywania pozycji wierzchołków za pomocą funkcji glGenBuffers
	glGenBuffers(1, &VBO);
	// 4. Powiąż stworzone VBO za pomocą funkcji glBindBuffer(GLenum target, GLuint )
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 5. Zaalokuj w GPU miejsce na tablice box i zaalakokuj ją za pomocą glBufferData
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	// II Stwórz definicję danych w buforze
	// 6. Aktywuj atrybut powiązany z pozycją wierchołków za pomocą glEnableVertexAttribArray(GLuint index). Indeks jest zapisany w shaderze wierzchołków w 3. lini. Można odpytać shader o indeks za pomocą funkcji glGetAttribLocation(GL uint program, const GLchar *name)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));

}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	float angleSpeed = 0.005f;
	float moveSpeed = 0.005f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos -= cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));
	//zad 7*
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		cameraPos += glm::vec3(0, 1, 0) * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		cameraPos -= glm::vec3(0, 1, 0) * moveSpeed;

	cameraDir = glm::normalize(p - cameraPos);

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
//#include "glew.h"
//#include <GLFW/glfw3.h>
//#include "glm.hpp"
//#include "ext.hpp"
//#include <iostream>
//#include <cmath>
//
//#include "Shader_Loader.h"
//#include "Render_Utils.h"
//#include "Camera.h"
//
//#include "Box.cpp"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include <string>
//
//
//
//GLuint program, program_box;
//Core::Shader_Loader shaderLoader;
//
//Core::RenderContext shipContext;
//Core::RenderContext sphereContext;
//
//glm::vec3 cameraPos = glm::vec3(-1.f, 0, 0);
//glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
//
//GLuint VAO, VBO;
//
//float aspectRatio = 1.f;
//
//
//glm::mat4 createCameraMatrix() {
//
//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	// ! Macierz translation jest definiowana wierszowo dla poprawy czytelnosci. OpenGL i GLM domyslnie stosuje macierze kolumnowe, dlatego musimy ja transponowac !
//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	
//	glm::vec3 cameraSide = glm::normalize(cameraDir * glm::vec3(0.f, 1.f, 0.f));
//
//	glm::vec3 cameraUp = glm::normalize(cameraSide * cameraDir);
//	
//	glm::mat4 cameraRotationMatrix = glm::mat4({
//		cameraSide.x,cameraSide.y,cameraSide.z,0.,
//		cameraUp.x,cameraUp.y,cameraUp.z,0.,
//		-cameraDir.x,-cameraDir.y,-cameraDir.z,0.,
//		0.,0.,0.,1.,
//		});
//
//	cameraRotationMatrix = glm::transpose(cameraRotationMatrix);
//	glm::mat4 cameraTranslation = glm::mat4({
//		1.,0.,0.,0.,
//		0.,1.,0.,0.,
//		0.,0.,1.,0.,
//		-cameraPos.x,-cameraPos.y,-cameraPos.z,1.,
//	});
//
//
//	glm::mat4 cameraMatrix;
//	cameraMatrix = cameraRotationMatrix * cameraTranslation;
//	return cameraMatrix;
//}
//
//glm::mat4 createPerspectiveMatrix() {
//
//
//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	// ! Macierz translation jest definiowana wierszowo dla poprawy czytelnosci. OpenGL i GLM domyslnie stosuje macierze kolumnowe, dlatego musimy ja transponowac !
//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	
//
//	float nearPlane = 0.1f;  
//	float farPlane = 100.f;
//	float S;
//	float fov = 45.0f;
//	float m33, m34;
//
//
//
//	m33 = (nearPlane + farPlane) / (nearPlane - farPlane);
//	m34 = (2 * nearPlane * farPlane) / (nearPlane - farPlane);
//	
//	S = 1.f / (tan((fov / 2)*(3.14159f/180.f)));
//
//	glm::mat4 perspectiveMatrix = glm::mat4({
//		S,0.,0.,0.,
//		0.,S*aspectRatio,0.,0.,
//		0.,0.,m33,m34,
//		0.,0.,-1.,0.,
//		});
//
//	
//
//
//	perspectiveMatrix = glm::transpose(perspectiveMatrix);
//	return perspectiveMatrix;
//	
//	
//	/*glm::mat4 perspectiveMatrix = glm::mat4({
//		1.,0.,0.,0.,
//		0.,1.,0.,0.,
//		0.,0.,1.,0.,
//		0.,0.,0.,1.,
//		});
//
//	perspectiveMatrix = glm::transpose(perspectiveMatrix);
//	
//	return perspectiveMatrix;
//	*/
//		
//}
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	aspectRatio = width / float(height);
//	glViewport(0, 0, width, height);
//}
//void renderScene(GLFWwindow* window)
//{
//	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	int width, height;
//	glfwGetFramebufferSize(window, &width, &height);
//
//	glm::mat4 transformation;
//
//	glUseProgram(program_box);
//
//	framebuffer_size_callback(window, width, height);
//
//	glBindVertexArray(VAO);
//	
//	glm::mat4 perspective = createPerspectiveMatrix();
//	glm::mat4 cameraMatrix = createCameraMatrix();
//	transformation = perspective * cameraMatrix;
//
//	glUniformMatrix4fv(glGetUniformLocation(program_box, "transformation"), 1, GL_FALSE, (float*)&transformation);
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glBindVertexArray(0);
//
//	glUseProgram(0);
//	glfwSwapBuffers(window);
//}
//
//void loadModelToContext(std::string path, Core::RenderContext& context)
//{
//	Assimp::Importer import;
//	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
//
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
//		return;
//	}
//}
//
//void init(GLFWwindow* window)
//{
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	glEnable(GL_DEPTH_TEST);
//	program = shaderLoader.CreateProgram("shaders/shader_3_1.vert", "shaders/shader_3_1.frag");
//	program_box = shaderLoader.CreateProgram("shaders/shader_2_1.vert", "shaders/shader_2_1.frag");
//
//	glGenVertexArrays(1, &VAO);
//	// 2. Powiąż stworzone VAO za pomocą funkcji glBindVertexArray
//	glBindVertexArray(VAO);
//	// II Zainicjalizuj VBO
//	// 3. Stwórz VertexBufferObject do przechowywania pozycji wierzchołków za pomocą funkcji glGenBuffers
//	glGenBuffers(1, &VBO);
//	// 4. Powiąż stworzone VBO za pomocą funkcji glBindBuffer(GLenum target, GLuint )
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	// 5. Zaalokuj w GPU miejsce na tablice box i zaalakokuj ją za pomocą glBufferData
//	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
//	// II Stwórz definicję danych w buforze
//	// 6. Aktywuj atrybut powiązany z pozycją wierchołków za pomocą glEnableVertexAttribArray(GLuint index). Indeks jest zapisany w shaderze wierzchołków w 3. lini. Można odpytać shader o indeks za pomocą funkcji glGetAttribLocation(GL uint program, const GLchar *name)
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
//
//}
//
//void shutdown(GLFWwindow* window)
//{
//	shaderLoader.DeleteProgram(program);
//}
//
////obsluga wejscia
//void processInput(GLFWwindow* window)
//{
//	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
//	float angleSpeed = 0.05f;
//	float moveSpeed = 0.05f;
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		cameraPos += cameraDir * moveSpeed;
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		cameraPos -= cameraDir * moveSpeed;
//	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
//		cameraPos -= cameraSide * moveSpeed;
//	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
//		cameraPos += cameraSide * moveSpeed;
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));
//
//}
//
//// funkcja jest glowna petla
//void renderLoop(GLFWwindow* window) {
//	while (!glfwWindowShouldClose(window))
//	{
//		processInput(window);
//
//		renderScene(window);
//		glfwPollEvents();
//	}
//}
////}