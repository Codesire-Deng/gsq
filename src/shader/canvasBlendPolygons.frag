#version 460 core


layout (binding=0, rgba32i) uniform iimage2DArray result;
layout (binding=1, rgba32i) uniform iimage2DArray polygons0;
layout (binding=2, rgba32i) uniform iimage2DArray polygons1;

// out vec4 FragColor;

void main()
{
    ivec4 s0;
    ivec4 s1;

    // column 0 & column 2
    for (int i = 0; i < 3; i+=2) {
        s0 = imageLoad(polygons0, ivec3(gl_FragCoord.x, gl_FragCoord.y, i));
        s1 = imageLoad(polygons1, ivec3(gl_FragCoord.x, gl_FragCoord.y, i));
        s0 = ivec4(0, 0, s1.z, 0);
        imageStore(
            result,
            ivec3(gl_FragCoord.x, gl_FragCoord.y, i),
            s0
        );
    }

    // column 1
    s0 = imageLoad(polygons0, ivec3(gl_FragCoord.x, gl_FragCoord.y, 1));
    s1 = imageLoad(polygons1, ivec3(gl_FragCoord.x, gl_FragCoord.y, 1));
    s0 = ivec4(0, 0, s0.z + s1.z, 0);
    imageStore(
        result,
        ivec3(gl_FragCoord.x, gl_FragCoord.y, 1),
        s0
    );

    discard;
} 