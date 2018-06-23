
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <typeinfo>

#define BOOST_TEST_MODULE filesystem
#include <boost/test/included/unit_test.hpp>
// #include <boost/test/minimal.hpp>

#include <donny/filesystem.hpp>

using namespace std;
using namespace donny::filesystem;

const string text[] = {
    u8"豆沙包",
    u8"好吃！",
};
const int len = donny::length_of_array(text);

BOOST_AUTO_TEST_CASE( writefile )
{
    file ofs("test.txt", "wb");
    for (string str : text) {
        ofs.puts(str);
        ofs.newLine();
    }
    ofs.flush(); // need flush to write data to disk at once, or ifs will read nothing.
    ofs.close();

    ifstream ifs("test.txt", ios_base::binary);
    string tmp;
    int ind = 0;
    while (std::getline(ifs, tmp)) {
        BOOST_REQUIRE(ind < len);
        BOOST_CHECK(tmp == text[ind++]);
    }
    BOOST_CHECK(ind == len);
}

BOOST_AUTO_TEST_CASE( readfile )
{
    ofstream ofs("test.txt", ios_base::binary);
    for (string str : text) {
        ofs << str << endl;
    }
    ofs.flush(); // need flush to write data to disk at once, or ifs will read nothing.
    ofs.close();

    file ifs("test.txt", "rb");
    int ind = 0;
    string tmp = ifs.gets('\n', false);
    while (!tmp.empty()) {
        BOOST_REQUIRE(ind < len);
        BOOST_CHECK(tmp == text[ind++]);
        tmp = ifs.gets('\n', false);
    }
    BOOST_CHECK(ind == len);
    ifs.close();
}

const u16string u16text[] = {
    u"豆沙包",
    u"豆沙包",
};
const int u16len = donny::length_of_array(u16text);

BOOST_AUTO_TEST_CASE( writeu16file )
{
    u16file ofs("u16test.txt", "wb");
    for (u16string str : u16text) {
        ofs.puts(str);
        ofs.newLine();
    }    
    // locale loc(locale(), new codecvt_utf8_utf16<wchar_t>);
    // ofs.write((char*)str.c_str(), str.length() * sizeof(str[0]));
    ofs.flush(); // need flush to write data to disk at once, or ifs will read nothing.
    ofs.close();

    ifstream ifs("u16test.txt", std::ios_base::binary);
    u16string tmp;
    char16_t c;
    int ind = 0;
    ifs.read((char*)&c, sizeof(char16_t));
    while (c != -1)
    {
        while (c != -1 && c != u'\n') {
            tmp += c;
            ifs.read((char*)&c, sizeof(char16_t));
        }
        BOOST_REQUIRE(ind < u16len);
        BOOST_CHECK(tmp == u16text[ind++]);
    }
    BOOST_CHECK(ind == u16len);
}

// void readfile()
// {
//     u16file ifs("test.txt", "rb");

//     // string str(10, '\0');
//     // ifs.read((char*)str.c_str(), 10);
//     u16string str16 = ifs.gets();
//     ifs.close();

//     // cout << str[0] << endl;
//     // printf("%d\n", (u_char)str[0]);
//     // printf("%d\n", (u_char)str[1]);

//     wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> converter;
//     string str = converter.to_bytes(str16);
//     cout << str << endl;
// }

// void stdinout()
// {
//     string str;
//     cout << "Enter some text, end with a return: ";
//     str = din.gets('\n');
//     cout << "You have entered: ";
//     dout.puts(str);
// }

// int main(int, char**)
// {
//     stdinout();
//     writefile();
//     readfile();
//     return 0;
// }
