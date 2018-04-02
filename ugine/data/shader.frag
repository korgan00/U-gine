
varying vec2 f_uv;
uniform bool isTexturized;
uniform sampler2D tex;

void main() {
	if (!isTexturized) {
		gl_FragColor = vec4(1, 1, 1, 1);
	} else {
		gl_FragColor = texture2D(tex, f_uv);
	}
}
