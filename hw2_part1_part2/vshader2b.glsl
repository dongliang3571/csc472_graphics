attribute vec2  a_Position;	// attribute variable: position vector
varying   vec4  v_Color;
attribute vec3  a_Color;
uniform   mat4  u_ModelMatrix;
uniform   int   u_Twist;
uniform   float   u_angle;

void main() {
	if(u_Twist==1){
		float d;
		float sinTheta;
		float cosTheta;
		d = sqrt(a_Position.x * a_Position.x + a_Position.y * a_Position.y);

		sinTheta = sin(d*u_angle);
		cosTheta = cos(d*u_angle);
    mat4 twist=mat4(cosTheta,sinTheta,0.0,0.0,
										-sinTheta,cosTheta,0.0,0.0,
										0.0,      0.0,     1.0,0.0,
										0.0,      0.0,     0.0,1.0);
		gl_Position= gl_ModelViewProjectionMatrix * twist * vec4(a_Position,0,1);
		v_Color     = vec4(a_Color,1);
	}
	else{
		gl_Position = gl_ModelViewProjectionMatrix * u_ModelMatrix * vec4(a_Position,0,1);
		v_Color     = vec4(a_Color,1);
	}
}
