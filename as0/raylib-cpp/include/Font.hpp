#ifndef RAYLIB_CPP_INCLUDE_FONT_HPP_
#define RAYLIB_CPP_INCLUDE_FONT_HPP_

#include <cassert>
#include <string>
#include <string_view>
#include <span>

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./RaylibException.hpp"
#include "./TextureUnmanaged.hpp"

namespace raylib {
/**
 * Font type, includes texture and charSet array data
 */
class Font : public ::Font {
 public:
    Font(int baseSize,
            int glyphCount,
            int glyphPadding,
            ::Texture2D texture,
            ::Rectangle *recs = nullptr,
            ::GlyphInfo *glyphs = nullptr) : ::Font{baseSize, glyphCount, glyphPadding, texture, recs, glyphs} {
        // Nothing.
    }

    Font(int baseSize,
            int glyphPadding,
            ::Texture2D texture,
            std::span<::Rectangle> recs = {},
            std::span<::GlyphInfo> glyphs = {}) : ::Font{baseSize, static_cast<int>((assert(glyphs.size() == recs.size()), glyphs.size())), glyphPadding, texture, recs.data(), glyphs.data()} {
        // Nothing.
    }

    /**
     * Retrieves the default Font.
     */
    Font() {
        set(::GetFontDefault());
    }

    Font(const ::Font& font) {
        set(font);
    }

    /**
     * Loads a Font from the given file.
     *
     * @param fileName The file name of the font to load.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     */
    Font(const std::string_view fileName) {
        Load(fileName);
    }

    /**
     * Loads a Font from the given file, with generation parameters.
     *
     * @param fileName The file name of the font to load.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontEx
     */
    Font(const std::string_view fileName, int fontSize, int* fontChars = 0, int charCount = 0)  {
        Load(fileName, fontSize, fontChars, charCount);
    }

    /**
     * Loads a Font from the given file, with generation parameters.
     *
     * @param fileName The file name of the font to load.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontEx
     */
    Font(const std::string_view fileName, int fontSize, std::span<int> fontChars)  {
        Load(fileName, fontSize, fontChars.data(), static_cast<int>(fontChars.size()));
    }

    /**
     * Loads a Font from the given image with a color key.
     *
     * @param image The image to load the fond from.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontFromImage()
     */
    Font(const ::Image& image, ::Color key, int firstChar) {
        Load(image, key, firstChar);
    }

    /**
     * Loads a font from memory, based on the given file type and file data.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontFromMemory()
     */
    Font(const std::string_view fileType, const unsigned char* fileData, int dataSize, int fontSize,
            int *fontChars, int charsCount)  {
        Load(fileType, fileData, dataSize, fontSize, fontChars, charsCount);
    }

    Font(const std::string_view fileType, const unsigned char* fileData, int dataSize, int fontSize,
            std::span<int> fontChars)  {
        Load(fileType, fileData, dataSize, fontSize, fontChars.data(), static_cast<int>(fontChars.size()));
    }

    Font(const Font&) = delete;

    Font(Font&& other) {
        set(other);

        other.baseSize = 0;
        other.glyphCount = 0;
        other.glyphPadding = 0;
        other.texture = {};
        other.recs = nullptr;
        other.glyphs = nullptr;
    }

    ~Font() {
        Unload();
    }

    void Unload() {
        // Protect against calling UnloadFont() twice.
        if (baseSize != 0) {
            UnloadFont(*this);
            baseSize = 0;
        }
    }

    GETTER(int, BaseSize, baseSize)
    GETTER(int, GlyphCount, glyphCount)
    GETTER(int, GlyphPadding, glyphPadding)
    GETTER(::Rectangle*, Recs, recs)
    GETTER(::GlyphInfo*, Glyphs, glyphs)

    /**
     * Get the texture atlas containing the glyphs.
     */
    TextureUnmanaged GetTexture() {
        return texture;
    }

    /**
     * Set the texture atlas containing the glyphs.
     */
    void SetTexture(const ::Texture& newTexture) {
        texture = newTexture;
    }

    Font& operator=(const ::Font& font) {
        Unload();
        set(font);
        return *this;
    }

    Font& operator=(const Font&) = delete;

    Font& operator=(Font&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Unload();
        set(other);

        other.baseSize = 0;
        other.glyphCount = 0;
        other.glyphPadding = 0;
        other.texture = {};
        other.recs = nullptr;
        other.glyphs = nullptr;

        return *this;
    }

    /**
     * Loads a font from a given file.
     *
     * @param fileName The filename of the font to load.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFont()
     */
    void Load(const std::string_view fileName) {
        set(::LoadFont(fileName.data()));
        if (!IsReady()) {
            throw RaylibException("Failed to load Font with from file: " + std::string(fileName));
        }
    }

    /**
     * Loads a font from a given file with generation parameters.
     *
     * @param fileName The filename of the font to load.
     * @param fontSize The desired size of the font.
     *
     * @throws raylib::RaylibException Throws if the given font failed to initialize.
     *
     * @see ::LoadFontEx()
     */
    void Load(const std::string_view fileName, int fontSize, int* fontChars, int charCount)  {
        set(::LoadFontEx(fileName.data(), fontSize, fontChars, charCount));
        if (!IsReady()) {
            throw RaylibException("Failed to load Font with from file with font size: " + std::string(fileName));
        }
    }

    void Load(const std::string_view fileName, int fontSize, std::span<int> fontChars)  {
        set(::LoadFontEx(fileName.data(), fontSize, fontChars.data(), static_cast<int>(fontChars.size())));
        if (!IsReady()) {
            throw new RaylibException("Failed to load Font with from file with font size: " + std::string(fileName));
        }
    }

    void Load(const ::Image& image, ::Color key, int firstChar) {
        set(::LoadFontFromImage(image, key, firstChar));
        if (!IsReady()) {
            throw RaylibException("Failed to load Font with from image");
        }
    }

    void Load(const std::string_view fileType, const unsigned char* fileData, int dataSize, int fontSize,
            int *fontChars, int charsCount)  {
        set(::LoadFontFromMemory(fileType.data(), fileData, dataSize, fontSize, fontChars,
            charsCount));
        if (!IsReady()) {
            throw RaylibException("Failed to load Font " + std::string(fileType) + " with from file data");
        }
    }

    void Load(const std::string_view fileType, const unsigned char* fileData, int dataSize, int fontSize,
            std::span<int> fontChars)  {
        set(::LoadFontFromMemory(fileType.data(), fileData, dataSize, fontSize, fontChars.data(),
            static_cast<int>(fontChars.size())));
        if (!IsReady()) {
            throw new RaylibException("Failed to load Font " + std::string(fileType) + " with from file data");
        }
    }

    void Load(const std::string_view fileType, std::span<unsigned char> fileData, int fontSize,
            int *fontChars, int charsCount)  {
        set(::LoadFontFromMemory(fileType.data(), fileData.data(), static_cast<int>(fileData.size()), fontSize, fontChars,
            charsCount));
        if (!IsReady()) {
            throw RaylibException("Failed to load Font " + std::string(fileType) + " with from file data");
        }
    }

    void Load(const std::string_view fileType, std::span<unsigned char> fileData, int fontSize,
            std::span<int> fontChars)  {
        set(::LoadFontFromMemory(fileType.data(), fileData.data(), static_cast<int>(fileData.size()), fontSize, fontChars.data(),
            static_cast<int>(fontChars.size())));
        if (!IsReady()) {
            throw new RaylibException("Failed to load Font " + std::string(fileType) + " with from file data");
        }
    }

    /**
     * Returns if the font is ready to be used.
     */
    bool IsReady() const {
        return ::IsFontReady(*this);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const char* text, ::Vector2 position, float fontSize,
            float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text, position,  fontSize,  spacing,  tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const std::string_view text, ::Vector2 position, float fontSize,
            float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text.data(), position,  fontSize,  spacing,  tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const char* text, int posX, int posY, float fontSize,
            float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text,
            { static_cast<float>(posX), static_cast<float>(posY) },
            fontSize, spacing, tint);
    }

    /**
     * Draw text using font and additional parameters.
     */
    void DrawText(const std::string_view text, int posX, int posY, float fontSize,
            float spacing, ::Color tint = WHITE) const {
        ::DrawTextEx(*this, text.data(),
            { static_cast<float>(posX), static_cast<float>(posY) },
            fontSize, spacing, tint);
    }

    void DrawText(
            const char* text,
            ::Vector2 position,
            ::Vector2 origin,
            Degree rotation,
            float fontSize,
            float spacing,
            ::Color tint = WHITE) const {
        ::DrawTextPro(*this, text,
            position, origin,
            rotation, fontSize,
            spacing, tint);
    }

    void DrawText(
            const std::string_view text,
            ::Vector2 position,
            ::Vector2 origin,
            Degree rotation,
            float fontSize,
            float spacing,
            ::Color tint = WHITE) const {
        ::DrawTextPro(*this, text.data(),
            position, origin,
            rotation, fontSize,
            spacing, tint);
    }

    /**
     * Draw one character (codepoint)
     */
    void DrawText(int codepoint,
            ::Vector2 position,
            float fontSize,
            ::Color tint = { 255, 255, 255, 255 }) const {
        ::DrawTextCodepoint(*this, codepoint, position, fontSize, tint);
    }

    /**
     * Draw multiple character (codepoint)
     */
    void DrawText(const int *codepoints,
            int count, ::Vector2 position,
            float fontSize, float spacing,
            ::Color tint = { 255, 255, 255, 255 }) const {
        ::DrawTextCodepoints(*this,
            codepoints, count,
            position, fontSize,
            spacing, tint);
    }

    /**
     * Draw multiple character (codepoint)
     */
    inline void DrawText(std::span<const int> codepoints, ::Vector2 position,
            float fontSize, float spacing,
            ::Color tint = { 255, 255, 255, 255 }) const {
        ::DrawTextCodepoints(*this,
            codepoints.data(), static_cast<int>(codepoints.size()),
            position, fontSize,
            spacing, tint);
    }

    /**
     * Measure string size for Font
     */
    Vector2 MeasureText(const char* text, float fontSize, float spacing) const {
        return ::MeasureTextEx(*this, text, fontSize, spacing);
    }

    /**
     * Measure string size for Font
     */
    Vector2 MeasureText(const std::string_view text, float fontSize, float spacing) const {
        return ::MeasureTextEx(*this, text.data(), fontSize, spacing);
    }

    /**
     * Get index position for a unicode character on font
     */
    int GetGlyphIndex(int character) const {
        return ::GetGlyphIndex(*this, character);
    }

    /**
     * Create an image from text (custom sprite font)
     */
    ::Image ImageText(const char* text, float fontSize,
            float spacing, ::Color tint) const {
        return ::ImageTextEx(*this, text, fontSize, spacing, tint);
    }

    /**
     * Create an image from text (custom sprite font)
     */
    ::Image ImageText(const std::string_view text, float fontSize,
            float spacing, ::Color tint) const {
        return ::ImageTextEx(*this, text.data(), fontSize, spacing, tint);
    }

 protected:
    void set(const ::Font& font) {
        baseSize = font.baseSize;
        glyphCount = font.glyphCount;
        glyphPadding = font.glyphPadding;
        texture = font.texture;
        recs = font.recs;
        glyphs = font.glyphs;
    }
};
}  // namespace raylib

using RFont = raylib::Font;

#endif  // RAYLIB_CPP_INCLUDE_FONT_HPP_
