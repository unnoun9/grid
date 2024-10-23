uniform float square_size;
uniform float zoom_factor;
uniform vec2 canvas_size;

void main()
{
    float actual_square_size = min(max(square_size * zoom_factor * 4.0, 8), min(canvas_size.x * 0.05, canvas_size.y * 0.05));
    vec2 position = gl_FragCoord.xy / actual_square_size;
    float result = mod(floor(position.x) + floor(position.y), 2.0);
    gl_FragColor = vec4(vec3(result * 0.05), 1.0);
}