
attribute vec2 a_Position;
attribute vec2 a_UVCoord;

uniform vec2 u_ModelScale;
uniform vec2 u_ModelTranslation;

uniform vec2 u_CameraPos;
uniform vec2 u_ProjectionScale;

uniform vec2 u_uvOffset;
uniform vec2 u_uvSize;
uniform vec2 u_spritesheetSize;

varying vec2 v_UVCoord;

void main()
{
	vec2 finalpos = a_Position;

	// Object Space
	finalpos *= u_ModelScale;

	// World Space
	finalpos += u_ModelTranslation;

	// View Space
	finalpos += u_CameraPos * -1.0;

	// Clip Space
	finalpos *= 1 / u_ProjectionScale;


    gl_Position = vec4( finalpos, 0, 1 );

	// spritesheet uv calc
	vec2 uv = u_uvOffset;
	uv += u_uvSize  * a_UVCoord;
	uv = uv / u_spritesheetSize;

	v_UVCoord = uv;
}
