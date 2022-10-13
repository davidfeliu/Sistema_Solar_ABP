// llum_Patro.lght: Fitxer patró per a definir paràmetres d'una font de llums
0		// Index de la llum a definir (0-MaxLights).
1		// sw_light: Booleana si llum encesa (0: Apagada (false), 1: Encesa (true)).
200.0 0.0 0.0 	// position: Posició font de llum en coordenades Esfèriques (R,alpha,beta).
1.0 1.0,1.0 	// diffuse: Intensitat Difusa (r,g,b).
1.0 1.0 1.0 	// specular: Intensitat Especular (r,g,b).
0.0 0.0 1.0 	// attenuation: Coeficients Atenuació (a,b,c) --> ax2+by+cz
0		// restricted: Booleana si font de llum restringida (0: No restringida (false), 1: Restringida (true))
0.0 0.0 -1.0	// spotDirection: Vector de llum font restringida.
0.707		// spotCosCutoff: Coseno angle obertura font de llum restringida.
45.0		// spotExponent (Model de Warn).
