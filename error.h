#ifndef ERROR_H
#define ERROR_H

extern unsigned line;
void error(char const *msg);
void error_str(char const *fmt, char const *str);

#endif
