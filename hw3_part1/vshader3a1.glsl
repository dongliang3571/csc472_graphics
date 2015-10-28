
attribute vec2  a_Position;	// attribute variable: position vector
varying   vec2  v_TexCoord;
attribute vec2  a_TexCoord;
uniform   mat4  u_ModelMatrix;
uniform   bool   u_Twist;
uniform   float   u_Theta;

void main() {
	if(u_Twist==true){
		float d;
		float sinTheta;
		float cosTheta;
		d = sqrt(a_Position.x * a_Position.x + a_Position.y * a_Position.y);

		sinTheta = sin(d*u_Theta);
		cosTheta = cos(d*u_Theta);
    mat4 twist=mat4(cosTheta,sinTheta,0.0,0.0,
										-sinTheta,cosTheta,0.0,0.0,
										0.0,      0.0,     1.0,0.0,
										0.0,      0.0,     0.0,1.0);
		gl_Position = gl_ModelViewProjectionMatrix * twist * vec4(a_Position,0,1);
		v_TexCoord  = a_TexCoord;
	}
	else{
            gl_Position = gl_ModelViewProjectionMatrix * u_ModelMatrix * vec4(a_Position,0,1);
            v_TexCoord  = a_TexCoord;
	}
}
