#version 460 core


layout (binding=0, rgba32i) uniform iimage2DArray result;
layout (binding=1, rgba32i) uniform iimage2DArray points;
layout (binding=2, rgba32i) uniform iimage2DArray polygons;

// out vec4 FragColor;

void main()
{
    ivec4 s0;
    ivec4 s1;
    // column 0 & column 1 & column 2
    for (int i = 0; i < 3; ++i) {
        s0 = imageLoad(points, ivec3(gl_FragCoord.x, gl_FragCoord.y, i));
        s1 = imageLoad(polygons, ivec3(gl_FragCoord.x, gl_FragCoord.y, i));
        s0 = ivec4(s0.x, 0, s1.z, 0);
        imageStore(
            result,
            ivec3(gl_FragCoord.x, gl_FragCoord.y, i),
            s0
        );
    }

    discard;
} 