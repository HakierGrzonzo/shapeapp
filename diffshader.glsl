uniform sampler2D current;
uniform sampler2D goal;

void main() {
  vec4 currentPixel = texture2D(current, gl_TexCoord[0].xy);
  vec3 goalPixel = texture2D(goal, gl_TexCoord[0].xy).rgb;
  vec3 diff = abs(currentPixel.rgb - goalPixel);
  if (currentPixel.a < 0.1) {
    diff.r = 1.0;
  } else {
    diff.r = (diff.r + diff.g + diff.b) / 3.0;
  }
  gl_FragColor = gl_Color * vec4(diff, 1.0);
}
