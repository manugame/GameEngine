#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Gui.h"

using namespace std;


int main(void)
{
	if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    //540
    GLFWwindow* window = glfwCreateWindow(960, 540, "GameEngine v0.1", nullptr, nullptr);

	
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK)
        cout << "Glew initialization error" << endl;

    cout << "Runtime informations" << endl;
    cout << glGetString(GL_VERSION) << endl;
	
    {

        float positions[] = {

             100.0f,  100.0f, 0.0f, 0.0f,//0
             200.5f,  100.0f, 1.0f, 0.0f,//1
             200.0f,  200.0f, 1.0f, 1.0f,//2
             100.0f,  200.0f, 0.0f, 1.0f,//3
        };

        unsigned int indices[] = {

            0, 1, 2, //triangle 1
            2, 3, 0  //triangle 2
        };


        //enable alpha
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

        VertexArray vertexArray;
        VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));
		
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        vertexArray.AddBuffer(vertexBuffer, layout);
        vertexArray.Bind();

        IndexBuffer indexBuffer(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
		
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/github.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();
		

        Renderer renderer;
		

        Gui::Setup(window);



        glm::vec3 translation(200, 200, 0);

		
        while (!glfwWindowShouldClose(window))
        {


            renderer.Clear();

            Gui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

            glm::mat4 mvp = proj * view * model;


            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
         
            renderer.Draw(vertexArray, indexBuffer, shader);

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("AMONG US ULTIMATE EDITION");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("Oui.");               // Display some text (you can use a format strings too)
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            Gui::Render();
        	
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

	
    glfwTerminate();
    return 0;
}
