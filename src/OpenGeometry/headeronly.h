#ifndef HEADERONLY_H
#define HEADERONLY_H

/// Make sure HEADERONLY is either nothing or the keyword "inline"
#ifdef HEADERONLY
    #undef HEADERONLY_INLINE
    #define HEADERONLY_INLINE inline
#else
    #undef HEADERONLY_INLINE
	#define HEADERONLY_INLINE
#endif

#endif
