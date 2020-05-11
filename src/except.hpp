#ifndef __EXCEPT_H__
#define __EXCEPT_H__
#include <exception>
#include <string.h>
#include <iostream>
/********************************************************************
 * Declarations
 * ****************************************************************/

class Throwable : public std::exception
{/*{{{*/
    typedef std::exception super;
protected:
    Throwable(const char *Msg = 0, int Err=0, bool Warn = false) noexcept;
public:
    Throwable(const Throwable &) noexcept;
    virtual ~Throwable() noexcept;
    const char *getMsg() const noexcept { return msg; }
    int getErrno() const noexcept { return err; }
    bool isWarning() const noexcept { return is_warning; }
    friend std::ostream & operator<<(std::ostream &o, const Throwable & t)
    {
	    o << t.getMsg() << "\nErr_no:" <<t.getErrno() ;
	    return o;
    }
private:
    char *msg;
    int err;
protected:
    bool is_warning;                    // can be set by subclasses

private:
    // disable assignment
    Throwable& operator= (const Throwable &);

private:
    static unsigned long counter;       // for debugging
};/*}}}*/

// Exceptions can/should be caught
class Exception : public Throwable
{/*{{{*/
    typedef Throwable super;
public:
    Exception(const char *Msg = 0, int Err = 0, bool Warn = false) noexcept : super(Msg,Err,Warn){}
};/*}}}*/

class Error : public Throwable
{/*{{{*/
    typedef Throwable super;
public:
    Error(const char *m = 0, int e = 0) noexcept : super(m,e) { }
};/*}}}*/

class fileExcept :public Exception
{/*{{{*/
	typedef Exception super;
	public:
	fileExcept(const char * m=0)noexcept : super(m){}
};/*}}}*/





//******************************************************************************
//Impletations******************************************************************
//******************************************************************************
//
//
Throwable::Throwable(const char *m, int e, bool w) noexcept
    : super(), msg(NULL), err(e), is_warning(w)
{
    if (m)
        msg = strdup(m);
}
Throwable::Throwable(const Throwable &other) noexcept
    : super(other), msg(NULL), err(other.err), is_warning(other.is_warning)
{
    if (other.msg)
        msg = strdup(other.msg);
}
Throwable::~Throwable()
{
	free(msg);
}

#include <stdarg.h>
#include <memory>
std::string string_format(const std::string fmt_str, ...) 
{/*{{{*/
    int final_n, n = ((int)fmt_str.size()) * 2; 
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}/*}}}*/
#endif
