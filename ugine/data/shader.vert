
uniform mat4 mvp;
attribute vec3 vpos;
attribute vec2 v_uv;
varying vec2 f_uv;

void main() {
	gl_Position = mvp * vec4(vpos, 1);
	f_uv = v_uv;
}