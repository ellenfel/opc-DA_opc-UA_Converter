from opcua import Client
from node import *
client = Client("opc.tcp://ellenfel-Inspiron-3580:4840")
client.connect()
client.get_root_node()
client.get_objects_node()
Server_Nodes = ServerNodes()
Server_Nodes.build_list(Node(client.get_objects_node(), None))
Server_Nodes.show_hierarchie()
Server_Node.node_list[449].get_node_value()
