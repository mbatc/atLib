#ifndef atHTTPProtocol_h__
#define atHTTPProtocol_h__

#include "atString.h"

enum atHTTPMethod
{
  atHTTP_Get,
  atHTTP_Post,
  atHTTP_Put,
  atHTTP_Head,
  atHTTP_Options,
  atHTTP_Delete,
  atHTTP_Trace,
  atHTTP_Patch,
  atHTTPMethod_Count,
  atHTTPMethod_Unknown
};

static atVector<atString> atHTTPMethodNames =
{
  "GET",
  "POST",
  "PUT",
  "HEAD",
  "OPTIONS",
  "DELETE",
  "TRACE",
  "PATCH"
};

enum atHTTPProtocol
{
  atHTTP_Ver_1_1,
  atHTTPProtocol_Count,
  atHTTPProtocol_Unknown
};

static atVector<atString> atHTTPProtocolNames =
{ 
  "HTTP/1.1"
};

enum atHTTPResponseCode
{
  atHTTP_100,
  atHTTP_101,

  atHTTP_200,
  atHTTP_201,
  atHTTP_202,
  atHTTP_203,
  atHTTP_204,
  atHTTP_205,
  atHTTP_206,

  atHTTP_300,
  atHTTP_301,
  atHTTP_302,
  atHTTP_303,
  atHTTP_304,
  atHTTP_305,
  atHTTP_307,

  atHTTP_400,
  atHTTP_401,
  atHTTP_402,
  atHTTP_403,
  atHTTP_404,
  atHTTP_405,
  atHTTP_406,
  atHTTP_407,
  atHTTP_408,
  atHTTP_409,
  atHTTP_410,
  atHTTP_411,
  atHTTP_412,
  atHTTP_413,
  atHTTP_414,
  atHTTP_415,
  atHTTP_416,
  atHTTP_417,

  atHTTP_500,
  atHTTP_501,
  atHTTP_502,
  atHTTP_503,
  atHTTP_505,
  atHTTPResponseCode_Count,
  atHTTPResponseCode_Unknown
};

static atVector<int64_t> atHTTPResponseCodeValue =
{
  100,
  101,

  200,
  201,
  202,
  203,
  204,
  205,
  206,
  300,
  301,
  302,
  303,
  304,
  305,
  307,

  400,
  401,
  402,
  403,
  404,
  405,
  406,
  407,
  408,
  409,
  410,
  411,
  412,
  413,
  414,
  415,
  416,
  417,

  500,
  501,
  502,
  503,
  505,
};

static atVector<atString> atHTTPResponseCodeMessage =
{
  "Continue",                         // 100
  "Switching Protocols",              // 101

  "OK",                               // 200
  "Created",                          // 201
  "Accepted",                         // 202
  "Non-Authoritative Information",    // 203
  "No Content",                       // 204
  "Reset Content",                    // 205
  "Partial Content",                  // 206

  "Multiple Choices",                 // 300
  "Moved Permanently",                // 301
  "Found"                             // 302
  "See Other",                        // 303
  "Not Modified",                     // 304
  "Use Proxy",                        // 305
  "Temporary Redirect",               // 307

  "Bad Request",                      // 400
  "Unauthorized",                     // 401
  "Payment Required",                 // 402
  "Forbidden",                        // 403
  "Not Found",                        // 404
  "Method Not Allowed",               // 405
  "Not Acceptable",                   // 406
  "Proxy Authentication Required",    // 407
  "Request Time-out",                 // 408
  "Conflict",                         // 409
  "Gone",                             // 410
  "Length Required",                  // 411
  "Precondition Failed",              // 412
  "Request Entity Too Large",         // 413
  "Request-URI Too Large",            // 414
  "Unsupported Media Type",           // 415
  "Requested range not satisfiable",  // 416
  "Expectation Failed",               // 417

  "Internal Server Error",            // 500
  "Not Implemented",                  // 501
  "Bad Gateway",                      // 502
  "Service Unavailable",              // 503
  "Gateway Time-out",                 // 504
  "HTTP Version not supported",       // 505
};

atString atToString(const atHTTPResponseCode &code);
atString atToString(const atHTTPProtocol &protocol);
atString atToString(const atHTTPMethod &method);

template<> atHTTPMethod atFromString<atHTTPMethod>(const atString &method);
template<> atHTTPProtocol atFromString<atHTTPProtocol>(const atString &protocol);
template<> atHTTPResponseCode atFromString<atHTTPResponseCode>(const atString &code);

#endif // atHTTPProtocol_h__
