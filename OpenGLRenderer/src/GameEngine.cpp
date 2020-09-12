#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

using namespace std;


int main(void)
{
	if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    GLFWwindow* window = glfwCreateWindow(480, 480, "GameEngine v0.1", nullptr, nullptr);

	
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        cout << "Glew initialization error" << std::endl;

    cout << "Runtime informations" << endl;
    cout << glGetString(GL_VERSION) << std::endl;
	
    {

        float positions[] = {

            -0.5f, -0.5, //0
             0.5f, -0.5f,//1
             0.5f,  0.5f,//2
            -0.5f,  0.5f,//3
        };

        unsigned int indices[] = {

            0, 1, 2, //triangle 1
            2, 3, 0  //triangle 2
        };


        //enable alpha
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        VertexArray vertexArray;
        VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));
		
        VertexBufferLayout layout;
        layout.Push<float>(2);

        vertexArray.AddBuffer(vertexBuffer, layout);
        vertexArray.Bind();

        IndexBuffer indexBuffer(indices, 6);

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
		
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();
		
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


        float r = 0.0f;
        float increment = 0.05f;


        while (!glfwWindowShouldClose(window))
        {

            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            indexBuffer.Bind();
            vertexArray.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


            glfwSwapBuffers(window);
            glfwPollEvents();


            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;
        }
    }
	
    glfwTerminate();
    return 0;
}
