in vec4 vPosition;
in vec4 inVColor;
in float applyAnimation;
out vec4 exVColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 animation_matrix;

void
main()
{
	if(applyAnimation > 0.5)
    gl_Position = proj * view * model * animation_matrix * vPosition;
	else
	gl_Position = proj * view * model * vPosition;
	exVColor = inVColor;
}
