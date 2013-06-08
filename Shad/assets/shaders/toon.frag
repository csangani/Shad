varying vec3 modelPos;
varying vec3 lightSource;
varying vec3 normal;

uniform float hasTexture;
uniform sampler2D texture;
varying vec2 texCoord;

void main()
{
    vec3 C = vec3(0.0, 0.0, 0.0); // positions are in camera coords

    vec3 ambientColor  = gl_LightSource[0].ambient.xyz;
    vec3 diffuseColor  = gl_LightSource[0].diffuse.xyz;
    vec3 specularColor = gl_LightSource[0].specular.xyz;

    vec3 materialColor = gl_FrontMaterial.ambient.xyz;
    vec3 materialSpec  = gl_FrontMaterial.specular.xyz;
    float shininess    = gl_FrontMaterial.shininess;

	// Phong reflection
	vec3 Lm = normalize(lightSource - modelPos);
	vec3 N = normalize(normal);
	vec3 Rm = normalize(reflect(-Lm, N));
	vec3 Vm = normalize(C - modelPos);
	
	// dot products
	float dot1 = max(0.0, dot(Lm, N));
	float dot2 = max(0.0, dot(Rm, Vm));
	
	// compute I
	vec3 ambient = materialColor * ambientColor;
	vec3 diffuse = (dot1) * materialColor * diffuseColor;
	vec3 specular = (pow(dot2, shininess)) * materialSpec * specularColor;

	vec3 result = ambient + diffuse + specular;
	if (hasTexture > 0.0)
		result = result * texture2D(texture, texCoord).xyz;

	float threshold = dot1;
	if (threshold > 0.8)
		gl_FragColor = vec4(result, 1.0);
	else if (threshold > 0.5)
		gl_FragColor = vec4(0.8*result, 1.0);
	else if (threshold > 0.4)
		gl_FragColor = vec4(0.6*result, 1.0);
	else if (threshold > 0.2)
		gl_FragColor = vec4(0.4*result, 1.0);
	else
		gl_FragColor = vec4(0.2*result, 1.0);

	// TODO: add contours
	/*float dot3 = dot(N, Vm);
	if (dot3 < 0.01 && dot3 > -0.01)
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);*/
}
