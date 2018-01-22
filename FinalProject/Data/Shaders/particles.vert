
attribute vec2 a_Position;
attribute vec2 a_UVCoord;
attribute vec4 a_Color;

uniform vec2 u_CameraPos;
uniform vec2 u_ProjectionScale;

uniform vec2 u_uvOffset;
uniform vec2 u_uvSize;
uniform vec2 u_spritesheetSize;

varying vec2 v_UVCoord;
varying vec4 v_Color;

void main()
{
	// Grab Position
	vec2 finalpos = a_Position;

	// View Space
	finalpos += u_CameraPos * -1.0;

	// Clip Space
	finalpos *= 1.0 / u_ProjectionScale;

	// Final Position
	gl_Position = vec4(finalpos, 0, 1);

	v_Color = a_Color;

	
	// spritesheet uv calc
	vec2 uv = u_uvOffset;
	uv += u_uvSize  * a_UVCoord;
	uv = uv / u_spritesheetSize;

	v_UVCoord = uv;
}

