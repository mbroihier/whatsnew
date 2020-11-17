#ifndef INCLUDE_WHATSNEWFILE_H_
#define INCLUDE_WHATSNEWFILE_H_
/*
 *      whatsnewfile.h - header for What's new object (file)
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
class whatsnewfile : public whatsnew {
  std::map<std::string, time_t> files;
  whatsnewfile();
  ~whatsnewfile();
  whatsnewfile(whatsnewfile const&) = delete;
  whatsnewfile(whatsnewfile const&&) = delete;
  whatsnewfile& operator=(whatsnewfile const&) = delete;
  whatsnewfile& operator=(whatsnewfile const&&) = delete;
 public:
  static whatsnewfile * getInstance();
  bool task(char * payload, bool firstTime = false);
};
#endif  // INCLUDE_WHATSNEWFILE_H_
