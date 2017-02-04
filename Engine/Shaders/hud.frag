#version 130

// Some drivers require the following
precision highp float;

// Uniforms
uniform sampler2D u_textureUnit;

// Inputs
in vec2 out_TexCoord;


// Outputs
/*layout(location = 0)*/ out vec4 out_Color;
 
void main()
{
	out_Color = texture2D(u_textureUnit, out_TexCoord);

	if (out_Color[3] < 0.7)
	{
		discard;
	}
}
