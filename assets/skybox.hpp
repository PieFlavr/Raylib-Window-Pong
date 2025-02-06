/*******************************************************************************************
*
*   raylib [models] example - Skybox loading and drawing
*
*   Example originally created with raylib 1.8, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib-cpp.hpp"

namespace cs381 {
	struct SkyBox {
		constexpr static std::string_view vertexShader =
			#include "../generated/skybox.vs"
		;
		constexpr static std::string_view fragmentShader =
			#include "../generated/skybox.fs"
		;
		constexpr static std::string_view cubemapVertexShader =
			#include "../generated/cubemap.vs"
		;
		constexpr static std::string_view cubemapFragmentShader =
			#include "../generated/cubemap.fs"
		;

		static raylib::Shader cubemapShader;

		raylib::Texture texture;
		raylib::Shader shader;
		raylib::Model cube;

		SkyBox() : shader(0) {};
		SkyBox(SkyBox&) = delete;
		SkyBox(SkyBox&&) = default;
		SkyBox(const std::string_view filename, bool isEnviornment = false) : SkyBox() {
			Load(filename, isEnviornment);
		}

		~SkyBox() {
			if(cube.IsReady())
				UnloadTexture(cube.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
		}

		SkyBox& Init();
		SkyBox& Load(const std::string_view filename, bool isEnviornment = false);
		SkyBox& Draw();


	private:
		// Generate cubemap texture from HDR texture
		static TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format);
	};
}