#ifndef H_PixyNet_Handler_H
#define H_PixyNet_Handler_H

#include "message.hpp"

template <typename T>
class handler {
  public:
    typedef void (T::*msg_handler_t)(const message&) const;

    handler(const T* instance, msg_handler_t method) : __instance(instance), __method(method) {
    }

    void call(const message& msg) {
      (__instance->*__method)(msg);
    }

  private:
    T* __instance;
    msg_handler_t* __method;

};

#endif
