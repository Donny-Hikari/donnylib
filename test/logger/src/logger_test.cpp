
#include <string>
#include <boost/test/minimal.hpp>
#include <iostream>

#include <donny/logger.hpp>

using namespace donny;
using namespace donny::filesystem;

int test_main(int, char**)
{
    file logfile("test-output.txt", "w");
    donny::logger<> log(logfile);
    
    std::string sHello = "Hello %s\n";
    std::string sWorld = "World";
    int nHelloWorld = sHello.size() - 2 + sWorld.size();
    
    BOOST_CHECK( log.print(sHello, sWorld.c_str()) == nHelloWorld );
    
    BOOST_CHECK( log.println(sHello, sWorld.c_str()) == nHelloWorld + logfile.nLineBreak );

    BOOST_CHECK( log.i(sHello, sWorld.c_str()) > nHelloWorld + logfile.nLineBreak );

    log << "True: " << true << donny::endl;
    log << "int: " << 123 << donny::endl;
    log << "double: " << 1.0f/3 << donny::endl;

    std::string sHelloWorld = "Hello World";
    log << sHelloWorld << donny::endl;
    log.i() << sHelloWorld << donny::endl;
    log.e() << sHelloWorld << donny::endl;
    log.d() << sHelloWorld << donny::endl;
    log.v() << sHelloWorld << donny::endl;

    return 0;
}
