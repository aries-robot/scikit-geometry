#include "skgeom.hpp"
#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/minkowski_sum_3.h>
#include <CGAL/Small_side_angle_bisector_decomposition_2.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
#include <CGAL/polygon_mesh_processing.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <vector>

typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron_3;
typedef CGAL::Nef_polyhedron_3<Kernel>              Nef_polyhedron_3;
typedef typename Kernel::Point_3                    Point_3;
typedef CGAL::Surface_mesh<Point_3>                 Surface_mesh;

typedef struct {
    std::vector<std::vector<double>> verts;
    std::vector<std::vector<uint32_t>> faces;
} mesh3d;

template <typename T1, typename T2>
Polygon_with_holes_2 get_minkowski(T1 p, T2 q) {
    return CGAL::minkowski_sum_2(p, q);
}

template <typename T1, typename T2>
Polygon_with_holes_2 get_minkowski_decomp(T1 p, T2 q) {
    CGAL::Small_side_angle_bisector_decomposition_2<Kernel> ssab_decomp1;
    CGAL::Small_side_angle_bisector_decomposition_2<Kernel> ssab_decomp2;
    return CGAL::minkowski_sum_2(p, q, ssab_decomp1, ssab_decomp2);
}

typedef struct {
    std::vector<std::vector<double>> verts;
    std::vector<std::vector<uint32_t>> faces;
} mesh3d;

mesh3d polyhedron_minkowski_sum_3(Polyhedron_3& p, Polyhedron_3& q) {
    // convert Polyhedron to NEF
    Nef_polyhedron_3 a(p);
    Nef_polyhedron_3 b(q);
    Nef_polyhedron_3 summed = CGAL::minkowski_sum_3(a, b);
    for (auto v = P.vertices_begin(); v != P.vertices_end(); ++v) {
        out << "v " << v->point().x() << " " << v->point().y() << " " << v->point().z() << "\n";
    }
    return summed;
}

void init_minkowski(py::module & m) {
    auto sub = m.def_submodule("minkowski");
    sub.def("minkowski_sum", &get_minkowski<Polygon_2, Polygon_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_2, Polygon_with_holes_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_with_holes_2>);
    sub.def("minkowski_sum", &polyhedron_minkowski_sum_3);
}
