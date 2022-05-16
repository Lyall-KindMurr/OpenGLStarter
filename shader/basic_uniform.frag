#version 460

//in vec3 LightIntensity;
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D Tex1;


uniform struct Foginfo {
    float MaxDist;
    float MinDist;
    vec3 Color;
}Fog;

uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float shininess;
}material;

uniform struct LightInfo {
    vec3 La; // ambient intensity
    vec3 L; // Diffuse and specular light intensity
    vec4 LightPosition;
}lights;

uniform struct SpotlightInfo {
    vec3 Position; // Position in cam coords
    vec3 L; // Diffuse/spec intensity
    vec3 La; // Amb intensity
    vec3 Direction; // Direction of the spotlight in cam coords.
    float Exponent; // Angular attenuation exponent
    float Cutoff; // Cutoff angle (between 0 and pi/2)
}Spot;



vec3 phongModel(vec3 position, vec3 n)
{
    vec3 _ambient = lights.La * material.Ka;

    vec3 _s = normalize(lights.LightPosition.xyz);

    // calculate diffuse
    float _sDotN = max(dot(_s,n), 0.0 );
    vec3 _diffuse = material.Kd  * _sDotN;

    //calculate specular
    vec3 _spec = vec3(0.0);

    if(_sDotN > 0.0)
    {
        vec3 _v = normalize(-position.xyz);
        vec3 _r = reflect(-_s,n);
        _spec = material.Ks * pow(max(dot(_r,_v),0.0), material.shininess);
    }

    return _ambient + lights.L * (_diffuse + _spec);
}

vec3 blinnPhong(vec3 position, vec3 n)
{
    vec3 texColor = texture(Tex1,TexCoord).rgb;
    vec3 _ambient = lights.La * texColor;

    vec3 _s = normalize(lights.LightPosition.xyz);

    // calculate diffuse
    float _sDotN = max(dot(_s,n), 0.0 );
    vec3 _diffuse = material.Kd  * _sDotN;

    //calculate specular
    vec3 _spec = vec3(0.0);

    if(_sDotN > 0.0)
    {
        vec3 _v = normalize(-position.xyz);
        vec3 _h = normalize(_v + _s);
        _spec = material.Ks * pow(max(dot(_h,_v),0.0), material.shininess);
    }

    return _ambient + lights.L * (_diffuse + _spec);
}

vec3 blinnPhongSpot(vec3 position, vec3 n)
{
    vec3 _spec = vec3(0.0);
    vec3 _diffuse = vec3(0.0f);
    vec3 _ambient = lights.La * material.Ka;

    vec3 _s = normalize(lights.LightPosition.xyz);

    float cosAng = dot(-_s, normalize(Spot.Direction));
    float angle = acos(cosAng); //returns an actual angle

    float spotScale = 0.0;

    if(angle < Spot.Cutoff)
    {
        spotScale = pow (cosAng, Spot.Exponent);
        float _sDotN = max(dot(_s,n), 0.0 );
        _diffuse = material.Kd  * _sDotN;        

        if(_sDotN > 0.0)
        {
            vec3 _v = normalize(-position.xyz);
            vec3 _h = normalize(_v + _s);
            _spec = material.Ks * pow(max(dot(_h,_v),0.0), material.shininess);
        }
    }
    return _ambient + lights.L * (_diffuse + _spec);
}

void main() {
    float dist = abs( Position.z ); //distance calculations

    //fogFactor calculation based on the formula presented earlier
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);

    fogFactor = clamp( fogFactor, 0.0, 1.0 ); //we clamp values

    //colour we receive from blinnPhong calculation
    vec3 shadeColor = blinnPhong(Position, normalize(Normal));

    //we assign a colour based on the fogFactor using mix
    vec3 color = mix( Fog.Color, shadeColor, fogFactor );


    FragColor = vec4(color, 1.0); //final colour
}
