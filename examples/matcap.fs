#version 330

// Eye-space.
in vec3 fNormal;

in vec2 fTexCoord;

uniform struct Material {
    sampler2D matcap_texture;
    
    bool use_diffuse_texture;
    sampler2D diffuse_texture;
    
    bool use_normal_map;
    sampler2D normal_map;
} material;

uniform mat3 uNormalMatrix;

// gl_FragColor is old-fashioned, but it's what WebGL 1 uses.
// From: https://stackoverflow.com/questions/9222217/how-does-the-fragment-shader-know-what-variable-to-use-for-the-color-of-a-pixel
layout(location = 0) out vec4 FragColor;

void main()
{
    vec3 n = fNormal;
    vec4 color;

    //Use normals from normal maps
   if ( material.use_normal_map ) {
       n = uNormalMatrix * ( 2 * texture( material.normal_map, fTexCoord ).rgb - 1 );
   }

    color = texture( material.matcap_texture, n.xy*0.5 + 0.5 ).rgba;

    // add diffuse texture
    if ( material.use_diffuse_texture ) {
        color *= texture( material.diffuse_texture, fTexCoord ).rgba;
    }
    
    FragColor = clamp( color, 0.0, 1.0 );
}
