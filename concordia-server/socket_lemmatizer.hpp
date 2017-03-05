#ifndef SOCKET_LEMMATIZER_HDR
#define SOCKET_LEMMATIZER_HDR

#include <string>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent
#include <unistd.h>

#include <concordia/concordia_exception.hpp>


class SocketLemmatizer {
public:
    /*! Constructor.
    */
    explicit SocketLemmatizer(int port) throw(ConcordiaException);
    /*! Destructor.
    */
    virtual ~SocketLemmatizer();

    std::string lemmatizeSentence(std::string languageCode, std::string sentence);
private:
    bool _connect();

    bool _disconnect();

    bool _send_data(std::string data);

    std::string _receive(int size);

    int _port;

    int _sock;

    struct sockaddr_in _server;
};

#endif
