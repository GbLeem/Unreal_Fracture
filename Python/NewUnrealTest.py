
import numpy as np
import sys
import igl
import tetgen
from context import fracture_utility as fracture
from context import gpytoolbox
from gpytoolbox.copyleft import lazy_cage

np.set_printoptions(threshold = sys.maxsize)

model = "E:/MY_GAME/UNREAL/Pearl_Fracture_Git/Content/models/bunny_oded.obj"

v_fine, f_fine = igl.read_triangle_mesh(model)

v_fine = gpytoolbox.normalize_points(v_fine)
v, f = lazy_cage(v_fine,f_fine,num_faces=2000)

tgen = tetgen.TetGen(v,f)
nodes, elements = tgen.tetrahedralize()

modes = fracture.fracture_modes(nodes,elements) 
params = fracture.fracture_modes_parameters(num_modes=6,verbose=True,d=3)


contact_point = nodes[1,:]
direction = np.array([1.0,0.0,0.0])

modes.compute_modes(parameters=params)
modes.impact_precomputation(v_fine=v_fine,f_fine=f_fine)
modes.impact_projection(contact_point=contact_point,direction=direction)
pieces, Vs, Fs = modes.new_return_ui_gi()

# 기본 형태의 vertex와 face를 보내는 작업
def first_send():
    vertex = get_str_Vs(v_fine)
    face = get_str_Fs(f_fine)
    return vertex, face

def runtime_impact_projection(impact):
    modes.impact_projection(contact_point = impact,threshold = 10.0, direction = direction)
    pieces, Vs, Fs = modes.new_return_ui_gi()

    return pieces, Vs, Fs

# =====================================for runtime pieces=================================
def get_num_str_vs(num_pieces, v=Vs):
    temp=""
    for idx in range(num_pieces):
        for iv in v[idx]:
            for iiv in iv:
                temp+=f"{iiv}@"
            temp += "#"
        temp += "%"
    return temp

def get_num_str_fs(num_pieces, f= Fs):
    temp=""
    for idx in range(num_pieces):
        for i_f in f[idx]:
            for iif in i_f:
                temp+=f"{iif}@"
            temp += "#"
        temp += "%"
    return temp

# =====================================test) input value================================
def get_str_Vs(v=Vs):
    tempStr = ""
    for i in v:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

def get_str_Fs(f=Fs):
    tempStr = ""
    for i in f:
        for j in i:
            tempStr+= f"{j}@"
        tempStr += "#"

    return tempStr

# ===================================== test) two pieces fracture===================================
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