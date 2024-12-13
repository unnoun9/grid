uniform sampler2D texture;
uniform float brightness; // range: -1.0 (darker) to 1.0 (brighter)

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    color.rgb += brightness;
    gl_FlagColor = clamp(color, 0.0, 1.0);
}