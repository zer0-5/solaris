#pragma once

#include "parse/parse_error.hpp"
#include "result.hpp"
#include "scene/model.hpp"

#include <unordered_map>

class ParseState {
  private:
    std::unordered_map<std::string, ModelBuffer> _model_buffers;
    std::unordered_map<std::string, TextureBuffer> _texture_buffers;

  public:
    auto insert_model_buffer(std::string const&) noexcept
        -> cpp::result<ModelBuffer, ParseError>;
    auto insert_texture_buffer(std::string const&) noexcept
        -> cpp::result<TextureBuffer, ParseError>;
};
