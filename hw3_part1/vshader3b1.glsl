attribute vec3  a_Position;	// attribute variable: position vector
varying   vec2  v_TexCoord;
attribute vec2  a_TexCoord;
uniform   mat4  u_MVPMatrix;
// uniform   mat4  u_ViewMatrix;
// uniform   mat4  u_ProjectMatrix;

void main() {
  gl_Position = u_MVPMatrix * vec4(a_Position,1.0);
  v_TexCoord  = a_TexCoord;

}
