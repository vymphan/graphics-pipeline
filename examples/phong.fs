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
const vec3 eye = vec3( 0.0, 0.0, 0.0 );

void main()
{

    vec3 c = vec3( 0.0, 0.0, 0.0 );
    vec3 n = normalize( fNormal );

    // Phong reflectance model
    // K_R*I_R + K_T*I_T + SUM_L( K_A*I_AL + [ K_D*I_L*(N.L) + K_S*I_L*(V.R)^n ])

    // Iterate through each light
    for (int i = 0; i < num_lights; i++) {

        vec3 l = normalize( lights[i].position - fPos);

        // Ambient lighting
        c += material.color_ambient * lights[i].color_ambient;

        if ( dot( n, l ) >= 0 ) {
            // Diffuse lighting
            vec3 K_D;
            if ( material.use_diffuse_texture ) {
                K_D = material.color_diffuse * vec3( texture( material.diffuse_texture, fTexCoord ) );
            }
            else {
                K_D = material.color_diffuse;
            }

            c += K_D * lights[i].color * max( 0, dot( n, l ) );

            // Specular ligting
            c += material.color_specular * lights[i].color
                    * pow( max( 0, dot( normalize ( eye - fPos ), normalize( reflect( -1.0*l, n ) ) ) ), material.shininess );
        }

    }

    // Reflection
    if ( material.reflective ) {
        c += material.color_reflect * vec3( texture( uEnvironmentTex, reflect( fPos - eye, n ) ) );
    }

    // Refraction
    if ( material.refractive ) {
        c += material.color_refract * vec3( texture( uEnvironmentTex, refract( fPos - eye, n, material.index_of_refraction ) ) );
    }

    FragColor = clamp( vec4( c, 1.0 ), 0.0, 1.0 );
}
