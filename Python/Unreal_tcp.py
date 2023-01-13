import socket
import Unreal_Fracture as u

# init 
Fracture_modes = u.Unreal_Fracture()

if __name__ == "__main__":
    ip = "127.0.0.1"
    port = 1111

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((ip, port))
    server.listen(1)
    
    while True:
        client, address = server.accept()
        print(f"Connection Established - {address[0]}:{address[1]}")

        # first send
        vertex, face = Fracture_modes.first_send()
        senddata = vertex+"%"+face+"$"
        client.send(senddata.encode())

        # receive data (impact point or "ok")
        impact = []
        data = client.recv(2048)
        data = data.decode("utf-8")
        print(data)        

        # receive impact data and calculate
        if(data != "ok"):
            print("impact ok!")
            temp = data.split("/")
            for t in temp:
                impact.append(float(t))

            # make new fracture model with new impact point
            Fracture_modes.runtime_impact_projection(impact)

            # VS, FS return
            strVS = Fracture_modes.get_num_str_vs(Fracture_modes.pieces, Fracture_modes.Vs)
            strFS = Fracture_modes.get_num_str_fs(Fracture_modes.pieces, Fracture_modes.Fs)
            
            # re-send data from python to unreal
            senddata = str(Fracture_modes.pieces) + "&" +strVS +"$" +strFS
            
            client.send(senddata.encode())
            print("send ok!")

        client.close()