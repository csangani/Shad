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
	vec4 redColor = vec4(1.0, 0.0, 0.0, 1.0);

	vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color2 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color3 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color4 = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color5 = vec4(0.0, 0.0, 0.0, 1.0);

	if (numFrames > 0)
		color1 = texture2D(frame1, texCoord) * vec4(1.0, 0.6, 0.6, 1.0);
	if (numFrames > 1)
		color2 = texture2D(frame2, texCoord) * vec4(1.0, 0.7, 0.7, 1.0);
	if (numFrames > 2)
		color3 = texture2D(frame3, texCoord) * vec4(1.0, 0.8, 0.8, 1.0);
	if (numFrames > 3)
		color4 = texture2D(frame4, texCoord) * vec4(1.0, 0.9, 0.9, 1.0);
	if (numFrames > 4)
		color5 = texture2D(frame5, texCoord);

	vec4 finalColor;
	if (numFrames == 5)
		finalColor = color1 * vec4(1.0, 1.0, 1.0, 0.2) + color2 * vec4(1.0, 1.0, 1.0, 0.4) + color3 * vec4(1.0, 1.0, 1.0, 0.6) + color4 * vec4(1.0, 1.0, 1.0, 0.8) + color5;
	else if (numFrames == 4)
		finalColor = color1 * vec4(1.0, 1.0, 1.0, 0.4) + color2 * vec4(1.0, 1.0, 1.0, 0.6) + color3 * vec4(1.0, 1.0, 1.0, 0.8) + color4;
	else if (numFrames == 3)
		finalColor = color1 * vec4(1.0, 1.0, 1.0, 0.6) + color2 * vec4(1.0, 1.0, 1.0, 0.8) + color3;
	else if (numFrames == 2)
		finalColor = color1 * vec4(1.0, 1.0, 1.0, 0.8) + color2;
	else
		finalColor = color1;
	gl_FragColor = vec4(min(finalColor.r, 1.0), min(finalColor.g, 1.0), min(finalColor.b, 1.0), 1.0);
}