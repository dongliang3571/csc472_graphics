
varying   vec2  v_TexCoord;
attribute vec2  a_TexCoord;


// Vertex Shader
attribute vec4 a_Position;
attribute vec4 vNormal;
attribute vec3 a_Color;
// output values that will be interpolatated per-fragment

varying vec3 position;
varying vec3 normal;
varying vec4 color;

uniform mat4 NormalMatrix;
uniform mat4 ModelView;
uniform mat4 u_MVPMatrix;

void main() {
    gl_Position = u_MVPMatrix * a_Position;
    position = vec3(ModelView * a_Position);
    normal = normalize(vec3(NormalMatrix * vNormal));
    color = vec4(a_Color,1.0);

    v_TexCoord  = a_TexCoord;

}
