#version 330

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat3 uNormalMatrix; // uNormalMatrix = transpose(inverse(mat3(uViewMatrix)))

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec3 fPos;
out vec3 fNormal;
out vec2 fTexCoord;

void main()
{

    // Convert Position from world-space to eye-space with uViewMatrix
    fPos = uViewMatrix * vPos;

    // Convert Normal from world-space to eye-space with uNormalMatrix
    fNormal = uNormalMatrix * vNormal;

    // Textcoords are the same
    fTexCoord = vTexCoord;


    gl_Position = vec4( uProjectionMatrix * fPos, 1.0 );
}
