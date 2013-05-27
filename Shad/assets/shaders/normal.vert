varying vec3 normal;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// send the normal to the fragment shader
	normal = normalize(gl_NormalMatrix * gl_Normal);
}
