
struct Light
{
  vec3 color;
  vec4 vector;
  float linearAttenuation;
};

varying vec2 f_uv;
varying vec4 f_shadowUV;
varying vec3 f_normal;
varying vec4 f_mvpos;
varying vec3 f_vertexCoords;
varying mat3 TBN;

uniform bool isTexturized;
uniform sampler2D tex;
uniform bool hasNormalTex;
uniform sampler2D normalTex;

uniform bool useShadows;
uniform sampler2D shadowDepthMap;

uniform bool hasReflectionTex;
uniform samplerCube reflectionTex;
uniform bool hasRefractionTex;
uniform samplerCube refractionTex;

uniform bool isCubeMap;
uniform samplerCube cubeTex;
uniform samplerCube cubeNormalTex;

uniform vec4 color;
uniform int shininess;
uniform float refractionCoef;
uniform vec3 ambient;
uniform int numLights;
uniform Light lights[8];

void main() {
	vec3 uvw = normalize(f_vertexCoords);
	
	vec3 diffuseContrib = numLights > 0? ambient: vec3(1.0, 1.0, 1.0);
	float specularContribFactor = 0.0;

	Light currLight;
	vec3 N = normalize(f_normal);
	if (hasNormalTex) {
		vec3 texNormal = texture2D(normalTex, f_uv).rgb;
		N = normalize(normalize(texNormal * 2.0 - 1.0) * TBN);
	}

	vec3 H, L;
	float atten, angle, NdotH;

	float shadowed = 1.0;
	if (useShadows) {
		if (texture2D(shadowDepthMap, f_shadowUV.xy).z < (f_shadowUV.z - 0.0009)) {
			shadowed = 0.0;
		}
		//occludded = texture2D(shadowDepthMap, f_shadowUV.xy).z < f_shadowUV.z ? 1.0 : 0.0;
	}
	
	for(int i = 0; i < numLights; ++i) {
		currLight = lights[i];
		
		atten = shadowed;
		L = currLight.vector.xyz;
		
		if (currLight.vector.w == 1.0) {
			L = L - f_mvpos.xyz;
			atten = 1.0 / (1.0 + currLight.linearAttenuation * length(L));
		}
		
		L = normalize(L);
		angle = dot(N, L);
		diffuseContrib += max(angle, 0.0) * currLight.color * atten;
		
		if (shininess > 0 && angle > 0.0) {
			H = normalize(L - normalize(f_mvpos.xyz));
			NdotH = clamp(dot(N,H.xyz), 0.0, 1.0);
			specularContribFactor += pow(NdotH, float(shininess)) * atten;
		}
	}
	vec4 specularContrib = vec4(specularContribFactor, specularContribFactor, specularContribFactor, 0.0);
	
	vec4 postLightColor = vec4(1.0, 1.0, 1.0, 1.0);
	if (isTexturized) {
		if(isCubeMap) {
			postLightColor *= textureCube(cubeTex, uvw);
		} else {
			postLightColor *= texture2D(tex, f_uv);
		}
	}

	postLightColor *= color;
	postLightColor *= vec4(diffuseContrib, 1.0);
	postLightColor += specularContrib;

	vec4 reflectionColor = vec4(0.0, 0.0, 0.0, 0.0);
	if (hasReflectionTex) {
		reflectionColor = textureCube(reflectionTex, uvw);
	}

	vec4 refractionColor = vec4(0.0, 0.0, 0.0, 0.0);
	if (hasRefractionTex) {
		refractionColor = textureCube(refractionTex, uvw);
	}
	
	vec3 mixedColor = mix(postLightColor.rgb, reflectionColor.rgb, reflectionColor.a);
	mixedColor = mix(mixedColor.rgb, refractionColor.rgb, refractionColor.a);

	gl_FragColor = vec4(mixedColor, 1.0);
}
