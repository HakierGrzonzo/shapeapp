uniform sampler2D target;
uniform vec2 position;
uniform vec2 size;

void main() {
  vec4 color = texture2D(target, position / size);
  gl_FragColor = gl_Color * color;
}
