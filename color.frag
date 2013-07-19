uniform float vm;
uniform float vmax;
uniform float vmin;
void main()
{
float mx = vmax;
float mn = vmin;
float max3=(mx-mn)/3.0;
vec3 rgb;

if(vm>=mx)
{
    rgb.x=rgb.y=rgb.z=1.0;
}
else if(vm<=vmin)
{
    {rgb.x=rgb.y=rgb.z=0.1;}
}
else if(vm<max3)
{
    rgb.x=(vm/max3); rgb.y=0.0 ; rgb.z=0.0;
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

	gl_FragColor = vec4(rgb,1.0);	
}
