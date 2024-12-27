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
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114)); // NTSC grayscale formula
    gl_FragColor = vec4(vec3(gray), color.a);
}