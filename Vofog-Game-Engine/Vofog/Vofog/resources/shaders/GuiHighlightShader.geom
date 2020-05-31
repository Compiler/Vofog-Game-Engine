#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 20) out;

//in  vec3 oPos[];
in  vec4 oNormal[];
in  mat3 oTBN[];


out vec4 geo_oColor;

void createSides(float s, int i){
	gl_Position = gl_in[i].gl_Position + vec4(s, s, 0.0, 0.0); 
    EmitVertex();


    gl_Position = gl_in[i].gl_Position + vec4(-s, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[i].gl_Position + vec4( 0.0, s, 0.0, 0.0);
    EmitVertex();
    
	gl_Position = gl_in[i].gl_Position + vec4( 0.0, -s, 0.0, 0.0);
    EmitVertex();

	gl_Position = gl_in[i].gl_Position + vec4( -s, 0.0, 0.0, 0.0);
    EmitVertex();
	
	gl_Position = gl_in[i].gl_Position + vec4( s, 0.0, 0.0, 0.0);
    EmitVertex();

	gl_Position = gl_in[i].gl_Position + vec4( 0.0, s, 0.0, 0.0);
    EmitVertex();

	gl_Position = gl_in[i].gl_Position + vec4( 0.0, -s, 0.0, 0.0);
    EmitVertex();

	gl_Position = gl_in[i].gl_Position + vec4( s, 0.0, 0.0, 0.0);
    EmitVertex();
}

void point(int i, float mag){
	geo_oColor = vec4((oTBN[i])[2].xyz, 1);
	gl_Position = gl_in[i].gl_Position;
	EmitVertex();

	geo_oColor = vec4((oTBN[i])[2].xyz, 1);
	gl_Position = gl_in[i].gl_Position + vec4(oNormal[i] * mag);
	EmitVertex();

    EndPrimitive();

}

void main() {    
	float s = 0.0125*3;
	//geo_oPos = oPos[0];
	//geo_oNormal = oNormal[0];
	int i =1;
	//createSides(s, 0);
	//createSides(s, 1);
	//createSides(s, 2);
	float mag = 0.05;
	point(0, mag);
	point(1, mag);
	point(2, mag);

}  