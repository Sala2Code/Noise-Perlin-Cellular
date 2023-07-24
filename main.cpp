#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include "common/include.hpp"
#include "include/constants.hpp"
#include <functional>

#include "common/loadShader.hpp"
#include "common/loadBMP.hpp"

#include "include/display.hpp"

int main(){
	// * display.cpp :
	double lastTime = glfwGetTime();
	double lastTimeFPS = lastTime;
	unsigned int n_frame = 0;
	float deltaTime = 0;

	// * Init OpengGL
	glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); 
	gladLoadGL();
	glViewport(0, 0, width, height);

	GLuint shaderPerlin_2D= LoadShaders("shader/basic.vert", "shader/2D/perlin.frag");
	GLuint shaderPerlin_3D= LoadShaders("shader/basic.vert", "shader/3D/perlin.frag");
	GLuint shaderCellular_2D= LoadShaders("shader/basic.vert", "shader/2D/cellular.frag");
	GLuint shaderCellular_3D= LoadShaders("shader/basic.vert", "shader/3D/cellular.frag");



	// it's more complexe than to write "if" in shaders but I do this to be more clear. Each shader will be in its own file.
	std::array<GLuint, 4> listShaders = {shaderPerlin_2D, shaderPerlin_3D, shaderCellular_2D, shaderCellular_3D};
	std::array<GLuint, 4> u_resolution = {0};
	std::array<GLuint, 4> u_time = {0};
	std::array<GLuint, 4> u_seed = {0};
	std::array<GLuint, 4> u_zoom = {0};
	std::array<GLuint, 4> u_blur = {0};
	std::array<GLuint, 4> u_blob = {0};
	std::array<GLuint, 4> u_size = {0};
	std::array<GLuint, 4> u_brightness = {0};
	std::array<GLuint, 4> u_wood = {0};
	std::array<GLuint, 4> u_highWave = {0};
	std::array<GLuint, 4> u_zoomWave = {0};
	std::array<GLuint, 4> u_patatoid = {0};
	std::array<GLuint, 4> u_detail = {0};
	std::array<GLuint, 4> u_blur2 = {0};





	int i = 0;
	for (auto shader : listShaders){
		u_resolution[i] = glGetUniformLocation(shader, "u_resolution");
		u_time[i] = glGetUniformLocation(shader, "u_time");
		u_seed[i] = glGetUniformLocation(shader, "u_seed");
		u_zoom[i] = glGetUniformLocation(shader, "u_zoom");
		u_blur[i] = glGetUniformLocation(shader, "u_blur");
		u_blob[i] = glGetUniformLocation(shader, "u_blob");
		u_size[i] = glGetUniformLocation(shader, "u_size");
		u_brightness[i] = glGetUniformLocation(shader, "u_brightness");
		u_wood[i] = glGetUniformLocation(shader, "u_wood");
		u_highWave[i] = glGetUniformLocation(shader, "u_highWave");
		u_zoomWave[i] = glGetUniformLocation(shader, "u_zoomWave");
		u_patatoid[i] = glGetUniformLocation(shader, "u_patatoid");
		u_detail[i] =  glGetUniformLocation(shader, "u_detail");
		u_blur2[i] = glGetUniformLocation(shader, "u_blur2");

		i++;
	}

	GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };
	GLuint indices[] = {
        0, 1, 2, 
        0, 2, 3
    };

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


    glfwSetCursorPos(window, width/2, height/2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// * Init ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// var ImGUi 
	bool is3D = false;
	bool isCellular = false; 

	bool blobCheck = false;
	bool woodCheck = false;
	bool patatoidCheck = false;

	float zoomSlider = 10.f;
	float seedSlider = 0.5f;
	float blurSlider = 1.0f;
	float sizeSlider = 0.05f;
	float brightnessSlider = 0.5f;
	float highWaveSlider = 0.5f;
	float zoomWaveSlider = 0.5f;
	float detailSlider = 10.;
	float blur2Slider = 0.5f;



	while (!glfwWindowShouldClose(window))
	{
		fps(window, lastTime, lastTimeFPS, n_frame, deltaTime); // Update deltaTime and display fps

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// * Code
		// 2D : even ; 3D : odd
		// Perlin - Cellular
		int numShader = 0;
		if(is3D){
			numShader = 1;
		}
		if(isCellular){
			numShader += 2;
		}

		glUseProgram(listShaders[numShader]);

		glUniform2f(u_resolution[numShader], width, height);
		glUniform1f(u_time[numShader], (float) glfwGetTime());

		glUniform1i(u_blob[numShader], blobCheck);
		glUniform1i(u_wood[numShader], woodCheck);
		glUniform1i(u_patatoid[numShader], patatoidCheck);


		glUniform1f(u_seed[numShader], seedSlider);
		glUniform1f(u_zoom[numShader], zoomSlider);
		glUniform1f(u_blur[numShader], blurSlider);
		glUniform1f(u_size[numShader], sizeSlider);
		glUniform1f(u_brightness[numShader], brightnessSlider);
		glUniform1f(u_highWave[numShader], highWaveSlider);
		glUniform1f(u_zoomWave[numShader], zoomWaveSlider);
		glUniform1f(u_detail[numShader], detailSlider);
		glUniform1f(u_blur2[numShader], blur2Slider);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
		// * ImGUI features
		ImGui::Begin("Parameters");

		// * Type random (2D / 3D - Perlin / Cellular)
		ImGui::Text("Type of random :");
		ToggleButton("2D / 3D", &is3D);
		ToggleButton("Perlin / Cellular (Voronoï)", &isCellular);

		ImGui::Text("Select your effect :");
		// * checkbox
		if(isCellular){
			ImGui::Checkbox("Blob", &blobCheck);
		}else{
			ImGui::Checkbox("WoodPlanch", &woodCheck);
			ImGui::Checkbox("Patatoïd (& nested circle)", &patatoidCheck);

		}

		ImGui::Text("Features :");
		// * slider
		ImGui::SliderFloat("Seed", &seedSlider, 0.001f, 1.0f); // possible some seed be "periodic"
		ImGui::SliderFloat("Zoom", &zoomSlider, 0.1f, 50.0f);
		if(isCellular){
			if(blobCheck){
				ImGui::SliderFloat("Size", &sizeSlider, 0.0f, 1.0f);
			}
		}else{
			ImGui::SliderFloat("Brightness", &brightnessSlider, 0.0f, 1.0f);

			if(woodCheck){
				ImGui::SliderFloat("HighWave", &highWaveSlider, 0.0f, 5.0f);
				ImGui::SliderFloat("ZoomWave", &zoomWaveSlider, 0.0f, 15.0f);
			}else if(patatoidCheck){
				ImGui::SliderFloat("Detail", &detailSlider, 0.0f, 50.0f);
				ImGui::SliderFloat("Blur (linked to brightness)", &blur2Slider, 0.0f, 2.0f);
			}else{ // perlin basic
				ImGui::SliderFloat("Blur", &blurSlider, 1, 100);
			}
			
		}




		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
					
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	for(auto shader : listShaders){
		glDeleteProgram(shader);
	}
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}