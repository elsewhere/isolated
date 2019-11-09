COMPUTE_SHADER
{
	#version 430
	layout(local_size_x = 32, local_size_y = 32) in;
	layout(rgba32f, binding = 0) uniform image2D img_output;

	uniform float time;

	void main() 
	{
		  // base pixel colour for image
		  vec4 pixel = vec4(0.0, 1.0, 0.0, 1.0);
		  // get index in global work group i.e x,y position
		  ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);


		  vec2 uv = pixel_coords * 0.02;

		  float s = 0.5 + 0.5 * (0.5 * sin(uv.x + time * 3.0) + 0.5 * cos(uv.y * 1.4 + uv.x * 1.7 - time * 2.0) - sin(uv.y - time * 2.5));
		  pixel = vec4(s, s, s, 1.0);

		  // output to a specific pixel in the image
		  imageStore(img_output, pixel_coords, pixel);
	}
}
