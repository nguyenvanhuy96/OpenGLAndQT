#version 330 core
layout (lines) in;
layout (line_strip, max_vertices = 2) out;
in VS_OUT{
   vec3 color;
} gs_in[];
out vec3 fcolor;
void build_house(vec4  position1, vec4 position2){
    fcolor= gs_in[0].color;
    gl_Position = position1 + vec4(0.0, -0.2,0.0,0.0) ; // 1: bottom-left
    EmitVertex();
    gl_Position = position2 + vec4(0.0, 0.2,0.0,0.0) ; // 2: bottom-right
    EmitVertex();
    EndPrimitive();
}
void main(){
  build_house(gl_in[0].gl_Position,gl_in[1].gl_Position );
}
