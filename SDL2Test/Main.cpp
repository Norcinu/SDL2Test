//
//  main.cpp
//  SDL2Test
//
//  Created by Steven Gleed on 15/07/2013.
//  Copyright (c) 2013 Steven Gleed. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// http://wiki.libsdl.org/moin.fcg/MigrationGuide#Looking_for_information
// http://wiki.libsdl.org/moin.fcg/SDL_RenderPresent

int main(int argc, char *argv[])
{
	
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return -2;
	
	
	SDL_Window * window = SDL_CreateWindow("Steven Test SDL2",
					       SDL_WINDOWPOS_CENTERED,
					       SDL_WINDOWPOS_CENTERED-350,
					       640,
					       480,
					       SDL_WINDOW_OPENGL |
					       SDL_RENDERER_ACCELERATED |
					       SDL_WINDOW_RESIZABLE);
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	glViewport( 0.f, 0.f, 640, 480 );
	
	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0.0, 640, 480, 0.0, 1.0, -1.0 );
	
	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	GLint compile_ok = GL_FALSE;
	GLint link_ok = GL_FALSE;
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const char *vs_source =
	"#version 120\n"
	"attribute vec2 coord2d;                  "
	"void main(void) {                        "
	"  gl_Position = vec4(coord2d, 0.0, 1.0); "
	"}";
	
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok) {
		std::cerr << "Error in vertex shader\n";
	}
	
	GLuint fs =  glCreateShader(GL_FRAGMENT_SHADER);
	const char *fs_source =
	"#version 120\n"
	"void main(void) {	 "
	" gl_FragColor[0] = gl_FragCoord.x / 640.0; "
	" gl_FragColor[1] = gl_FragCoord.y / 480.0; "
	" gl_FragColor[2] = 0.5; "
	"}";
	
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok) {
		std::cerr << "error in fragment shader\n";
	}
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		std::cerr << "link not ok\n";
	}
	
	const char* attribute_name = "coord2d";
	GLint attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}
	
	// original
	bool running = true;
	SDL_Event event;
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
				running = false;
		}
		glClearColor(1.0, 1.0, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef( 640 / 2.f, 480 / 2.f, 0.f );
		glUseProgram(program);
		glEnableVertexAttribArray(attribute_coord2d);
		GLfloat triangle_vertices[] = {
			0.0,  0.8,
			-0.8, -0.8,
			0.8, -0.8,
		};
		/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
		glVertexAttribPointer(
				      attribute_coord2d, // attribute
				      2,                 // number of elements per vertex, here (x,y)
				      GL_FLOAT,          // the type of each element
				      GL_FALSE,          // take our values as-is
				      0,                 // no extra data between each position
				      triangle_vertices  // pointer to the C array
				      );
		
		/* Push each element in buffer_vertices to the vertex shader */
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(attribute_coord2d);
		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
    
	SDL_DestroyWindow(window);
	window = nullptr;
    
	SDL_Quit();
	
	return 0;
}