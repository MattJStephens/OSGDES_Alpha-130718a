varying vec3 vertex_color;
void main()
{
  gl_Position = ftransform();
  vertex_color = gl_Vertex.xyz;
} 
