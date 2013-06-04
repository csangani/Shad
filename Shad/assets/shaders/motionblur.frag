uniform int numFrames;

uniform sampler2D frame1;
uniform sampler2D frame2;
uniform sampler2D frame3;
uniform sampler2D frame4;
uniform sampler2D frame5;

varying vec2 texCoord;

void main ()
{
	vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color2 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color3 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color4 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color5 = vec4(0.0, 0.0, 0.0, 1.0);

	if (numFrames > 0)
		color1 = texture2D(frame1, texCoord);
	if (numFrames > 1)
		color2 = texture2D(frame2, texCoord);
	if (numFrames > 2)
		color3 = texture2D(frame3, texCoord);
	if (numFrames > 3)
		color4 = texture2D(frame4, texCoord);
	if (numFrames > 4)
		color5 = texture2D(frame5, texCoord);

	vec4 finalColor = (color1+color2+color3+color4+color5)/numFrames;
	gl_FragColor = vec4(min(finalColor.r, 1.0), min(finalColor.g, 1.0), min(finalColor.b, 1.0), 1.0);
}