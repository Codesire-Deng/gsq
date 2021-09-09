#version 460 core

uniform vec3 customColor;
uniform ivec3 sColumn0;
uniform ivec3 sColumn1;
uniform ivec3 sColumn2;

layout (binding=0, rgba32i) uniform iimage2DArray canvas;

// in S {
//     ivec3 sColumn0;
//     ivec3 sColumn1;
//     ivec3 sColumn2;
// };

out vec4 FragColor;

void main()
{
    FragColor = vec4(customColor, 1.0f);
    imageStore(
        canvas,
        ivec3(gl_FragCoord.x, gl_FragCoord.y, 0),
        ivec4(sColumn0, 0)
    );
    imageStore(
        canvas,
        ivec3(gl_FragCoord.x, gl_FragCoord.y, 1),
        ivec4(sColumn1, 0)
    );
    imageStore(
        canvas,
        ivec3(gl_FragCoord.x, gl_FragCoord.y, 2),
        ivec4(sColumn2, 0)
    );
    vec4 receive;
    receive = imageLoad(canvas, ivec3(gl_FragCoord.x, gl_FragCoord.y, 0));
    FragColor = vec4(receive.xyz/3.0, 1.0f);
    // FragColor = vec4(sColumn0.xyz/3.0, 1.0f);
} 