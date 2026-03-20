#ifndef UTIL_COLOR_RECT_HPP
#define UTIL_COLOR_RECT_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Core/Drawable.hpp"
#include "Core/Program.hpp"
#include "Core/UniformBuffer.hpp"
#include "Core/VertexArray.hpp"

namespace Util {
/**
 * @brief A solid-colored filled rectangle drawable.
 *
 * Used to render plain colored quads without a texture.
 * Useful for debug overlays such as grid lines.
 */
class ColorRect : public Core::Drawable {
public:
    /**
     * @param size   Width and height of the rectangle in pixels.
     * @param color  RGBA color in [0, 1] range.
     */
    ColorRect(glm::vec2 size, glm::vec4 color);

    glm::vec2 GetSize() const override { return m_Size; }

    void Draw(const Core::Matrices &data) override;

private:
    void InitProgram();
    void InitVertexArray();

    static std::unique_ptr<Core::Program>     s_Program;
    static std::unique_ptr<Core::VertexArray> s_VertexArray;

    std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_UniformBuffer;

    glm::vec2 m_Size;
    glm::vec4 m_Color;
    GLint     m_ColorLocation = -1;
};
} // namespace Util

#endif // UTIL_COLOR_RECT_HPP
