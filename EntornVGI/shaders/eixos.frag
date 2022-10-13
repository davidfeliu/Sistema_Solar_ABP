//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interficie MFC/GLFW amb Status Bar
//******** Enric Marti (Març 2022)
// eixos.frag: Fragment Program en GLSL en versió OpenGL 3.3 o 4.00 per a dibuixar eixos Coordenades Món

#version 330 core
//#version 400 core

// ---- L9- Variable in
in vec4 VertexColor;

// ---- L12- Variable in
out vec4 FragColor;

void main ()	// --- L15
{
// ---- L17- Intensitat sense textura
    FragColor = VertexColor; 
}
