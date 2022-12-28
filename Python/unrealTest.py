
import numpy as np
import sys
import igl
import tetgen
from context import fracture_utility as fracture
from context import gpytoolbox
from gpytoolbox.copyleft import lazy_cage

#a = "data\bunny_oded.obj"
#b = "E:\CODING\my-fracture\data\bunny_oded.obj"
#c = "E:/MY_GAME/UNREAL/PearlAbyss_project/Content/MyTask/Fracture-modes/Models/bunny/bunny_oded.obj"
#d = "E:/MY_GAME/UNREAL/PearlAbyss_project/Content/MyTask/Fracture-modes/Models/ice/ice.obj"

np.set_printoptions(threshold = sys.maxsize)
#np.set_printoptions(precision=8)

model = "E:/MY_GAME/UNREAL/Pearl_Git/Content/models/bunny_oded.obj"

#ue.ProceduralMeshLibrary.get_section_from_static_mesh
v_fine, f_fine = igl.read_triangle_mesh(model)

v_fine = gpytoolbox.normalize_points(v_fine)
v, f = lazy_cage(v_fine,f_fine,num_faces=2000)

tgen = tetgen.TetGen(v,f)
nodes, elements = tgen.tetrahedralize()


# def makearray():
#     nodeArray = unreal.Array()
#     nodeArray.append(nodes)
#     return nodeArray

# print(makearray())

modes = fracture.fracture_modes(nodes,elements) 
params = fracture.fracture_modes_parameters(num_modes=3,verbose=True,d=3)

contact_point = nodes[1,:]
direction = np.array([1.0,0.0,0.0])

modes.compute_modes(parameters=params)
modes.impact_precomputation(v_fine=v_fine,f_fine=f_fine)
modes.impact_projection(contact_point=contact_point,direction=direction)
ui, gi = modes.return_ui_gi()


def get():
    return modes

def get_ui():
    return ui

def get_gi():
    return gi

def get_str_gi():
    tempStr = ""
    for i in gi:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

def get_str_ui():
    tempStr = ""
    for i in ui:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr


# print(type(ui))
# sui = np.array2string(ui)

# #sgi = gi.tostring()
# f = open("UIandGI.txt","w")
# for i in sui:
#     f.write(i)

#f.close()



# modes.impact_projection(contact_point=contact_point,direction=direction)


# compute modes의 return 값
#exploded_vertices, exploded_elements, modes, labels, tet_to_vertex_matrix, tet_neighbors,
#massmatrix, unexploded_to_exploded_matrix


# 필요한것
# impact_precomputed, impact_projected -> boolean
# modes.shape
# modes.vertices
# modes.vertices.shape
# modes.elements
# piece_impact
# wave_piece_lsqr

# print(modes.impact)
# print(type(modes.impact))
# print(contact_point)
# print(type(contact_point))



