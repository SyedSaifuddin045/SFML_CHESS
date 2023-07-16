// Fragment shader code
uniform vec4 baseColor;
uniform vec4 highlightColor;
uniform float fresnelExponent;

void main()
{
    // Calculate the view direction (normalized)
    vec2 viewDir = normalize(vec2(gl_FragCoord.xy) - vec2(gl_FragCoord.w * 0.5));

    // Calculate the dot product of the view direction and the surface normal
    float fresnel = pow(1.0 - abs(viewDir.y), fresnelExponent);

    // Interpolate between the base color and the highlight color based on the Fresnel factor
    vec4 color = mix(baseColor, highlightColor, fresnel);

    gl_FragColor = color;
}