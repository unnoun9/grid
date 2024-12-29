uniform sampler2D texture;
uniform float pixelate_size;

void main()
{
    vec2 texel_size = vec2(1.0 / textureSize(texture, 0).x, 1.0 / textureSize(texture, 0).y);
    vec2 tex_coords = gl_TexCoord[0].st;

    vec4 color = texture2D(texture, tex_coords);

    if (color.a == 0.0)
    {
        gl_FragColor = color;
        return;
    }

    vec2 pixelated_coords = floor(tex_coords / (pixelate_size * texel_size)) * (pixelate_size * texel_size);
    color = texture2D(texture, pixelated_coords);

    gl_FragColor = color;
}
