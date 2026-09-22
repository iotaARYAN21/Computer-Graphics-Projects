#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "shadersUtil.h"
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

float nodes[] = {
    // x, y, z,      r, g, b
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f, // 0
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, // 1
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, // 2
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f, // 3

    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f, // 4
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f, // 5
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, // 6
    -0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.0f  // 7
};

unsigned int indices[] = {
    1, 2, 0,
    2, 3, 0,

    5, 6, 1,
    6, 2, 1,

    4, 7, 5,
    7, 6, 5,

    4, 0, 3,
    7, 4, 3,

    3, 2, 6,
    7, 3, 6,

    5, 1, 4,
    1, 0, 4
};

unsigned int VAO, VBO, EBO;

void changeNodeColor(int node, float rcol, float gcol, float bcol){
    int colorIndex = node * 6 + 3;

    nodes[colorIndex]     = rcol;
    nodes[colorIndex + 1] = gcol;
    nodes[colorIndex + 2] = bcol;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        colorIndex * sizeof(float),
        3 * sizeof(float),
        &nodes[colorIndex]
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    cout << "Node " << node
         << " changed to RGB: "
         << rcol << " "
         << gcol << " "
         << bcol << endl;
}

// unsigned int VAO, VBO;

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
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(nodes),
        nodes,
        GL_DYNAMIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

            int node;
            float r, g, b;

            cout << "Enter node number (0-7): ";
            cin >> node;

            if(node < 0 || node > 7){
                cout << "Invalid node number" << endl;
            }
            else{
                cout << "Enter RGB values (0.0 - 1.0): ";
                cin >> r >> g >> b;

                if(r < 0.0f || r > 1.0f ||
                   g < 0.0f || g > 1.0f ||
                   b < 0.0f || b > 1.0f){
                    cout << "Invalid RGB values" << endl;
                }
                else{
                    changeNodeColor(node, r, g, b);
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

        glDrawElements(
            GL_TRIANGLES,
            36,
            GL_UNSIGNED_INT,
            0
        );

        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, 8);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}