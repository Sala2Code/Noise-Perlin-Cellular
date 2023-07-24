#include "common/include.hpp"
#include "include/display.hpp"
#include "include/constants.hpp"
#include <ImGUI/imgui_internal.h>

void generateBuffer(
            GLuint &EBO, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer,
            std::vector<unsigned short> &indices, std::vector<glm::vec3> &indexed_vertices, 
            std::vector<glm::vec2> &indexed_uvs, std::vector<glm::vec3> &indexed_normals)
{
    
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

}

void fps(GLFWwindow *window, double &lastTime, double &lastTimeFPS, unsigned int &n_frame, float &deltaTime){
	    double currentTime = glfwGetTime();
		n_frame++;
		if(currentTime - lastTimeFPS >=1){
			lastTimeFPS+=1;
			std::string title = nameProject + std::to_string(n_frame);
			glfwSetWindowTitle(window, title.c_str());
			n_frame = 0;
		}
        deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;
}

void bindBuffer(GLuint &EBO, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer){
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2,GL_FLOAT,GL_FALSE, 0,(void*)0);
		        
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(2, 3,GL_FLOAT,GL_FALSE, 0,(void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void unbindBuffer(unsigned short n){
	for(unsigned short i=0;i<n;i++){
		glDisableVertexAttribArray(i);
	}
}

void bindTexture(GLuint &Texture, GLuint &TextureID){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);
}

// ImGUI
void ToggleButton(const char* str_id, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    float t = *v ? 1.0f : 0.0f;

    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
	draw_list->AddText(ImVec2(p.x + 1.5*width, p.y+radius/4), IM_COL32(255, 255, 255, 255), str_id);
}