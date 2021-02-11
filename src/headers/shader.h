#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;

	Shader();
	Shader(const char* vertex_path, const char* fragment_path);

	void use();

	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;
	void set_vector2(const std::string& name, const glm::vec2& value) const;
	void set_vector2(const std::string& name, float x, float y) const;
	void set_vector3(const std::string& name, const glm::vec3& value) const;
	void set_vector3(const std::string& name, float x, float y, float z) const;
	void set_vector4(const std::string& name, const glm::vec4& value) const;
	void set_vector4(const std::string& name, float x, float y, float z, float w) const;
	void set_matrix4(const std::string& name, const glm::mat4& mat) const;
};

#endif