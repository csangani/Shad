varying vec3 modelPos;
varying vec3 lightSource;
varying vec3 normal;

attribute vec3 positionIn;
attribute vec3 normalIn;

void main()
{
    vec4 posTemp = gl_ModelViewMatrix * vec4(positionIn,1.0);
	gl_Position = gl_ProjectionMatrix * posTemp;
	// send the model's position to the fragment shader
    modelPos = posTemp.xyz;

	// send the normal to the fragment shader
	normal = normalize(gl_NormalMatrix * normalIn);

    // send the light position to the fragment shader.
    lightSource = gl_LightSource[0].position.xyz;
}
