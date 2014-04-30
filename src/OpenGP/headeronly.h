#pragma once

/// Make sure HEADERONLY_INLINE is either nothing or the keyword "inline"
#ifndef DISABLE_HEADERONLY
    #undef HEADERONLY_INLINE
    #define HEADERONLY_INLINE inline
#else
    #undef HEADERONLY_INLINE
	#define HEADERONLY_INLINE
#endif