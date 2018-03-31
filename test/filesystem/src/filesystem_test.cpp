#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <typeinfo>

#include <donny/filesystem.hpp>

using namespace std;
using namespace donny::filesystem;

void writefile()
{
    u16file ofs("test.txt", "wb");
    // locale loc(locale(), new codecvt_utf8_utf16<wchar_t>);
    u16string str = u"豆沙包";
    ofs.puts(str);
    // ofs.write((char*)str.c_str(), str.length() * sizeof(str[0]));
    ofs.close();
}

void readfile()
{
    u16file ifs("test.txt", "rb");

    // string str(10, '\0');
    // ifs.read((char*)str.c_str(), 10);
    u16string str16 = ifs.gets();
    ifs.close();

    // cout << str[0] << endl;
    // printf("%d\n", (u_char)str[0]);
    // printf("%d\n", (u_char)str[1]);

    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> converter;
    string str = converter.to_bytes(str16);
    cout << str << endl;
}

void stdinout()
{
    string str;
    cout << "Enter some text, end with a return: ";
    str = din.gets('\n');
    cout << "You have entered: ";
    dout.puts(str);
}

int main()
{
    stdinout();
    writefile();
    readfile();
    return 0;
}
