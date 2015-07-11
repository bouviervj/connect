#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <lib/socket/socket.h>

#include <lib/extern/json/include/ArduinoJson.h>

void callbackReceiveMessage(net::socketconnect* sock, net::message* msg){

    DynamicJsonBuffer jsonBuffer;

    JsonObject& root = jsonBuffer.parseObject((char*) msg->_payload.c_str());
 
    const char* from = root["from"];
    printf("JSON from:%s\n", from);

    msg->_type = 1;
    msg->_payload = "{\"from\":\"super\", \"to\" : \"test\", \"services\" : [ { \"type\" : \"light\" } ] }";

    sock->sendMessage(msg);		

}

int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    net::socketconnect aSck(argv[1], 5000);
    aSck.setCallback(&callbackReceiveMessage);

    pthread_exit(0);    	    

    return 0;
}
