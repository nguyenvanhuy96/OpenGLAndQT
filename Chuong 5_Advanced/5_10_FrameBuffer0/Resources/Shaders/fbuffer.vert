#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
out vec2 TexCoords;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    //nếu muốn vẽ trong không gian tính toán các thông số khác như thông thường:
    //ví dụ:projection * view* model * vec4(aPos.x,aPos.y,aPos.z,1.0);
    // Ở đây muốn vẽ đối tượng ở góc 1/16 dưới cùng bên trái màn hình
    gl_Position = vec4(aPos.x, aPos.y,aPos.z, 1.0);
}
