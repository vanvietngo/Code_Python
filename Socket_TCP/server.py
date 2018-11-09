import socket

def getInfo():
    global HOST_NAME
    HOST_NAME = socket.gethostname()
    global HOST_ADDRESS
    HOST_ADDRESS = socket.gethostbyname(HOST_NAME)
    print("HOST NAME : ",HOST_NAME)
    print("HOST ADDRESS : ",HOST_ADDRESS)

def choosePort():
    global PORT
    PORT = int(input("chon PORT de su dung : "))

def createSever():
    soc = socket.socket(socket.AF_INET,socket.SOCK_STREAM,0) #welcom tro TCP socket
    soc.bind((HOST_ADDRESS,PORT)) #GAN SEVER PORT = 
    soc.listen(1)
    clientConection, clientAdr = soc.accept()
    print("Conected to Client : ",clientAdr)
    # con = "Conected to Sever !!"
    # clientConection.send(con.encode())
    # con1 = clientConection.recv(1024).decode()
    # print(con1)

    while 1:

        content = input("send to Client : ")
        clientConection.send(content.encode())
        content2 = clientConection.recv(1024).decode()
        print("Client say     : ",content2)

if(__name__ == "__main__"):
    getInfo()
    choosePort()
    createSever()

