uniform sampler2D texture;
uniform vec2 resolution;
uniform vec2 brush_pos;
uniform vec2 prev_brush_pos;
uniform float brush_size;
uniform vec4 brush_color;
uniform float brush_opacity;
uniform float brush_hardness;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    
    if (brush_color.a == 0.0)
    {
        gl_FragColor = color;
        return;
    }

    vec2 p = gl_TexCoord[0].xy * resolution;
    vec2 a = prev_brush_pos;
    vec2 b = brush_pos;
    
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    float dist = length(pa - ba * h);
    
    float opacity = (1.0 - smoothstep(brush_size * (brush_hardness), brush_size, dist)) * brush_opacity;
    
    gl_FragColor = mix(color, brush_color, opacity * brush_color.a);
}