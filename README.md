COMPASS INDICATOR SIMULATION
**********************************************************
LIBRARIES

* #include <EGL/egl.h>: This header file is used to include the necessary declarations and functions for using the EGL (Embedded Graphics Library) API. EGL is used in this code to manage graphics contexts, surfaces, and rendering for OpenGL ES.

* #include <GLES2/gl2.h>: This header file includes the declarations and functions for using the OpenGL ES 2.0 API. It provides the necessary functions and constants for rendering graphics using OpenGL ES.

* #include <GLFW/glfw3.h>: This header file is used to include the declarations and functions from the GLFW (Graphics Library Framework) library. GLFW is a library that provides a simple and consistent interface for creating windows, handling input, and managing OpenGL contexts.

* #include <iostream>: This is the standard C++ input-output stream library. It is used in this code to output error messages to the console using std::cerr.

* #include <math.h>: This header file includes various mathematical functions and constants. It's used in the code to perform mathematical calculations, such as trigonometric functions for positioning and rotating elements.

* "stb_image.h": This is a custom header file that likely includes declarations and functions for loading image data using the STB Image library. STB Image is a popular library for loading various image formats, and it's used here to load texture data for the OpenGL application.
**********************************************************
FUNCTIONS

* createEGLContext() Function:
  
This function creates an EGL context and surface. It initializes EGL, selects an appropriate configuration, creates a window surface, and establishes a rendering context.



* createProgram() Function:

This function creates an OpenGL shader program. It compiles and attaches both vertex and fragment shaders, links them into a program, and manages attribute locations.

* loadTexture() Function:

This function loads an OpenGL texture from image data. It generates a texture ID, sets texture parameters, specifies texture data, and frees the image data memory.

* drawCircleWithTexture() Function:

This function draws a circle using a texture. It uses a shader program, calculates vertex positions and texture coordinates, enables attribute arrays, and performs the drawing using glDrawArrays.

* drawNeedle() Function:

This function draws the needle of a compass. It creates two colored triangles representing the needle, sets up shader uniform values, and draws the triangles using glDrawArrays.

* rotateNeedle() Function:

This function rotates the needle towards a target rotation angle. It calculates the angle difference, determines the rotation direction, and updates the needle's rotation based on a rotation speed.

* prepareMoving() Function:

This function prepares and performs needle rotation towards a target angle. It calculates the target rotation angle, rotates the needle, and checks if the target is reached.

* destroyEGLContext() Function:

This function destroys the EGL context and surface, cleaning up resources.

* main() Function:

The main entry point of the program. It initializes GLFW, creates a window, sets up OpenGL context, loads shader programs, loads a texture, enters the main loop for drawing, and finally cleans up resources.

* Compile Code for Linux:
g++ pusula.cpp -o pusula -lEGL -lGLESv2 -lglfw -lstb










