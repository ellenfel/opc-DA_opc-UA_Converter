from time import sleep
from opcua import Client
from node import *

client = Client("opc.tcp://ellenfel-Inspiron-3580:4840")
client.connect()
rootNode = client.get_root_node()
print("\nroot node =", rootNode)

objectNode = client.get_objects_node()
print("\nobject node =", objectNode)

print("\nloading server hierarchie...\n")
sleep(3)

Server_Nodes = ServerNodes()
Server_Nodes.build_list(Node(client.get_objects_node(), None))
Server_Nodes.show_hierarchie()


#Server_Node.node_list[449].get_node_value()
#NameError

