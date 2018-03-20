#ifndef STRACE_XLAT_H
#define STRACE_XLAT_H

# include <stdint.h>
# include <stddef.h>

struct xlat {
	uint64_t val;
	const char *str;
};

# define XLAT(val)			{ (unsigned)(val), #val }
# define XLAT_PAIR(val, str)		{ (unsigned)(val), str  }
# define XLAT_TYPE(type, val)		{     (type)(val), #val }
# define XLAT_TYPE_PAIR(type, val, str)	{     (type)(val), str  }
# define XLAT_END			{		0, 0    }

int
printxvals(const uint64_t val, const char *dflt, const struct xlat *xlat, ...);
int
sprintxval(char *const buf, const size_t size, const struct xlat *const x,
	   const unsigned int val, const char *const dflt);

#endif /* !STRACE_XLAT_H */
