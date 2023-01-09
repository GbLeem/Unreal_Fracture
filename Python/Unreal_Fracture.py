import numpy as np
import igl
import tetgen
from context import fracture_utility as fracture
from context import gpytoolbox
from gpytoolbox.copyleft import lazy_cage

model = "E:/MY_GAME/UNREAL/Pearl_Fracture_Git/Content/models/bunny_oded.obj"    
class Unreal_Fracture:
    def __init__(self):
        self.v_fine, self.f_fine = igl.read_triangle_mesh(model)
        self.v_fine = gpytoolbox.normalize_points(self.v_fine)
        self.v, self.f = lazy_cage(self.v_fine, self.f_fine, num_faces=2000)

        self.tgen = tetgen.TetGen(self.v,self.f)
        self.nodes, self.elements = self.tgen.tetrahedralize()

        self.modes = fracture.fracture_modes(self.nodes,self.elements) 
        self.params = fracture.fracture_modes_parameters(num_modes=3,verbose=True,d=1)

        self.contact_point = self.nodes[1,:]
        self.direction = np.array([1.0,0.0,0.0])

        self.modes.compute_modes(parameters=self.params)
        self.modes.impact_precomputation(v_fine=self.v_fine,f_fine=self.f_fine)
        self.modes.impact_projection(contact_point=self.contact_point)
        self.pieces, self.Vs, self.Fs = self.modes.new_return_ui_gi()

    # for first send data
    def get_str_Vs(self, v):
        tempStr = ""
        for i in v:
            for j in i:
                tempStr+= f"{j}@"
            tempStr += "#"
        return tempStr

    def get_str_Fs(self, f):
        tempStr = ""
        for i in f:
            for j in i:
                tempStr+= f"{j}@"
            tempStr += "#"
        return tempStr

    def first_send(self):
        vertex = self.get_str_Vs(self.v_fine)
        face = self.get_str_Fs(self.f_fine)
        return vertex, face

    def runtime_impact_projection(self, impact):
        direction = np.array([1])
        self.modes.impact_projection(contact_point = impact, threshold = 10.0, direction = direction)
        #self.pieces, self.Vs, self.Fs = self.modes.new_return_ui_gi()
        self.pieces, self.Vs, self.Fs = self.modes.new_return_ui_gi_v2()
        print(len(self.Vs))
        print(len(self.Fs))
        print(self.pieces)
        #return pieces, Vs, Fs

    # for impact send data
    def get_num_str_vs(self, num_pieces, v):
        temp=""
        for idx in range(num_pieces):
            if(len(v)<=idx):
                break
            for iv in v[idx]:
                for iiv in iv:
                    temp+=f"{iiv}@"
                temp += "#"
            temp += "%"
        return temp

    def get_num_str_fs(self, num_pieces, f):
        temp=""
        for idx in range(num_pieces):
            if(len(f)<=idx):
                break
            for i_f in f[idx]:
                for iif in i_f:
                    temp+=f"{iif}@"
                temp += "#"
            temp += "%"
        return temp