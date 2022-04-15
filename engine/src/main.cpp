#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif
#include "logger.hpp"
#include "parse/parse_error.hpp"
#include "parse/parser.hpp"
#include "render/renderer.hpp"

#include <chrono>


int main(int argc, char** argv) {
    info("loading XML file...");
    auto start = std::chrono::high_resolution_clock::now();
    auto world_res = parse(argv[1]);
    if (world_res.has_error()) {
        error(error_msg(world_res.error()));
        return 1;
    }
    auto finish = std::chrono::high_resolution_clock::now();
    info(fmt::format(
        "loaded XML in {}s!",
        std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)
            .count() / 1000.f
    ).c_str());

    Renderer(*world_res).run();

    return 0;
}
