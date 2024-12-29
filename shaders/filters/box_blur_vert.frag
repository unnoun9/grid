uniform sampler2D texture;
uniform float blur_size;
uniform vec2 texel_size;

void main()
{
    vec2 tex_coord = gl_TexCoord[0].st;

    vec4 color = texture2D(texture, tex_coord);

    if (color.a == 0.0)
    {
        gl_FragColor = color;
        return;
    }
    
    for (float y = -blur_size; y <= blur_size; y++)
    {
        color += texture2D(texture, tex_coord + vec2(0.0, y * texel_size.y));
    }

    color /= (2.0 * blur_size + 1.0);
    gl_FragColor = color;
}
