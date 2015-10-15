attribute vec2  a_Position;	// attribute variable: position vector
//varying   vec4  v_Color;
//attribute vec3  a_Color;
void main() {
	gl_Position = gl_ModelViewProjectionMatrix * vec4(a_Position,0,1);
	//v_Color     = vec4(a_Color,1);
}
