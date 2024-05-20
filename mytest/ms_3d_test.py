from matplotlib import pyplot as plt
import open3d as o3d
import os
import trimesh
import numpy as np

import skgeom as sg
from skgeom.draw import draw
from skgeom import minkowski

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

def open3d_trimesh_list_visualizer(coords_3d_list, indices_3d_list, **option):
    ### Set options
    mesh_show_wireframe = option.get('mesh_show_wireframe', True)
    mesh_show_back_face = option.get('mesh_show_back_face', True)
    axis_mesh = [o3d.geometry.TriangleMesh.create_coordinate_frame(0.5)] if option.get('show_axis_mesh', True) else []
    ### Import open3d
    ### Convert as open3d mesh
    o3d_mesh_list = []
    for coords_3d, indices_3d in zip(coords_3d_list, indices_3d_list):
        o3d_mesh_list.append(o3d.geometry.TriangleMesh(o3d.utility.Vector3dVector(coords_3d), o3d.utility.Vector3iVector(indices_3d)))
    ### Visualize
    vis_list = o3d_mesh_list + axis_mesh # add origin coordinate
    o3d.visualization.draw_geometries(vis_list, mesh_show_wireframe=mesh_show_wireframe, mesh_show_back_face=mesh_show_back_face)

def get_mesh(path):
    path = os.path.expanduser(path)
    mesh = trimesh.load(path, force='mesh')
    return np.array(mesh.vertices), np.array(mesh.faces)

poly1 = sg.Polyhedron3(*get_mesh("./mytest/hexagonal.stl"))
poly2 = sg.Polyhedron3(*get_mesh("./mytest/key.stl"))

result = minkowski.minkowski_sum(poly1, poly2)

open3d_trimesh_list_visualizer([result.verts], [result.faces], show_axis_mesh=False)