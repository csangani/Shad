uniform float hasTexture;
uniform sampler2D texture;
uniform vec2 textureSize;
varying vec2 texCoord;

void main()
{
	vec3 colorNW = vec3(0.0, 0.0, 0.0);
	vec3 colorNE = vec3(0.0, 0.0, 0.0);
	vec3 colorSW = vec3(0.0, 0.0, 0.0);
	vec3 colorSE = vec3(0.0, 0.0, 0.0);

	if (texCoord.x > 1.0/textureSize.x && texCoord.y < (textureSize.y - 1.0/textureSize.y))
		colorNW = texture2D(texture, texCoord + (vec2(-1.0, 1.0)/textureSize)).rgb;

	if (texCoord.x < (1.0 - 1.0/textureSize.x) && texCoord.y < (textureSize.y - 1.0/textureSize.y))
	    colorNE = texture2D(texture, texCoord + (vec2(1.0, 1.0)/textureSize)).rgb;

	if (texCoord.x > 1.0/textureSize.x && texCoord.y > 1.0/textureSize.y)
		colorSW = texture2D(texture, texCoord + (vec2(-1.0, -1.0)/textureSize)).rgb;

	if (texCoord.x < (1.0 - 1.0/textureSize.x) && texCoord.y > 1.0/textureSize.y)
	    colorSE = texture2D(texture, texCoord + (vec2(1.0, -1.0)/textureSize)).rgb;

    vec3 color = (colorNW + colorNE + colorSW + colorSE + texture2D(texture, texCoord).rgb)/3.0;

    gl_FragColor = vec4(color, 1.0);
}
