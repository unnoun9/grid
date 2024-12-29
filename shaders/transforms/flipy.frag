uniform sampler2D texture;

void main()
{
    vec2 tex_coords = gl_TexCoord[0].st;
    tex_coords.y = 1.0 - tex_coords.y;
    vec4 color = texture2D(texture, tex_coords);
    gl_FragColor = color;
}
