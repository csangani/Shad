uniform int numFrames;

// frame1 is the oldest, frame<numFrames> is the newest
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

	vec4 finalColor = vec4(0, 0, 0, 1);
	if (numFrames == 5)
		finalColor = (color1 + color2*2.0 + color3*3.0 + color4*4.0 + color5*5.0)/15.0;
	else if (numFrames == 4)
		finalColor = color1 * vec4(1.0, 1.0, 1.0, 0.4) + color2 * vec4(1.0, 1.0, 1.0, 0.6) + color3 * vec4(1.0, 1.0, 1.0, 0.8) + color4;
	else if (numFrames == 3)
		finalColor = color1 * vec4(1.0, 1.0, 1.0, 0.6) + color2 * vec4(1.0, 1.0, 1.0, 0.8) + color3;
	else if (numFrames == 2)
		finalColor = color1 * vec4(1.0, 1.0, 1.0, 0.8) + color2;
	else
		finalColor = color1;
	gl_FragColor = finalColor;
}