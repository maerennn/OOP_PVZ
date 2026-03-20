#include "Util/ColorRect.hpp"

#include "Core/IndexBuffer.hpp"
#include "Core/VertexBuffer.hpp"

namespace Util {

ColorRect::ColorRect(glm::vec2 size, glm::vec4 color)
    : m_Size(size), m_Color(color) {
    if (s_Program == nullptr) {
        InitProgram();
    }
    if (s_VertexArray == nullptr) {
        InitVertexArray();
    }

    m_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
        *s_Program, "Matrices", 0);

    m_ColorLocation = glGetUniformLocation(s_Program->GetId(), "u_Color");
}

void ColorRect::Draw(const Core::Matrices &data) {
    m_UniformBuffer->SetData(0, data);

    s_Program->Bind();
    glUniform4f(m_ColorLocation, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
    s_Program->Validate();

    s_VertexArray->Bind();
    s_VertexArray->DrawTriangles();
}

void ColorRect::InitProgram() {
    s_Program = std::make_unique<Core::Program>(
        RESOURCE_DIR "/shaders/ColorRect.vert",
        RESOURCE_DIR "/shaders/ColorRect.frag");
    s_Program->Bind();
}

void ColorRect::InitVertexArray() {
    s_VertexArray = std::make_unique<Core::VertexArray>();

    // Unit quad: corners at (-0.5, ±0.5) and (0.5, ±0.5)
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
        std::vector<float>{
            -0.5F,  0.5F,
            -0.5F, -0.5F,
             0.5F, -0.5F,
             0.5F,  0.5F,
        },
        2));

    s_VertexArray->SetIndexBuffer(
        std::make_unique<Core::IndexBuffer>(std::vector<unsigned int>{
            0, 1, 2,
            0, 2, 3,
        }));
}

std::unique_ptr<Core::Program>     ColorRect::s_Program     = nullptr;
std::unique_ptr<Core::VertexArray> ColorRect::s_VertexArray = nullptr;

} // namespace Util
