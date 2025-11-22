#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 cameraPos;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

// --- Starfield & Nebula ---
// Pseudo-random number generator
float hash(vec3 p) {
    p = fract(p * 0.3183099 + .1);
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

float noise(vec3 x) {
    vec3 i = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
	
    return mix(mix(mix( hash(i+vec3(0,0,0)), 
                        hash(i+vec3(1,0,0)),f.x),
                   mix( hash(i+vec3(0,1,0)), 
                        hash(i+vec3(1,1,0)),f.x),f.y),
               mix(mix( hash(i+vec3(0,0,1)), 
                        hash(i+vec3(1,0,1)),f.x),
                   mix( hash(i+vec3(0,1,1)), 
                        hash(i+vec3(1,1,1)),f.x),f.y),f.z);
}

vec3 GetNebula(vec3 dir) {
    // Multi-layered noise for nebula clouds
    float n = noise(dir * 3.0);
    n += 0.5 * noise(dir * 6.0);
    n += 0.25 * noise(dir * 12.0);
    n /= 1.75;
    
    // Color mapping: Dark Blue/Purple -> Bright Blue
    vec3 color = mix(vec3(0.05, 0.0, 0.1), vec3(0.1, 0.4, 0.8), pow(n, 3.0));
    return color;
}

vec3 GetStarfield(vec3 dir) {
    // Map direction to a grid
    vec3 p = dir * 150.0; 
    vec3 id = floor(p);
    
    // Hash the grid cell ID to get a random value
    float rnd = hash(id);
    
    // Threshold to decide if a star exists in this cell
    float star = step(0.995, rnd); 
    
    return vec3(star) + GetNebula(dir); // Combine Stars + Nebula
}

// --- General Relativity ---
struct BlackHoleData {
    vec3 pos;
    float rs;
    float diskInner;
    float diskOuter;
};

uniform BlackHoleData uBlackHoles[4];
uniform int uNumBlackHoles;

#define MAX_STEPS 200
#define MAX_DIST 1e10

// Traces a ray through curved spacetime
vec3 TraceGeodesic(vec3 ro, vec3 rd) {
    vec3 p = ro;
    vec3 dir = rd;
    vec3 accumColor = vec3(0.0); // Volumetric color accumulation
    
    float h = 0.1; // Adaptive step size
    
    for(int i=0; i<MAX_STEPS; i++) {
        // Find closest black hole for step size and gravity
        float minR = MAX_DIST;
        vec3 totalForce = vec3(0.0);
        int closestBH = -1;
        
        for(int j=0; j<uNumBlackHoles; j++) {
            vec3 toBH = uBlackHoles[j].pos - p;
            float r = length(toBH);
            
            // Keep track of closest distance for adaptive stepping
            if (r < minR) {
                minR = r;
                closestBH = j;
            }
            
            // Gravity Bending (Sum of forces)
            // Newtonian approximation: F ~ Rs / r^2
            float bendingStrength = 1.5;
            float force = bendingStrength * uBlackHoles[j].rs / (r * r);
            totalForce += normalize(toBH) * force;
        }
        
        // Adaptive Step Size
        h = max(0.05, minR * 0.08);
        
        // Check Event Horizons
        for(int j=0; j<uNumBlackHoles; j++) {
            if(length(uBlackHoles[j].pos - p) < uBlackHoles[j].rs) {
                return accumColor; // Black
            }
        }
        
        // Check Accretion Disks
        for(int j=0; j<uNumBlackHoles; j++) {
            vec3 bhPos = uBlackHoles[j].pos;
            float distToPlane = abs(p.y - bhPos.y);
            float r = length(bhPos - p);
            
            float dInner = uBlackHoles[j].diskInner;
            float dOuter = uBlackHoles[j].diskOuter;
            
            if(distToPlane < 0.1 && r > dInner && r < dOuter) {
                float density = 2.0 * (1.0 - distToPlane/0.1); 
                float temp = (r - dInner) / (dOuter - dInner);
                vec3 diskColor = mix(vec3(1.0, 0.8, 0.5), vec3(0.8, 0.2, 0.1), temp);
                accumColor += diskColor * density * h; 
            }
        }
        
        // Escape Check
        if(minR > 5000.0) {
             return accumColor + GetStarfield(dir);
        }
        
        // Apply Gravity
        dir = normalize(dir + totalForce * h);
        
        // Move Position
        p += dir * h;
        
        // Max Distance Check
        if(length(p - ro) > MAX_DIST) break;
    }
    
    return accumColor + GetStarfield(dir); // Fallback
}

void main()
{
    // 1. Calculate Ray Direction
    vec2 ndc = TexCoords * 2.0 - 1.0;
    vec4 clipCoords = vec4(ndc.x, ndc.y, -1.0, 1.0);
    vec4 eyeCoords = inverse(projection) * clipCoords;
    eyeCoords = vec4(eyeCoords.xy, -1.0, 0.0);
    vec3 rd = normalize(vec3(inverse(view) * eyeCoords));
    vec3 ro = cameraPos;
    
    // 2. Trace Geodesic
    vec3 col = TraceGeodesic(ro, rd);
    
    FragColor = vec4(col, 1.0);
}
