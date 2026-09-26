#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "shadersUtil.h"
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;



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
    
    // point
    // float point[] = { 0.0f, 0.0f, 0.0f }; 
    // unsigned int pointVAO,pointVBO;
    // glGenVertexArrays(1,&pointVAO);
    // glGenBuffers(1,&pointVBO);
    // glBindVertexArray(pointVAO);
    // glBindBuffer(GL_ARRAY_BUFFER,pointVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // point end

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f;
    glUseProgram(shaderProgram);
    
    // glm::mat4 view = glm::lookAt(
    //     glm::vec3(0.0f,0.0f,3.0f),
    //     glm::vec3(0.0f,0.0f,0.0f),
    //     glm::vec3(0.0f,1.0f,0.0f)
    // );
    
    
    glm::mat4 projection  = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
    // if we not define the perspective and view then also it works why. -> perspective becomes bad , no clear near and far 
    int projectionLoc = glGetUniformLocation(shaderProgram,"projection");
    glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
    
    int modelLoc = glGetUniformLocation(shaderProgram,"model");
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
    bool xpress = false;
    bool ypress = false;
    bool zpress = false;

    
    glm::vec3 camCord(0.0f,0.f,3.0f);
    int viewLoc = glGetUniformLocation(shaderProgram,"view");
    
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS && !ypress){
            ypress=true;
            rotationY += 1.0f;
        }
        if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS && !ypress){
            ypress=true;
            rotationY -= 1.0f;
        }
        if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS && !xpress){
            xpress=true;
            rotationX += 1.0f;
        }   
        if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS && !xpress){
            xpress = true;
            rotationX -= 1.0f;
        }
        if(glfwGetKey(window,GLFW_KEY_Z)==GLFW_PRESS && !zpress){
            zpress = true;
            rotationZ += 1.0f;
        }
        if(glfwGetKey(window,GLFW_KEY_C)==GLFW_PRESS && !zpress){
            zpress = true;
            rotationZ -= 1.0f;
        }
        
        if(glfwGetKey(window,GLFW_KEY_W)==GLFW_RELEASE)ypress=false;
        if(glfwGetKey(window,GLFW_KEY_S)==GLFW_RELEASE)ypress=false;
        if(glfwGetKey(window,GLFW_KEY_A)==GLFW_RELEASE)xpress=false;
        if(glfwGetKey(window,GLFW_KEY_D)==GLFW_RELEASE)xpress=false;
        if(glfwGetKey(window,GLFW_KEY_Z)==GLFW_RELEASE)zpress=false;
        if(glfwGetKey(window,GLFW_KEY_C)==GLFW_RELEASE)zpress=false;
        
        // glBindVertexArray(pointVAO);
        // glDrawArrays(GL_POINTS, 0, 1); 
        
        glm::mat4 camRotation = glm::mat4(1.0f);

        camRotation = glm::rotate(camRotation,glm::radians(rotationX),glm::vec3(1.0f,0.0f,0.0f));
        camRotation = glm::rotate(camRotation,glm::radians(rotationY),glm::vec3(0.0f,1.0f,0.0f));
        camRotation = glm::rotate(camRotation,glm::radians(rotationZ),glm::vec3(0.0f,0.0f,1.0f));
        
        glm::vec3 rotatedCamPos = glm::vec3(camRotation*glm::vec4(camCord,1.0f));
        glm::vec3 up = glm::vec3(camRotation*glm::vec4(0.0f,1.0f,0.0f,0.0f));
        glm::mat4 view = glm::lookAt(
            rotatedCamPos,
            glm::vec3(0.0f,0.0f,0.0f),
            up
        );
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));

        // new model created so we need to point to it again

        // glUniform1f(uniId,0.2f);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
        glPointSize(6.0f);
        glDrawArrays(GL_POINTS, 0, 24);
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