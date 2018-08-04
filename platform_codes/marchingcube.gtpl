#version 450
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(std430, binding = 0) buffer OUTPUT{
    uint total;
};

uniform float width;
uniform float isolevel;
@<uniform>

layout(std430, binding = 1) buffer TABLE{
    int otable[];
};

layout(std430, binding = 2) buffer TEMP{
    vec4 result[];
};

layout(std430, binding = 8) buffer DEBUG{
    uint align[];
};

float func(vec3 pos){
    return @<func>;
}

//those cubes can share vertices

vec4 interp(vec4 v1, vec4 v2){
    return vec4(mix(v1.xyz, v2.xyz, (isolevel - v1.w)/(v2.w - v1.w)), 1.0f);
}

shared int table[256 * 16];
shared vec3 basePos;
shared uint vNum[8 * 8 * 8];
shared uint unitAlign;

void main(){
    if(gl_LocalInvocationIndex == 0){
        basePos = (8 * width) * (vec3(gl_WorkGroupID) - (vec3(gl_NumWorkGroups) - vec3(1.0))/ 2.0f) + @<center>;
        unitAlign = 0;
    }

    for(int i = 0; i < 8; i++){
        table[gl_LocalInvocationIndex * 8 + i] = otable[gl_LocalInvocationIndex * 8 + i];
    }

    barrier();
    memoryBarrierShared();

    uint index = 0;
    vec3 centerPos = (vec3(gl_LocalInvocationID) - (vec3(gl_WorkGroupSize)- vec3(1.0))/ 2.0f) * width + basePos;
    vec4 verts[8];
    vec4 interpPos[13];

    verts[0].xyz = centerPos + vec3(0.5, -0.5, 0.5) * width;
    verts[1].xyz = centerPos + vec3(0.5, 0.5, 0.5) * width;
    verts[2].xyz = centerPos + vec3(0.5, 0.5, -0.5) * width;
    verts[3].xyz = centerPos + vec3(0.5, -0.5, -0.5) * width;
    verts[4].xyz = centerPos + vec3(-0.5, -0.5, 0.5) * width;
    verts[5].xyz = centerPos + vec3(-0.5, 0.5, 0.5) * width;
    verts[6].xyz = centerPos + vec3(-0.5, 0.5, -0.5) * width;
    verts[7].xyz = centerPos + vec3(-0.5, -0.5, -0.5) * width;

    verts[0].w = func(verts[0].xyz);
    verts[1].w = func(verts[1].xyz);
    verts[2].w = func(verts[2].xyz);
    verts[3].w = func(verts[3].xyz);
    verts[4].w = func(verts[4].xyz);
    verts[5].w = func(verts[5].xyz);
    verts[6].w = func(verts[6].xyz);
    verts[7].w = func(verts[7].xyz);

    index |= uint(verts[0].w < isolevel) << 0;
    index |= uint(verts[1].w < isolevel) << 1;
    index |= uint(verts[2].w < isolevel) << 2;
    index |= uint(verts[3].w < isolevel) << 3;
    index |= uint(verts[4].w < isolevel) << 4;
    index |= uint(verts[5].w < isolevel) << 5;
    index |= uint(verts[6].w < isolevel) << 6;
    index |= uint(verts[7].w < isolevel) << 7;

    uint unitNum = 0;
    for(; table[index * 16 + unitNum] != -1; unitNum++) continue;
    vNum[gl_LocalInvocationIndex] = unitNum;
    atomicAdd(unitAlign, unitNum);

    barrier();
    memoryBarrierShared();

    uint uid = gl_WorkGroupID.x + gl_NumWorkGroups.x * gl_WorkGroupID.y+ gl_NumWorkGroups.x * gl_NumWorkGroups.y * gl_WorkGroupID.z;

    if(gl_LocalInvocationIndex == 0){
        align[uid] = unitAlign;
        atomicAdd(total, unitAlign);
    }

    if(unitNum == 0) return;

    uint vertAlign = 0;
    for(uint i = 0; i < gl_LocalInvocationIndex; i++){
        vertAlign += vNum[i];
    }

    interpPos[0] = interp(verts[0], verts[1]);
    interpPos[1] = interp(verts[1], verts[2]);
    interpPos[2] = interp(verts[2], verts[3]);
    interpPos[3] = interp(verts[3], verts[0]);
    interpPos[4] = interp(verts[4], verts[5]);
    interpPos[5] = interp(verts[5], verts[6]);
    interpPos[6] = interp(verts[6], verts[7]);
    interpPos[7] = interp(verts[7], verts[4]);
    interpPos[8] = interp(verts[0], verts[4]);
    interpPos[9] = interp(verts[1], verts[5]);
    interpPos[10] = interp(verts[2], verts[6]);
    interpPos[11] = interp(verts[3], verts[7]);

    for(uint i = 0; i < unitNum; i++){
        result[uid * 3000 + vertAlign + i] = interpPos[table[index * 16 + i]];
    }
}
