Computer Graphics - Homework Assignment 6 - Shaders
===================================================

Goals:
------

* Understand how to program shaders that run on the GPU.

* Become familiar with setting up a modern GPU drawing pipeline.

* Understand cartoon shading.

* Understand texture coordinates.

* Understand normal mapping.

* Understand cube maps.

* (optional) Understand how to compute a tangent-space from texture coordinates.

Background
---------

* Topics: The Graphics Pipeline, Viewing, Illumination, Texture Mapping, Meshes
* Book (FoCG,4e): Chapter 17 *Using Graphics Hardware*.
* Video: "Lecture 8: The Graphics Pipeline" and "Lecture 9: Texture Mapping", "Assignment 6: Shaders aka The Graphics Pipeline", and "Assignment 6: Shaders addendum"
* Quiz: Mesh Processing

(FoCG,4e is *Fundamentals of Computer Graphics (4th edition)* by Steve Marschner and Peter Shirley, 4th edition.)

Getting Started & Handing In:
-----------------------------

* Download or clone this code repository. I recommend cloning so you can easily integrate updates I release while you are working on the project. Don't fork it on GitHub, or else your code will be visible to everyone.

* Follow the instructions to install a working development environment: <https://github.com/yig/graphics101>.

  * Optional: Install the [glfw](https://www.glfw.org/) library for creating an OpenGL window. If you don't install it this way, the `CMakeLists.txt` will download and compile `glfw` on the fly.

      * Mac with Homebrew package manager: `brew install glfw`.
      * Ubuntu Linux: `sudo apt-get install libglfw3-dev`.
      * Windows: It's complicated. Just let the `CMakeLists.txt` compile it for you. If you really want to, you can use [vcpkg](https://github.com/microsoft/vcpkg). If you cloned `vcpkg` as `C:\src\vcpkg`:
      
            git clone https://github.com/microsoft/vcpkg
            .\vcpkg\bootstrap-vcpkg.bat
            .\vcpkg\vcpkg install glfw3
        
        if you are using a 64-bit Windows compiler toolchain, replace the last command with:
        
            .\vcpkg\vcpkg install glfw3:x64-windows
        
        and then copy the GLFW folder `vcpkg` installs to your `Program Files` or else add the following flag to your `cmake ..`:
        
            -DCMAKE_TOOLCHAIN_FILE=C:\src\vcpkg/scripts/buildsystems/vcpkg.cmake

* The code will be written in GLSL (OpenGL shading language) and C++. You
are encouraged to write helper functions as you like.

* There are separate downloads for three large examples. Unzip them and
move the `head`, `lemon`, and `hercules` directories into your
`pipeline/examples/` directory so that you end up with
`pipeline/examples/head`, `pipeline/examples/lemon`, and
`pipeline/examples/hercules`.

* Copy the `mesh.cpp`'s functions `computeNormals()`, `normalizingTransformation()`, and `applyTransformation()` from the `mesh.cpp` you wrote for the previous homework on [meshes](https://github.com/yig/graphics101-meshes). Don't replace the `mesh.cpp` file with the one you previously wrote, because there's a new function for this project and some functions aren't needed (the half-edge support code is missing from this project). If you already turned in your [meshes](https://github.com/yig/graphics101-meshes) homework, you can email us to request a guaranteed-correct version of `meshes.cpp`.

* Build and run the code. The code should compile. When it runs, it will
ask you to choose a JSON file specifying a scene. It can also be run
as a command-line program taking a single argument, the path to the JSON
file. That way you can specify a command-line argument in Qt Creator and
simply run. Run these two JSON scene files to verify that everything is
working. They don't depend on any code you write:

        simplescene.json
        sphere.json

    You should see:

    ![simplescene.json](docs/images/simplescene.png)
    ![sphere.json](docs/images/sphere-first.png)
    
    Once you add the missing code to `vao.cpp`, `sphere.json` will look as follows:
    
    ![sphere.json](docs/images/sphere.jpg)

* Implement `vao.cpp`'s `makeFromMesh()` to upload normals and texture coordinates to the GPU as per-vertex attributes. Create your vertex shader and fragment shaders for each shader. Modify the JSON scene files as needed. Tangent-space normal mapping will require you to modify additional C++ code (`mesh.cpp`'s `computeTangentBitangent()` and `fancyscene.cpp`'s `vaoFromOBJPath()`).

* Build and run and test that it is working correctly. If you run the `pipeline` program with the `-h` or `--help` flag, you should see the following:

        Usage: ./pipeline [--width pixels] [--height pixels] [--screenshot path/to/save.png] [path/to/scene.json]

* Here are the JSON
scene files that will verify your output.
Save screenshots by pressing the `s` key or by passing `--screenshot output.png` to the executable. Save a screenshot for each of the JSON
scenes into an output subdirectory `screenshots`.

    * `phong_bunny.json`
    * `phong_sphere.json`
    * `cel_bunny.json`
    * `matcap_bunny.json`
    * `matcap_head.json`
    * `matcap_hercules.json`
    * `matcap_lemon.json`
    * `matcap_sphere.json`
    * `normalmap_cube.json`
    * `normalmap_head.json`
    * `normalmap_lemon.json`
    * `normalmap_hercules_bronze.json`
    * `normalmap_hercules_marble.json`
    * ... and any others that you make. You are required to make at least one (discussed below).

* There is a cmake target `screenshots` that will do this for you, so you can just type `make screenshots` or `cmake --target screenshots --build .` in your `build` directory.

* You will find reference output in the `examples/` directory.
For each `<filename>.json` file, there is a `<filename>.png` screenshot
taken from the default point of view.

* Qt Creator has a great debugger for C++. For shaders, debugging is a bit
trickier. When there is a GLSL error, you will see the shader code and
the line number printed out to *Application Output* in Qt Creator.

* Create a file named `Notes.txt` in the folder. Describe any known issues or extra features or things you attempted but did not finish.
Name people in the class who deserve a star for
helping you (not by giving your their code!).

* When done, run the the `cpack` command from inside your build directory
to generate an appropriate zip file of your `pipeline` project.
The zip file it creates, `pipeline.zip`, will include
the `screenshots` subdirectory and your `Notes.txt` file.
It will ignore unneeded large and numerous directories
(e.g. `build`, `examples/head`, `examples/hercules`, `examples/lemon`).
Upload your `pipeline.zip` to Blackboard before the deadline.

* **THIS IS AN INDIVIDUAL, NOT A GROUP ASSIGNMENT. That means all code
written for this assignment should be original! Although you are
permitted to consult with each other while working on this assignment,
code that is substantially the same will be considered cheating.** In your
`Notes.txt`, please note who deserves a star (who helped you with the
assignment).

Overview:
---------

In this assignment, you will be writing GLSL shaders. You will write a
Phong shader (the same lighting model as in your ray tracer). You will
implement reflections with a cube map. You will load high resolution
normals from a texture and implement tangent-space normal mapping. (For
this, you will also need to implement some additional mesh processing to
create additional per-vertex attributes.) You will write a cartoon
shader. For bonus, you can also write additional shaders and write
OpenGL code to setup a different rendering pipeline. Your output will
look like this:

![phong_bunny.json without refraction](docs/images/phong_bunny_norefract.jpg)
![phong_bunny.json with refraction](docs/images/phong_bunny_refract.jpg)
![phong_sphere.json](docs/images/phong_sphere.jpg)
![cel_bunny.json](docs/images/cel_bunny.jpg)
![cel_sphere.json](docs/images/cel_sphere.jpg)
![matcap_head.json](docs/images/matcap_head.jpg)
![matcap_hercules.json](docs/images/matcap_hercules.jpg)
![matcap_lemon.json](docs/images/matcap_lemon.jpg)
![normalmap_cube.json](docs/images/normalmap_cube.jpg)
![normalmap_head.json](docs/images/normalmap_head.jpg)
![normalmap_hercules_bronze.json](docs/images/normalmap_hercules_bronze.jpg)
![normalmap_hercules_marble.json](docs/images/normalmap_hercules_marble.jpg)
![normalmap_lemon.json](docs/images/normalmap_lemon.jpg)

Rubric:
-------

1. **(40 points)** Phong reflectance model. Your vertex shader only
needs to transform the positions and normals. Your fragment shader is
where the lighting is calculated. This is the same lighting model as you
implemented for your ray tracer. However, you won't be able to implement
the global illumination terms the same way (shadow and reflection and
refraction). In this assignment, you won't be able to implement shadows
at all. The simplified formula is:
    
    *K<sub>R</sub> I<sub>R</sub> +
    K<sub>T</sub> I<sub>T</sub> +
    ∑<sub>L</sub> (
        K<sub>A</sub> I<sub>AL</sub>
        + [ K<sub>D</sub> I<sub>L</sub> ( N · L ) + K<sub>S</sub> I<sub>L</sub> ( V · R )<sup>n</sup> ] )*

    Read on for details. In this assignment, all the parameters are stored as
uniforms, which are global variables for shaders programs. *K<sub>A</sub>*,
*K<sub>S</sub>*, *n*, *K<sub>R</sub>*, and *K<sub>T</sub>* refer to
`material.color_ambient`, `material.color_specular`, `material.shininess`,
`material.color_reflect`, and `material.color_refract`. If the boolean
`material.use_diffuse_texture` is false, then *K<sub>D</sub>* =
`material.color_diffuse`. If `material.use_diffuse_texture` is true, use *K<sub>D</sub>* =
`material.color_diffuse` times the color stored in the texture map:
*K<sub>D</sub>* = `material.color_diffuse*texture( material.diffuse_texture,
fTexCoord )`. The summation is over all `num_lights` of the `Light` structures;
*I<sub>AL</sub>* and *I<sub>L</sub>* refer to `lights[i].color_ambient` and
`.color`. The light `.position` should be interpreted as existing in eye-space,
not world-space. See *Tips* below for converting between world and eye space.

    * **(10 points)** Upload the normals and texture coordinates to the GPU as per-vertex attributes. To do this, you will implement `vao.cpp`'s `makeFromMesh()`. You will see in that function how `mesh.positions` are uploaded. Do the same for `mesh.normals` and `mesh.texcoords`.

    * **(5 points)** Vertex shader `phong.vs`. Use the provided uniforms
`uProjectionMatrix`, `uViewMatrix`, and `uNormalMatrix` to transform the input
world-space position `vPos` and normal `vNormal` into eye-space and pass them *out* to the
fragment shader and set the output variable `gl_Position`. Pass the
texture coordinate `vTexCoord` out unchanged.

        * `fPos`, `fNormal`, and `fTexCoord` are output variables, which is how you pass
along this information to the fragment shader, which is where you do
your lighting calculations. You can pass along `fPos` and `fNormal` in
world-space or in eye-space, so long as you use a consistent space when
you compute your lighting. (If you want to compute lighting in
world-space, you will have to convert the light position and the eye
position into world-space by multiplying them with `inverse(uViewMatrix)`.)

        * `uViewMatrix` is the matrix that converts a position in world-space into a
position in eye-space: `uViewMatrix*p` converts `p` from world to
eye-space.
`uNormalMatrix` is the matrix that converts a normal from world-space to a
normal in camera-space. It is equal to
`transpose(inverse(mat3(uViewMatrix)))`.

        * `uProjectionMatrix` is the matrix that converts a position from
camera-space to normalized-device coordinates.

        * `gl_Position` is the one variable you are required to assign in any
vertex shader, so that the GPU knows which pixels are inside the
triangle. You must set `gl_Position` to the position in normalized device
coordinates.

    * **(20 points)** Fragment shader `phong.fs`. Implement ambient, diffuse,
and specular illumination.

        * **(2 points)** Ambient lighting: *K<sub>A</sub> I<sub>AL</sub>*.

        * **(8 point)** Diffuse lighting: *K<sub>D</sub> I<sub>L</sub> ( N · L )*.
*N* is the (normalized) surface normal vector
and *L* is the (normalized) vector from the surface position to the light's position.
Note that if this dot product is negative, then the light is behind the surface
and you should not add diffuse OR specular lighting.

        * **(10 points)** Specular lighting: K<sub>S</sub> I<sub>L</sub> ( V · R )<sup>n</sup>.
*V* is the (normalized) vector from the surface position to the eye position.
In eye-space the eye is located at the origin. *R* is the (normalized)
direction from the surface position to the light position, reflected
across the surface normal. The formula for reflecting a vector across
another vector is given below under *Implementation Details* (or you can
use the GLSL `reflect()` function). Note that if the dot product is
negative, then the light is reflected away from the viewer and you
should not add any specular lighting. (You can use
max( 0, V · R) instead of an `if` statement if you
desire.) Also note that if the dot product used for diffuse lighting is
zero (the light is behind the surface), then you also should not add
specular lighting.

    * **(5 points)** Implement reflections with a cube map (in the fragment
shader) if `material.reflective`. Use the *world-space* reflected view
direction as the parameter to the cube map: `texture( uEnvironmentTex, direction )`.
The resulting color is *I<sub>R</sub>*. If the direction you pass
to the cube map is in eye-space, it's as if the environment is rotating
with the viewer, which means that the reflection will not change as you
rotate the camera with the mouse.

    * **(bonus 5 points)** Implement refraction if `material.refractive`. Use
the same cube map texture as in reflections to get *I<sub>T</sub>*.
You can get the refraction direction via the GLSL `refract()` function.

2. **(30 points)** Cel or toon shading. You only need to write a fragment shader `cel.fs`,
since the same vertex shader as the one you wrote for Phong shading `phong.vs` can be re-used.
Cel shading is the lighting effect used in games like
*Zelda: Wind Waker* and *Breath of the Wild*, *Jet Set Radio*, and *Naruto Shippuden*.
To achieve cel shading, we compute a scalar brightness value,
round it to discrete levels, and then use that to scale the material color.
The scalar brightness value *F* is computed with a simplified Phong reflectance
in which no material colors *K* appear and the light color terms *I<sub>AL</sub>* and *I<sub>L</sub>* for `.color_ambient` and `.color`
are simplified into scalar intensities:

    *F = ∑<sub>L</sub> (I<sub>AL</sub> + [I<sub>L</sub> ( N · L ) + I<sub>L</sub> ( V · R )<sup>n</sup>])*

    Use the integer uniform `material.bands` to round *F* into discrete levels:
*F<sub>discrete</sub>* = min( floor( *F* * *F* * `material.bands` ), `material.bands` - 1 )/( `material.bands` - 1).
(Squaring *F* makes the bands more uniformly spaced.) The final color
for a pixel is *K* * *F<sub>discrete</sub>*, where *K* is the
material color. 
If the boolean `material.use_diffuse_texture` is false, then *K* =
`material.color`. If `material.use_diffuse_texture` is true,
use *K* = `material.color` times the color stored in the texture map:
*K* = `material.color*texture( material.diffuse_texture, fTexCoord )`.
You will re-use your `phong.vs` and only write a different `cel.fs`.

3. **(20 points + bonus 3 points)** MatCaps with object-space normal mapping.
You only need to write a fragment shader `matcap.fs`,
since the same vertex shader as the one you wrote for Phong shading `phong.vs` can be re-used.
A MatCap (Material Capture) is a delightfully simple and flexible technique to create realistic or artistic lighting for a shape. The idea is simple. Different points on a shape look different because the normals are different.
Think about your Phong reflectance model. The material parameters and lights are shared by every point on the object. The vector *V* from the point to the eye varies slightly, but we could approximate this with a constant direction. This is all to say that the appearance of an object is almost entirely dependent on its normals in eye space.
A MatCap is a lookup table from a normal to a color. A sphere has normals pointing in all
directions. All we need to do is render a sphere and save its colors. To render,
we just need to look up the color of the sphere with that normal. In fact,
we only render the part of the shape we can see, so we only need to worry about
front-facing normals (with *z*>0 in eye-space). This gives us a very convenient way
to store the lookup table:

![matcap-blue.png](docs/images/matcap-blue.png)

We store the front half of the sphere as a circle. To look up the color from the matcap, we just use the *x,y* component of the normal scaled and shifted to lie in the range [0,1]² instead of [-1,1]²: `color = texture( material.matcap_texture, n.xy*0.5 + 0.5 ).rgb`.
We can create MatCaps any way we like. We can render a sphere with physically-based materials in a sophisticated lighting environment:

![matcap-gold.png](docs/images/matcap-gold.png)
![matcap-lake.png](docs/images/matcap-lake.png)

We can also create an artistic sphere:

![matcap-artred.png](docs/images/matcap-artred.png)

Those MatCaps create the following bunnies:

<img src="examples/matcap_bunny-blue.png" width="256" height="256" />
<img src="examples/matcap_bunny-gold.png" width="256" height="256" />
<img src="examples/matcap_bunny-lake.png" width="256" height="256" />
<img src="examples/matcap_bunny-artred.png" width="256" height="256" />

You can see lots of MatCaps visualized on 3D shapes [here](https://observablehq.com/@makio135/matcaps?ui=classic).

* **(10 points)** In `matcap.fs`, set the output color
to the color stored in the MatCap texture for the eye-space normal `fNormal`.
If `material.use_diffuse_texture`, multiply the color by the diffuse texture color `texture( material.diffuse_texture, fTexCoord ).rgba`.

* **(10 points)** Object-space normal mapping. Texture resolution is typically much higher than vertex resolution. This is why we typically store colors in a texture map and not as vertex attributes. Along these same lines, we can store normals in a normal map in order to have render super-detailed surfaces. Normal maps are typically stored
in tangent-space, which is substantially more difficult to implement. (Tangent-space normal mapping is described below for bonus points.)
In `matcap.fs`, if `material.use_normal_map`, read the object-space
normal from the texture via
`texture( material.normal_map, fTexCoord ).rgb`.
Normal components range
from [-1,1], while colors range from [0,1], so each normal component
is stored in the texture as `0.5*(n+1)`. Convert the color stored in the normalmap texture back to a normal via `2*color-1`.
This normal is in object-space. Convert it to eye-space using `uNormalMatrix` and then use it instead of `fNormal` to read from the MatCap.

* **(bonus 3 points)** Make your own MatCap! You can make a toon shader MatCap by taking a screenshot of a toon-shaded sphere. You can also make much cooler stuff. Have fun with this.

4. **(10 points)** Be creative! Create a time-varying artistic shader of
your own design. Make use of the uniform `uTime`, which stores the seconds
since the program was launched. Declare it in your shader as `uniform float uTime;`
Be sure to change `TimerMilliseconds` in the scene JSON file to something like 16
(which corresponds to 60 frames-per-second). See `sphere.vs` for an example.

4. **(bonus 30 points)** Normal mapping (`normalmap.vs` and `normalmap.fs`). This is an
extension of your Phong reflectance model shader. With normal mapping,
the texture stores a normal vector. Because lighting is entirely
determined by the normal vector, high resolution normals make a surface
look incredibly detailed. The normals in a normal maps are typically
stored as vectors in the *tangent space* of the surface. This technique
requires you to compute a "tangent frame" for each vertex (tangent and
bitangent vectors to accompany the normal) and upload that to the GPU as
additional attributes. To do this, you will implement `mesh.cpp`
`computeTangentBitangent()` to compute the tangent frame and `fancyscene.cpp`
`vaoFromOBJPath()` to upload the additional attributes to the GPU.
(Computing and uploading this additional data for this shader won't get
in the way of the other shaders.) Each face has a well-defined tangent
frame derived from the texture coordinates of its vertices; see the
accompanying handout for details. Just like with per-vertex normals, you
will pre-compute each face's tangent frame vectors and average them at
the vertices. In the vertex shader, you will convert the tangent and
bitangent vectors from world-space to eye-space (using `uViewMatrix`) and
pass them to the fragment shader. In the fragment shader, you will
extract the tangent-space normal from the texture and convert it to
world-space with the tangent frame matrix. That will be the normal you
use for your lighting calculations. (You can reconstruct the
tangent-frame matrix from the tangent, bitangent, and normal. Don't
forget to normalize them.) Implementation note: Normal components range
from [-1,1], while colors range from [0,1], so each normal component
is stored in the texture as `0.5*(n+1)`. Convert the color back to a
normal via `2*color-1`.

    * For a deeper explanation of the coordinate systems
    involved in tangent-space normal mapping, see [this PDF](docs/tangent_space_normal_and_bump_mapping.pdf).
    
    * For a video explanation of normal mapping, see [this YouTube video](https://www.youtube.com/watch?v=yHzIx41eiD4).

5. **(bonus variable points):**

    * Image Processing (blur, edge detect, etc.). Draw a full-screen textured
square. Your vertex shader should pass along the position and texture
coordinates, ignoring the view and projection transformations. Your
fragment shader accesses the texture value at the pixel and at nearby
pixels. You can use the GLSL function `textureSize( tex, 0 )` to get the texture
width and height in pixels so that you can generate texture coordinates
for the surrounding pixels.

    * Ray Tracer or Signed Distance Field renderer. Implement ray tracing or
ray marching (as in some [ShaderToy](https://www.shadertoy.com) examples).
Just like with Image Processing, the vertex shader does nothing. The
fragment shader implements an entire ray tracer or ray marching. All
geometry is specified via uniforms.

    * Animation. You can create a sequence of transformation matrices and
interpolate between them (linearly or with an easing function). You can
go further and implement linear blend skinning, in which there are
multiple sequences of transformation matrices animating in parallel, and
each vertex has a set of associated "skin" weights. The vertex is
transformed by the weighted average transformation matrix. Some of
the infrastructure for this is already implemented.

    * Ambient Occlusion. This provides a much more accurate surface ambient
color *K<sub>A</sub>* by computing the fraction of directions that are blocked
by the mesh (a ray exiting the point with that direction would intersect
the mesh). You can compute this in a pre-computation step or approximate
it with a multi-pass shader. When computing it as a pre-process, you can
store per-vertex attributes or a texture map.

    * Procedural textures. Generate a wood or marble color as a 3D function of
position.

    * Geometry or tessellation shaders. We haven't talked about these more
exotic shaders, but the framework supports them.

    * Physically-Based Rendering. There are many ways to pursue this. Note that the `lemon`
    example comes with a gloss map.

    * Something else!

Tips
----

* If you see a blank screen (all background color and no shape), the problem is most likely in your vertex shader. This is assuming that your environment works (e.g. that `simplescene.json` and `sphere.json` produce the expected output).

* The glm library is modeled after the OpenGL shading language GLSL.
Anything involving `vec2`, `vec3`, `vec4`, `mat3`, `mat4`, etc. and virtually any
GLSL functions should be accessible in C++ with the `glm::` prefix.
There are many convenience constructors and math operations.
Here is a nice reference: [GLSL Programming: Vector and Matrix Operations](https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations).

* The GLSL (and glm) matrix constructors that take vectors use those vectors
as the columns of the matrix, not the rows.
(If you want to fill out the rows, fill out the columns and then call `transpose()`.)
In fact, all matrix indexing uses column-then-row (column-major) ordering,
which is different from what mathematicians do.

* In C++ you can call the constructor of an object when you declare it,
like so:

        vec4 foo( 1.0, 2.0, 3.0, 4.0 );

    GLSL does not allow this and you will get a compile error. Instead, you
have to write:

        vec4 foo = vec4( 1.0, 2.0, 3.0, 4.0 );

* Unless you assign a value, a local variable you declare will be uninitialized.
While some implementations will initialize, for example, `vec3 foo;` to 0 by default,
the GLSL specification does not require this. (The grader's graphics driver
will probably initialize to random values.) Manually initialize to 0,
`vec3 foo = vec3(0);`, if that's what you want.

* There is a simple, sample 3D shader in `sphere.json` / `sphere.vs` /
`sphere.fs`. (The simplest possible shader is in `simplescene.json` /
`simplest.vs` / `simplest.fs` and is based off the `simplescene.h/cpp` C++ setup
code.)

* To convert a position from world-space to eye-space, left-multiply the
provided uniform `uViewMatrix` by the position. To convert a normal from
world-space to eye-space, use the provided uniform `uNormalMatrix`, which
is the inverse-transpose of the upper-left 3x3 portion of `uViewMatrix`.
To convert a position or direction from eye-to-world, use `inverse( uViewMatrix )`.
To convert a direction (other than a normal) from eye-to-world, you can be
more efficient and use `transpose( uNormalMatrix )`.

* The GLSL function `texture(...)`, which loads a value from a texture,
returns a `vec4`. Some overly permissive OpenGL drivers will allow you to
use it as a `vec3`, but that behavior is wrong. Use `texture(...).rgb` to
get the RGB components as a `vec3` or call
`vec3(texture(...))`.

* To reflect a (not necessarily normalized) vector **v** across a
normalized vector **n**, the formula for the reflected vector **r** is
**r = v - 2(v·n)n**.

    ![Illustration of a vector v reflected across a vector n](docs/eqs/reflection.svg)

* All of the required portions of the assignment can use the same C++
setup code (defined in `fancyscene.cpp`/`fancyscene.h`) and differ only in which
shaders and uniforms are specified in the JSON file. (Only normal
mapping requires you to modify some of the C++ setup code.)

* You can achieve better code re-use in the shaders if you split them up. When
you specify a shader in the JSON file, you can pass an array of file paths which
will be concatenated together in sequence and then compiled. This allows you to
write common helper functions in a `common.glsl` file if you so desire. The
first line must always be a `#version` directive, so I often split my complex
shaders into `[ shaderTop.vs, common.glsl, shaderBottom.vs ]`. It may also make
sense to share a bottom but have different middles (e.g. normal mapping and
Phong shading could share a bottom but have different middle files implementing
an appropriate `get_normal()` function).

* You can create your own C++ scene if you want to customize the camera or
attributes. You will need to duplicate the scene class (`SimpleScene` or
`FancyScene`) and register it with `pipelineguifactory.cpp`. For reference, you
can see/study the simplest-possible OpenGL scene in `simplescene.h` and
`simplescene.cpp`.

* Almost everything in a JSON scene will live reload if using a
`FancyScene`, including shaders, uniforms, and textures.
The only property that will not live reload is `PipelineGUI`.

* You can find lots of Creative Commons environment (cube) maps on the
website of Emil Persson, aka Humus:
<http://www.humus.name>

* You can find lots of MatCaps on this website: <https://github.com/nidorx/matcaps>. You can see all the MatCaps nicely visualized here: <https://observablehq.com/@makio135/matcaps?ui=classic>
# graphics-pipeline
