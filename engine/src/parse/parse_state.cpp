#include "parse/parse_state.hpp"

auto ParseState::insert_model_buffer(std::string const& file_path) noexcept
    -> cpp::result<ModelBuffer, ParseError>
{
    auto stored_buffer = _model_buffers.find(file_path);
    if (stored_buffer == _model_buffers.end()) {
        auto buffer = ModelBuffer::try_from_file(file_path);
        CHECK_RESULT(buffer);
        _model_buffers.insert(std::make_pair(file_path, *buffer));

        return *buffer;
    }
    return stored_buffer->second;
}

auto ParseState::insert_texture_buffer(std::string const& file_path) noexcept
    -> cpp::result<TextureBuffer, ParseError>
{
    auto stored_buffer = _texture_buffers.find(file_path);
    if (stored_buffer == _texture_buffers.end()) {
        auto buffer = TextureBuffer::try_from_file(file_path);
        CHECK_RESULT(buffer);
        _texture_buffers.insert(std::make_pair(file_path, *buffer));

        return *buffer;
    }
    return stored_buffer->second;
}
