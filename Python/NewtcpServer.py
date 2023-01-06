import socket
import NewUnrealTest as u

bIsImpact = False

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
        vertex, face = u.first_send()
        senddata = vertex+"%"+face+"$"
        client.send(senddata.encode())

        # for receive data
        impact = []

        data = client.recv(2048)
        data = data.decode("utf-8")
        print(data)        

        # receive impact data and calculate
        if(data != "ok"):
            bIsImpact = True
            print("impact ok!")
            temp = data.split("/")
            for t in temp:
                impact.append(float(t))

            pieces, Vs, Fs = u.runtime_impact_projection(impact)

            # 이게 아니고 Vs랑 Fs를 넣어서 pieces에 맞게 쪼개서 send 하는 것 구현하기
            # strui = u.get_str_Vs1()
            # strgi = u.get_str_Fs1()
            # strui2 = u.get_str_Vs2()
            # strgi2 = u.get_str_Fs2()

            # 새로운 VS, FS return 방식
            strVS = u.get_num_str_vs(pieces, Vs)
            strFS = u.get_num_str_fs(pieces, Vs)
            
            # re-send data from python to unreal
            #senddata = strui + "%" + strgi+ "$" + strui2 + "%" + strgi2
            senddata = str(pieces) + "&" +strVS +"$" +strFS
            
            client.send(senddata.encode())
            print("send ok!")

        client.close()