varying vec3 modelPos;
varying vec3 lightSource;
varying vec3 normal;

varying vec2 texCoord;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	// send the model's position to the fragment shader
    modelPos = (gl_ModelViewMatrix * gl_Vertex).xyz;

	// send the normal to the fragment shader
	normal = normalize(gl_NormalMatrix * gl_Normal);

    // send the light position to the fragment shader.
    lightSource = gl_LightSource[0].position.xyz;

	// send texcoord to fragment shader
	texCoord = gl_MultiTexCoord0.xy;
}
