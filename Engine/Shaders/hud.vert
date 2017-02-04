#version 130

// Uniforms
uniform mat4 u_trans;
uniform mat4 u_pos;
uniform vec2 u_offset;

// Inputs
in  vec3 in_Position;
in 	vec2 in_TexCoord;

// Outputs
out vec2 out_TexCoord;

void
main(void)
{
	out_TexCoord = in_TexCoord - u_offset;

    gl_Position = (u_trans * u_pos) * vec4(in_Position, 1.0);
}