// Include standard libraries
#include <iostream>
#include <vector>

// Include header files
#include "opengl.hpp"
#include "timecontrol.hpp"
// Include class
#include "Shader.hpp"
#include "Camera.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "Model.hpp"
#include "Object.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	initTime();

	GLFWwindow* window = createWindow("WindowName",800,600);

	// Other Default settings
	// Dark blue background
	glClearColor(0.9,0.9,0.9,1.0);
	// Set input mode GLFW_STICKY_KEYS
	glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);


    // configure global opengl state
    // -----------------------------
    // glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    // Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.model_loading.vs.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/1.model_loading.fs.glsl");
		Shader ourShader = Shader("/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/vertexshader.glsl", "/Users/haijian/Documents/OpenGL/PBR/PBR/Shader/fragmentshader.glsl");


    // load models
    // -----------
    // Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/nanosuit/nanosuit.obj");

		Model ourModel = Model("/Users/haijian/Documents/OpenGL/PBR/PBR/Model/WPN_AKM/WPN_AKM.obj");
		Texture texDiffuse;
		texDiffuse.id = loadTexture("WPNT_AKM_Base_Color.tga",ourModel.directory);
		texDiffuse.type = UNIFORM_TEX_DIFFUSE;
		texDiffuse.path = ourModel.directory;
		Texture texNormal;
		texNormal.id = loadTexture("WPNT_AKM_DirectX.tga",ourModel.directory);
		texNormal.type = UNIFORM_TEX_NORMAL;
		texNormal.path = ourModel.directory;
		Texture texMetal;
		texMetal.id = loadTexture("WPNT_AKM_Metallic.tga",ourModel.directory);
		texMetal.type = UNIFORM_TEX_METAL;
		texMetal.path = ourModel.directory;
		Texture texRough;
		texRough.id = loadTexture("WPNT_AKM_Roughness.tga",ourModel.directory);
		texRough.type = UNIFORM_TEX_ROUGH;
		texRough.path = ourModel.directory;
		Texture texAO;
		texAO.id = loadTexture("WPNT_AKM_Ambient_occlusion.tga",ourModel.directory);
		texAO.type = UNIFORM_TEX_AO;
		texAO.path = ourModel.directory;

		for (unsigned int i = 0; i < ourModel.meshes.size(); i++) {
			ourModel.meshes[i].textures.push_back(texDiffuse);
			ourModel.meshes[i].textures.push_back(texNormal);
			ourModel.meshes[i].textures.push_back(texMetal);
			ourModel.meshes[i].textures.push_back(texRough);
			ourModel.meshes[i].textures.push_back(texAO);

		}

		Camera camera = Camera();

		Object ourObject = Object();

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
				camera.translate(0.0,16.0,20.0);
				camera.setTarget(0.0,0.0,0.0);
				ourObject.scale(0.5);
				ourObject.rotate(0.5*currentTime(), glm::vec3(0.0,1.0,0.0));

        // ourShader.setMat4("projection", camera.getMatrixProjection());
        // ourShader.setMat4("view", camera.getMatrixView());
        // ourShader.setMat4("model", ourObject.getMatrixModel());

				ourShader.setMat4("matrixModel",ourObject.getMatrixModel());
				ourShader.setMat4("matrixMVP",camera.getMatrixProjection()*camera.getMatrixView()*ourObject.getMatrixModel());

        ourModel.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.ProcessKeyboard(FORWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     camera.ProcessKeyboard(BACKWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.ProcessKeyboard(LEFT, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
// void mouse_callback(GLFWwindow* window, double xpos, double ypos)
// {
//     if (firstMouse)
//     {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }
//
//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//     lastX = xpos;
//     lastY = ypos;
//
//     camera.ProcessMouseMovement(xoffset, yoffset);
// }

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     camera.ProcessMouseScroll(yoffset);
// }
