import socket
import NewUnrealTest as u

if __name__ == "__main__":
    ip = "127.0.0.1"
    port = 1112

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((ip, port))
    server.listen(1)

    while True:
        client, address = server.accept()
        print(f"Connection Established - {address[0]}:{address[1]}")

        # send data
        strui = u.get_str_Vs1()
        strgi = u.get_str_Fs1()
        strui2 = u.get_str_Vs2()
        strgi2 = u.get_str_Fs2()

        senddata = strui + "%" + strgi+ "$" + strui2 + "%" + strgi2
        client.send(senddata.encode())

        # for receive data
        #impact = []
        impact = [-0.49879505, 0.06937706, 0.15410506]

        data = client.recv(2048)
        data = data.decode("utf-8")
        print(data)        

        if(data[0] == "X"):
            pieces, Vs, Fs = u.runtime_impact_projection(impact)
            strui = u.get_str_Vs(Vs)
            strgi = u.get_str_Fs(Fs)
            senddata = strui + "%"+strgi+"$"
            client.send(senddata.encode())
            
        client.close()