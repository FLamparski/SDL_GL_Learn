#version 130

in vec3 pos_3d;
uniform vec2 offset;
uniform int window_width;
uniform int window_height;

float PI = 3.141592653589793;

mat4 create_perspective_matrix (in float fov, in float aspect, in float near, in float far) {
  mat4 m = mat4(0.0);

  float angle = (fov / 180.0f) * PI; // convert fov in degrees to radians
  float f = 1 / tan(angle * 0.5f); // cot (angle / 2)

  m[0][0] = f / aspect;
  m[1][1] = f;
  m[2][2] = (far + near) / (near - far);
  m[2][3] = -1.0f;
  m[3][2] = (2.0f * far * near) / (near - far);

  return m;
}

void main() {
  vec4 camera_pos = vec4(pos_3d, 0.0) + vec4(offset.x, offset.y, -1.0, 0.0);
  mat4 mat_perspective = create_perspective_matrix(90.0f, 1.3333f, 0.0f, 10.0f);

  gl_Position = mat_perspective * camera_pos;

  //gl_Position = vec4(pos_3d, 2.0);
}
