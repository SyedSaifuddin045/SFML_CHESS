uniform sampler2D texture;
uniform float redIncrease;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Increase the red channel value
    float modifiedRed = clamp(color.r + redIncrease, 0.0, 1.0);
    vec3 modifiedColor = vec3(modifiedRed, color.g, color.b);

    gl_FragColor = vec4(modifiedColor, color.a * alpha);
}
