#version 330 core
in vec3 color;
in vec2 tex_coord;
in vec3 pos;
out vec4 FragColor;
int iterations = 1000;

uniform float time;
uniform mat4 transform;
uniform sampler1D palette;

float mod_sqrd(vec2 v){
  return v.x*v.x + v.y*v.y;
}

vec2 f(int n, vec2 c){
  //f(0, c) = 0,
  //f(1, c) = c,
  //f(2, c) = c²+c...
  vec2 result = vec2(0.0);
  for(int i = 0; i < n; i++)
  {
    result = vec2(result.x*result.x - result.y*result.y, 2*result.x*result.y) + c;
  }
  return result;
}

void main()
{
    //float t = 0.5f*sin(2.0f*time) + 0.5f;
    //vec4 myColor = texture(palette, tex_coord.x); //* vec4(t);
    vec2 c =  pos.xy;//(transform * vec4(pos,1.0)).xy;
    vec4 mandel_color;
    vec2 result = vec2(0.0);
    int i;
    for(i = 0; i < iterations; i++)
    {
      result = vec2(result.x*result.x - result.y*result.y, 2*result.x*result.y) + c;
      if(mod_sqrd(result) > 4.0) break;
    }

    mandel_color = texture1D(palette, (i == iterations) ? 0.0 : float(i)/iterations);


    FragColor =  mandel_color;
}
