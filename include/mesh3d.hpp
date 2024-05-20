#include <vector>
#include <array>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

typedef struct {
    py::array_t<double> verts;
    py::array_t<uint32_t> faces;
} mesh3d;