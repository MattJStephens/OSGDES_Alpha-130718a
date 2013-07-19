
varying vec3 rgb;
varying vec3 Normal;

void main()
{
	vec3 DiffuseColor = vec3(0.75, 0.25,0.25);
	vec3 PhongColor = vec3(0.75,0.75,0.0);
	float Edge = 0.64;
	float Phong = 0.8;
	

	vec3 color = DiffuseColor;
	float f = dot(vec3(0,0,1),Normal);
	if (abs(f) < Edge)
		color = vec3(0);
	if (f > Phong)
		color = PhongColor;
		
	gl_FragColor = vec4(rgb*color,1.0);	
}
