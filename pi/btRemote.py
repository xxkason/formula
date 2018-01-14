import bluetooth

BT_SERVER_PORT = 1
server_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
server_socket.bind(("", BT_SERVER_PORT))
server_socket.listen(1) # Server listens to accept 1 connection at a time

print "Waiting for a connection..."
client_socket, address = server_socket.accept()
print "Welcom ", address

while True:
    cmd = client_socket.recv(128)
    print "Received command: %s" % cmd
    if cmd == 'f':
        print "Yes, my lord. Go! Go! Go!"
    elif cmd == 'b':
        print "Yes, my lord. Go backward"
    elif cmd == 'l':
        print "Turning left"
    elif cmd == 'r':
        print "Turning right"
    elif cmd == 's':
        print "Stop now!"
    elif cmd == 'acc':
        print "Turbo!"
    elif cmd == 'deacc':
        print "Slow down..."
    elif cmd == 'q':
        print "Quit"
        break

client_socket.close()
server_socket.close()