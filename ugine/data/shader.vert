
uniform mat4 mvp;
uniform mat4 mv;
uniform mat4 m;
uniform mat4 normalMat;
uniform mat4 depthBiasMatrix;
uniform vec3 eyePos;

uniform bool hasReflectionTex;
uniform bool hasRefractionTex;
uniform float refractionCoef;

uniform bool skinned;
uniform mat4 animMatrices[64];

attribute vec3 vpos;
attribute vec3 vnormal;
attribute vec2 vuv;
attribute vec3 vtangent;
attribute vec4 vboneIndices;
attribute vec4 vboneWeights;

varying vec2 f_uv;
varying vec4 f_shadowUV;
varying vec3 f_normal;
varying vec4 f_mvpos;
varying vec3 f_vertexCoords;
varying mat3 TBN;

void main() {
	vec4 vpos4 = vec4(vpos, 1);

	if (skinned) {
		mat4 boneTransform = mat4(1);
		for (int i = 0; i < 4; ++i) {
			int index = int(vboneIndices[i]);
			if (index > -1) {
				boneTransform += animMatrices[index] * vboneWeights[i];
			}
		}
		vpos4 = boneTransform * vpos4;
	}
	gl_Position = mvp * vpos4;

	f_shadowUV = depthBiasMatrix * m * vpos4;
	f_uv = vuv;
	f_normal = (normalMat * vec4(vnormal, 1.0)).xyz;
	f_mvpos = mv * vpos4;

	vec3 tangent = (normalMat * vec4(vtangent, 1.0)).xyz;
	vec3 bitan = cross(f_normal, tangent);
	TBN = transpose(mat3(tangent, bitan, f_normal));

	vec3 eye = normalize( vec3(m * vpos4) - eyePos);
	vec3 normal = vec3(m * vec4(vnormal, 0));
	if (hasReflectionTex) {
		f_vertexCoords = reflect(eye, normal);
	} else if (hasRefractionTex) {
		f_vertexCoords = refract(eye, normal, refractionCoef);
	} else {
		f_vertexCoords = vpos4.xyz;
	}
}