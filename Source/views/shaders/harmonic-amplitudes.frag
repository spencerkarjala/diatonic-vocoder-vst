#version 330 core

in vec4 fragColor;

uniform vec2 windowBounds;
uniform float pointPositions[24];
uniform float radius;
uniform int hoveredSlider;

out vec4 color;

void main() {
  int pointIndex = 2 * int(gl_FragCoord.x / (windowBounds.x / 12));
  vec2 pointPosition = vec2(pointPositions[pointIndex], pointPositions[pointIndex + 1]);
  vec2 pointPositionScaled = (pointPosition + 1.0) / 2.0 * windowBounds;

  float dist = distance(pointPositionScaled, gl_FragCoord.xy);

  float yAdj = (pointPosition.y + 1) / 2;
  float yCurr = gl_FragCoord.y / windowBounds.y;
  float xMin = -1;
  float xMax = (pointPositions[0] + pointPositions[2]) / 2;
  float xCurr = 6*gl_FragCoord.x / windowBounds.x;

  if (dist > radius) {
    if (yCurr < yAdj + 0.005 && yCurr > yAdj - 0.005) {
      color = vec4(1.0, 1.0, 1.0, 0.3);
    }
    else {
      color = fragColor;
    }
  }
  else {
    if (hoveredSlider == pointIndex / 2) {
      color = vec4(1.0, 1.0, 1.0, 0.4);
    }
    else {
      color = vec4(1.0, 1.0, 1.0, 0.2);
    }
  }
}
