uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    color.rgb = veec3(1.0) - color.rgb;
    gl_FragColor = color;
}