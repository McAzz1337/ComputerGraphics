#version 410 core
in vec2 f_uv;
in vec3 f_norm;
flat in int faceIndex;
uniform vec2 resolution;
uniform float time;
uniform int type;
uniform mat3 viewMat;
uniform vec3 cameraPos;
uniform float borderSize = 0.05;

out vec4 color;

const float AMBIENT  = 0.3;
vec3 bg = vec3(0.1, 0.1, 0.3);
vec3 black = vec3(0.0);
vec3 white = vec3(1.0);
vec3 red = vec3(1.0, 0.0, 0.0);
vec3 green = vec3(0.0, 1.0, 0.0);
vec3 blue = vec3(0.0, 0.0, 1.0);
vec3 purple = vec3(0.5, 0.0, 0.8);
vec3 teal = vec3(0.0, 0.8, 1.0);

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

Material materials[6];

void createMaterials() {
    //ruby
    materials[0] = Material(vec3(0.1745, 0.01175, 0.01175), vec3(0.61424, 0.04136, 0.04136), vec3(0.727811, 0.626959, 0.626959), 0.6);
    // copper
    materials[1] = Material(vec3(0.19125, 0.0735, 0.0225), vec3(0.7038, 0.27048, 0.0828), vec3(0.256777, 0.137622, 0.086014), 0.1);
    //obsidian 	
    materials[2] = Material(vec3(0.05375, 0.05, 0.06625), vec3(0.18275, 0.17, 0.22525), vec3(0.332741, 0.328634, 0.346435), 0.3);
    //emerald
   materials[3] = Material(vec3(0.0215, 0.1745, 0.0215), vec3(0.07568, 0.61424, 0.07568), vec3(0.633, 0.727811, 0.633), 0.6);
   //jade
   materials[4] = Material(vec3(0.135, 0.2225, 0.1575), vec3(0.54, 0.89, 0.63), vec3(0.316228, 0.316228, 0.316228),	0.1);
   //silver
   materials[5] = Material(vec3(0.19225, 0.19225, 0.19225), vec3(0.50754, 0.50754, 0.50754), vec3(0.508273, 0.508273, 0.508273), 0.4);
}

Material getMaterial(int index) {
    return materials[index];
}



mat3 rotationY(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat3(
        c,  0.0, -s,
        0.0, 1.0, 0.0,
        s,  0.0,  c
    );
}

mat3 rotationX(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat3 (
        1.0, 0.0, 0.0,
        0.0, c, -s,
        0.0, s, c
    );
}

struct Light {
   vec3 pos; 
};

struct Ray {
    vec3 origin;
    vec3 dir;
};

Ray newRay(vec3 origin, vec2 uv, float depth) {
    vec3 rd = normalize(vec3(vec2(uv), depth) - origin);
    return Ray(origin, rd);
}


Light makeLight(vec3 pos) {
   return Light(pos);
}

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
    int materialIdx;
};

struct Hit {
    bool exists;
    vec3 point;
    float lambda;
    vec3 incidence;
    vec3 norm;
    vec3 color;
    int materialIdx;
};

Hit newHit() {
    return Hit(false, vec3(0.0), 0.0, vec3(0.0), vec3(0.0), vec3(0.0),  -1);
}


Hit hit_sphere(Sphere s, Ray ray) {
    
    vec3 v = ray.origin - s.center;
    float a = dot(ray.dir, ray.dir);
    float b = 2.0 * dot(ray.dir, v);
    float c = dot(v, v) - pow(s.radius, 2.0);

    float d = b * b - 4.0 * a * c;

    if (d >= 0.0) {
        float pos = (-b + sqrt(d)) / (2.0 * a);
        float neg = (-b - sqrt(d)) / (2.0 * a);
        float l = neg < 1e-6 ? pos : neg;
        vec3 q = ray.origin + l * ray.dir;
        vec3 norm = normalize(q - s.center);
        return  Hit(true, q, l, ray.dir, norm, s.color, s.materialIdx);
     }
     return newHit();
}

struct Triangle {
    vec3 a;
    vec3 b;
    vec3 c;
    vec3 color;
    int materialIdx;
};

Hit hit_triangle(Triangle tri, Ray ray) {
    const float EPS = 1e-6;

    vec3 edge1 = tri.b - tri.a;
    vec3 edge2 = tri.c - tri.a;
    vec3 h = cross(ray.dir, edge2);
    float det = dot(edge1, h);

    if (abs(det) < EPS)
        return newHit();

    float invDet = 1.0 / det;
    vec3 s = ray.origin - tri.a;
    float u = dot(s, h) * invDet;
    if (u < 0.0 || u > 1.0)
        return newHit();

    vec3 q = cross(s, edge1);
    float v = dot(ray.dir, q) * invDet;
    if (v < 0.0 || u + v > 1.0)
        return newHit();

    float t = dot(edge2, q) * invDet;
    if (t > EPS) {
        vec3 hitPoint = ray.origin + t * ray.dir;
        vec3 norm = normalize(cross(edge1, edge2));
        return Hit(true, hitPoint, t, ray.dir, norm, tri.color, tri.materialIdx);
    }

    return newHit();
}



bool inShade(Hit hit, Light light, Sphere sphere) {
    vec3 toLight = normalize(light.pos - hit.point);
    Ray r = Ray(hit.point, toLight);

    Hit h = hit_sphere(sphere, r);
    if (h.exists && h.lambda > 0.0) {
	    return true;  
    }

    return false;
}


float shade(Hit hit, Light light){
   vec3 toLight = normalize(light.pos - hit.point);
   return max(0.0,  dot(toLight, hit.norm));
}


float specular(Hit hit, Light light) {
    vec3 toLight = normalize(light.pos - hit.point);
    vec3 viewDir = normalize(-hit.incidence); 
    vec3 reflectDir = reflect(-toLight, hit.norm);
    return  pow(max(dot(viewDir, reflectDir), 0.0), 64.0); // shininess factor
}

vec3 finalColor(Hit hit, Light light) {
    return (AMBIENT + shade(hit, light) + specular(hit, light)) * hit.color; 
}

vec3 computeLighting(Hit hit, Light light, int materialIdx) {
    Material m = getMaterial(materialIdx);
    vec3 toLight = normalize(light.pos - hit.point);
    vec3 viewDir = normalize(-hit.incidence);
    vec3 reflectDir = reflect(-toLight, hit.norm);

    // Components
    float diff = max(dot(hit.norm, toLight), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), m.shininess * 128.0);

    // Combine
    vec3 ambient = AMBIENT * m.ambient;
    vec3 diffuse = m.diffuse * diff;
    vec3 specular = m.specular * spec;

    return ambient + diffuse + specular;
}

vec3 traceSphere(Ray ray, int materialIdx) {
    Sphere s = Sphere(vec3(0.0), 0.5, vec3(1.0, 0.0, 0.0), materialIdx);
    Light light = makeLight(vec3(sin(time), 0.0, cos(time)) * 2.0);
	Hit hit = hit_sphere(s, ray);
	if (hit.exists) {
        return computeLighting(hit, light, hit.materialIdx);
	}
    return bg;
}

vec3 traceTriangle(Ray ray) {
    Triangle t = Triangle(vec3(-0.5, -0.5, 0.0), vec3(0.5, -0.5, 0.0), vec3(0.0, 0.5, 0.0), vec3(0.0, 0.0, 1.0), 1);
    Light light = makeLight(vec3(sin(time), 0.0, cos(time)) * 2.0);

    Hit hit = hit_triangle(t, ray);
    if (hit.exists) {
        return  hit.color;
    }
    return bg;
}

struct Pyramid {
    Triangle tris[6];
    int materialIdx;
};


Hit hit_pyramid(Pyramid p, Ray ray) {
    Hit bestHit = newHit();
    float lambda = 1e9;

    for (int i = 0; i < 6; i++) {
        Hit h = hit_triangle(p.tris[i], ray);
        if (h.exists && h.lambda < lambda) {
            bestHit = h;
            lambda = h.lambda;
        }
    }

    return bestHit;
}

bool triangleInShade(Triangle tris[6], int index, Hit hit, Light light) {
    vec3 toLight = normalize(light.pos - hit.point);
    Ray ray = Ray(hit.point, toLight);
    for (int i = 0; i < 6; i++) {
        if (i == index) continue;
        Hit hit = hit_triangle(tris[i], ray);
        if (hit.exists) {
            return true; 
        }
    }   
    return false;
}


vec3 tracePyramid(Ray ray, int materialIdx) {
    float size = 0.4;
    float angle = time * -0.1 * 22.0 / 7.0;
    mat3 rot = rotationY(radians(45.0) + angle);
    Pyramid p;
    p.materialIdx = materialIdx;
    p.tris[0] = Triangle(rot * vec3(0.0, size, 0.0), rot * vec3(size, -size, -size), rot * vec3(-size, -size, -size), teal, p.materialIdx);
    p.tris[1] = Triangle(rot * vec3(0.0, size, 0.0), rot * vec3(size, -size, size), rot * vec3(size, -size, -size), teal, p.materialIdx);
    p.tris[2] = Triangle(rot * vec3(0.0, size, 0.0), rot * vec3(-size, -size, size), rot * vec3(size, -size, size), teal, p.materialIdx);
    p.tris[3] = Triangle(rot * vec3(0.0, size, 0.0), rot * vec3(-size, -size, -size), rot * vec3(-size, -size, size), teal, p.materialIdx);
    p.tris[4] = Triangle(rot * vec3(-size, -size, size), rot * vec3(size, -size, size), rot * vec3(size, -size, -size),  teal, p.materialIdx),
    p.tris[5] = Triangle(rot * vec3(-size, -size, size), rot * vec3(size, -size, -size), rot * vec3(-size, -size, -size),  teal, p.materialIdx);
    Light light = makeLight(vec3(sin(time) * 2.0, 1.0, cos(time) * 2.0) * 0.4);

    Hit hit = hit_pyramid(p, ray);
    if (hit.exists) {
        return computeLighting(hit, light, hit.materialIdx);
    }
    return bg;
}

struct Diamond {
    Triangle tris[80];
    int materialIdx;
};

Hit hit_diamond(Diamond d, Ray ray) {
    Hit bestHit = newHit();
    float lambda = 1e9;
    int index = -1;

    for (int i = 0; i < 80; i++) {
        Hit h = hit_triangle(d.tris[i], ray);
        if (h.exists && h.lambda < lambda) {
            bestHit = h;
            lambda = h.lambda;
            index = i;
        }
    }
    return bestHit;
}

vec3 traceDiamond(Ray ray, int materialIdx) {
    Diamond diamond;
    diamond.materialIdx = materialIdx;
    int segments = 20;
    float angleStep = 22.0 / 7.0 * 2.0 / segments;
    float r = 0.4;
    float R = 0.5;
    mat3 rot = rotationX(radians(20.0));
    vec3 centerTop = vec3(0.0, 0.4, 0.0);
    vec3 centerBottom = rot * vec3(0.0, -0.4, 0.0);
    for (int i = 0; i < segments; i++) {
       int index = i * 4;
       float cc = cos(i * angleStep);
       float ccc = cos((i + 1) * angleStep);
       vec3 a = rot * vec3(cc * r, 0.4, sin(i * angleStep) * r); 
       vec3 b = rot * vec3(ccc  * r, 0.4, sin((i + 1) * angleStep) * r);
       vec3 c = rot * vec3(ccc * R, 0.3, sin((i + 1) * angleStep) * R) ;
       vec3 d = rot * vec3(cc * R, 0.3, sin(i * angleStep) * R); 
       diamond.tris[index] = Triangle(a, b, c, purple, diamond.materialIdx); 
       diamond.tris[index + 1] = Triangle(a, c, d, purple, diamond.materialIdx);
       diamond.tris[index + 2] = Triangle(d, c, centerBottom, purple, diamond.materialIdx);
       diamond.tris[index + 3] = Triangle(centerTop, b, a, purple, diamond.materialIdx);
    }
    Light light = makeLight(vec3(0.0, cos(time), sin(time)) * 2.0);

    Hit hit = hit_diamond(diamond, ray);
    if (hit.exists) {
        return computeLighting(hit, light, hit.materialIdx);
	}
    return bg;
}

mat3 faceMatrix(int i) {
    if (i == 0) return mat3(vec3(0, 0, 1),  vec3(0, 1, 0), vec3( 1, 0, 0));
    if (i == 1) return mat3(vec3(0, 0, -1), vec3(0, 1, 0), vec3(-1, 0, 0));
    if (i == 2) return mat3(vec3(1, 0,  0), vec3(0, 0, 1), vec3( 0, 1, 0));
    if (i == 3) return mat3(vec3(-1, 0,  0), vec3(0, 0, 1), vec3(0, -1,  0));
    if (i == 4) return mat3(vec3(1, 0,  0), vec3(0, 1, 0), vec3(0, 0, 1) );
    if (i == 5) return mat3(vec3(-1, 0,  0), vec3(0, 1, 0), vec3(0, 0, -1));
    return mat3(            vec3(-2, 0,  0), vec3(0, 1, 0), vec3( 0, 0, -1));
}

bool inRange(float x, float l, float r) {
    return x >= l && x <= r;
}

int onBorder(vec2 uv) {
    float size = (sin(time) * 0.5 + 1.0) * (borderSize * 0.5);
    bool xOnBorder = !inRange(uv.x, size, 1.0 - size);
    bool yOnBorder = !inRange(uv.y, size, 1.0 - size);
  if (xOnBorder) {
    if (yOnBorder)  return 1;  
    else  return 2; 
  } else if (yOnBorder) {
    if (xOnBorder) return 1; 
    else return 2;
  }
  return 0;
}

void main() {
    float aspect = resolution.x / resolution.y;
    vec2 uv = f_uv;
    uv -= 0.5;
    uv.x *= aspect;

    Light light = makeLight(vec3(sin(time), 0.0, cos(time)) * 2.0);
    mat3 faceRot = faceMatrix(faceIndex);
    float dist = (faceIndex == 4 || faceIndex == 2) ? 4.0 : 3.0;
    vec3 origin = viewMat * faceRot *  vec3(0.0, 0.0, -dist);
    Ray ray = newRay(origin, uv, -1.0);
    createMaterials();

    int border = onBorder(f_uv);
    if (border == 1) {
        color =  vec4(white, 1.0);
    } else if (border == 2) {
        color = vec4(black, 1.0);
    } else {
		if (faceIndex == 0) color = vec4(tracePyramid(ray, 4), 1.0);
		else if (faceIndex == 1) color = vec4(tracePyramid(ray, 5), 1.0);
		else if (faceIndex == 2) color = vec4(traceSphere(ray, 1), 1.0);
		else if (faceIndex == 3) color = vec4(traceSphere(ray, 2), 1.0);
		else if (faceIndex == 4) color = vec4(traceDiamond(ray, 0), 1.0);
		else if (faceIndex == 5) color = vec4(traceDiamond(ray, 3), 1.0);
		else color = vec4(1.0);
    }
}