varying vec2 v_TexCoord;
uniform sampler2D u_Sampler;

varying vec3 position;
varying vec3 normal;
//varying vec4 color;
uniform vec3 AmbientLight;
//uniform vec3 DiffuseLight;
//uniform vec3 SpecularLight;
//uniform mat4 ModelView;
uniform vec3 LightPosition;
uniform float Shininess;

uniform vec3 u_LightColor;
void main(){

        gl_FragColor = texture2D(u_Sampler,v_TexCoord);

        vec3 N = normalize(normal);
        vec3 L = normalize(LightPosition-position);
        float nDotL = max(dot(L, normal),0.0);
        vec3 E = -position;
        vec3 H = normalize(L + E);
        float Ks = pow(max(dot(N, H), 0.0),Shininess);
        vec3 diffuse = u_LightColor * texture2D(u_Sampler,v_TexCoord).rgb * nDotL;
        vec3 ambient = AmbientLight * texture2D(u_Sampler,v_TexCoord).rgb;
        vec3 specular = u_LightColor * texture2D(u_Sampler,v_TexCoord).rgb * Ks ;
        gl_FragColor = vec4(diffuse + ambient + specular, texture2D(u_Sampler,v_TexCoord).a);
}
