uniform sampler2D texture;
uniform float brightness; // range: -1 (darker) to 1 (brighter)

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    if (color.a == 0.0)
    {
        gl_FragColor = color;
        return;
    }

    color.rgb += brightness;
    gl_FragColor = clamp(color, 0.0, 1.0);
}