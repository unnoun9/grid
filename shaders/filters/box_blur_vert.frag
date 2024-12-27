uniform sampler2D texture;
uniform float blur_size;
uniform vec2 texel_size;
uniform vec2 layer_pos;
uniform vec2 canvas_size;

vec2 adjust_coords(vec2 tex_coord)
{
    return (tex_coord * canvas_size + layer_pos) / canvas_size;
}

void main()
{
    vec2 tex_coord = adjust_coords(gl_TexCoord[0].st);

    vec4 color = vec4(0.0);

    for (float y = -blur_size; y <= blur_size; y++)
    {
        color += texture2D(texture, tex_coord + vec2(0.0, y * texel_size.y));
    }

    color /= (2.0 * blur_size + 1.0);
    gl_FragColor = color;
}
