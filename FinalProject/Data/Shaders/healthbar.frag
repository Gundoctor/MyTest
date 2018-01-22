uniform vec4 u_Color;
uniform vec4 u_LostHealthCol;

uniform float u_Health;
uniform float u_MaxHealth;

varying float v_PosX; // should be between -1 and 1

void main()
{
	// compare posx vs health
	// figure if on left, do one color, if on right, do another
	float normHealth = u_Health / u_MaxHealth;

	// relate to v_PosX space
	float displayHealth = (normHealth * 2) - 1;

	// lower the health, the redder it gets
	u_Color.r = 2.0 * (1.0 - normHealth);
	u_Color.g = 2.0 * normHealth;


	// display current health if v_posx > normHealth
	if (displayHealth <= v_PosX)
		gl_FragColor = u_LostHealthCol;
	else
		gl_FragColor = u_Color;
}
