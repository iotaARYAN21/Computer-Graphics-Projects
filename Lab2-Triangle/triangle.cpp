#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <fstream>
#include <sstream>
// #include<GL/glut.h>
using namespace std;

string readFile(const string& filePath){
    ifstream file(filePath);
    if(!file.is_open()){
        cerr<<"Could not open file\n";
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void vertexShaderCompileLog(unsigned int vertexShader){
    int success;
    char infoLog[512]; // why 512
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        cerr<<"Vertex shader compiler faild\n"<<infoLog<<endl;
    }
}

void fragmentShaderCompileLog(unsigned int fragmentShader){
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        cerr<<"Fragment shader compile failed\n"<<infoLog<<endl;
    }
}

void ShaderLinkingCheck(unsigned int shaderProgram){
    int success;
    char infoLog[512]; // why 512
    glGetShaderiv(shaderProgram,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(shaderProgram,512,NULL,infoLog);
        cerr<<"shaderProgram compiler faild\n"<<infoLog<<endl;
    }
}

int main(){
    
    if(!glfwInit())return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800,600,"Triangle Phase 1",NULL,NULL);
    if(window == NULL){
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){
        cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }
    
    glViewport(0,0,800,600);
    
    
    float vertices[] = {
        -0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
        0.5f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
        0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
    };


    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    string vertexShaderSource = readFile("vertexShader.glsl");

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCode = vertexShaderSource.c_str();

    glShaderSource(vertexShader,1,&vertexShaderCode,NULL);

    glCompileShader(vertexShader);

    vertexShaderCompileLog(vertexShader);
    
    string fragmentShaderSource = readFile("fragmentShader.glsl");
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* fragmentShaderCode = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader,1,&fragmentShaderCode,NULL);

    glCompileShader(fragmentShader);
    fragmentShaderCompileLog(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);

    glLinkProgram(shaderProgram);

    ShaderLinkingCheck(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

    glDeleteProgram(shaderProgram);

    glfwTerminate();

}
