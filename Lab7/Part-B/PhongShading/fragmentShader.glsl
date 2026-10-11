#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    // normalize the interpolated normal
    vec3 N = normalize(Normal);

    // Direction toward the light
    vec3 L = normalize(lightPos - FragPos);

    // direction toward the viewer
    vec3 V = normalize(viewPos - FragPos);

    // ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * vec3(1.0);

    // diffuse lighting
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // Specular lighting
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);
    vec3 specular = 0.5 * spec * vec3(1.0);

    // blue color
    vec3 objectColor = vec3(0.2, 0.5, 1.0);

    // Combine lighting components
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}