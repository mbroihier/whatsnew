/*
 *      whatsnewfile - what's new in a directory task
 *
 *      Copyright (C) 2020
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
#include <dirent.h>
#include <getopt.h>
#include <unistd.h>
#include "whatsnewConfig.h"
#include "../include/whatsnew.h"
#include "../include/whatsnewfile.h"
/* ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      task.cc -- task to perform - in base, get a new file from the 
 *                 directory being watched
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
bool whatsnewfile::task(char * payload, bool firstTime) {
  char cmd[1024];
  bool foundSomething = false;
  DIR *directory;
  struct dirent * entry;
  time_t  now;
  memset(cmd, 0, sizeof(cmd));
  time(&now);
  if (debug) fprintf(stdout, "Directory path is: %s\n", dir_path);
  if ((directory = opendir(dir_path)) != NULL) {
    while ((entry = readdir(directory)) != NULL) {
      if (debug) fprintf(stdout, "Directory entry: %s \n", entry->d_name);
      std::map<std::string, time_t>::iterator location = files.find(entry->d_name);
      if (location == files.end()) {
        if (!firstTime) {
          snprintf(cmd, sizeof(cmd), "/usr/bin/scp -p %s%s surcam@surCamMaster:/blks/surCam/", dir_path, entry->d_name);
          FILE * scpCmd = popen(cmd, "r");
          if (scpCmd) {
            if (pclose(scpCmd)) {
              fprintf(stdout, "Failure detected when closing copy pipe for: %s\n", entry->d_name);
              // do not record so it will be repeated
            } else {  // file was successfully transferred, make a record of that
              if (debug) fprintf(stdout, "New file being reported: %s \n", entry->d_name);
              files[std::string(entry->d_name)] = now;
            }
            memcpy(payload, entry->d_name, strlen(entry->d_name) + 1);
            if (debug) fprintf(stdout, "payload: %s\n", payload);
          } else {
            fprintf(stdout, "Failure detected when attempting to copy file: %s\n", entry->d_name);
          }
        } else {  // when it is the first time, record all files
          if (debug) fprintf(stdout, "New file being reported: %s \n", entry->d_name);
          files[std::string(entry->d_name)] = now;
          memcpy(payload, entry->d_name, strlen(entry->d_name) + 1);
          if (debug) fprintf(stdout, "payload: %s\n", payload);
        }
        foundSomething = true;
      } else {
        files[std::string(entry->d_name)] = now;
      }
    }
    closedir(directory);
  }
  // The following will remove files from the map that have been deleted and have been gone N polling cycles
  std::map<std::string, time_t>::iterator iterator;
  for (iterator = files.begin() ; iterator != files.end(); iterator++) {
    if (difftime(now, iterator->second) > (pollRate * 3)) {
      files.erase(iterator);
    }
  }
  // remove old files on remote machine
  snprintf(cmd, sizeof(cmd), "/usr/bin/ssh surcam@surCamMaster \"find /blks/surCam/ -type f -mtime +3 | xargs rm \"");
  FILE * delCmd = popen(cmd, "r");
  if (delCmd) {
    if (pclose(delCmd)) {
      fprintf(stdout, "Failure detected when closing delete pipe\n");
    }
  } else {
    fprintf(stdout, "Failure detected when attempting to delete files\n");
  }
  if (debug) {
    for (auto & thing : files) {
      fprintf(stdout, "files content: %s\n", thing.first.c_str());
    }
    fprintf(stdout, "contents of payload: %s\n", payload);
  }
  if (debug) fprintf(stdout, "returning an indicator that %s", foundSomething ?
                     "we found something\n" : "we didn't find anything new\n");
  return foundSomething;
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      whatsnewfile - constructor
 *
 *      Copyright (C) 2020
 *          Mark Broihier
 *
 */

/* ---------------------------------------------------------------------- */

whatsnewfile::whatsnewfile() : whatsnew() {
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      whatsnewfile - destructor
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */

whatsnewfile::~whatsnewfile() {
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      getInstance - return the singleton instance
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */

whatsnewfile * whatsnewfile::getInstance() {
  static whatsnewfile instance;
  return &instance;
}
/* ---------------------------------------------------------------------- */
