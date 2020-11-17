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
#include <time.h>
#include <iostream>
#include <map>
#include <string>
/* ---------------------------------------------------------------------- */
#define STRINGIZER(arg)  #arg
#define STR_VALUE(arg) STRINGIZER(arg)
const char url[] = STR_VALUE(PROTOCOL) "//" STR_VALUE(URL);
const char source_destination[] = STR_VALUE(SOURCE_DESTINATION);
const char credentials[] = STR_VALUE(CREDENTIALS);
const char dir_path[] = STR_VALUE(DIR_PATH);
static bool debug = DEBUG;
static int  pollRate = POLL;
static char payload[1024];
class whatsnew {
 private:
  CURL *sendMail;

 public:
  void send(void);
  size_t static sendInfo(void *whatsGoing, size_t size, size_t blocks, void *myPointer);
  virtual bool task(char * payload, bool firstTime = false) = 0;
  whatsnew();
  ~whatsnew();
};
#endif  // INCLUDE_WHATSNEW_H_
