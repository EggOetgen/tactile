// fragment shader

#version 150

//unforms from openframeworks
uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

uniform vec2 f1;
uniform vec2 f2;
uniform vec2 f3;
uniform vec2 f4;
uniform vec2 f5;

uniform vec2 ires;

out vec4 outputColor;

//a pseudorandom function Mick Grierson's graphics labs. At high frequencies and amplitudes, pseudorandom patterns begin to appear 
float randfm(float x, float freq, float amp){

	return sin((cos(x*freq)*amp));

}

void main()
{

	//normalized coordinate of the current fragment
	vec2 normcoord = gl_FragCoord.xy/resolution;

	//array to contain normalized finger positions
	vec2 fingers[5];

	fingers[0]=f1/ires;
	fingers[1]=f2/ires;
	fingers[2]=f3/ires;
	fingers[3]=f4/ires;
	fingers[4]=f5/ires;
	
	
	//array of colours for each finger
	float cols[5];

	
	for(int i=0; i<5; i++){
		
		//creating a distance field for each finger
		cols[i]=distance(fingers[i],normcoord);
	
		//smoothstepping the distance field with the input value for the interpolation being a randfm function with a distancefield from the center of the screen to the current fragment
		cols[i]=smoothstep(cols[i],cols[i]+.8,0.25+randfm(distance(normcoord,vec2(0.5,0.5)),5.,1.)*0.05);
	}

	//variable to contain sum of all finger colours
	float col;

	col = cols[0]+cols[1]+cols[2]+cols[3]+cols[4];

	//smoothstepping this output with another smoothstep containing a randfm as the input for the interpolation. The amplitude of this increases and decreases with the sine of the time
	col = smoothstep(col,col-1.,randfm(col,2.,sin(time*0.0005)*100.+3.));
 
	mat4 rot = mat4(cos(time*.0005),sin(time*.0005),0.,0.,-sin(time*.0005),cos(time*.0005),0.,0.,0.,0.,1.,0.,0.,0.,0.,1.);//rotation matrix to apply to the final colour. doesnt really rotate through all colours but has an interesting effect
	mat4 trans = mat4(1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.5,0.5,0.5,0.5,1.);//translation matrix to push the colours a bit
	
    outputColor = vec4(col, col, col, 1.)*rot*trans;//multiplying the final vec4 by rot and trans and outputting it

}