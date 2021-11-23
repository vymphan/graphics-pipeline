#version 330

// Eye-space.
in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoord;

uniform mat3 uNormalMatrix;

uniform samplerCube uEnvironmentTex;

uniform struct Material {
    vec3 color_ambient;
    vec3 color_diffuse;
    vec3 color_specular;
    float shininess;
    
    bool reflective;
    vec3 color_reflect;
    
    bool refractive;
    vec3 color_refract;
    float index_of_refraction;
    
    bool use_diffuse_texture;
    sampler2D diffuse_texture;
} material;

struct Light {
    // Eye-space.
    vec3 position;
    vec3 color;
    vec3 color_ambient;
};
const int MAX_LIGHTS = 5;
uniform Light lights[ MAX_LIGHTS ];

uniform int num_lights;

// gl_FragColor is old-fashioned, but it's what WebGL 1 uses.
// From: https://stackoverflow.com/questions/9222217/how-does-the-fragment-shader-know-what-variable-to-use-for-the-color-of-a-pixel
layout(location = 0) out vec4 FragColor;

// eye position in eye-space = origin
const vec3 eye( 0.0, 0.0, 0.0 );

void main()
{

    vec3 c( 0.0, 0.0, 0.0 );

    // Phong reflectance model
    // K_R*I_R + K_T*I_T + SUM_L( K_A*I_AL + [ K_D*I_L*(N.L) + K_S*I_L*(V.R)^n ])

    if ( material.reflective ) {
        c += texture( uEnvironmentTex, normalize( fPos - eye ) );
    }


    // Iterate through each light
    for (int i = 0; i < MAX_LIGHTS, i++) {

        // Ambient lighting
        c += material.color_ambient * light[i].color_ambient;

        // Diffuse lighting
        c += material.color_diffuse * light[i].color
                    * max( 0, dot( normalize( fNormal ), normalize( light.position - fPos) ) );

        // Specular ligting
        if ( DL != 0 )
            c += material.color_specular * light[i].color
                    * pow( max( 0, dot( normalize ( eye - fPos ), normalize( reflect( fPos - light.position, fNormal ) ) ) ), material.shininess );

    }



    
    FragColor = vec4( 1.0, 0.0, 1.0, 1.0 );
}
