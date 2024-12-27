uniform sampler2D texture;
uniform vec2 layer_pos;
uniform vec2 canvas_size;

vec2 adjust_coords(vec2 tex_coord)
{
    return (tex_coord * canvas_size + layer_pos) / canvas_size;
}

void main()
{
    vec4 color = texture2D(texture, adjust_coords(gl_TexCoord[0].xy));
    color.rgb = vec3(1.0) - color.rgb;
    gl_FragColor = color;
}