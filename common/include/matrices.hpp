#pragma once

#include <array>
#include <stddef.h>

namespace matrices {
template <class T1, class T2, class T3, size_t N, size_t M, size_t P>
auto mult(
    const std::array<std::array<T1, M>, N>& mat1,
    const std::array<std::array<T2, P>, M>& mat2,
    std::array<std::array<T3, P>, N>& res
) -> void {
    for (size_t n = 0; n < N; ++n) {
        for (size_t p = 0; p < P; ++p) {
            for (size_t m = 0; m < M; ++m) {
                res[n][p] += mat1[n][m] * mat2[m][p];
            }
        }
    }
}
}
