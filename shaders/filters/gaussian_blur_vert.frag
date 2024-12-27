uniform sampler2D texture;
uniform float blur_size;
uniform vec2 texel_size;
uniform vec2 layer_pos;
uniform vec2 canvas_size;

vec2 adjust_coords(vec2 tex_coord)
{
    return (tex_coord * canvas_size + layer_pos) / canvas_size;
}

float gaussian(float x, float sigma)
{
    return exp(- (x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * 3.1415926535897932384626433832795) * sigma);
}

void main()
{
    vec2 tex_coord = adjust_coords(gl_TexCoord[0].st);
    
    vec4 color = vec4(0.0);
    float total = 0.0;

    for (float y = -blur_size; y <= blur_size; y++)
    {
        float weight = gaussian(y, blur_size);
        color += texture2D(texture, tex_coord + vec2(0.0, y * texel_size.y)) * weight;
        total += weight;
    }

    color /= total;
    gl_FragColor = color;
}
