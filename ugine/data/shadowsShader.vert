
uniform mat4 mvp;
attribute vec3 vpos;

void main() {
	gl_Position = mvp * vec4(vpos, 1);
}