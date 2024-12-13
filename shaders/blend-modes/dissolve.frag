uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha; // alpha value for blending
uniform float seed; // seed for random number generation

float random(vec2 co, float seed)
{
    return fract(sin(dot(co.xy + seed, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);
    float rand = random(gl_FragCoord.xy, seed);
    gl_FragColor = rand < alpha ? color2 : color1;
}
