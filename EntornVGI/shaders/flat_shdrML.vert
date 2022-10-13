//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (EE-UAB)
//******** Entorn basic amb interficie MFC/GLFW amb Status Bar
//******** Enric Marti (Maig 2022)
// flat_shdrML.vert: Vertex Program en GLSL en versió OpenGL 3.3 o 4.00 per a implementar:
//     a) Iluminació plana
//     b) Fonts de llum puntuals o direccionals
//     c) Fonts de llum restringides
//     d) Atenuació de fonts de llum
//     e) Modus d'assignació de textura MODULATE o DECAL

#version 330 core
//#version 400 core

#define MaxLights 8

// --- L16- Estructures Font de LLum i Material (coeficients reflectivitat).
struct Light
{	bool sw_light;		// Booleana si font encesa (TRUE) o apagada (FALSE).
	vec4 position;		// Posició de la font de llum (x,y,z,w).
	vec4 ambient;		// Intensitat ambient de la font de llum (r,g,b,a).
	vec4 diffuse;		// Intensitat difusa de la font de llum (r,g,b,a).
	vec4 specular;		// Intensitat especular de la font de llum (r,g,b,a).
	vec3 attenuation;	// Vector coeficients atenuació per distància (a,b,c).
	bool restricted;	// Booleana si font de llum restringida
	vec3 spotDirection;	// Vector de direcció de la font restringida (Coord. Món).
	float spotCosCutoff;	// Angle d'obertura de la font restringids (en graus). 
	float spotExponent;	// Exponent per al càlcul intenstitat font restringida segons model Warn.
};

struct Material
{	vec4 emission;		// Coeficients reflexió llum emissiva (r,g,b,a).
	vec4 ambient;		// Coeficients reflexió llum ambient (r,g,b,a).
        vec4 diffuse;		// Coeficients reflexió llum difusa (r,g,b,a).
	vec4 specular;		// Coeficients reflexió llum especular (r,g,b,a).
	float shininess;	// Exponent reflexió llum especular (n).
};

// --- L38- Variables in
layout (location = 0) in vec3 in_Vertex; 	// Coordenades (x,y,z) posicio Vertex.
layout (location = 1) in vec3 in_Normal; 	// Coordenades (x,y,z) Vector Normal.
layout (location = 2) in vec2 in_TexCoord; 	// Coordenades (s,t) Coordenada textura.
layout (location = 3) in vec4 in_Color; 	// Coordenades (r,g,b,a) Color.

// --- L44- Variables uniform
uniform mat4 normalMatrix;	// “the transpose of the inverse of the ModelViewMatrix.” 
uniform mat4 projectionMatrix;	// Projection Matrix.
uniform mat4 viewMatrix; 	// View Matrix.
uniform mat4 modelMatrix;	// Model Matrix.

uniform sampler2D texture0;	// Imatge textura
uniform bool textur;		// Booleana d’activació (TRUE) de textures o no (FALSE).
uniform bool flag_invert_y;	// Booleana que activa la inversió coordenada textura t (o Y) a 1.0-cty segons llibreria SOIL (TRUE) o no (FALSE).
uniform bool fixedLight;	// Booleana que defineix la font de llum fixe en Coordenades Món (TRUE) o no (FALSE).
uniform bool sw_material;	// Booleana que indica si el color del vèrtex ve del Material emission, ambient, diffue, specular (TRUE) o del vector de color del vèrtex in_Color (FALSE)
uniform bvec4 sw_intensity;	// Filtre per a cada tipus de reflexió: Emissiva (sw_intensity[0]), Ambient (sw_intensity[1]), Difusa (sw_intensity[2]) o Especular (sw_intensity[2]).
uniform vec4 LightModelAmbient;	// Intensitat de llum ambient (r,g,b,a)-
uniform Light LightSource[MaxLights];	// Vector de fonts de llum.
uniform Material material;	// Vector de coeficients reflectivitat de materials.

// --- L60- Variables out
flat out vec4 VertexColor;	// Color del vèrtex
out vec2 VertexTexCoord;	// Coordenades textura del vèrtex

void main ()	// --- L64
{
// --- L66- Calcul variables previes.
    //mat4 NormalMatrix = transpose(inverse(viewMatrix * modelMatrix));
    vec3 N = vec3(normalMatrix * vec4(in_Normal,1.0));
    N = normalize(N);
    vec3 vertexPV = vec3(viewMatrix * modelMatrix * vec4(in_Vertex,1.0));
    vec3 V = normalize(-vertexPV);

// --- L73- Multiples llums
    vec3 ILlums = vec3 (0.0,0.0,0.0);	// Intensitat acumulada per a totes les fonts de llum.
    vec3 Idiffuse = vec3 (0.0,0.0,0.0);	// Intensitat difusa d’una font de llum.
    vec3 Ispecular = vec3 (0.0,0.0,0.0);	// Intensitat especular d’una font de llum.
    vec4 lightPosition = vec4(0.0,0.0,0.0,1.0);	// Posició de la font de llum en coordenades Punt de Vista.
    vec3 L = vec3 (0.0,0.0,0.0);	// Vector L per a càlcul intensitat difusa i especular.
    float fatt=1.0;				// Factor d'atenuació llum per distància (fatt).
						// fatt = 1/Light.attenuation.x*d2 + Light.attenuation.y * d + Light.attenuation.z
    vec3 spotDirectionPV = vec3 (0.0,0.0,0.0);	// Vector spotDirection en coordenades càmera i normalitzat.
    float spotDot; 				// Angle d'obertura en radians entre el vèrtex i el vector de la font restringida (spotDirection).

// --- L84- Textura
    //VertexTexCoord = in_TexCoord;
    if (flag_invert_y) VertexTexCoord = vec2(in_TexCoord.x,1.0-in_TexCoord.y); // SOIL_FLAG_INVERT_Y
      else VertexTexCoord = vec2(in_TexCoord.x,in_TexCoord.y);
 
// --- L89- Compute emissive term
    vec3 Iemissive = vec3 (0.0,0.0,0.0);	// Intensitat emissiva de l’objecte.
    if (sw_intensity[0]) { if (sw_material) Iemissive = material.emission.rgb;
					else Iemissive = in_Color.rgb;
                         }

// --- L95- Compute ambient term
    vec3 Iambient = vec3 (0.0,0.0,0.0);		// Intensitat ambient reflexada
    if (sw_intensity[1]) {	if (sw_material) Iambient = material.ambient.rgb * LightModelAmbient.rgb;
					else Iambient = in_Color.rgb * LightModelAmbient.rgb;
        			Iambient = clamp(Iambient, 0.0, 1.0);
    			}

// --- L102- Multiples llums
    for (int i=0;i<MaxLights;i++) {
	if (LightSource[i].sw_light) { 
		fatt = 1.0; 	// Inicialitzar factor d'atenuació. 
		// --- L106- Compute light position (fixed in WC of attached to camera)
		if (fixedLight) lightPosition = viewMatrix * LightSource[i].position;
			else lightPosition = vec4(-vertexPV,1.0);

		// --- L110- Compute point light source (w=1) or direccional light (w=0)
		if (LightSource[i].position.w == 1) 
		  {  //L = normalize(lightPosition.xyz - vertexPV);
		     L = lightPosition.xyz - vertexPV;
		     // -L114- Compute Attenuation factor
                     fatt = length(L);		// Càlcul de la distància entre la posició de la font de llum i el vèrtex.
		     fatt = 1 / (LightSource[i].attenuation.x * fatt * fatt + LightSource[i].attenuation.y * fatt + LightSource[i].attenuation.z);
		     L = normalize(L);		// Normalitzar el vector.
		  }
                     else L = normalize(lightPosition.xyz);

		// --- L121- Compute Spot Light Factor
		if (LightSource[i].restricted) 
                  { spotDirectionPV = vec3(normalMatrix * vec4(LightSource[i].spotDirection,1.0));
  		    spotDirectionPV = normalize(spotDirectionPV);
		    spotDot = dot(-L,spotDirectionPV);
		    if (spotDot > LightSource[i].spotCosCutoff)
		      { spotDot = pow(spotDot, LightSource[i].spotExponent); // Model de Warn
			fatt = spotDot * fatt;
                      }
		      else fatt=0.0;
                  }

		// --- L133- Compute the diffuse term
		Idiffuse = vec3 (0.0,0.0,0.0);
     		if (sw_intensity[2]) {
        		float diffuseLight = max(dot(L, N), 0.0);
			//Idiffuse = material.diffuse.rgb * in_Color.rgb * LightSource[i].diffuse.rgb * diffuseLight;
			Idiffuse = material.diffuse.rgb * LightSource[i].diffuse.rgb * diffuseLight * fatt;
        		Idiffuse = clamp(Idiffuse, 0.0, 1.0);
     			}

		// --- L142- Compute the specular term
    		Ispecular = vec3 (0.0,0.0,0.0);
    		if (sw_intensity[3]) {
        		//vec3 V = normalize(-vertexPV);
        		vec3 R = normalize(-reflect(L,N));
        		float specularLight = pow(max(dot(R, V), 0.0), material.shininess);
			//Ispecular = material.specular.rgb * in_Color.rgb * LightSource[i].specular.rgb * specularLight;
			Ispecular = material.specular.rgb * LightSource[i].specular.rgb * specularLight * fatt;
        		Ispecular = clamp(Ispecular, 0.0, 1.0);
    			}

   		ILlums += Idiffuse + Ispecular;
		}
   }
    
// --- L157- Transformacio de Visualitzacio amb Matriu Projeccio (projectionMatrix), Matriu Càmera (viewMatrix) i Matriu TG (modelMatrix)
    gl_Position = vec4(projectionMatrix * viewMatrix * modelMatrix * vec4(in_Vertex,1.0));

// --- L160- Calcul intensitat final del vertex
    VertexColor.rgb = Iemissive + Iambient + ILlums;
    VertexColor.a = 1.0;
}