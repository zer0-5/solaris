#include "scene/model.hpp"

#include <fstream>

auto ModelBuffer::try_from_file(std::string_view file_path) noexcept
    -> cpp::result<ModelBuffer, ParseError>
{
    std::ifstream file_stream(file_path.data());
    if (!file_stream.is_open()) {
        return cpp::fail(ParseError::PRIMITIVE_FILE_NOT_FOUND);
    }
    float x, y, z;
    auto vertices = std::vector<float>();
    while (file_stream >> x >> y >> z) {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }
    file_stream.close();

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * vertices.size(),
        vertices.data(),
        GL_STATIC_DRAW
    );

    return ModelBuffer(vbo, vertices.size());
}

auto ModelBuffer::draw() const noexcept -> void {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, _n_vertices);
}

auto Model::draw() const noexcept -> void {
    glColor3f(_color.r, _color.g, _color.b);
    _buffer.draw();
}
