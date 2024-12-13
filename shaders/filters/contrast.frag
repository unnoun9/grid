uniform sampler2D texture;
uniform float contrast; // range: -1.0 (less contrast) to 1.0 (more contrast)

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    color.rgb = (color.rgb - 0.5) * max(contrast, 0.0) + 0.5;
    gl_FlagColor = clamp(color, 0.0, 1.0);
}