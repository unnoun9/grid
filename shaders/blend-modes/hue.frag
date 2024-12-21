uniform sampler2D texture1; // underlying layer
uniform sampler2D texture2; // upper layer
uniform float alpha;        // alpha value for blending

vec3 rgb_2_hsl(vec3 color)
{
    float max_value = max(color.r, max(color.g, color.b));
    float min_value = min(color.r, min(color.g, color.b));
    float delta = max_value - min_value;

    float hue = 0.0;
    float saturation = 0.0;
    float lightness = (max_value + min_value) / 2.0;

    if (delta > 0.0)
    {
        saturation = (lightness < 0.5) ? (delta / (max_value + min_value)) : (delta / (2.0 - max_value - min_value));

        if (color.r == max_value)
        {
            hue = (color.g - color.b) / delta + (color.g < color.b ? 6.0 : 0.0);
        }
        else if (color.g == max_value)
        {
            hue = (color.b - color.r) / delta + 2.0;
        }
        else
        {
            hue = (color.r - color.g) / delta + 4.0;
        }

        hue /= 6.0;
    }

    return vec3(hue, saturation, lightness);
}

vec3 hsl_2_rgb(vec3 hsl)
{
    float hue = hsl.x;
    float saturation = hsl.y;
    float lightness = hsl.z;

    float chroma = (1.0 - abs(2.0 * lightness - 1.0)) * saturation;
    float x = chroma * (1.0 - abs(mod(hue * 6.0, 2.0) - 1.0));
    float m = lightness - chroma / 2.0;

    vec3 rgb;

    if (hue < 1.0 / 6.0)
    {
        rgb = vec3(chroma, x, 0.0);
    }
    else if (hue < 2.0 / 6.0)
    {
        rgb = vec3(x, chroma, 0.0);
    }
    else if (hue < 3.0 / 6.0)
    {
        rgb = vec3(0.0, chroma, x);
    }
    else if (hue < 4.0 / 6.0)
    {
        rgb = vec3(0.0, x, chroma);
    }
    else if (hue < 5.0 / 6.0)
    {
        rgb = vec3(x, 0.0, chroma);
    }
    else
    {
        rgb = vec3(chroma, 0.0, x);
    }

    return rgb + vec3(m);
}

void main()
{
    vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
    vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

    vec3 hsl1 = rgb_2_hsl(color1.rgb);
    vec3 hsl2 = rgb_2_hsl(color2.rgb);
    vec3 blended_rgb = hsl_2_rgb(vec3(hsl2.x, hsl1.y, hsl1.z));

    gl_FragColor = vec4(blended_rgb, alpha * color1.a);
}
