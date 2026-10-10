#version 330 core

layout(location = 0) in vec3 aPos;
layout(location=1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 vertexColor;

void main()
{
    // Transform the vertex position into world space
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform the normal into world space
    vec3 N = normalize(mat3(transpose(inverse(model))) * aNormal);

    // Calculate light and viewer directions
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(viewPos - FragPos);

    // Ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * vec3(1.0);

    // Diffuse lighting
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // Specular lighting: Phong reflection model
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);
    vec3 specular = 0.5 * spec * vec3(1.0);

    // Blue material color
    vec3 objectColor = vec3(0.2, 0.5, 1.0);

    // Combine lighting components -> calculating for each vertex
    vertexColor = (ambient + diffuse + specular) * objectColor;

    // Calculate final vertex position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}