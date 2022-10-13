//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interficie MFC/GLFW amb Status Bar
//******** Enric Marti (Maig 2022)
// skybox.vert: Vertex Program en GLSL en versió OpenGL 3.3 o 4.00 per a implementar Skybox

#version 330 core
//#version 400 core

// ---- L9- Variable
layout (location = 0) in vec3 in_Vertex;

// ---- L12- Variables uniform
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// ---- L17- Variables out
out vec3 TexCoords;

void main()
{
    TexCoords = in_Vertex;
    vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Vertex, 1.0);
    gl_Position = pos.xyww;
}  