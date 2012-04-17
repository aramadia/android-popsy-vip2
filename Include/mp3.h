#ifndef _VORBIS_H
#define _VORBIS_H 1

#include <stdio.h>

#include "mpg123.h"
#include "mpglib.h"

class MP3AudioProvider {
public:
    MP3AudioProvider(const char* filename);
    ~MP3AudioProvider();

    int fill_buf(char* buf, int bytes);

private:
    FILE* fle;

    char buf[16384];
    struct mpstr mp;
    bool data_left;
};

#endif /* !defined(_VORBIS_H) */
