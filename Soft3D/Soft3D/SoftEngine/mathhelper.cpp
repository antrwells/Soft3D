#include "mathhelper.h"
#include <random>
glm::vec2 rotate(glm::vec2 p, float r)
{
	float theta = glm::radians(r);
	float rot[2][2] = {
	{cos(theta), -sin(theta)},
	{sin(theta),  cos(theta)}
	};

	// Rotate the point
	float x_new = p.x * rot[0][0] + p.y * rot[0][1];
	float y_new = p.x * rot[1][0] + p.y * rot[1][1];

	return glm::vec2(x_new, y_new);


}

float smallest_delta(float current_angle, float target_angle) {
	float delta = target_angle - current_angle;
	if (delta > 180.0f) {
		delta -= 360.0f;
	}
	else if (delta < -180.0f) {
		delta += 360.0f;
	}
	return delta;
}


float angle_to(glm::vec2 start,glm::vec2 end) {

	float rad = atan2(end.y - start.y, end.x - start.x);

	return glm::degrees(rad);

}


glm::vec3 random_vec3(glm::vec3 min, glm::vec3 max) {
	return glm::vec3(
		glm::linearRand(min.x, max.x),
		glm::linearRand(min.y, max.y),
		glm::linearRand(min.z, max.z)
	);
}


glm::vec2 random_vec2(glm::vec2 min, glm::vec2 max) {

	return glm::vec2(glm::linearRand(min.x, max.x), glm::linearRand(min.y, max.y));

}

glm::vec2 reflect_vector(glm::vec2 vector, glm::vec2 normal) {
	// Dot product of the vector and the normal
	float dotProduct = glm::dot(vector, normal);
	// Scale the normal by 2 times the dot product
	glm::vec2 scaledNormal = normal * 2.0f * dotProduct;
	// Subtract the scaled normal from the original vector
	return vector - scaledNormal;
}

float randomFloatInRange(float min, float max)
{
	std::random_device rd;  // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_real_distribution<float> distr(min, max); // define the range

	return distr(eng); // generate the random float value
}
