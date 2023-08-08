//Author: Hakan KABAYEL

// EGL context and surface variables
EGLDisplay display;
EGLContext context;
EGLSurface surface;

// OpenGL ES program and shader variables
GLuint circleProgram;
GLint circlePositionLoc, circleTexCoordLoc, circleTextureLoc;

// OpenGL ES program and shader variables
GLuint triangleProgram;
GLint trianglePositionLoc, triangleColorLoc;

// Circle parameters
const float radius = 0.7f;
const int numSegments = 360;
const float angleIncrement = (2.0f * M_PI) / numSegments;
const float radians = cos(42.0f * M_PI / 180.0f);

//Rotation parameters
float needleRotation = 0.0f;
float artificialData[2] = {-19.5f,0.0f};
int artificialDataCounter = 0;
int firstplaceOfNeedle = 0;
float epsilon = 0.035f;