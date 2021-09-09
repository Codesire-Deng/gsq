#version 460 core
layout (location = 0) in vec2 aPos;
// layout (location = 1) in ivec3 sRow0;
// layout (location = 2) in ivec3 sRow1;
// layout (location = 3) in ivec3 sRow2;
// layout (location = 4) in ivec3 sRow3;

// out S {
//     ivec3 sColumn0;
//     ivec3 sColumn1;
//     ivec3 sColumn2;
// };

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    // sColumn0 = ivec3(sRow0.x, sRow1.x, sRow2.x);
    // sColumn1 = ivec3(sRow0.y, sRow1.y, sRow2.y);
    // sColumn2 = ivec3(sRow0.z, sRow1.z, sRow2.z);
}