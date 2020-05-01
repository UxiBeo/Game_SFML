uniform sampler2D 	source;
uniform vec2 		offsetFactor;
uniform  int		r;
uniform float 		coefficients[15];

void main()
{
	vec3 color = vec3(0.0f);
	for(int i = -r; i < r; i++)
	{
		color += texture(source, gl_TexCoord[0].xy + i * offsetFactor).rgb * coefficients[i + r];
	}
	gl_FragColor = vec4(color,1.0f);
}