#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings for program
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
std::string LoadVertexShader(const std::string filepath)
{
    std::ifstream shaderFile(filepath);
    if (!shaderFile.is_open())
    {
        std::cout << "Failed to open vertexShader file" + filepath << std::endl;
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}
std::string vertexShaderSourceStr = LoadVertexShader("VertexShader.vert");
const GLchar* vertexShaderSource = vertexShaderSourceStr.c_str();

std::string LoadFragmentShader(const std::string filepath)
{
    std::ifstream shaderFile(filepath);
    if (!shaderFile.is_open())
    {
        std::cout << "Failed to open fragmentShader file" + filepath << std::endl;
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}
std::string fragmentShaderSourceStr = LoadFragmentShader("FragmentShader.frag");
const GLchar* fragmentShaderSource = fragmentShaderSourceStr.c_str();

int main()
{
    //initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //checks for apple device and acts accordingly
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load all OpenGL function pointer
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // build and compile Shaders
    //Vertex Shader:
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //check for errors compiling
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error Vertex Shader Compilation failed\n" << infoLog << std::endl;
    }
    //Fragment Shader:
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error Fragment Shader Compilation failed\n" << infoLog << std::endl;
    }
    //Linking the Shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //error checking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader Linking failed\n" << infoLog << std::endl;
    }
    //remove now unneeded shaders to free memory
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //vertices to display
    float vertices[] = {
        -1.0f, 1.0f, 0.0f, //Top Left
        1.0f, 1.0f, 0.0f, //Top Right
        1.0f, -1.0f, 0.0f, //Bottom Right
        -1.0f, -1.0f, 0.0f, //Bottom Left
    };
    unsigned int indices[] = {
        0, 1, 3, //first triangle
        1, 2, 3 //second triangle
    };
    //generating buffers of Vertices and Indices to draw on the screen
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //uncomment for wireframe :)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Loop until Window is closed
    while (!glfwWindowShouldClose(window))
    {
        // input processing
        processInput(window);
        //change colour of window
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Drawing Vertices
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // make window interactable
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //deleting resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // terminate process
    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//whenever the window size changed this function executes and change window dimensions
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}