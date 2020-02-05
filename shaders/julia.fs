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

void main()
{
    vec2 c = 0.7885 * vec2(cos(0.25*time),sin(0.25*time));
    c =  (transform * vec4(c,0.0,1.0)).xy;
    float R = (1.0 + sqrt(1+4*length(c)))/2.0;
    vec2 result = pos.xy;
    int i;
    for(i = 0; i < iterations; i++)
    {
      result = vec2(result.x*result.x - result.y*result.y, 2*result.x*result.y) + c;
      if(mod_sqrd(result) > R*R) break;
    }

    FragColor = texture1D(palette, (i == iterations) ? 0.0 : 10.0*float(i)/iterations);
}
