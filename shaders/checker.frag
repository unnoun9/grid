uniform float sq_size;

void main()
{
    vec2 position = gl_FragCoord.xy / sq_size;
    float result = mod(floor(position.x) + floor(position.y), 2.0);
    gl_FragColor = vec4(vec3(result * 0.1), 1.0);
}

/*
...###...###...
...###...###...
###...###...###
###...###...###
...###...###...
...###...###...
###...###...###
###...###...###
*/