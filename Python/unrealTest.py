
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

#model = "E:/MY_GAME/UNREAL/Pearl_Git/Content/models/bunny_oded.obj"
model = "E:/MY_GAME/UNREAL/Pearl_Fracture_Git/Content/models/bunny_oded.obj"

v_fine, f_fine = igl.read_triangle_mesh(model)

v_fine = gpytoolbox.normalize_points(v_fine)
v, f = lazy_cage(v_fine,f_fine,num_faces=2000)

tgen = tetgen.TetGen(v,f)
nodes, elements = tgen.tetrahedralize()

modes = fracture.fracture_modes(nodes,elements) 
params = fracture.fracture_modes_parameters(num_modes=6,verbose=True,d=3)

#contact_point = nodes[1,:]
#direction = [1]
contact_point = [-0.49879505, 0.06937706, 0.15410506]
direction = np.array([1.0,0.0,0.0])

modes.compute_modes(parameters=params)
modes.impact_precomputation(v_fine=v_fine,f_fine=f_fine)
modes.impact_projection(contact_point=contact_point,direction=direction)
ui, gi,write_vertices,write_triangles, Vs, Fs = modes.return_ui_gi()



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

def get_str_write_vertices():
    tempStr = ""
    for i in write_vertices:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

def get_str_write_triangles():
    tempStr = ""
    for i in write_triangles:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

def get_str_Vs1():
    tempStr = ""
    for i in Vs[0]:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

def get_str_Fs1():
    tempStr = ""
    for i in Fs[0]:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

def get_str_Vs2():
    tempStr = ""
    for i in Vs[1]:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

def get_str_Fs2():
    tempStr = ""
    for i in Fs[1]:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

# filename = None
# igl.write_obj(filename, Vs[1], Fs[1])
# igl.write_obj(filename, Vs[0], Fs[0])

# vs1 = get_str_Vs1()
# print(len(vs1))
# vs2 = get_str_Vs2()
# print(len(vs2))
# print("calculate done")
# fs1 = get_str_Fs1()
# fs2 = get_str_Fs2()
# print(fs1)
# print("\n")
# print(fs2)




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



