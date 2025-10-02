#include "gltexture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gldebug.h"

#include "stb/stb_image.h"




GLTexture::GLTexture() {}

GLTexture::GLTexture(const GLTexture& other) : id(other.id), w(other.w), h(other.h) {

}

GLTexture::GLTexture(const std::string& path) : file(path) {

	int comp = 0;
	unsigned char* data = nullptr;
	unsigned char* flipped = nullptr;

	{
		data = stbi_load(path.c_str(), &w, &h, &comp, STBI_rgb_alpha);
		flipped = new unsigned char[w * comp * h];
		flipImage(data, w, h, comp, flipped);
		stbi_image_free(data);
	}

	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, flipped));

	//stbi_image_free(data);
	delete[] flipped;
}

GLTexture::~GLTexture() {
	if (id != 0)
		glDeleteTextures(1, &id);
}

void GLTexture::bind(int index) const {
	GL_CALL(glActiveTexture(GL_TEXTURE0 + index));
	GL_CALL(glBindTextureUnit(index, id));
	//printf("texture bound at index : %i\n", index);
}

GLTexture* GLTexture::createEmptyTexture(int width, int height) {

	GLTexture* tex = new GLTexture();
	tex->w = width;
	tex->h = height;

	int length = height * width * 4;
	unsigned char* data = new unsigned char[length];
	for (int i = 0; i < length; i++) {
		data[i] = 0;
	}

	glGenTextures(1, &tex->id);
	glBindTexture(GL_TEXTURE_2D, tex->id);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

	delete[] data;

	return tex;
}


GLTexture* GLTexture::createTextureFromData(const unsigned char* data, int width, int height) {

	GLTexture* tex = new GLTexture();

	tex->w = width;
	tex->h = height;

	glGenTextures(1, &tex->id);
	glBindTexture(GL_TEXTURE_2D, tex->id);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return tex;
}
void GLTexture::flipImage(const unsigned char* src, int width, int height, int comp, unsigned char* dst) {
	int rowSize = width * comp;
	int lastLine = rowSize * (height - 1);

	for (int y = 0; y < height; y++) {
		int srcI = y * rowSize;
		int dstI = lastLine - y * rowSize;
		memcpy(&dst[dstI], &src[srcI], rowSize);
	}
}
