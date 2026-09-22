#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "shadersUtil.h"
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

float nodes[] = {
    // Front face
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

    // Back face
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

    // Top face
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

    // Bottom face
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,

    // Right face
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,

     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 1.0f,

    // Left face
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f
};
unsigned int VAO, VBO;

void changeFaceColor(int face, float rcol, float gcol, float bcol){
    const int floatsPerVertex = 6;
    const int verticesPerFace = 6;
    const int floatsPerFace = verticesPerFace * floatsPerVertex;

    int startIndex = (face - 1) * floatsPerFace;

    for(int i = 0; i < verticesPerFace; i++){
        int colorIndex = startIndex + i * floatsPerVertex + 3;

        nodes[colorIndex]     = rcol;
        nodes[colorIndex + 1] = gcol;
        nodes[colorIndex + 2] = bcol;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        startIndex * sizeof(float),
        floatsPerFace * sizeof(float),
        &nodes[startIndex]
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    cout << "Face " << face
         << " changed to RGB: "
         << rcol << " "
         << gcol << " "
         << bcol << endl;
}

int main(){
    if(!glfwInit())return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800,600,"Input Color Cube",NULL,NULL);
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(nodes),
        nodes,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    string vertexShaderSource = readFile("vertexShader.glsl");

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCode = vertexShaderSource.c_str();

    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    vertexShaderCompileLog(vertexShader);

    string fragmentShaderSource = readFile("fragmentShader.glsl");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCode = fragmentShaderSource.c_str();

    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    fragmentShaderCompileLog(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    ShaderLinkingCheck(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float rotationX = 0.0f;
    float rotationY = 0.0f;

    bool iPressed = false;

    glUseProgram(shaderProgram);

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(
        viewLoc,
        1,
        GL_FALSE,
        glm::value_ptr(view)
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(
        projectionLoc,
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );

    int modelLoc = glGetUniformLocation(shaderProgram, "model");

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            rotationY -= 1.0f;

        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            rotationY += 1.0f;

        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            rotationX += 1.0f;

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            rotationX -= 1.0f;

        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !iPressed){
            iPressed = true;

            int face;
            float rcol, gcol, bcol;

            cout << "Enter face number (1-6):\n";
            cout << "1 = Front, 2 = Back, 3 = Top, 4 = Bottom, "
                    "5 = Right, 6 = Left\n";
            cin >> face;

            if(face < 1 || face > 6){
                cout << "Invalid face number\n";
                
            }
            else{
                cout << "Enter RGB values (0.0 - 1.0): ";
                cin >> rcol >> gcol >> bcol;

                if(rcol < 0.0f || rcol > 1.0f ||
                   gcol < 0.0f || gcol > 1.0f ||
                   bcol < 0.0f || bcol > 1.0f){
                    cout << "Invalid RGB values\n";
                }
                else{
                    changeFaceColor(face, rcol, gcol, bcol);
                }
            }
        }

        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)
            iPressed = false;

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::rotate(
            model,
            glm::radians(rotationX),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        model = glm::rotate(
            model,
            glm::radians(rotationY),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}