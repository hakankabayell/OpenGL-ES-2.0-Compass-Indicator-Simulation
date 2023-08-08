//Author: Hakan KABAYEL

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "stb_image.h"
#include "pusula_degiskenler.h"
#include "pusula_shaderSource.h"
#include "textureData.h"

// Create EGL context and surface
void createEGLContext(GLFWwindow* window)
{
    EGLint numConfigs;
    EGLConfig config;
    EGLint attributes[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, NULL, NULL);

    eglChooseConfig(display, attributes, &config, 1, &numConfigs);

    surface = eglCreateWindowSurface(display, config, EGLNativeWindowType(window), NULL);

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    eglMakeCurrent(display, surface, surface, context);
}

// Create shader program
GLuint createProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glBindAttribLocation(program, 0, "position");

    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// Load texture image
GLuint loadTexture()
{

    glGetTextureData(); 
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

    stbi_image_free(texData);

    return textureId;
}

// Draw circle using texture
void drawCircleWithTexture(GLuint textureId)
{
    glUseProgram(circleProgram);

    int texWidth, texHeight;
    glBindTexture(GL_TEXTURE_2D, textureId);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);

    GLfloat vertices[(numSegments + 1) * 2];
    GLfloat texCoords[(numSegments + 1) * 2];

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * angleIncrement;
        vertices[i * 2] = radius * std::cos(angle);
        vertices[i * 2 + 1] = radius * std::sin(angle);

        texCoords[i * 2] = (std::cos(angle-needleRotation) + 1.0f) * 0.5f;
        texCoords[i * 2 + 1] = (std::sin(angle-needleRotation) + 1.0f) * 0.5f;
    }

    GLint positionLoc = glGetAttribLocation(circleProgram, "position");
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    GLint texCoordLoc = glGetAttribLocation(circleProgram, "texCoord");
    glEnableVertexAttribArray(texCoordLoc);
    glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, texCoords);

    GLint textureLoc = glGetUniformLocation(circleProgram, "texture");
    glUniform1i(textureLoc, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 1);

    eglSwapBuffers(display, surface);
}

// Draw needle of compass
void drawNeedle()
{

    GLint rotationLoc = glGetUniformLocation(triangleProgram, "rotation");
    glUseProgram(triangleProgram);
    glUniform1f(rotationLoc, needleRotation);

    GLfloat vertices[6] = {
        0.0f, 0.5f,
        0.02f, 0.0f,
        -0.02f, 0.0f
    };

    GLint positionLoc = glGetAttribLocation(triangleProgram, "position");
    GLint topColorLoc = glGetUniformLocation(triangleProgram, "topColor");
    GLint bottomColorLoc = glGetUniformLocation(triangleProgram, "bottomColor");

    glEnableVertexAttribArray(positionLoc);

    glUniform4f(topColorLoc, 1.0f, 0.0f, 0.0f, 0.5f);  // RED COLOR
    glUniform4f(bottomColorLoc, 0.0f, 0.0f, 1.0f, 0.5f); // BLUE COLOR

    // draw first triangle
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    GLfloat verticesReverse[6] = {
        -0.02f, 0.0f,
        0.0f, -0.5f,
        0.02f, 0.0f
    };

    glUniform4f(topColorLoc, 0.0f, 0.0f, 1.0f, 0.5f); // BLUE COLOR
    glUniform4f(bottomColorLoc, 1.0f, 0.0f, 0.0f, 0.5f); // RED COLOR

    // draw second triangle
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, verticesReverse);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    eglSwapBuffers(display, surface);
}

// Rotate needle according to the targetRotation
void rotateNeedle(float targetRotationAngle)
{
    float rotationSpeed = 0.01f; 

    float angleDiff = targetRotationAngle - needleRotation;

    angleDiff = fmodf(angleDiff + 3.04f + 3.04f, 3.04f) - 3.04f;

    float direction = (targetRotationAngle > 0) ? 1.0f : -1.0f;

    angleDiff = fabs(angleDiff);

    if (angleDiff > epsilon)
    {
        if (angleDiff > rotationSpeed)
            angleDiff = rotationSpeed;

        needleRotation += direction * angleDiff;
    }

    // Boundries
    if (needleRotation < -4*radians)
        needleRotation = -4*radians;
    else if (needleRotation > 4*radians)
        needleRotation = 4*radians;
}

// prepare for rotateNeedle function
void prepareMoving(int i)
{
    float targetRotationAngle;
    if (i < (sizeof(artificialData) / sizeof(*artificialData)))
    {
        float step = ((artificialData[i]) - firstplaceOfNeedle);
        if (step == 0)
        {
            step = (artificialData[i] - firstplaceOfNeedle);
        }
        targetRotationAngle = (step / 19.0f * radians)/2;
        // Rotate Needle
        rotateNeedle(targetRotationAngle);
    }

    // Iteration is done if the goal is reached
    if ((fabs(needleRotation - targetRotationAngle) <= epsilon) && artificialDataCounter < (sizeof(artificialData)/sizeof(*artificialData)))
    {
        firstplaceOfNeedle = artificialData[artificialDataCounter];
        artificialDataCounter++;
    }
}
// Destroy EGL context and surface
void destroyEGLContext()
{
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(800, 800, "PUSULA", NULL, NULL);
    if (!window)
    {
        std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    createEGLContext(window);

    circleProgram = createProgram(circleVertexShaderSource, circleFragmentShaderSource);
    triangleProgram = createProgram(triangleVertexShaderSource, triangleFragmentShaderSource);

    GLuint textureId = loadTexture();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircleWithTexture(textureId);
        drawNeedle();

        prepareMoving(artificialDataCounter);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    destroyEGLContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// compile code
// g++ pusula.cpp -o pusula -lEGL -lGLESv2 -lglfw -lstb


