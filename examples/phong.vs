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
    fPos = vec3( uViewMatrix * vec4( vPos, 1.0 ) );

    // Convert Normal from world-space to eye-space with uNormalMatrix
    fNormal = uNormalMatrix * vNormal;

    // Textcoords are the same
    fTexCoord = vTexCoord;

    // Convert Position from camera-space to normalized-device coordinates
    gl_Position = uProjectionMatrix * vec4( fPos, 1.0 );


}

// Note to self:
// fPos, fNormal, fTextCoord are for Phong model in fragment shader to get the color
// gl_Position is the position in perspective for the screen
