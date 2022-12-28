import socket
import unrealTest as u

#modes = u.get()
ui = u.get_ui()
gi = u.get_gi()

if __name__ == "__main__":
    ip = "127.0.0.1"
    port = 1111

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((ip, port))
    server.listen(1)

    while True:
        client, address = server.accept()
        print(f"Connection Established - {address[0]}:{address[1]}")


        #senddata = str(1234)
        #client.send(bytes(senddata, "utf-8"))
        #client.send(senddata.encode())

        #senddata2 = str(modes.elements)
        #senddata = str(ui) + "/" + str(gi)
        #senddata = str(ui) + "%" + str(gi)
        strui = u.get_str_ui()
        strgi = u.get_str_gi()
        senddata = strui + "%" + strgi
        client.send(senddata.encode())

        data = client.recv(2048)
        data = data.decode("utf-8")
        print(data)

        client.close()