uniform sampler2D target;
uniform vec2 position;
uniform vec2 size;
uniform float opacity;

vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  // taken from https://github.com/Experience-Monks/glsl-fast-gaussian-blur/blob/master/13.glsl
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}


void main() {
  vec2 uv = position / size;
  vec3 color = blur13(target, uv, size, position / length(position)).rgb;
  gl_FragColor = gl_Color * vec4(color, min(max(0.1, opacity), 1.0));
}
