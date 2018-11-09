import socket

def chooseSever():
    global SEVER_ADDRESS
    SEVER_ADDRESS = (input("Chon Sever_Address : "))
    global PORT
    PORT = int(input("Chon Port : "))

def createClient():
    soc = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0)
    soc.connect((SEVER_ADDRESS,PORT))
    print("Sever conected to Client !!\n")
    # soc.send("Client conected !!".encode())
    # content1 = soc.recv(1024).decode()
    # print(content1)
    # con = soc.send("Clien conected to Sever !!")
    while 1:
        content1 = soc.recv(1024).decode()
        print(content1)
        content2 = input("send data to Sever : ")
        soc.sendall(content2.encode())
        if content2 == "close":
            soc.sendall("Client disconected . Google !!")
            soc.close

if(__name__ == "__main__"):
    chooseSever()
    createClient()