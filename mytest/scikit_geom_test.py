import skgeom as sg
from skgeom.draw import draw
from skgeom import minkowski

from matplotlib import pyplot as plt

# p1_verts = [sg.Point2(-2, 2), sg.Point2(3, 2), sg.Point2(3, 3), sg.Point2(0, 4), sg.Point2(-4, 2), 
#             sg.Point2(-4, -3), sg.Point2(-4, -3), sg.Point2(0, -5), sg.Point2(3, -4), sg.Point2(3, -3), sg.Point2(-2, -3)]
# p1_verts = sg.Polygon([sg.Point2(3, 3), sg.Point2(0, 4), sg.Point2(-4, 2), sg.Point2(-4, -3), sg.Point2(-4, -3), sg.Point2(0, -5), sg.Point2(3, -4)])
# p1_holes = sg.Polygon([sg.Point2(-2, 2), sg.Point2(3, 2), sg.Point2(3, -3), sg.Point2(-2, -3)])
# p1 = sg.PolygonWithHoles(p1_verts, [p1_holes])
# p2 = sg.Polygon([sg.Point2(0.68, 1.50), sg.Point2(-1.48, -0.53), sg.Point2(0.70, -2.58), sg.Point2(2.92, -0.63)])
# draw(p1, facecolor='red')
# draw(p2, facecolor='blue')
# plt.show()
# exit()

poly1 = sg.polyhedron_from_string("./hexagonal.obj")
poly2 = sg.polyhedron_from_string("./key.obj")

result = minkowski.minkowski_sum(poly1, poly2)

res_verts = []
res_faces = []

for vert in list(result.vertices):
    res_verts.append([vert.x(), vert.y(), vert.z()])

for face in list(result.faces):
    # print(face.is_triangle())
    print(face)

    # print(face.halfedge().vertex().point())
    # print(face.halfedge().next().vertex().point())
    # print("!!")
    # exit()
    # res_faces.append([vert.point().x(), vert.point().y(), vert.point().z()])


# result = minkowski.minkowski_sum(p1, p2)

# plt.show()

