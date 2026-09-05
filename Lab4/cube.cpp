#include<GL/glew.h>
#include<GLFW/glfw3.h>
// #include <glm/glm.hpp>
#include<iostream>
#include<fstream>
#include<sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    char infoLog[512]; 
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
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
        cerr<<"shaderProgram compiler faild\n"<<infoLog<<endl;
    }
}


int main(){
    if(!glfwInit())return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_ANY_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"Cube",NULL,NULL);
    if(window==NULL){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit()!=GLEW_OK){
        cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0,0,800,600);

    float nodes[] = {

       
        0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f, // a
        -0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f, // b
        -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, // c
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, // d


    
        0.5f,  0.5f, 0.5f,     0.0f, 1.0f, 0.0f, // e
        -0.5f,  0.5f, 0.5f,     0.0f, 1.0f, 0.0f, // f
        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, // g
        0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, // h


        -0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 1.0f, // i
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 1.0f, // j
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, // k
        -0.5f,  0.5f, 0.5f,     0.0f, 0.0f, 1.0f, // l


 
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f, // m
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 0.0f, // n
        0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 0.0f, // o
        0.5f,  0.5f, 0.5f,     1.0f, 1.0f, 0.0f, // p



        0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 1.0f, // q
        -0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 1.0f, // r
        -0.5f,  0.5f, 0.5f,     1.0f, 0.0f, 1.0f, // s
        0.5f,  0.5f, 0.5f,     1.0f, 0.0f, 1.0f, // t



        0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 1.0f, // u
        -0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 1.0f, // v
        -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 1.0f, // w
        0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 1.0f  // x
    };


    unsigned int indices[] = {


        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };



    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(nodes),
        nodes,
        GL_STATIC_DRAW
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    glEnable(GL_DEPTH_TEST);

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
    // GLuint uniId = glGetUniformLocation(shaderProgram,"scale");


    
    // model = glm::rotate(model,glm::radians(30.0f),glm::vec3(0.0f,1.0f,0.0f));

    
    
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    glUseProgram(shaderProgram);
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f,0.0f,3.0f),
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f)
    );
    
    int viewLoc = glGetUniformLocation(shaderProgram,"view");
    glUniformMatrix4fv(
        viewLoc,
        1,
        GL_FALSE,
        glm::value_ptr(view)
    );
    
    glm::mat4 projection  = glm::perspective(
        glm::radians(45.0f),
        800.0f/600.0f,
        0.1f,
        100.0f
    );
    
    int projectionLoc = glGetUniformLocation(shaderProgram,"projection");
    glUniformMatrix4fv(
        projectionLoc,
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );
    
    int modelLoc = glGetUniformLocation(shaderProgram,"model");
    
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(glfwGetKey(window,GLFW_KEY_LEFT)==GLFW_PRESS){
            rotationY -= 1.0f;
        }
        if(glfwGetKey(window,GLFW_KEY_RIGHT)==GLFW_PRESS){
            rotationY += 1.0f;
        }
        if(glfwGetKey(window,GLFW_KEY_DOWN)==GLFW_PRESS){
            rotationX += 1.0f;
        }
        if(glfwGetKey(window,GLFW_KEY_UP)==GLFW_PRESS){
            rotationX -= 1.0f;
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model,glm::radians(rotationX),glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(rotationY),glm::vec3(0.0f,1.0f,0.0f));
        // new model created so we need to point to it again
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));

        // glUniform1f(uniId,0.2f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}