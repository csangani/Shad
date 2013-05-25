varying vec3 modelPos;
varying vec3 lightSource;
varying vec3 normal;

void main()
{
    vec4 posTemp = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ProjectionMatrix * posTemp;
	// send the model's position to the fragment shader
    modelPos = posTemp.xyz;

	// send the normal to the fragment shader
	normal = normalize(gl_NormalMatrix * gl_Normal);

    // send the light position to the fragment shader.
    lightSource = gl_LightSource[0].position.xyz;
}
