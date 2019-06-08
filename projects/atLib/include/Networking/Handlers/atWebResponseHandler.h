#ifndef atWebResponseHandler_h__
#define atWebResponseHandler_h__

class atWebResponseHandler
{
public:
  virtual bool HandleResponse(atWebResponse *pResponse);
};

#endif // atWebResponseHandler_h__
