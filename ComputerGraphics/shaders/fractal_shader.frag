#version 410 core
in vec2 f_uv;

out vec4 color;

uniform float time;

vec3 colorPalette(float t) {
	//[[0.168 0.500 0.758] [0.968 0.188 0.698] [1.048 1.000 1.000] [-0.042 0.258 0.667]]
	vec3 a = vec3(0.168, 0.500, 0.758);
	vec3 b = vec3(0.968, 0.188, 0.698);
	vec3 c = vec3(1.048, 1.000, 1.000);
	vec3 d = vec3(-0.042, 0.258, 0.667);

	return a * b*cos(6.28218*(c*t+d));
}

float sdHexagon(in vec2 p, in float r)
{
    const vec3 k = vec3(-0.866025404,0.5,0.577350269);
    p = abs(p);
    p -= 2.0*min(dot(k.xy,p),0.0)*k.xy;
    p -= vec2(clamp(p.x, -k.z*r, k.z*r), r);
    return length(p)*sign(p.y);
}

float dot2(in vec2 v) { return dot(v,v); }

float sdHeart(in vec2 p)
{
    p.x = abs(p.x);

    if( p.y+p.x>1.0 )
        return sqrt(dot2(p-vec2(0.25,0.75))) - sqrt(2.0)/4.0;
    return sqrt(min(dot2(p-vec2(0.00,1.00)),
                    dot2(p-0.5*max(p.x+p.y,0.0)))) * sign(p.x-p.y);
}

float sdCross(in vec2 p, in vec2 b, float r) 
{
    p = abs(p); p = (p.y>p.x) ? p.yx : p.xy;
    vec2  q = p - b;
    float k = max(q.y,q.x);
    vec2  w = (k>0.0) ? q : vec2(b.y-p.x,-k);
    return sign(k)*length(max(w,0.0)) + r;
}

float direction(float d, float time, int i) {
	if (i % 2 ==  1){
		return  abs(sin(d * 8.0 + time) / 8.0);
	} else {
		return  abs(sin(d * 8.0 - time) / 8.0);
	}
}

void main() {

	vec3 finalColor = vec3(0.0);
	
	vec2 uv = f_uv;
	float circle = length(uv - 0.5);
	vec3 circleColor = colorPalette(circle * 5 + time);
	circle = abs(sin(circle * 8.0 + time) / 8.0);
	circle = 0.02 / circle;
	for (int i = 0; i < 3; i++) {
		 uv = fract(uv * 2.0);
		 float d;
		 if (i == 0) {
			 d = sdCross(uv - 0.5, vec2(0.55, 0.25), 0.5);
		} else if (i ==1) {
			d = sdHexagon(uv - 0.5, 0.5);
		} else {
			d = sdHeart(vec2(uv.x - 0.5, uv.y));
		}
		vec3 c = colorPalette(d * i + time);
		d = direction(d, time, i);
		d = 0.02 / d;

		finalColor += c * d;
	}

	color = vec4(finalColor + circleColor * circle, 1.0);
}