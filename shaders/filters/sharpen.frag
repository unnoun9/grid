uniform sampler2D texture;

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

    float kernel[9];
    kernel[0] =  0.0; kernel[1] = -0.5; kernel[2] =  0.0;
    kernel[3] = -0.5; kernel[4] =  3.0; kernel[5] = -0.5;
    kernel[6] =  0.0; kernel[7] = -0.5; kernel[8] =  0.0;

    vec3 sum = vec3(0.0);

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            color = texture2D(texture, tex_coords + vec2(i, j) * texel_size);
            sum += color.rgb * kernel[(i + 1) * 3 + (j + 1)];
        }
    }

    gl_FragColor = vec4(sum, 1.0);
}
