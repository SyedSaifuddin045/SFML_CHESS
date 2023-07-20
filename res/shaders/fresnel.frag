uniform sampler2D textureSampler;
uniform vec2 textureSize;
uniform vec4 edgeColor;

uniform float boundaryWidth; // New uniform for boundary width

void main()
{
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(textureSampler, texCoord);
    
    // Check if the current pixel is transparent
    if (texColor.a == 0.0) {
        discard; // Discard transparent pixels
    }
    
    // Determine the pixel size in texture coordinates
    vec2 pixelSize = 1.0 / textureSize;
    
    // Sample neighboring pixels
    vec4 leftPixel = texture2D(textureSampler, texCoord - vec2(pixelSize.x, 0.0));
    vec4 rightPixel = texture2D(textureSampler, texCoord + vec2(pixelSize.x, 0.0));
    vec4 topPixel = texture2D(textureSampler, texCoord + vec2(0.0, pixelSize.y));
    vec4 bottomPixel = texture2D(textureSampler, texCoord - vec2(0.0, pixelSize.y));
    
    // Calculate the intensity of the edge
    float edgeIntensity = 0.0;
    
    if (texColor != leftPixel) {
        edgeIntensity += 1.0;
    }
    if (texColor != rightPixel) {
        edgeIntensity += 1.0;
    }
    if (texColor != topPixel) {
        edgeIntensity += 1.0;
    }
    if (texColor != bottomPixel) {
        edgeIntensity += 1.0;
    }
    
    // Calculate the threshold based on boundary width
    float threshold = boundaryWidth * 0.5;
    
    // Apply the edge color based on the edge intensity
    vec4 highlightedColor = edgeIntensity > threshold ? edgeColor : texColor;
    
    gl_FragColor = highlightedColor;
}
