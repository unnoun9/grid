uniform sampler2D texture;
uniform float contrast;
uniform vec2 layer_pos;
uniform vec2 canvas_size;

vec2 adjust_coords(vec2 tex_coord)
{
    return (tex_coord * canvas_size + layer_pos) / canvas_size;
}

void main()
{
    vec4 color = texture2D(texture, adjust_coords(gl_TexCoord[0].st));
    float contrast_factor = (259 * (contrast + 255)) / (255 * (259 - contrast));
    color.rgb = clamp(contrast_factor * (color.rgb - 0.5) + 0.5, 0.0, 1.0);
    gl_FragColor = color;
}

