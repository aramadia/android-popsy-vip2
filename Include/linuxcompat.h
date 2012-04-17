#ifndef _LINUXCOMPAT_H
#define _LINUXCOMPAT_H 1

#include <stdio.h>

typedef const char* LPCSTR;

/* quick and dirty hacked MFC replacement classes;-) */
class CString {
private:
    char* str;

public:
    CString();
    operator char* ();
    void operator= (const char* blah);
    bool operator== (LPCSTR blah);
    void Empty();
};
class CArchive {
private:
    FILE* file;

public:
    CArchive(FILE*);
    void Close();

    bool IsStoring() {
        return false;
    }

    void operator>> (char*& blah);
    void operator<< (char*& blah) {}

    void operator>> (CString& blah);
    void operator<< (CString& blah) {}

    void operator>> (float& blah);
    void operator<< (float& blah) {}

    void operator>> (bool& blah);
    void operator<< (bool& blah) {}

    void operator>> (unsigned char& blah);
    void operator<< (unsigned char& blah) {}

    void operator>> (unsigned short& blah);
    void operator<< (unsigned short& blah) {}

    void operator>> (unsigned int& blah);
    void operator<< (unsigned int& blah) {}

    void WriteString(const char* blah);
    void Read(unsigned char* buf, int num_bytes);
    int ReadString(char* blah, int num_bytes);
};

#endif
