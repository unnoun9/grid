uniform sampler2D texture;
uniform float angle;

void main()
{
    vec2 tex_coords = gl_TexCoord[0].st;
    vec2 center = vec2(0.5, 0.5);
    vec2 rotated_coords = vec2(
        cos(angle) * (tex_coords.x - center.x) - sin(angle) * (tex_coords.y - center.y) + center.x,
        sin(angle) * (tex_coords.x - center.x) + cos(angle) * (tex_coords.y - center.y) + center.y
    );
    vec4 color = texture2D(texture, rotated_coords);
    gl_FragColor = color;
}
