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


customNodeSerialNumber= Server_Nodes.node_list[455].get_node_value()
print("\nSerial Number of Temperature Sensor :",customNodeSerialNumber)

customNodeTemp= Server_Nodes.node_list[454].get_node_value()
print("\nOutput From Temperature Sensor :",customNodeTemp)

customNodeVendorName= Server_Nodes.node_list[456].get_node_value()
print("\nVendor Name :",customNodeVendorName)

sleep(3)
Server_Nodes.node_list[2].node.set_value(True)

sleep(3)
Server_Nodes.node_list[2].node.set_value(False)


