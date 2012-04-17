#include <stdio.h>
#include <stdlib.h>

#include "mp3.h"

MP3AudioProvider::MP3AudioProvider(const char* filename) {
    this->fle = fopen(filename, "r");
    if (fle == NULL) {
        perror(filename);
        exit(-1);
    }

    InitMP3(&mp);
    data_left = false;
}

MP3AudioProvider::~MP3AudioProvider() {
    fclose(fle);
}

int MP3AudioProvider::fill_buf(char* buf, int bytes) {
    int size, ret;

    int to_read = (bytes > 8192) ? 8192 : bytes;

    if (data_left) {
        ret = decodeMP3(&mp, NULL, 0, buf, to_read, &size);
        if (ret == MP3_NEED_MORE)
            data_left = false;
    }
    if (!data_left) {
        int len = fread(buf, 1, 16384, fle);
        if (len < 0) {
            perror("fread");
            exit(-1);
        }
        if (len == 0) {
            memset(buf, 0, bytes);
            return bytes;
        }
        ret = decodeMP3(&mp, buf, len, buf, to_read, &size);
    }

    if (ret == MP3_OK) {
        data_left = true;
    }

    return size;
}

