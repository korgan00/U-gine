
struct Light
{
  vec3 color;
  vec4 vector;
  float linearAttenuation;
};

varying vec2 f_uv;
varying vec3 f_normal;
varying vec4 f_mvpos;

uniform bool isTexturized;
uniform sampler2D tex;
uniform vec4 color;

uniform vec3 ambient;
uniform int numLights;
uniform Light lights[8];
uniform int shininess;

void main() {

	vec3 diffuseContrib = numLights > 0? ambient: vec3(0, 0, 0);
	float specularContribFactor = 0.0;

	Light currLight;
	vec3 N = normalize(f_normal), H;
	vec3 L, LtoPos;
	float atten, angle, NdotH;
	
	for(int i = 0; i < numLights; ++i) {
		currLight = lights[i];
		
		atten = 1.0;
		L = currLight.vector.xyz;
		
		if (currLight.vector.w == 1.0) {
			LtoPos = L - f_mvpos;
			atten = 1.0 / (1.0 + currLight.linearAttenuation * length(LtoPos));
		}
		
		L = normalize(L);
		angle = dot(N, L);
		diffuseContrib += max(angle, 0.0) * currLight.color * atten;
		
		if (shininess > 0 && angle > 0.0) {
			H = normalize(L - normalize(f_mvpos));
			NdotH = clamp(dot(N,H.xyz), 0.0, 1.0);
			specularContribFactor += pow(NdotH, shininess) * atten;
		}
	}
	
	vec4 finalColor = vec4(1.0, 1.0, 1.0, 1.0);
	if (isTexturized) {
		finalColor *= texture2D(tex, f_uv);
	}
	finalColor *= color;
	finalColor *= vec4(diffuseContrib, 1.0);
	
	vec4 specularContrib = vec4(specularContribFactor, specularContribFactor, specularContribFactor, 0.0);
	finalColor += specularContrib;

	gl_FragColor = finalColor;
}
