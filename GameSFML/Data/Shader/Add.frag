uniform sampler2D source;
uniform sampler2D bloom;
void main()
{
    gl_FragColor = texture2D(source, gl_TexCoord[0].xy) + texture2D(bloom, gl_TexCoord[0].xy);
}