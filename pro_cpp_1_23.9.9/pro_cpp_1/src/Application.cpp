#include <iostream>
#include <string> 

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    /*您提出了一个好问题。从语法角度来分析一下,enum class 为什么被称为"带作用域的枚举类型":

- 普通的 enum 定义是:

  enum EnumName {
   value1, 
   value2
  }

- 枚举值不加作用域,可以直接使用值名

- 而 enum class 定义是:

  enum class EnumName {
   value1,
   value2
  } 

- 这里使用了class关键字

- 根据C++标准,class关键字会为枚举类型生成一个新的作用域

- 枚举值名会放在这个新的作用域中

- 所以要使用枚举值名,需要加上作用域操作符::

 如EnumName::value1

- 这样就隔离开其他作用域中的可能重复名称

- 并防止枚举值名与其他名称冲突

所以,从enum class语法中class关键字产生的作用域来看:

- 它为枚举类型值名生成了一个独立的命名空间

- 这就产生了"带作用域"的语义

希望这个分析可以帮您理解enum class的语法机制!*/
    enum class ShaderType { /* 带作用域的枚举类型，不是类*/
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) { /* 找到了*/
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}
 

/*方便起见,写成一个函数*/
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);/*vertex 或者 fragment */
    const char* src = source.c_str(); /*或者写 &source[0]*/
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // char message[length]; /*这里会发现因为长度不定，无法栈分配，但你仍要这么做*/
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex":"fragment" )<< "shader!请定位到此行" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/*使用static是因为不想它泄露到其他翻译单元？
使用string不是最好的选择，但是相对安全, int类型-该着色器唯一标识符，一个ID*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /*使用unsigned是因为它接受的参数就是这样，
    或者可以使用 GLuint，但是作者不喜欢这样，因为它要使用多个图像api*/
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //if (glewInit() != GLEW_OK)/*glew文档，这里会报错，因为需要上下文，而上下文在后面*/
    //    std::cout << "ERROR!-1" << std::endl;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(5); // 屏幕刷新率？

    if (glewInit() != GLEW_OK)/*这里就不会报错了*/
        std::cout << "ERROR!-2" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = { /*冗余的点，因此需要index buffer*/
        -0.5f, -0.5f,// 0
        0.5f, -0.5f,// 1
        0.5f, 0.5f, // 2
        -0.5f, 0.5f, // 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    /*vertex array object 顶点缓冲区对象*/
    //unsigned int vao;
    //GLCall(glGenVertexArrays(1, &vao));/* 1表示生成一个vao,第二个参数是接收地址*/
    //GLCall(glBindVertexArray(vao));

    VertexArray va; 
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
   
    VertexBufferLayout layout; 
    layout.Push<float>(2);
    va.AddBuffer(vb, layout); 
    
    /* ibo - index buffer object 索引缓冲区 ; 使用类代替下面的操作*/
    IndexBuffer ib(indices, 6);
  
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); 
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader)); 
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);/*条件为假，弹出断点。如果没读到，弹出断点*/
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f)); 

    float r = 0.2f;
    float increment = 0.05f;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
 
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        if (r > 1.0f || r < -1.0f )
        {
            increment = -increment;
        } 
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}