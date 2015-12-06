attribute vec4 a_Position;
attribute vec4 vNormal;
varying vec4 color; // vertex shade
// light and material properties
uniform vec3 AmbientProduct;
uniform vec3 DiffuseProduct;
uniform vec3 SpecularProduct;
uniform mat4 u_MVPMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ModelView;
uniform vec3 LightPosition;
uniform float Shininess;

void main()
{

    gl_Position = u_MVPMatrix * a_Position;
    // transform vertex position into eye coordinates
    vec4 pos = ModelView * a_Position;
    vec3 L = normalize( LightPosition - pos.xyz );
    vec3 E = normalize( -pos.xyz );
    vec3 H = normalize( L + E );
    // transform vertex normal into eye coordinates


    vec3 N = normalize(vec3(NormalMatrix * vNormal));

    float nDotL = max(dot(L, N), 0.0);
    vec3 ambient = AmbientProduct;
    // diffuse lighting term

    vec3 diffuse = DiffuseProduct * nDotL;
    // specular lighting term
    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec3 specular = Ks * SpecularProduct;
    if( dot(L, N) < 0.0 ) specular = vec3(0.0, 0.0, 0.0);
    // add lighting terms to form color
    color = vec4(diffuse, 1.0) + vec4(ambient, 1.0)+ vec4(specular, 1.0);
//    color = vec4(0.8, 0.2, 0.8,1.0);

}
