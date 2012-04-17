#include <linuxcompat.h>
#include <string.h>
#include <stdlib.h>
#include <endian.h>

CString::CString() {
    this->str = NULL;
}

CString::operator char* () {
    return this->str;
}

void CString::operator= (const char* blah) {
    free(this->str);
    this->str = strdup(blah);
}

bool CString::operator== (LPCSTR blah) {
    return (strcmp(this->str, blah) == 0);
}

void CString::Empty() {
    *this = "";
}

CArchive::CArchive(FILE* file) {
    this->file = file;
}
void CArchive::Close() {
}

void CArchive::operator>> (char*& blah) {
    char buf[256];
    unsigned char len;
    *this >> len;

    fread(buf, 1, len, file);
    buf[len] = 0;

    blah = strdup(buf);
}

void CArchive::operator>> (CString& blah) {
    char buf[256];
    unsigned char len;

    *this >> len;
    fread(buf, 1, len, file);
    buf[len] = 0;
    blah = buf;
}

void CArchive::operator>> (float& blah) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    fread(&blah, sizeof(float), 1, file);
#else
    unsigned char tmp[4], t;
    fread(tmp, 4, 1, file);
    t = tmp[0];
    tmp[0] = tmp[3];
    tmp[3] = t;
    t = tmp[1];
    tmp[1] = tmp[2];
    tmp[2] = t;
    memcpy(&blah, tmp, 4);
#endif
}

void CArchive::operator>> (unsigned char& blah) {
    fread(&blah, sizeof(unsigned char), 1, file);
}

void CArchive::operator>> (unsigned short& blah) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    fread(&blah, sizeof(unsigned short), 1, file);
#else
    unsigned char tmp[2], t;
    fread(tmp, 2, 1, file);
    t = tmp[0];
    tmp[0] = tmp[1];
    tmp[1] = t;
    memcpy(&blah, tmp, 2);
#endif
}

void CArchive::operator>> (unsigned int& blah) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    fread(&blah, sizeof(unsigned int), 1, file);
#else
    unsigned char tmp[4], t;
    fread(tmp, 4, 1, file);
    t = tmp[0];
    tmp[0] = tmp[3];
    tmp[3] = t;
    t = tmp[1];
    tmp[1] = tmp[2];
    tmp[2] = t;
    memcpy(&blah, tmp, 4);
#endif
}

void CArchive::operator>> (bool& blah) {
    unsigned int k;
    fread(&k, 1, 4, file);
    blah = (k != 0);
}

void CArchive::WriteString(const char* blah) {
}

void CArchive::Read(unsigned char* buf, int num_bytes) {
    fread(buf, 1, num_bytes, file);
}

int CArchive::ReadString(char* buf, int num_bytes) {
    fgets(buf, num_bytes, file);
    return strlen(buf);
}

