uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114)); // NTSC grayscale formula
    gl_FragColor = vec4(vec3(gray), color.a);
}