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



GLuint program;
Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;
Core::RenderContext asteroidContext;

glm::vec3 cameraPos = glm::vec3(-4.f, 0, 0);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);

glm::vec3 cameraSide;
glm::vec3 cameraUp;

GLuint VAO,VBO;

//glm::vec3 spaceshipPos = cameraPos + 1.5 * cameraDir + glm::vec3(0, -0.5f, 0);
//glm::vec3 spaceshipDir = cameraDir;

float aspectRatio = 1.f;

glm::vec3 lightColor = glm::vec3(0.9, 0.7, 0.8);

glm::mat4 createCameraMatrix()
{
	cameraSide = glm::normalize(glm::cross(cameraDir,glm::vec3(0.f,1.f,0.f)));
	cameraUp = glm::normalize(glm::cross(cameraSide,cameraDir));
	//glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});

	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);

	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos);

	//cameraMatrix = glm::mat4({
	//	1.,0.,0.,cameraPos.x,
	//	0.,1.,0.,cameraPos.y,
	//	0.,0.,1.,cameraPos.z,
	//	0.,0.,0.,1.,
	//	});

	//cameraMatrix = glm::transpose(cameraMatrix);
	//return Core::createViewMatrix(cameraPos, cameraDir, up);

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix()
{
	
	glm::mat4 perspectiveMatrix;
	float n = 0.05;
	float f = 20.;
	float a1 = glm::min(aspectRatio, 1.f);
	float a2 = glm::min(1 / aspectRatio, 1.f);
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,1,0.,0.,
		0.,0.,(f+n) / (n - f),2*f * n / (n - f),
		0.,0.,-1.,0.,
		});

	
	perspectiveMatrix=glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}

void drawObjectColor(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color) {

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;

	glUniform3fv(glGetUniformLocation(program, "colorFrag"), 1, (float*)&color);

	

	//glm::mat4 rotation = glm::mat4({
	//	1.,0.,0.,0.,
	//	0.,1.,0.,0.,
	//	0.,0.,1.,0.,
	//	0.,0.,0.,1.

	//});
	//transformation *= scaleMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);

	Core::DrawContext(context);
}
glm::vec3 calculateCircularOrbit(float radius, float speed, float time) {
	float x = 0.0f; 
	float y = radius * std::cos(speed * time);
	float z = radius * std::sin(speed * time);
	return glm::vec3(x, y, z);
}


void renderScene(GLFWwindow* window)
{
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 transformation;
	float time = glfwGetTime();
	

	//earth
	glm::vec3 earthModelsOrbit = calculateCircularOrbit(2.5f, 1.2f, time);
	glm::mat4 earthModelCirculating = glm::translate(glm::mat4(1.f), earthModelsOrbit);
	glm::mat4 earthModelScaled = glm::scale(earthModelCirculating, glm::vec3(0.f, 0.5f, 0.5f));
	//moon
	glm::vec3 moonCirculating = calculateCircularOrbit(1.f, 2.8f, time);
	glm::mat4 moonScaled = glm::scale(glm::translate(earthModelCirculating, moonCirculating), glm::vec3(0.f, 0.3f, 0.3f));
	
	//1b
	//planet 1
	glm::vec3 planet1ModelsOrbit = calculateCircularOrbit(4.f, 0.25f, time+5.9f);
	glm::mat4 planet1ModelCirculating = glm::translate(glm::mat4(1.f), planet1ModelsOrbit);
	glm::mat4 planet1ModelScaled = glm::scale(planet1ModelCirculating, glm::vec3(0.f, 0.7f, 0.7f));
	//planet 2
	glm::vec3 planet2ModelsOrbit = calculateCircularOrbit(3.1f, 1.9f, time+3.f);
	glm::mat4 planet2ModelCirculating = glm::translate(glm::mat4(1.f), planet2ModelsOrbit);
	glm::mat4 planet2ModelScaled = glm::scale(planet2ModelCirculating, glm::vec3(0.f, 0.15f, 0.15f));
	//planet 3
	glm::vec3 planet3ModelsOrbit = calculateCircularOrbit(2.9f, 0.9f, time+.9f);
	glm::mat4 planet3ModelCirculating = glm::translate(glm::mat4(1.f), planet3ModelsOrbit);
	glm::mat4 planet3ModelScaled = glm::scale(planet3ModelCirculating, glm::vec3(0.f, 0.46f, 0.46f));
	//planet 4
	glm::vec3 planet4ModelsOrbit = calculateCircularOrbit(5.f, 1.f, time+3.9f);
	glm::mat4 planet4ModelCirculating = glm::translate(glm::mat4(1.f), planet4ModelsOrbit);
	glm::mat4 planet4ModelScaled = glm::scale(planet4ModelCirculating, glm::vec3(0.f, 0.31f, 0.31f));
	//planet 5
	glm::vec3 planet5ModelsOrbit = calculateCircularOrbit(4.9f, 1.9f, time+8.9f);
	glm::mat4 planet5ModelCirculating = glm::translate(glm::mat4(1.f), planet5ModelsOrbit);
	glm::mat4 planet5ModelScaled = glm::scale(planet5ModelCirculating, glm::vec3(0.f, 0.89f, 0.89f));

	//asteroid 1
	glm::vec3 astroStartPos1 = glm::vec3(0.f, -5.f, -7.0f);
	glm::vec3 astroDirectionMove1 = astroStartPos1 + glm::vec3(0.f, time * 0.5f, time*0.5f);
	glm::mat4 astro1 = glm::translate(glm::mat4(1.f), astroDirectionMove1);
	glm::mat4 astertoidGoesBrr1 = glm::translate(astro1, astroDirectionMove1);
	astertoidGoesBrr1 = glm::scale(astertoidGoesBrr1, glm::vec3(0.0f,0.005f,0.005f));
	//asteroid 2
	glm::vec3 astroStartPos2 = glm::vec3(0.f, -4.4f, -6.2f);
	glm::vec3 astroDirectionMove2 = astroStartPos2 + glm::vec3(0.f, time * 0.59f, time*0.59f);
	glm::mat4 astro2 = glm::translate(glm::mat4(1.f), astroDirectionMove2);
	glm::mat4 astertoidGoesBrr2 = glm::translate(astro2, astroDirectionMove2);
	astertoidGoesBrr2 = glm::scale(astertoidGoesBrr2, glm::vec3(0.0f,0.019f,0.019f));
	//asteroid 3
	glm::vec3 astroStartPos3 = glm::vec3(0.f, -9.4f, -4.6f);
	glm::vec3 astroDirectionMove3 = astroStartPos3 + glm::vec3(0.f, time +9 * 0.59f, time+2*0.59f);
	glm::mat4 astro3 = glm::translate(glm::mat4(1.f), astroDirectionMove3);
	glm::mat4 astertoidGoesBrr3 = glm::translate(astro3, astroDirectionMove3);
	astertoidGoesBrr3 = glm::scale(astertoidGoesBrr3, glm::vec3(0.0f,0.009f,0.009f));

	
	//spacecraft
	/*glm::mat4 spaceshipTranslation = glm::translate(glm::mat4(1.0f), spaceshipPos);
	glm::mat4 spaceshipRotation = glm::transpose(glm::mat4({
		cameraSide.x, cameraUp.x, -cameraDir.x, 0,
		cameraSide.y, cameraUp.y, -cameraDir.y, 0,
		cameraSide.z, cameraUp.z, -cameraDir.z, 0,
		0, 0, 0, 1
		}));
	glm::mat4 spaceshipModelMatrix = spaceshipTranslation * spaceshipRotation;
	*/
	//glm::transpose(spaceshipModelMatrix); 
	/////////////////

	//spaceshipTranslation = glm::translate(glm::mat4(1.0f), spaceshipPos);

	 //glm::vec3 cameraOffset = glm::vec3(0.0f, 1.0f, -3.0f); // Adjust the values for the desired offset
  //  glm::vec3 cameraPosition = spaceshipPos + cameraOffset;
    
    // Update camera based on the new position and direction
   /* cameraPos = cameraPosition;
    cameraDir = spaceshipPos - cameraPos;*/


	glUseProgram(program);
	
	drawObjectColor(sphereContext, glm::mat4(), glm::vec3(0.9, 0.9, 0.2)); // sun
	drawObjectColor(sphereContext, earthModelScaled, glm::vec3(0.0f, 0.0f, 0.91f)); // earth
	drawObjectColor(sphereContext, moonScaled, glm::vec3(0.3, 0.3, 0.3)); // moon
	
	//1b start
	drawObjectColor(sphereContext, planet1ModelScaled, glm::vec3(0.319f, 0.165f, 0.175f)); // planet 1
	drawObjectColor(sphereContext, planet2ModelScaled, glm::vec3(0.5f, 0.72f, 0.331f)); // planet 2
	drawObjectColor(sphereContext, planet3ModelScaled, glm::vec3(0.13f, 0.333f, 0.51f)); // planet 3
	drawObjectColor(sphereContext, planet4ModelScaled, glm::vec3(0.0f, 0.9f, 0.9f)); // planet 4
	drawObjectColor(sphereContext, planet5ModelScaled, glm::vec3(0.95f, 0.35f, 0.79f)); // planet 5
	//1b end

	//drawObjectColor(asteroidContext, asteroidScaled, glm::vec3(0.f, 0.f, 0.f)); // asteroid 1
	drawObjectColor(asteroidContext, astertoidGoesBrr1, glm::vec3(0.5f, 0.5f,0.5f));
	drawObjectColor(asteroidContext, astertoidGoesBrr2, glm::vec3(0.7f, 0.5f,0.6f));
	drawObjectColor(asteroidContext, astertoidGoesBrr3, glm::vec3(0.9f, 0.f,0.1f));

	//drawObjectColor(shipContext, spaceshipTranslation, glm::vec3(0.0f, 0.0f, 0.0f));


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
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_4_1.vert", "shaders/shader_4_1.frag");

	loadModelToContext("./models/sphere.obj",sphereContext);
	loadModelToContext("./models/asteroid.obj", asteroidContext);
	loadModelToContext("./models/spaceship.obj", shipContext);

}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{

	//glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f, 1.f, 0.f)));
	//float angleSpeed = 0.005f;
	//float moveSpeed = 0.005f;
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	//	glfwSetWindowShouldClose(window, true);
	//}
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//	spaceshipPos += spaceshipDir * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//	spaceshipPos -= spaceshipDir * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	//	spaceshipPos += spaceshipSide * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	//	spaceshipPos -= spaceshipSide * moveSpeed;
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//	spaceshipDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(spaceshipDir, 0));
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//	spaceshipDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(spaceshipDir, 0));

	//// Update camera based on spaceship position and direction
	//cameraPos = spaceshipPos + 1.5f * spaceshipDir + glm::vec3(0, -0.5f, 0);
	//cameraDir = spaceshipDir;



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
		cameraPos += cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos -= cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));

	//cameraDir = glm::normalize(-cameraPos);
	



	//cameraDir = glm::normalize(-cameraPos);

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