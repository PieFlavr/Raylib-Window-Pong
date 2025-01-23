#ifndef RAYLIB_CPP_INCLUDE_SHADER_HPP_
#define RAYLIB_CPP_INCLUDE_SHADER_HPP_

#include <string>
#include <string_view>
#include <span>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "Texture.hpp"

namespace raylib {
/**
 * Shader type (generic)
 */
class Shader : public ::Shader {
 public:
    Shader(const ::Shader& shader) {
        set(shader);
    }

    Shader(unsigned int id, int* locs = nullptr) : ::Shader{id, locs} {}
    Shader(unsigned int id, std::span<int> locs) : ::Shader{id, locs.data()} {}

    Shader(const std::string_view vsFileName, const std::string_view fsFileName) {
        set(::LoadShader(vsFileName.data(), fsFileName.data()));
    }

    Shader(const char* vsFileName, const char* fsFileName) {
        set(::LoadShader(vsFileName, fsFileName));
    }

    Shader(const Shader&) = delete;

    Shader(Shader&& other) {
        set(other);

        other.id = 0;
        other.locs = nullptr;
    }

    /**
     * Load shader from files and bind default locations.
     *
     * @see ::LoadShader
     */
    static ::Shader Load(const std::string_view vsFileName, const std::string_view fsFileName) {
        return ::LoadShader(vsFileName.data(), fsFileName.data());
    }

    static ::Shader Load(const char* vsFileName, const char* fsFileName) {
        return ::LoadShader(vsFileName, fsFileName);
    }

    /**
     * Load a shader from memory.
     *
     * @see ::LoadShaderFromMemory
     */
    static ::Shader LoadFromMemory(const std::string_view vsCode, const std::string_view fsCode) {
        return ::LoadShaderFromMemory(vsCode.data(), fsCode.data());
    }

    static ::Shader LoadFromMemory(const char* vsCode, const char* fsCode) {
        return ::LoadShaderFromMemory(vsCode, fsCode);
    }

    GETTER(unsigned int, Id, id)
    GETTER(int*, Locs, locs)

    Shader& operator=(const ::Shader& shader) {
        set(shader);
        return *this;
    }

    Shader& operator=(const Shader&) = delete;

    Shader& operator=(Shader&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other);

        other.id = 0;
        other.locs = nullptr;

        return *this;
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    ~Shader() {
        Unload();
    }

    /**
     * Unload shader from GPU memory (VRAM)
     */
    void Unload() {
        if (locs != nullptr) {
            ::UnloadShader(*this);
        }
    }

    /**
     * Begin custom shader drawing.
     */
    Shader& BeginMode() {
        ::BeginShaderMode(*this);
        return *this;
    }

    /**
     * End custom shader drawing (use default shader).
     */
    Shader& EndMode() {
        ::EndShaderMode();
        return *this;
    }

    /**
     * Get shader uniform location
     *
     * @see GetShaderLocation()
     */
    int GetLocation(const std::string_view uniformName) const {
        return ::GetShaderLocation(*this, uniformName.data());
    }

    /**
     * Get shader attribute location
     *
     * @see GetShaderLocationAttrib()
     */
    int GetLocationAttrib(const std::string_view attribName) const {
        return ::GetShaderLocationAttrib(*this, attribName.data());
    }

    /**
     * Set shader uniform value
     *
     * @see SetShaderValue()
     */
    Shader& SetValue(int uniformLoc, const void* value, int uniformType) {
        ::SetShaderValue(*this, uniformLoc, value, uniformType);
        return *this;
    }

    /**
     * Set shader uniform value
     * Automatically finds the location of the uniform given its name
     *
     * @see SetShaderValue()
     */
    inline Shader& SetValue(std::string_view uniformName, const void* value, int uniformType) {
        ::SetShaderValue(*this, GetLocation(uniformName), value, uniformType);
        return *this;
    }

    /**
     * Set shader uniform value
     *
     * @see SetShaderValue()
     */
    template<typename T>
    inline Shader& SetValue(int uniformLoc, const T& value, int uniformType) {
        ::SetShaderValue(*this, uniformLoc, &value, uniformType);
        return *this;
    }

    /**
     * Set shader uniform value
     * Automatically finds the location of the uniform given its name
     *
     * @see SetShaderValue()
     */
    template<typename T>
    inline Shader& SetValue(std::string_view uniformName, const T& value, int uniformType) {
        ::SetShaderValue(*this, GetLocation(uniformName), &value, uniformType);
        return *this;
    }

    /**
     * Set shader uniform value vector
     *
     * @see SetShaderValueV()
     */
    Shader& SetValue(int uniformLoc, const void* value, int uniformType, int count) {
        ::SetShaderValueV(*this, uniformLoc, value, uniformType, count);
        return *this;
    }

     /**
     * Set shader uniform value vector
     * Automatically finds the location of the uniform given its name
     *
     * @see SetShaderValueV()
     */
    inline Shader& SetValue(std::string_view uniformName, const void* value, int uniformType, int count) {
        ::SetShaderValueV(*this, GetLocation(uniformName), value, uniformType, count);
        return *this;
    }

    /**
     * Set shader uniform value vector (represented as a std::span)
     *
     * @see SetShaderValueV()
     */
    template<typename T>
    inline Shader& SetValue(int uniformLoc, std::span<T> data, int uniformType) {
        ::SetShaderValueV(*this, uniformLoc, data.value(), uniformType, data.count());
        return *this;
    }

    /**
     * Set shader uniform value vector (represented as a std::span)
     * Automatically finds the location of the uniform given its name
     *
     * @see SetShaderValueV()
     */
    template<typename T>
    inline Shader& SetValue(std::string_view uniformName, std::span<T> data, int uniformType) {
        ::SetShaderValueV(*this, GetLocation(uniformName), data.value(), uniformType, data.count());
        return *this;
    }

    /**
     * Set shader uniform value (matrix 4x4)
     *
     * @see SetShaderValueMatrix()
     */
    Shader& SetValue(int uniformLoc, const ::Matrix& mat) {
        ::SetShaderValueMatrix(*this, uniformLoc, mat);
        return *this;
    }

    /**
     * Set shader uniform value (matrix 4x4)
     * Automatically finds the location of the uniform given its name
     *
     * @see SetShaderValueMatrix()
     */
    inline Shader& SetValue(std::string_view uniformName, const ::Matrix& mat) {
        ::SetShaderValueMatrix(*this, GetLocation(uniformName), mat);
        return *this;
    }

    /**
     * Set shader uniform value for texture
     *
     * @see SetShaderValueTexture()
     */
    Shader& SetValue(int uniformLoc, const ::Texture2D& texture) {
        ::SetShaderValueTexture(*this, uniformLoc, texture);
        return *this;
    }

    /**
     * Set shader uniform value for texture
     * Automatically finds the location of the uniform given its name
     *
     * @see SetShaderValueTexture()
     */
    inline Shader& SetValue(std::string_view uniformName, const ::Texture2D& texture) {
        ::SetShaderValueTexture(*this, GetLocation(uniformName), texture);
        return *this;
    }

    /**
     * Retrieves whether or not the shader is ready.
     */
    bool IsReady() const {
        return id != 0 && locs != nullptr;
    }

 protected:
    void set(const ::Shader& shader) {
        id = shader.id;
        locs = shader.locs;
    }
};
}  // namespace raylib

using RShader = raylib::Shader;

#endif  // RAYLIB_CPP_INCLUDE_SHADER_HPP_
