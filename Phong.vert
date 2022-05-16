#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Colour;

uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float shininess;
}material;

uniform struct LightInfo {
    vec3 L; //diffuse and specular intensity
    vec3 La; // ambient intensity
    vec4 LightPosition;
}lights[3];

uniform struct viewInfo {
    mat4 ModelViewMatrix;
    mat3 NormalMatrix;
    mat4 MVP;
} itemView;

vec3 phongModel(int light, vec3 position, vec3 n) 
{
    vec3 _ambient = lights[light].La * 0.2;

    vec4 _vpos = itemView.ModelViewMatrix * vec4(VertexPosition, 1.0);

    vec3 _s;
    if (lights[0].LightPosition.w == 0)
        _s = normalize(lights[0].LightPosition.xyz);
    else
        _s = normalize(vec3(lights[0].LightPosition - _vpos));
    float _sDotN = max(dot(_s,n), 0.0 );
    vec3 _diffuse = material.Kd * _sDotN;

    vec3 _spec = vec3(0.0);
    if( _sDotN > 0.0 ) 
    {
        vec3 v = normalize(-position.xyz);
        vec3 r = reflect( -_s, n );
        _spec = material.Ks * pow( max( dot(r,v), 0.0 ), material.shininess);
    }

    return _ambient + lights[light].L * (_diffuse + _spec);
}

void main()
{
    vec3 _n = normalize(itemView.NormalMatrix * VertexNormal);
    vec4 _vpos = itemView.ModelViewMatrix * vec4(VertexPosition, 1.0);
    Colour = vec3(0.0);
    for( int i = 0; i < 3; i++ )
        Colour += phongModel( i, lights[i].LightPosition.xyz, _n );


    gl_Position = itemView.MVP * vec4(VertexPosition, 1.0);
}