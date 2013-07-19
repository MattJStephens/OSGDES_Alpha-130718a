uniform float vm;
uniform float vmax;
uniform float vmin;

varying vec3 Normal;

varying vec3 rgb;

void main()
{
float mx = vmax;
float mn = vmin;
float max3=(mx-mn)/3.0;

if(vm>=mx)
{
    rgb.x=rgb.y=rgb.z=1.0;
}
else if(vm<0.0)
{
    {rgb.x=rgb.y=rgb.z=0.0;}
}
else if(vm<max3)
{
    rgb.x=(vm/max3); rgb.y=0.1 ; rgb.z=0.1;
}
else if(vm<2.0*max3)
{
    rgb.x=1.0; rgb.y = ((vm-max3)/max3); rgb.z=0.0;
}
else if(vm<mx)
{
    rgb.y=1.0;rgb.y = 1.0; rgb.z = ((vm-2.0*max3)/max3);
}
else 
{
	rgb.x=rgb.y=rgb.z=1.0;
}
	vec4 cp = gl_Vertex;
	gl_Position = gl_ModelViewProjectionMatrix * cp;
		Normal = normalize(gl_NormalMatrix * gl_Normal);
	
} 
