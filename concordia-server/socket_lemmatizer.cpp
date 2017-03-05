#include "socket_lemmatizer.hpp"

#include "config.hpp"
#include <boost/lexical_cast.hpp>

SocketLemmatizer::SocketLemmatizer(int port) throw(ConcordiaException) :
                    _port(port) {
}

SocketLemmatizer::~SocketLemmatizer() {
}

/**
    Connect to a host on a certain port number
*/
bool SocketLemmatizer::_connect() {

    //Create socket
    _sock = socket(AF_INET , SOCK_STREAM , 0);
    if (_sock == -1) {
        throw ConcordiaException("Could not create socket for the lemmatizer.");
    }

    std::string address = "127.0.0.1";

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
    _server.sin_port = htons(_port);

    //Connect to remote server
    if (connect(_sock , (struct sockaddr *) & _server , sizeof(_server)) < 0) {
        throw ConcordiaException("Connect failed. Error on address: "+address+":"+boost::lexical_cast<std::string>(_port));
    }

    return true;
}

bool SocketLemmatizer::_disconnect() {
    close(_sock);
    _sock = -1;
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
    _connect();
    _send_data(languageCode+sentence+LEMMATIZER_DELIMITER);
    std::string reply = _receive(512);
    _disconnect();
    return reply.substr(0,reply.find(LEMMATIZER_DELIMITER));
}
