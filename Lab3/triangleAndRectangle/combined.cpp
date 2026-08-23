#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
string readFile(const string& filePath){
    ifstream file(filePath);
    if(!file.is_open()){
        cerr<<"Could not open file\n";
        return "";
    }
    stringstream buffer;
    buffer<<file.rdbuf();
    return buffer.str();
}
void vertexShaderCompileLog(unsigned int vertexShader){
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        cerr<<"Vertex shader compile failed\n"<<infoLog<<endl;
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
    char infoLog[512];
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

    GLFWwindow* window = glfwCreateWindow(800,600,"Triangle & Rectangle",NULL,NULL);
    if(window == NULL){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK){
        cerr<<"Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0,0,800,600);

    float vertices[]={
        -0.55f,0.9f,0.0f,
        -0.9f,0.0f,0.0f,
        -0.1f,0.0f,0.0f,
    };

    float rectangle[] = {
        0.7f,  0.9f, 0.0f,   // top right  a
        0.7f, 0.0f, 0.0f,  // bottom right  b
        0.1f, 0.0f, 0.0f,  // bottom left  c
        0.1f,  0.9f, 0.0f  // top left d
    };
    unsigned int indices[]={
        0,1,2,
        0,2,3
    };
    unsigned int VBO1,VAO1,VBO2,VAO2,EBO;
    glGenVertexArrays(1,&VAO1);
    glGenVertexArrays(1,&VAO2);
    glGenBuffers(1,&VBO1);
    glGenBuffers(1,&VBO2);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER,VBO1);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(rectangle),
        rectangle,
        GL_STATIC_DRAW
    );
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
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

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES,0,3);

        glBindVertexArray(VAO2);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO1);
    glDeleteBuffers(1,&VBO1);
    glDeleteVertexArrays(1,&VAO2);
    glDeleteBuffers(1,&VBO2);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

}