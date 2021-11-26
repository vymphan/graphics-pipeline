#version 330

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat3 uNormalMatrix;
uniform float uTime;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec3 fPos;
out vec3 fNormal;
out vec2 fTexCoord;


// Helper function to get a rotation matrix around Y axis
mat4 rotationYAxis( float theta ) {
    return mat4( cos( theta ), 0.0, -sin( theta ), 0.0,
                0.0, 1.0, 0.0, 0.0,
                sin( theta ), 0.0, cos( theta ), 0.0,
                0.0, 0.0, 0.0, 1.0 );
}

void main(void)
{
    float theta = uTime; // angle spun with time

    // Convert Position from world-space to eye-space with uViewMatrix
    fPos = vec3( uViewMatrix * rotationYAxis( theta ) * vec4( vPos, 1.0 ) );

    // Convert Normal from world-space to eye-space with uNormalMatrix
    fNormal = uNormalMatrix * vNormal;

    //Textcoords are the same
    fTexCoord = vTexCoord;

    // Convert Position from camera-space to normalized-device coordinates
    gl_Position = uProjectionMatrix * vec4( fPos, 1.0 );



}
