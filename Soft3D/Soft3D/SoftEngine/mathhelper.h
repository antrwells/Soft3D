#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>



glm::vec2 rotate(glm::vec2 p, float r);

float smallest_delta(float current_angle, float target_angle);

float angle_to(glm::vec2 start, glm::vec2 end);

glm::vec3 random_vec3(glm::vec3 min, glm::vec3 max);

glm::vec2 random_vec2(glm::vec2 min, glm::vec2 max);

glm::vec2 reflect_vector(glm::vec2 vector, glm::vec2 normal);

float randomFloatInRange(float min, float max);
