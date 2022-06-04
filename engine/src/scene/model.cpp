#include "scene/model.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <fstream>

auto ModelBuffer::try_from_file(std::string_view file_path) noexcept
    -> cpp::result<ModelBuffer, ParseError>
{
    std::ifstream file_stream(file_path.data());
    if (!file_stream.is_open()) {
        return cpp::fail(ParseError::PRIMITIVE_FILE_NOT_FOUND);
    }
    float x, y, z, nx, ny, nz, tx, ty;
    auto positions = std::vector<float>();
    auto normals = std::vector<float>();
    auto texcoords = std::vector<float>();
    while (file_stream >> x >> y >> z >> nx >> ny >> nz >> tx >> ty) {
        positions.push_back(x);
        positions.push_back(y);
        positions.push_back(z);

        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);

        texcoords.push_back(tx);
        texcoords.push_back(ty);
    }
    file_stream.close();

    GLuint buffers[3];
    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * positions.size(),
        positions.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * normals.size(),
        normals.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * texcoords.size(),
        texcoords.data(),
        GL_STATIC_DRAW
    );

    return ModelBuffer(buffers[0], buffers[1], buffers[2], positions.size());
}

auto ModelBuffer::draw() const noexcept -> void {
    glBindBuffer(GL_ARRAY_BUFFER, _positions);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, _normals);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, _texcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, _n_vertices);
}

auto TextureBuffer::try_from_file(std::string_view file_path) noexcept
    -> cpp::result<TextureBuffer, ParseError>
{
    ILuint t;
    ilGenImages(1, &t);
    ilBindImage(t);
    if (!ilLoadImage(file_path.data())) {
        return cpp::fail(ParseError::TEXTURE_FILE_NOT_FOUND);
    }
    auto width = ilGetInteger(IL_IMAGE_WIDTH);
    auto height = ilGetInteger(IL_IMAGE_HEIGHT);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    ILinfo image_info;
    iluGetImageInfo(&image_info);
    if (image_info.Origin == IL_ORIGIN_LOWER_LEFT) {
        iluFlipImage();
    }

    auto image_data = ilGetData();

    GLuint id;
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return TextureBuffer(id);
}

auto TextureBuffer::bind() const noexcept -> void {
    glBindTexture(GL_TEXTURE_2D, _id);
}

auto Model::draw() const noexcept -> void {
    float d[] = {_diffuse.r, _diffuse.g, _diffuse.b, 1.0f};
    float a[] = {_ambient.r, _ambient.g, _ambient.b, 1.0f};
    float s[] = {_specular.r, _specular.g, _specular.b, 1.0f};
    float e[] = {_emissive.r, _emissive.g, _emissive.b, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
    glMaterialfv(GL_FRONT, GL_AMBIENT, a);
    glMaterialfv(GL_FRONT, GL_SPECULAR, s);
    glMaterialfv(GL_FRONT, GL_EMISSION, e);
    glMaterialf(GL_FRONT, GL_SHININESS, _shininess);
    if(_texture_buffer) {
        _texture_buffer->bind();
    }
    _model_buffer.draw();
}
