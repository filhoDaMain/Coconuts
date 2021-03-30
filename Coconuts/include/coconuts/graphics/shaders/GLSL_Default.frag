#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[16];

void main()
{
    int idx = int(v_TexIndex);

    // Done this way to avoid 'Dynamic Indexing' errors on older drivers
    switch(idx)
    {
    case 15: color = texture(u_Textures[15], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 14: color = texture(u_Textures[14], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 13: color = texture(u_Textures[13], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 12: color = texture(u_Textures[12], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 11: color = texture(u_Textures[11], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 10: color = texture(u_Textures[10], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  9: color = texture(u_Textures[9], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  8: color = texture(u_Textures[8], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  7: color = texture(u_Textures[7], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  6: color = texture(u_Textures[6], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  5: color = texture(u_Textures[5], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  4: color = texture(u_Textures[4], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  3: color = texture(u_Textures[3], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  2: color = texture(u_Textures[2], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  1: color = texture(u_Textures[1], v_TexCoord * v_TilingFactor) * v_Color; break;
    case  0:
    default: color = texture(u_Textures[0], v_TexCoord * v_TilingFactor) * v_Color; break;
    }
}