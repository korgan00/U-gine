
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 normalMat;
attribute vec3 vpos;
attribute vec3 vnormal;
attribute vec2 vuv;
varying vec2 f_uv;
varying vec3 f_normal;
varying vec4 f_mvpos;

void main() {
	gl_Position = mvp * vec4(vpos, 1);
	f_uv = vuv;
	f_normal = (normalMat * vec4(vnormal, 1.0)).xyz;
	f_mvpos = mv * vec4(vpos, 1);
}