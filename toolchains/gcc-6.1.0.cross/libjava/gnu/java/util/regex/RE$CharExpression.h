
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __gnu_java_util_regex_RE$CharExpression__
#define __gnu_java_util_regex_RE$CharExpression__

#pragma interface

#include <java/lang/Object.h>
extern "Java"
{
  namespace gnu
  {
    namespace java
    {
      namespace util
      {
        namespace regex
        {
            class RE$CharExpression;
        }
      }
    }
  }
}

class gnu::java::util::regex::RE$CharExpression : public ::java::lang::Object
{

  RE$CharExpression();
public:
  virtual ::java::lang::String * toString();
public: // actually package-private
  RE$CharExpression(::gnu::java::util::regex::RE$CharExpression *);
  jchar __attribute__((aligned(__alignof__( ::java::lang::Object)))) ch;
  ::java::lang::String * expr;
  jint len;
public:
  static ::java::lang::Class class$;
};

#endif // __gnu_java_util_regex_RE$CharExpression__