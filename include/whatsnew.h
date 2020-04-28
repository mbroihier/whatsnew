#ifndef INCLUDE_WHATSNEW_H_
#define INCLUDE_WHATSNEW_H_
/*
 *      whatsnew.h - header for What's new object
 *
 *      Copyright (C) 2020
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <iostream>
#include <string>
/* ---------------------------------------------------------------------- */
#define STRINGIZER(arg)  #arg
#define STR_VALUE(arg) STRINGIZER(arg)
const char url[] = STR_VALUE(PROTOCOL) "//" STR_VALUE(URL);
const char source_destination[] = STR_VALUE(SOURCE_DESTINATION);
const char credentials[] = STR_VALUE(CREDENTIALS);
static bool debug = DEBUG;
static int  pollRate = POLL;
class whatsnew {
 private:
  CURL *sendMail;

 public:
  void send(void);
  bool getIP(void);
  whatsnew();
  ~whatsnew();
};
#endif  // INCLUDE_WHATSNEW_H_
