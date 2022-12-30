import socket
import unrealTest as u

if __name__ == "__main__":
    ip = "127.0.0.1"
    port = 1111

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((ip, port))
    server.listen(1)

    while True:
        client, address = server.accept()
        print(f"Connection Established - {address[0]}:{address[1]}")
        
        strui = u.get_str_Vs1()
        strgi = u.get_str_Fs1()
        strui2 = u.get_str_Vs2()
        strgi2 = u.get_str_Fs2()

        #if(data == "ok"):
        senddata = strui + "%" + strgi+ "$" + strui2 + "%" + strgi2
        client.send(senddata.encode())

        data = client.recv(2048)
        #if(data is not None):
        data = data.decode("utf-8")
        print(data)

        client.close()