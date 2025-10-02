#pragma once

#include "glm/vec2.hpp"
#include <string>


class GLTexture {

protected:
	std::string file;
	uint32_t id = 0;
	int w = 0;
	int h = 0;

public:
	GLTexture();
	GLTexture(const GLTexture& other);
	GLTexture(const std::string& path);
	virtual ~GLTexture();

	void bind(int index = 0) const;


	inline const std::string& getFilePath() const { return file; }
	inline uint32_t getId() const { return id; }
	inline glm::vec2 getSize() const { return glm::vec2((float) w, (float) h); }

	static GLTexture* createEmptyTexture(int width, int height);
	static GLTexture* createTextureFromData(const unsigned char* data, int width, int height);
	static void flipImage(const unsigned char* src, int width, int height, int comp, unsigned char* dst);
};



