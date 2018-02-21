// vertex shader

#version 150

//unforms from openframeworks to send to frag shader
uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

uniform vec2 f1;
uniform vec2 f2;
uniform vec2 f3;
uniform vec2 f4;
uniform vec2 f5;

uniform vec2 ires;




//this is automatically sent from openframeworks
uniform mat4 modelViewProjectionMatrix;
in vec4 position;

void main(){
    gl_Position = modelViewProjectionMatrix * position;
}