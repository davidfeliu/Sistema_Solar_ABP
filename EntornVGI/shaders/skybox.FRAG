//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interficie MFC/GLFW amb Status Bar
//******** Enric Marti (Maig 2022)
// skybox.frag: Fragment Program en GLSL en versió OpenGL 3.3 o 4.00 per a implementar Skybox

#version 330 core
//#version 400 core

// ---- L9- Variables in, uniform i out
in vec3 TexCoords;
uniform samplerCube skybox;
out vec4 FragColor;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}