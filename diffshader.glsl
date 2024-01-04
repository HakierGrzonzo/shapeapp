uniform sampler2D a;
uniform sampler2D b;

float flatten(vec4 value) {
  return (value.r + value.g + value.b) / 3.0;
}

float score(float shape, float target) {
  if (abs(shape - target) < 0.3) {
      return 0.0;
  }
  return 1.0;
}

void main() {
  vec4 pixelA = texture2D(a, gl_TexCoord[0].xy);
  vec4 pixelB = texture2D(b, gl_TexCoord[0].xy);
  float shape = flatten(pixelA);
  float badApple = flatten(pixelB);
  float value = score(shape, badApple);
  gl_FragColor = gl_Color * vec4(vec3(value), 1.0);
}
