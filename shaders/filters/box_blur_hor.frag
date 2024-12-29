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

    color = vec4(0.0);

    for (float x = -blur_size; x <= blur_size; x++)
    {
        color += texture2D(texture, tex_coord + vec2(x * texel_size.x, 0.0));
    }

    color /= (2.0 * blur_size + 1.0);
    gl_FragColor = color;
}
