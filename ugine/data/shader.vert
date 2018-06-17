
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 m;
uniform mat4 normalMat;
uniform vec3 eyePos;

uniform bool hasReflectionTex;
uniform bool hasRefractionTex;
uniform float refractionCoef;

attribute vec3 vpos;
attribute vec3 vnormal;
attribute vec2 vuv;
attribute vec3 vtangent;

varying vec2 f_uv;
varying vec3 f_normal;
varying vec4 f_mvpos;
varying vec3 f_vertexCoords;
varying mat3 TBN;

void main() {
	gl_Position = mvp * vec4(vpos, 1);
	f_uv = vuv;
	f_normal = (normalMat * vec4(vnormal, 1.0)).xyz;
	f_mvpos = mv * vec4(vpos, 1);
	vec3 tangent = (normalMat * vec4(vtangent, 1.0)).xyz;
	vec3 bitan = cross(f_normal, tangent);
	TBN = transpose(mat3(tangent, bitan, f_normal));

	vec3 eye = normalize( vec3(m * vec4(vpos, 1)) - eyePos);
	vec3 normal = vec3(m * vec4(vnormal, 0));
	if (hasReflectionTex) {
		f_vertexCoords = reflect(eye, normal);
	} else if (hasRefractionTex) {
		f_vertexCoords = refract(eye, normal, refractionCoef);
	} else {
		f_vertexCoords = vpos;
	}
}