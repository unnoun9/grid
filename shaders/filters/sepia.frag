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
    float r = dot(color.rgb, vec3(0.393, 0.769, 0.189));
    float g = dot(color.rgb, vec3(0.349, 0.686, 0.168));
    float b = dot(color.rgb, vec3(0.272, 0.534, 0.131));
    gl_FragColor = vec4(clamp(vec3(r, g, b), 0.0, 1.0), color.a);
}