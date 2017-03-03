#include "socket_lemmatizer.hpp"

SocketLemmatizer::SocketLemmatizer() throw(ConcordiaException) :
                    _sock(-1) {
    _connect("127.0.0.1" , 11000);
}

SocketLemmatizer::~SocketLemmatizer() {
}

/**
    Connect to a host on a certain port number
*/
bool SocketLemmatizer::_connect(std::string address , int port)
{
    //create socket if it is not already created
    if(_sock == -1) {
        //Create socket
        _sock = socket(AF_INET , SOCK_STREAM , 0);
        if (_sock == -1) {
            throw ConcordiaException("Could not create socket for the lemmatizer.");
        }
    }

    //setup address structure
    if(inet_addr(address.c_str()) == -1) {
        struct hostent *he;
        struct in_addr **addr_list;

        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL) {
            //gethostbyname failed
            throw ConcordiaException("gethostbyname: Failed to resolve hostname");
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;

        for(int i = 0; addr_list[i] != NULL; i++) {
            _server.sin_addr = *addr_list[i];
            break;
        }
    } else { //plain ip address
        _server.sin_addr.s_addr = inet_addr(address.c_str());
    }

    _server.sin_family = AF_INET;
    _server.sin_port = htons(port);

    //Connect to remote server
    if (connect(_sock , (struct sockaddr *) & _server , sizeof(_server)) < 0) {
        throw ConcordiaException("connect failed. Error");
    }

    return true;
}

/**
    Send data to the connected host
*/
bool SocketLemmatizer::_send_data(std::string data)
{
    //Send some data
    if(send(_sock , data.c_str() , strlen(data.c_str() ) , 0) < 0)  {
        throw ConcordiaException("Send failed");
    }
    return true;
}

/**
    Receive data from the connected host
*/
std::string SocketLemmatizer::_receive(int size=512)
{
    char buffer[size];
    std::string reply;

    //Receive a reply from the server
    if(recv(_sock , buffer , sizeof(buffer) , 0) < 0) {
        throw ConcordiaException("Receive failed");
    }
    reply = buffer;
    return reply;
}

std::string SocketLemmatizer::lemmatizeSentence(std::string languageCode, std::string sentence) {
    _send_data(languageCode+sentence+"@#@");
    std::string reply = _receive(512);
    return reply.substr(0,reply.find("@#@"));
}
