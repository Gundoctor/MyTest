uniform sampler2D u_Texture;

varying vec2 v_UVCoord;
varying vec4 v_Color;


void main()
{
	gl_FragColor = texture2D(u_Texture, v_UVCoord);
	// gl_FragColor = vec4(1, 1, 1, 1);
	// gl_FragColor = v_Color;
}
