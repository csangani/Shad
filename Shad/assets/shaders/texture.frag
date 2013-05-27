varying vec2 texCoord;
uniform sampler2D renderedTexture;

void main()
{
    gl_FragColor = texture2D(renderedTexture, texCoord);
}
