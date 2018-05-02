#version 130
in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 out_color;

uniform sampler2D texSampler;

void main() {
    // out_color = fragmentColor;
    float texSize = textureSize(texSampler,0).x;
    // XXX how to disable texture?!
    if (texSize > 1) {
        // out_color = fragmentColor;
        vec4 textureColor = texture(texSampler, vec2(fragmentUV.x, -fragmentUV.y));
        out_color =  textureColor * fragmentColor;
    } else {
        out_color = fragmentColor;
    }
    
}