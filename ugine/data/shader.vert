attribute vec3 vpos;
uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(vpos, 1);
}