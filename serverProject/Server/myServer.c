#include <open62541.h>

#include <signal.h>
#include <stdlib.h>

static volatile UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

UA_Double Temperature = 20.0;

int main(int argc, char * argv[]) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();

    //Check for agruments
    if(argc > 2)	//hostname or ip address and a port number are available
    {
	    UA_Int16 port_number = atoi(argv[2]);
	    UA_ServerConfig_setMinimal(UA_Server_getConfig(server), port_number, 0);
    }
    else
	    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    //Check for arguments
    if(argc > 1)
    {		//hostname or ip address available
	//copy the hostname from char * to an open62541 variable
	UA_String hostname;
	UA_String_init(&hostname);
	hostname.length = strlen(argv[1]);
	hostname.data = (UA_Byte *) argv[1];

	//Change the configuration
	/* There is an issue with master branch relitive links:
	 * https://github.com/open62541/open62541/issues/5132
	 * https://github.com/open62541/open62541/issues/5159
	 * they removed the function. Now you can set an array of URLs instead of the hostname
	 *
	 *
	 * */
	UA_ServerConfig_setCustomHostname(UA_Server_getConfig(server), hostname);
    }

    //Add a new namespace to the server
    UA_Int16 ns_room1 = UA_Server_addNamespace(server,"Room1");
    UA_LOG_INFO(UA_log_Stdout, UA_LOGCATEGORY_USERLAND, "New namespace added with Nr. %d", ns_room1);

    //Add a new object called Temperature Sensor
    UA_NodeId r1_tempsens_Id; /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    UA_Server_addObjectNode(server,UA_NODEID_STRING(2,"R1_TemperatureSensor"),
		    UA_NODEID_NUMERIC(0,UA_NS0ID_OBJECTSFOLDER),
		    UA_NODEID_NUMERIC(0,UA_NS0ID_ORGANIZES),
		    UA_QUALIFIEDNAME(2, "Temperature Sensor"),
		    UA_NODEID_NUMERIC(0,UA_NS0ID_BASEOBJECTTYPE),
		    oAttr, NULL, &r1_tempsens_Id);

    //Add the variable vendor Name to server
    UA_VariableAttributes vnAttr = UA_VariableAttributes_default;
    UA_String vendorName = UA_STRINNG("Temp Sensors Ltd.");
    UA_Variant_setScalar(&vnAttr.value, &vendorName, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_VendorName"), r1_rempsens_Id,
		    UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
		    UA_QUALIFIEDNAME(2, "Vendor Name"), 
		    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), vnAttr, NULL, NULL);

    //Add the variable Serial Number to server
    UA_VariableAttributes snAttr = UA_VariableAttributes_default;
    UA_Int32 serialNumber = 12345678;
    UA_Variant_setScalar(&snAttr.value, &serialNumber, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_SerialNumber"), r1_rempsens_Id,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                    UA_QUALIFIEDNAME(2, "SerialNumber"),
                    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), snAttr, NULL, NULL);

    //Add the variable Temperature to server 
    UA_VariableAttributes tpAttr = UA_VariableAttributes_default;
    UA_Variant_setScalar(&tpAttr.value, &Temperature, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, "R1_TS1_Temperature"), r1_rempsens_Id,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                    UA_QUALIFIEDNAME(2, "Temperature"),
                    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), tpAttr, NULL, NULL)


    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server...");
    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Server was shut down.");

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
