#version 410  core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;

uniform mat4 mvp;

out vec2 f_uv;
out vec3 f_norm;
flat out  int faceIndex;

int  getFaceIndex(vec3 normal) {
    if (normal.x == 1.0) return 0;
    else if (normal.x == -1.0) return 1;
    else if (normal.y == 1.0) return 2;
    else if (normal.y == -1.0) return 3;
    else if (normal.z == 1.0) return 4;
    else if (normal.z == -1.0) return 5;
    else return -1;
}


void main() {
    gl_Position = mvp * vec4(pos, 1.0);
    f_uv = uv;
    f_norm = norm;
    faceIndex = getFaceIndex(norm);
}
