
#include <string>
#include <boost/test/minimal.hpp>
#include <iostream>

#include <donny/logger.hpp>

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

    log << "True: " << true << endl;
    log << "int: " << 123 << endl;
    log << "double: " << 1.0f/3 << endl;

    std::string sHelloWorld = "Hello World";
    log << sHelloWorld << endl;
    log.i() << sHelloWorld << endl;
    log.e() << sHelloWorld << endl;
    log.d() << sHelloWorld << endl;
    log.v() << sHelloWorld << endl;

    return 0;
}
