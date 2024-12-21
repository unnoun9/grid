uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha;        // alpha value for blending

float get_saturation(vec3 color)
{
    float max_val = max(max(color.r, color.g), color.b);
    float min_val = min(min(color.r, color.g), color.b);
    return max_val - min_val;
}

vec3 set_saturation(vec3 color, float saturation)
{
    float gray = dot(color, vec3(0.299, 0.587, 0.114));
    return mix(vec3(gray), color, saturation);
}

vec3 rgb2hsl(vec3 c)
{
    float max_val = max(max(c.r, c.g), c.b);
    float min_val = min(min(c.r, c.g), c.b);
    float h, s, l = (max_val + min_val) / 2.0;

    if (max_val == min_val)
    {
        h = s = 0.0;
    }
    else
    {
        float d = max_val - min_val;
        s = l > 0.5 ? d / (2.0 - max_val - min_val) : d / (max_val + min_val);

        if (max_val == c.r)
        {
            h = (c.g - c.b) / d + (c.g < c.b ? 6.0 : 0.0);
        }
        else if (max_val == c.g)
        {
            h = (c.b - c.r) / d + 2.0;
        }
        else
        {
            h = (c.r - c.g) / d + 4.0;
        }
        h /= 6.0;
    }
    return vec3(h, s, l);
}

float hue2rgb(float p, float q, float t)
{
    if (t < 0.0) t += 1.0;
    if (t > 1.0) t -= 1.0;
    if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
    if (t < 1.0 / 2.0) return q;
    if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    return p;
}

vec3 hsl2rgb(vec3 c)
{
    if (c.y == 0.0) return vec3(c.z);

    float q = c.z < 0.5 ? c.z * (1.0 + c.y) : c.z + c.y - c.z * c.y;
    float p = 2.0 * c.z - q;

    return vec3(
        hue2rgb(p, q, c.x + 1.0 / 3.0),
        hue2rgb(p, q, c.x),
        hue2rgb(p, q, c.x - 1.0 / 3.0)
    );
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

    vec3 hsl1 = rgb2hsl(color1.rgb);
    vec3 hsl2 = rgb2hsl(color2.rgb);

    // take saturation from top layer, keep hue and luminance from bottom
    vec3 blend = hsl2rgb(vec3(hsl1.x, hsl2.y, hsl1.z));

    vec4 blended_color = vec4(blend, color1.a);
    gl_FragColor = mix(color1, blended_color, alpha);
}
