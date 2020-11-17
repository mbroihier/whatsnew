/*
 *      whatsnewip - mail new IP information
 *
 *      Copyright (C) 2020
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
#include <getopt.h>
#include <unistd.h>
#include "whatsnewConfig.h"
#include "../include/whatsnewip.h"
/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      task.cc -- override base class definition - get this machine's 
 *                 public IP
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
bool whatsnewip::task(char * payload, bool firstTime) {
  char newPayload[1024];
  memset(newPayload, 0, sizeof(newPayload));
  FILE * digCmd = popen("/usr/bin/dig TXT +short o-o.myaddr.l.google.com @ns1.google.com", "r");
  fscanf(digCmd, "%s", newPayload);
  pclose(digCmd);
  int lenO = strlen(payload);
  int lenN = strlen(newPayload);
  if (debug) {
    fprintf(stdout, "Current IP is: %s\n", payload);
    fprintf(stdout, "New IP is: %s\n", newPayload);
    fprintf(stdout, "old length: %d, new length %d\n", lenO, lenN);
  }
  if (((lenO != lenN) && (lenN > 0)) || ((lenN > 0) && strncmp(payload, newPayload, lenO))) {
    memcpy(payload, newPayload, lenN+1);
    return true;
  }
  return false;
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      whatsnewip - constructor
 *
 *      Copyright (C) 2020
 *          Mark Broihier
 *
 */

/* ---------------------------------------------------------------------- */

whatsnewip::whatsnewip() : whatsnew() {
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      whatsnewip - destructor
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */

whatsnewip::~whatsnewip() {
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      getInstance - return singleton instance of whatsnewip
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */

whatsnewip* whatsnewip::getInstance() {
  static whatsnewip instance;
  return &instance;
}
/* ---------------------------------------------------------------------- */

