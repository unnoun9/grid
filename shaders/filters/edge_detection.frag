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

    float Gx[9];
    float Gy[9];

    Gx[0] = -1.0; Gx[1] = 0.0; Gx[2] = 1.0;
    Gx[3] = -2.0; Gx[4] = 0.0; Gx[5] = 2.0;
    Gx[6] = -1.0; Gx[7] = 0.0; Gx[8] = 1.0;

    Gy[0] = -1.0; Gy[1] = -2.0; Gy[2] = -1.0;
    Gy[3] =  0.0; Gy[4] =  0.0; Gy[5] =  0.0;
    Gy[6] =  1.0; Gy[7] =  2.0; Gy[8] =  1.0;

    float sum_x = 0.0;
    float sum_y = 0.0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            color = texture2D(texture, tex_coords + vec2(i, j) * texel_size);
            float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

            sum_x += gray * Gx[(i + 1) * 3 + (j + 1)];
            sum_y += gray * Gy[(i + 1) * 3 + (j + 1)];
        }
    }

    float edge = sqrt(sum_x * sum_x + sum_y * sum_y);
    gl_FragColor = vec4(vec3(edge), 1.0);
}
