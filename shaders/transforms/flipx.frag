uniform sampler2D texture;

void main()
{
    vec2 tex_coords = gl_TexCoord[0].st;
    tex_coords.x = 1.0 - tex_coords.x;
    vec4 color = texture2D(texture, tex_coords);
    gl_FragColor = color;
}
