#include <iostream>
#include <sstream>
#include <string>
#include <boost/algorithm/string/replace.hpp>
#include <fcgio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.hpp"
#include "config.hpp"
#include "concordia_server.hpp"

static const unsigned long STDIN_MAX = 1000000;

static std::string get_request_content(const FCGX_Request & request) {
    char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
    unsigned long content_length = STDIN_MAX;

    if (content_length_str) {
        content_length = strtol(content_length_str, &content_length_str, 10);
        if (*content_length_str) {
            std::cerr << "Can't Parse 'CONTENT_LENGTH='"
                      << FCGX_GetParam("CONTENT_LENGTH", request.envp)
                      << "'. Consuming stdin up to " << STDIN_MAX << std::endl;
        }

        if (content_length > STDIN_MAX) {
            content_length = STDIN_MAX;
        }
    } else {
        // Do not read from stdin if CONTENT_LENGTH is missing
        content_length = 0;
    }

    char * content_buffer = new char[content_length];
    std::cin.read(content_buffer, content_length);
    content_length = std::cin.gcount();

    // Chew up any remaining stdin - this shouldn't be necessary
    // but is because mod_fastcgi doesn't handle it correctly.

    // ignore() doesn't set the eof bit in some versions of glibc++
    // so use gcount() instead of eof()...
    do std::cin.ignore(1024); while (std::cin.gcount() == 1024);

    std::string content(content_buffer, content_length);
    delete [] content_buffer;
    return content;
}

int main(int argc, char** argv) {
    Logger::log("Concordia server process start");

    // Backup the stdio streambufs
    std::streambuf * cin_streambuf  = std::cin.rdbuf();
    std::streambuf * cout_streambuf = std::cout.rdbuf();
    std::streambuf * cerr_streambuf = std::cerr.rdbuf();

    ConcordiaServer concordiaServer(CONFIG_FILE_PATH);
    Logger::log("Concordia server initiated successfully, waiting for requests");

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        std::cin.rdbuf(&cin_fcgi_streambuf);
        std::cout.rdbuf(&cout_fcgi_streambuf);
        std::cerr.rdbuf(&cerr_fcgi_streambuf);

        std::string content = get_request_content(request);
        
        std::string requestString(content);
        std::cout << concordiaServer.handleRequest(requestString);
        
        // Note: the fcgi_streambuf destructor will auto flush
    }

    // restore stdio streambufs
    std::cin.rdbuf(cin_streambuf);
    std::cout.rdbuf(cout_streambuf);
    std::cerr.rdbuf(cerr_streambuf);
 
    Logger::log("Shutting down Concordia server process");

    return 0;
}
