uniform int gua_lights_num;
uniform int gua_sun_lights_num;

struct LightSource {
  vec4   position_and_radius; // xyz - position (or direction for sun light), w - radius
  vec4   beam_direction_and_half_angle; //  xyz - direction, w - half angle
  vec4   color;
  float  falloff;
  float  brightness;
  float  softness;
  uint   type;           // 0 - point, 1 - spot, 2 - sun
  bool   diffuse_enable;
  bool   specular_enable;
  bool   casts_shadow;

  float  shadow_offset;
  uvec2  shadow_map;
  uvec2  pad;
};

layout(std140, binding=1) uniform lightBlock {
  LightSource gua_lights[@max_lights_num@];
};