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

using namespace std;


int main(void)
{
	if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    GLFWwindow* window = glfwCreateWindow(960, 540, "GameEngine v0.1", nullptr, nullptr);

	
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

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
		glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);

        glm::vec4 result = proj * vp;
		



        Shader shader("res/shaders/basic.shader");
        shader.Bind();
		
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj);

        Texture texture("res/textures/github.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();
		

        Renderer renderer;


        while (!glfwWindowShouldClose(window))
        {


            renderer.Clear();
         
            renderer.Draw(vertexArray, indexBuffer, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
	
    glfwTerminate();
    return 0;
}
