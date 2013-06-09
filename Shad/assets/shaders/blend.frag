/// Author:		Nathaniel Meyer
///
/// Copyright:	Nutty Software
///				http://www.nutty.ca


/// <summary>
/// Fragment shader for blending two textures using an algorithm that overlays the
/// glowmap.
/// </summary>


#ifdef GL_ES
	precision highp float;
#endif


/// <summary>
/// Uniform variables.
/// <summary>
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int BlendMode;


/// <summary>
/// Varying variables.
/// <summary>
varying vec2 texCoord;


/// <summary>
/// Fragment shader entry.
/// <summary>
void main ()
{
	vec4 dst = texture2D(texture1, texCoord); // rendered scene
	vec4 src = texture2D(texture2, texCoord); // glowmap

	if ( BlendMode == 0 )
	{
		// Additive blending (strong result, high overexposure)
		gl_FragColor = min(src + dst, 1.0);
	}
	else if ( BlendMode == 1 )
	{
		// Screen blending (mild result, medium overexposure)
		gl_FragColor = clamp((src + dst) - (src * dst), 0.0, 1.0);
		gl_FragColor.w = 1.0;
	}
	else if ( BlendMode == 2 )
	{
		// Softlight blending (light result, no overexposure)
		// Due to the nature of soft lighting, we need to bump the black region of the glowmap
		// to 0.5, otherwise the blended result will be dark (black soft lighting will darken
		// the image).
		src = (src * 0.5) + 0.5;
		
		if (src.x <= 0.5)
		{
			gl_FragColor.x = dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x);
		}
		else
		{
			if ((src.x > 0.5) && (dst.x <= 0.25))
			{
				gl_FragColor.x = dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x);
			}
			else
			{
				gl_FragColor.x = dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x);
			}
		}
		if (src.y <= 0.5)
		{
			gl_FragColor.y = dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y);
		}
		else
		{
			if ((src.y > 0.5) && (dst.y <= 0.25))
			{
				gl_FragColor.y = dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y);
			}
			else
			{
				gl_FragColor.y = dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y);
			}
		}
		if (src.z <= 0.5)
		{
			gl_FragColor.z = dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z);
		}
		else
		{
			if ((src.z > 0.5) && (dst.z <= 0.25))
			{
				gl_FragColor.z = dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z);
			}
			else
			{
				gl_FragColor.z = dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z);
			}
		}
		gl_FragColor.w = 1.0;
	}
	else
	{
		// Show just the glow map
		gl_FragColor = src;
	}
}