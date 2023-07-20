#version 300 es
precision mediump float;

out vec4 FragColor;
uniform vec2 vTextureCoord;

uniform vec2 uTextureSize;

void main() {
    vec2 texCoord = vTextureCoord * uTextureSize;
    
    // Define the boundary size (in pixels)
    float boundarySize = 1.2;
    
    // Check if the current fragment is on the boundary
    if (texCoord.x < boundarySize && texCoord.y < boundarySize) {
        FragColor = vec4(0.0, 0.0, 0.5, 1.0); // Set color to black
    } else {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Use texture color
    }
}