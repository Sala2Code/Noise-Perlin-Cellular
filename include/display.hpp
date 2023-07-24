#ifndef DISPLAY_HPP
#define DISPLAY_HPP


void generateBuffer(
            GLuint &EBO, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer,
            std::vector<unsigned short> &indices, std::vector<glm::vec3> &indexed_vertices, 
            std::vector<glm::vec2> &indexed_uvs, std::vector<glm::vec3> &indexed_normals
            );

void fps(GLFWwindow *window, double &lastTime, double &lastTimeFPS, unsigned int &n_frame, float &deltaTime);

void bindBuffer(GLuint &EBO, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer);
void unbindBuffer(unsigned short n);
void bindTexture(GLuint &Texture, GLuint &TextureID);

// ImGUI
void ToggleButton(const char* str_id, bool* v);


#endif
