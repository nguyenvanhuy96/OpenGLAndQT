#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;
void build_house(vec4  position){
    gl_Position = position + vec4(-0.2, -0.2,0.0,0.0) ; // 1: bottom-left
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2,0.0,0.0) ; // 2: bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2,0.0,0.0) ; // 3: Top - left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2,0.0,0.0) ; // 4: Top - Right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4,0.0,0.0) ; // 5: Top
    EmitVertex();
    EndPrimitive();
}
void main(){
  build_house(gl_in[0].gl_Position);
}
