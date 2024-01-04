uniform sampler2D target;
uniform vec2 position;
uniform vec2 size;
uniform float opacity;

void main() {
  vec3 color = texture2D(target, position / size).rgb;
  gl_FragColor = gl_Color * vec4(color, min(max(0.1, opacity), 1.0));
}
