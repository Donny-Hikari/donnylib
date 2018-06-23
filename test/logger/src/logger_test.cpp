
#include <string>
#include <iostream>
#include <chrono>

#include <boost/test/minimal.hpp>

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

    log << "True: " << (1 == 1) << endl;
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

    log.log() << "The next line shouldn't be displayed." << endl;
    log.close();
    log.log() << "This line won't be displayed." << endl;

    return 0;
}

int pressure_test()
{
    using namespace std::chrono;

    printf("Carrying out pressure test...\n");
    
    const ulong times = 999999L;
    auto tb = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    // tb = time(NULL);
    donny::logger<> log(file("pressure-test.txt", "w"));
    for (ulong i = 0; i < times; ++i)
        log << i << " messages" << endl;
    // te = time(NULL);
    auto te = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

    printf("Printed %lu messages in %ld ms.\n", times, te - tb);

    return 0;
}

int test_main(int, char**)
{
    test_logger();
    test_wlogger();
    test_screen_logger();
    pressure_test();

    return 0;
}
