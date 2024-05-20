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
#include <array>
#include "mesh3d.hpp"

typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron_3;
typedef CGAL::Nef_polyhedron_3<Kernel>              Nef_polyhedron_3;
typedef typename Kernel::Point_3                    Point_3;
typedef CGAL::Surface_mesh<Point_3>                 Surface_mesh;

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

mesh3d polyhedron_minkowski_sum_3(Polyhedron_3& p, Polyhedron_3& q) {
    // convert Polyhedron to NEF
    Nef_polyhedron_3 a(p);
    Nef_polyhedron_3 b(q);
    Nef_polyhedron_3 summed = CGAL::minkowski_sum_3(a, b);

    Polyhedron_3 P;
    // CGAL::convert_nef_polyhedron_to_polygon_mesh(summed, P); // not true
    summed.convert_to_Polyhedron(P);
    // a.convert_to_Polyhedron(P);

    std::vector<double> vertices;
    for (auto v = P.vertices_begin(); v != P.vertices_end(); ++v) {
        vertices.push_back(CGAL::to_double(v->point().x()));
        vertices.push_back(CGAL::to_double(v->point().y()));
        vertices.push_back(CGAL::to_double(v->point().z()));
    }

    std::vector<uint32_t> faces;
    for (auto f = P.facets_begin(); f != P.facets_end(); ++f) {
        auto h = f->facet_begin();
        do {
            faces.push_back(std::distance(P.vertices_begin(), h->vertex()));
        } while (++h != f->facet_begin());
    }

    py::array_t<double> vertex_array(vertices.size(), vertices.data());
    py::array_t<uint32_t> face_array(faces.size(), faces.data());

    vertex_array.resize({static_cast<int>(vertices.size() / 3), 3});
    face_array.resize({static_cast<int>(faces.size() / 3), 3});

    mesh3d mesh_out;
    mesh_out.verts = vertex_array;
    mesh_out.faces = face_array;

    return mesh_out;
}

void init_minkowski(py::module & m) {
    auto sub = m.def_submodule("minkowski");
    sub.def("minkowski_sum", &get_minkowski<Polygon_2, Polygon_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_2, Polygon_with_holes_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_with_holes_2>);
    sub.def("minkowski_sum", &polyhedron_minkowski_sum_3);
}
