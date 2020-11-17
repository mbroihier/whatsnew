#ifndef INCLUDE_WHATSNEWIP_H_
#define INCLUDE_WHATSNEWIP_H_
/*
 *      whatsnewip.h - header for What's new object (ip)
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
#include "../include/whatsnew.h"
/* ---------------------------------------------------------------------- */
class whatsnewip : public whatsnew {
 private:
  whatsnewip();
  ~whatsnewip();
  whatsnewip(whatsnewip const&) = delete;
  whatsnewip(whatsnewip const&&) = delete;
  whatsnewip& operator=(whatsnewip const&) = delete;
  whatsnewip& operator=(whatsnewip const&&) = delete;
 public:
  static whatsnewip * getInstance();
  bool task(char * payload, bool firstTime = false);
};
#endif  // INCLUDE_WHATSNEWIP_H_
