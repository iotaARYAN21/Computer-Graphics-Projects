#include<GL/glew.h>
#include<bits/stdc++.h>
#include<GLFW/glfw3.h>
#include<GL/glut.h>
using namespace std;
int main(int argc,char* argv[]){
    GLFWwindow* window;
    if(!glfwInit()){
        return -1;
    }
    window = glfwCreateWindow(640,640,"Lab 1",NULL,NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        cout << "GLEW init failed\n";
        return -1;
    }

    glutInit(&argc,argv);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        int glfwMajor,glfwMinor,glfwRev;
        glfwGetVersion(&glfwMajor,&glfwMinor,&glfwRev);
        auto glewVersion = glewGetString(GLEW_VERSION);
        auto openGLVersion =glGetString(GL_VERSION);
        string glfwVersion = to_string(glfwMajor)+"."+to_string(glfwMinor)+"."+to_string(glfwRev);
        
        glColor3f(1.,0.,0.);
        int len1 = strlen(reinterpret_cast<const char*>(glewVersion));
        int len2 = strlen(reinterpret_cast<const char*>(openGLVersion));
        int len3 = glfwVersion.length();
        
        glRasterPos2f(-0.9f,0.8f);
        string label = "OpenGL Version ";
        for(char ch:label){
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,ch);
        }
        for(int i=0;i<len2;i++){
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,openGLVersion[i]);
        }
        glRasterPos2d(-0.9f,0.6f);
        label = "GLEW Version ";
        for(char ch:label){
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,ch);
        }
        for(int i=0;i<len1;i++){
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,glewVersion[i]);
        }
        glRasterPos2f(-0.9f,0.4f);
        label = "GLFW Version ";
        for(char ch:label){
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,ch);
        }
        for(int i=0;i<len3;i++){
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13,glfwVersion[i]);
        }

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}