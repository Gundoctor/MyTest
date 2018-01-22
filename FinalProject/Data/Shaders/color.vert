
attribute vec2 a_Position;
attribute vec4 a_Color;

uniform vec2 u_ModelScale;
uniform vec2 u_ModelTranslation;
uniform vec2 u_CameraPos;
uniform vec2 u_ProjectionScale;
uniform vec4 u_Color;

varying vec4 v_Color;

void main()
{
	vec2 pos = a_Position;

	// Model to World space
	pos *= u_ModelScale; // Scale
	pos += u_ModelTranslation;

	// World to View space
	pos += u_CameraPos * -1;

	// View to Clip space
	pos *= 1 / u_ProjectionScale;

    gl_Position = vec4( pos, 0, 1 );

	v_Color = u_Color;
}
