
#include <string>
#include <boost/test/minimal.hpp>
#include <iostream>

#include <donny/logger.hpp>

using namespace donny::filesystem;

int test_logger()
{
    file logfile("test-output.txt", "w");
    donny::logger<> log(logfile);
    
    std::string sHello = "Hello %s\n";
    std::string sWorld = "World";
    int nHelloWorld = sHello.length() - 2 + sWorld.length();
    
    BOOST_CHECK( log.print(sHello, sWorld.c_str()) == nHelloWorld );
    
    BOOST_CHECK( log.println(sHello, sWorld.c_str()) == nHelloWorld + logfile.nLineBreak );

    BOOST_CHECK( log.i(sHello, sWorld.c_str()) > nHelloWorld + logfile.nLineBreak );

    std::string sHelloWorld = "Hello World";
    log << sHelloWorld << endl;
    log.i() << sHelloWorld << endl;
    log.e() << sHelloWorld << endl;
    log.d() << sHelloWorld << endl;
    log.v() << sHelloWorld << endl;
    log.log() << sHelloWorld << endl;

    log.println("");
    log << endl;

    log << "True: " << true << endl;
    log.i() << "int: " << 123 << endl;
    log.e() << "double: " << 1.0f/3 << endl;
    log.d() << "char: " << 'c' << endl;
    log.v() << "ulong: " << 342344534ul << endl;
    log.log() << "ushort: " << (short)19 << endl;

    log.setPrefix(log.LOG, "[CUSTOM] ");
    log.log() << "byte: " << (u_char)28 << endl;

    return 0;
}

// Test wide char
int test_wlogger()
{
    wfile logfile("test-woutput.txt", "w");
    donny::logger<wchar_t> log(logfile);
    
    std::wstring sHello = L"Hello %ls\n";
    std::wstring sWorld = L"World";
    int nHelloWorld = sHello.length() - 3 + sWorld.length();

    BOOST_CHECK( log.print(sHello, sWorld.c_str()) == nHelloWorld );
    
    BOOST_CHECK( log.println(sHello, sWorld.c_str()) == nHelloWorld + logfile.nLineBreak );

    BOOST_CHECK( log.i(sHello, sWorld.c_str()) > nHelloWorld + logfile.nLineBreak );

    std::wstring sHelloWorld = L"Hello World";
    log << sHelloWorld << endl;
    log.i() << sHelloWorld << endl;
    log.e() << sHelloWorld << endl;
    log.d() << sHelloWorld << endl;
    log.v() << sHelloWorld << endl;
    log.log() << sHelloWorld << endl;

    log.println(L"");
    log << endl;

    log << L"True: " << true << endl;
    log.i() << L"int: " << 123 << endl;
    log.e() << L"double: " << 1.0f/3 << endl;
    log.d() << L"char: " << L'c' << endl;
    log.v() << L"ulong: " << 342344534ul << endl;
    log.log() << L"ushort: " << (short)19 << endl;

    log.setPrefix(log.LOG, L"[CUSTOM] ");
    log.log() << L"byte: " << (u_char)28 << endl;

    return 0;
}

int test_screen_logger()
{
    donny::logger<> log(dout);

    log.println("Hello World");

    log << "True: " << true << endl;
    log.i() << "int: " << 123 << endl;
    log.e() << "double: " << 1.0f/3 << endl;
    log.d() << "char: " << 'c' << endl;
    log.v() << "ulong: " << 342344534ul << endl;
    log.log() << "ushort: " << (short)19 << endl;

    log.setPrefix(log.LOG, "[CUSTOM] ");
    log.log() << "Custom prefix." << endl;
    log.log() << "byte: " << (u_char)28 << endl;

    log.setTimeStampFormat("{%F}{%r}");
    log.log() << "New timestamp format." << endl;

    log.useTimeStamp(false);
    log.log() << "No timestamp." << endl;

    return 0;
}

int test_main(int, char**)
{
    test_logger();
    test_wlogger();
    test_screen_logger();

    return 0;
}
