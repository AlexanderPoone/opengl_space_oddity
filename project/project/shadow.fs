#version 430

// Ouputdata
out layout(location=0) float fragmentdepth;
void main() {
	// Not really needed, OpenGL does it anyway
	fragmentdepth=gl_FragCoord.y;
}