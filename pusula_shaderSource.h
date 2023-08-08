const char* circleVertexShaderSource =
    "attribute vec2 position;\n"
    "attribute vec2 texCoord;\n"
    "varying vec2 vTexCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(position, 0.0, 1.0);\n"
    "    vTexCoord = texCoord;\n"
    "}\n";

const char* circleFragmentShaderSource =
    "precision mediump float;\n"
    "uniform sampler2D texture;\n"
    "varying vec2 vTexCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = texture2D(texture, vTexCoord);\n"
    "}\n";

const char* triangleVertexShaderSource =
    "attribute vec2 position;\n"
    "uniform float rotation;\n" // Rotation angle
    "varying float distance;\n" // Distance from the origin
    "varying float maxY;\n" // Mutlak değeri en büyük olan Y noktası
    "varying float maxX;\n" // Mutlak değeri en büyük olan Y noktasına denk gelen X noktası
    "void main()\n"
    "{\n"
    "    float cosTheta = cos(rotation);\n" // Cosine of the rotation angle
    "    float sinTheta = sin(rotation);\n" // Sine of the rotation angle
    "    mat2 rotationMatrix = mat2(cosTheta, -sinTheta, sinTheta, cosTheta);\n" // Rotation matrix
    "    vec2 rotatedPosition = rotationMatrix * position;\n" // Apply rotation to position
    "    gl_Position = vec4(rotatedPosition, 0.0, 1.0);\n"
    "    float absoluteY = abs(rotatedPosition.y);\n" // Calculate the absolute value of Y
    "    if (absoluteY > abs(maxY)) {\n" // Check if the absolute Y value is greater than the current maximum
    "        maxY = rotatedPosition.y;\n" // Update the maximum Y value
    "        maxX = rotatedPosition.x;\n" // Update the X value corresponding to the maximum Y
    "    }\n"
    "    distance = sqrt(maxX * maxX + maxY * maxY);\n" // Calculate distance from the origin

    "}\n";

const char* triangleFragmentShaderSource =
    "precision mediump float;\n"
    "uniform vec4 topColor;\n"
    "uniform vec4 bottomColor;\n"
    "varying float distance;\n" // Use distance varying variable instead of yCoord
    "void main()\n"
    "{\n"
    "    if (0.0 < distance && distance <= 0.35)\n"
    "        gl_FragColor = topColor;\n"
    "    else if (distance > 0.35)\n"
    "        gl_FragColor = bottomColor;\n"
    "    else\n"
    "        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0f);\n"
    "}\n";