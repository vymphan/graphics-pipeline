#version 330

// Eye-space.
in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoord;

uniform mat3 uNormalMatrix;

uniform struct Material {
    vec3 color;
    float shininess;
    int bands;
    
    bool use_diffuse_texture;
    sampler2D diffuse_texture;
} material;

struct Light {
    // Eye-space.
    vec3 position;
    float color;
    float color_ambient;
};
const int MAX_LIGHTS = 5;
uniform Light lights[ MAX_LIGHTS ];

uniform int num_lights;

// gl_FragColor is old-fashioned, but it's what WebGL 1 uses.
// From: https://stackoverflow.com/questions/9222217/how-does-the-fragment-shader-know-what-variable-to-use-for-the-color-of-a-pixel
layout(location = 0) out vec4 FragColor;

const vec3 eye = vec3( 0.0 );

void main()
{
    vec3 n = normalize( fNormal );
    vec3 f = vec3( 0.0 );
    vec3 k;

    // Simplified Phong
    // F = ∑L (IAL + [IL ( N · L ) + IL ( V · R )^n])

    for ( int i = 0; i < num_lights; i++ ) {

        vec3 l = normalize( lights[i].position - fPos );

        f += lights[i].color_ambient; // I_AL

        if ( dot( n, l ) >= 0 ) {
            f += lights[i].color * dot( n, l ); // IL*(N . L)
            f += lights[i].color
               * pow( max( 0, dot( normalize( eye - fPos), normalize( reflect( -1.0*l, n ) ) ) ), material.shininess ); // IL ( V · R )^n]
        }
    }
    
    // F_discrete
    f = min( floor( f * f * material.bands ), material.bands - 1 ) / ( material.bands - 1 );

    if ( material.use_diffuse_texture ) {
        k = material.color * vec3( texture( material.diffuse_texture, fTexCoord ) );
    }
    else {
        k = material.color;
    }

    FragColor = clamp( vec4( k * f, 1.0 ), 0.0, 1.0 );
}
