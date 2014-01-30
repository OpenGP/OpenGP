#ifndef HEADERONLY_H
#define HEADERONLY_H

#ifdef HEADERONLY
    #undef HEADERONLY
    #define HEADERONLY inline
#endif

#endif
