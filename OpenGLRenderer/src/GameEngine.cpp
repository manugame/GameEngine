#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <fstream>
#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

using namespace std;

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function,const  char* file, int line)
{
    while (GLenum error = glGetError())
    {
        cout << "[OpenGL error] (" << error << "):" << function << " " << file << ":" << line << endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    string vertexSource;
    string fragmentSource;
};

static ShaderProgramSource ParseShader(const string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	string line;
	stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != string::npos)
		{
			if (line.find("vertex") != string::npos)
				type = ShaderType::VERTEX;

			else if (line.find("fragment") != string::npos)
				type = ShaderType::FRAGMENT;
		}

		else
		{
			ss[(int)type] << line << "\n";
		}
	}

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const string& source)
{

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = static_cast<char*>(_malloca(sizeof(char) * length));
        glGetShaderInfoLog(id, length, &length, message);

        cout << "failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
                     
    }

    return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
	const unsigned int program = glCreateProgram();

	const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
	
}


int main(void)
{
	if (!glfwInit())
        return -1;



    GLFWwindow* window = glfwCreateWindow(400, 400, "GameEngine v0.1", nullptr, nullptr);

	
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        cout << "Glew initialization error" << endl;

    cout << "Runtime informations" << endl;
    cout << glGetString(GL_VERSION) << std::endl;
	

    float positions[] = {

        -0.5f, -0.5, //0
         0.5f, -0.5f,//1
         0.5f,  0.5f,//2
        -0.5f,  0.5f,//3
    };

    unsigned int indices[] = {

        0, 1, 2,
    	2, 3, 0
    };

	


	//enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	


    //buffer
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));


    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6  * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	const unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);
    
	
    while (!glfwWindowShouldClose(window))
    {
       
        glClear(GL_COLOR_BUFFER_BIT);


        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


        glfwSwapBuffers(window);

    	
        glfwPollEvents();
    }

    glDeleteProgram(shader);
	
    glfwTerminate();
    return 0;
}