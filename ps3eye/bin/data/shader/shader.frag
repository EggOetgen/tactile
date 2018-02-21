// fragment shader

#version 150

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

float randfm(float x, float freq, float amp){

	return sin((cos(x*freq)*amp));

}

void main()
{

	vec2 normcoord = gl_FragCoord.xy/resolution;
	
	vec2 fingers[5];

	fingers[0]=f1/ires;
	fingers[1]=f2/ires;
	fingers[2]=f3/ires;
	fingers[3]=f4/ires;
	fingers[4]=f5/ires;
	
	
	float cols[5];
	
	for(int i=0; i<5; i++){
	
		cols[i]=distance(fingers[i],normcoord);
		
		cols[i]=smoothstep(cols[i],cols[i]+.8,0.25+randfm(distance(normcoord,vec2(0.5,0.5)),5.,1.)*0.05);
	}
	
	float col;
	
	col = cols[0]+cols[1]+cols[2]+cols[3]+cols[4];
	
	col = smoothstep(col,col-1.,randfm(col,2.,sin(time*0.0005)*100.+3.));
 
	mat4 rot = mat4(cos(time*.0005),sin(time*.0005),0.,0.,-sin(time*.0005),cos(time*.0005),0.,0.,0.,0.,1.,0.,0.,0.,0.,1.);
	mat4 trans = mat4(1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.5,0.5,0.5,0.5,1.);
	
    outputColor = vec4(col, col, col, 1.)*rot*trans;

}