#ifndef _LINUX_OSS_H
#define _LINUX_OSS_H 1

#include "mp3.h"

#include <time.h>
#include <sys/time.h>

class OSSAudioDriver {
public:
    OSSAudioDriver(MP3AudioProvider* prv);
    ~OSSAudioDriver();

    bool run();
    float get_time();

private:
    int oss_fd;
    struct timeval eof_time;

    char outbuf[65536];
    int in_outbuf;

    unsigned int last_fill, bytes_played;
    bool eof;

    MP3AudioProvider* prov;

    inline void set_ioctl(int command, int value);
};

#endif /* !defined(_LINUX_OSS_H) */
