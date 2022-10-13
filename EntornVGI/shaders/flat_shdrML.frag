//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interfície MFC/GLFW i Status Bar
//******** Enric Marti (Maig 2022)
// flat_shdrML.frag: Fragment Program en GLSL per en versió OpenGL 3.3 o 4.00 per a implementar:
//     a) Iluminació plana
//     b) Fonts de llum puntuals o direccionals
//     c) Fonts de llum restringides
//     d) Atenuació de fonts de llum
//     e) Modus d'assignació de textura MODULATE o DECAL

#version 330 core
//#version 400 core

// ---- L14- in Variables
flat in vec4 VertexColor;     // Color del vèrtex
in vec2 VertexTexCoord;	 // Coordenades textura del vèrtex

// ---- L18- uniform Variables
uniform sampler2D texture0;	// Imatge textura
uniform bool textur;		// Booleana d’activació (TRUE) de textures o no (FALSE).
uniform bool modulate;		// Booleana d'activació de barreja color textura- color intensitat llum (TRUE) o només color textura (FALSE)

// ---- L23- out Variables
out vec4 FragColor;		// Color fragment (r,g,b,a)

void main ()
{
if (textur) {	// Intensitat amb textura
		vec4 colorT = texture(texture0,VertexTexCoord);
		// Textura modulada amb intensitat llum
	    	if (modulate) FragColor = colorT * VertexColor;
       			else FragColor=colorT; // textura sense modular intensitat llum
    	    }
    else { // Intensitat sense textura
           FragColor = VertexColor;   
         }
}