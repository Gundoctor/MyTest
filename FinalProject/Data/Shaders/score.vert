attribute float a_UnitPos;
attribute vec2 a_Position;
attribute vec2 a_UVCoord;

uniform vec2 u_WorldPosition;
uniform vec2 u_Scale;
uniform int  u_Score;

uniform vec2 u_CamProjection;

varying vec2 v_UVCoord;

void main()
{
	vec2 finalPos = a_Position;
	finalPos *= u_Scale;

	// Translate to world
	finalPos += u_WorldPosition;

	finalPos *= 1 / u_CamProjection;

	gl_Position = vec4(finalPos, 0, 1);

	// Texture offset calculations
	float divisorUnit = pow(10, a_UnitPos);
	float divisorRemainder = pow(10, a_UnitPos - 1);

	// Isolate digit into texUOffset
	float texUOffset = mod(u_Score, divisorUnit);
	if (divisorRemainder > 1)
	{
		texUOffset -= mod(u_Score, divisorRemainder);
		texUOffset = texUOffset / divisorRemainder;
	}

	vec2 uv = a_UVCoord;
	uv.x = uv.x + texUOffset;
	uv.x = uv.x / 10.0; // divide by number of digits in spritesheet

	v_UVCoord = uv;
}