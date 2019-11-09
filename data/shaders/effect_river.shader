VERTEX_SHADER
{
	#version 330

	uniform mat4 camera;
	uniform mat4 model;

	in vec3 vertexPosition;
	in vec3 vertexNormal;
	in vec2 vertexTextureCoordinate;

	out vec2 uv;
//	out vec3 fragmentNormal;
//	out vec3 fragmentPosition;

	void main() 
	{
	    uv = vertexTextureCoordinate;

	    // Apply all matrix transformations to vert
	    gl_Position = camera * model * vec4(vertexPosition, 1);
	}
}

FRAGMENT_SHADER
{
	#version 330

//	uniform samplerCube tex;
	uniform vec3 campos;
	uniform vec3 camdir;

	uniform float time;

//	in vec3 fragmentPosition;
//	in vec3 fragmentNormal;

	in vec2 uv;
	out vec4 finalColor;


	struct Ray
	{
		vec3 origin;
		vec3 direction;
	};

	struct Camera
	{
		vec3 pos;
		vec3 forward;
		vec3 up;
		vec3 right;
	};

	struct Plane
	{
		vec3 point;
		vec3 normal;
	};

	float calculate(vec2 uv)
	{
		vec2 z, c;

		int iter = 150;
		float scale = 0.04;// * (0.5 + 0.5 * sin(time * 10.05));
		vec2 center = vec2(-0.393, 0.266);

	    c.x = 1.3333 * (uv.x - 0.5) * scale - center.x;
	    c.y = (uv.y - 0.5) * scale - center.y;

	    int i;
	    z = c;
	    for(i=0; i<iter; i++) 
	    {
	        float x = (z.x * z.x - z.y * z.y) + c.x;
	        float y = (z.y * z.x + z.x * z.y) + c.y;

	        if((x * x + y * y) > 4.0) break;
	        z.x = x;
	        z.y = y;
	    }
	    float a= i * (1.0 / iter);
	    return a * 1.4;


	}

	void main() 
	{
		vec3 campos = vec3(0.0, 2.0, time * 12.0);
		vec3 camtgt = vec3(1.0, -7.0, 10.0) + campos;

		Plane plane;
		plane.normal = normalize(vec3(-0.9, 1.0, 0.0));
		plane.point = vec3(0.0, 0.0, 0.0);

		Camera cam;
		cam.pos = campos;
		cam.forward = normalize(camtgt - campos);
		cam.right = cross(vec3(0.0, 1.0, 0.0), cam.forward);
		cam.up = cross(cam.forward, cam.right);

		Ray r;
		r.origin = cam.pos;
		r.direction = normalize(cam.forward + cam.right * (uv.x - 0.5) + cam.up * (uv.y - 0.5));

		//ray->plane

		bool hit = false;
		float dotProduct = dot(r.direction, plane.normal); 

		float distanceToHit = 0.0;
		vec3 hitPoint = vec3(0.0);
		if (dotProduct == 0.0)
		{
			finalColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
		else
		{
			distanceToHit = dot(plane.point - r.origin, plane.normal) / dotProduct;
			if (distanceToHit < 0.0)
			{
				hit = false; //takapuoli
				finalColor = vec4(vec3(0.0), 1.0);
			}
			else
			{
				hit = true;
				hitPoint = r.origin + r.direction * distanceToHit;

				float a = calculate(vec2(hitPoint.x, hitPoint.z));

				vec3 col = vec3(a);
				col *= 1.0 - distanceToHit * 0.054;

			    finalColor = vec4(col, 1.0);
			}
		}
	}	
}
