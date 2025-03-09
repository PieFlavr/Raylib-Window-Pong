#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform mat4 view;
uniform mat4 projection;
uniform float screenWidth;
uniform float screenHeight;

float sphere(vec3 ro, float r) {
    return length(ro) - r;  // Signed Distance Function (SDF) for a sphere
}

void main() {
    // Generate ray direction from the pixel coordinates
    vec2 screenPos = TexCoords * 2.0 - 1.0;  // Map [0,1] to [-1,1]
    screenPos.x *= screenWidth / screenHeight;  // Aspect ratio correction
    vec4 rayDir4 = inverse(projection * view) * vec4(screenPos, -1.0, 1.0);
    vec3 rayDir = normalize(rayDir4.xyz);

    // Raymarching loop
    float t = 0.0;
    const int maxSteps = 100;  // Max raymarch steps
    const float minDist = 0.01;  // Minimum distance to stop marching
    vec3 color = vec3(0.0);  // Background color

    // Raymarching process
    for (int i = 0; i < maxSteps; i++) {
        // Procedurally generate spheres in a sequence
        float dist = sphere(cameraPos + rayDir * t, 1.0); // Base sphere
        // Adding more spheres using a sine wave function for variation
        for (int j = 1; j < 10; j++) {  // 10 spheres
            float offset = sin(float(j) * 0.5 + t * 0.5) * 5.0;  // Sine wave for variation
            dist = min(dist, sphere(cameraPos + rayDir * (t + offset), 1.0));
            color = vec3(float(j) * 0.1, float(j) * 0.05, 1.0 - float(j) * 0.05);
        }
        
        if (dist < minDist) {
            color = vec3(1.0, 0.0, 0.0);  // Red color for intersection
            break;
        }
        t += dist;  // Move the ray forward
    }

    FragColor = vec4(color, 1.0);  // Set the final color
}
