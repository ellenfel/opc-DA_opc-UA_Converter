#conda run -n OPCUA clientConnects.py
#host ip 	= 192.168.70.71
#portnumber 	= 12345


from opcua import Client
client=Client("opc.tcp://127.0.1.1:12345/")
client.connect()

print(client.get_namespace_array())
