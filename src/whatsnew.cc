/*
 *      whatsnew - mail new information
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
#include "whatsnew.h"
/* ---------------------------------------------------------------------- */


static const char USAGE_STR[] = "\n"
        "Usage: %s \n"
        "  -h                       : help - no parameters - just does it\n"
        "  --help                   : help - no parameters - just does it\n";

static struct option longOpts[] = {
  {"help", no_argument, NULL, 1},
  { NULL, 0, NULL, 0 }
};

static char payload[1024];
static int lineNumber;

static const char * message[] = {
                           "Subject: Whats new\r\n",
                           "\r\n",
                           "payload",
                           0 };

/* ---------------------------------------------------------------------- */
/*
 *      sendInfo.c -- callback that uploads an email message to a SMTP
 *                    server
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */


size_t static sendInfo(void *whatsGoing,
                          size_t size, size_t blocks, void *myPointer) {
  if (debug) fprintf(stdout, "Entering upload callback\n");
  int line = *(reinterpret_cast<int *>(myPointer));
  if (debug) fprintf(stdout, "Working on line %d\n", line);
  const char * workingPointer = message[line++];
  *(reinterpret_cast<int *>(myPointer)) = line;
  size_t completeReturnBufferSize = size * blocks;
  size_t len = 0;
  if (workingPointer) {
    len = strlen(workingPointer);
    if (debug)
      fprintf(stdout,
            " About to copy whats going out into storage buffer - message size: %lu, buffer size: %lu\n",
            len, completeReturnBufferSize);
    if (len > 0) {
      if (strncmp(workingPointer, "payload", sizeof("payload")) == 0) {
        if (debug) fprintf(stdout, "copy payload to whatsGoing out (ie, the new information: %s\n", payload);
        memcpy(whatsGoing, payload, strlen(payload)+1);
        len = strlen(payload);  // change returned length to that of the payload
      } else {
        if (len < completeReturnBufferSize) {
          memcpy(whatsGoing, workingPointer, len);
        } else {
          len = 0;
        }
      }
    }
    if (debug) fprintf(stdout, "Sending:\n %s\n", whatsGoing);
  } else {
    if (debug) fprintf(stdout, "End of message\n");
  }
  return len;
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      getNewFile.cc -- Get a new file from the directory being watched
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */

/* ---------------------------------------------------------------------- */
bool whatsnew::getNewFile(bool firstTime) {
  char cmd[1024];
  bool foundSomething = false;
  DIR *directory;
  struct dirent * entry;
  time_t  now;
  memset(cmd, 0, sizeof(cmd));
  time(&now);
  if ((directory = opendir(DIR_PATH)) != NULL) {
    while ((entry = readdir(directory)) != NULL) {
      if (debug) fprintf(stdout, "Directory entry: %s \n", entry->d_name);
      std::map<std::string, time_t>::iterator location = files.find(entry->d_name);
      if (location == files.end()) {
        if (! firstTime) {
          sprintf(cmd, "/usr/bin/scp -p %s%s surcam@surCamMaster:/blks/surCam/", DIR_PATH, entry->d_name);
          FILE * scpCmd = popen(cmd, "r");
          if (scpCmd) {
            if (pclose(scpCmd)) {
              fprintf(stdout, "Failure detected when closing copy pipe for: %s\n", entry->d_name);
            } else { // file was successfully transferred, make a record of that
              if (debug) fprintf(stdout, "New file being reported: %s \n", entry->d_name);
              files[std::string(entry->d_name)] = now;
              memcpy(payload, entry->d_name, strlen(entry->d_name) + 1);
              if (debug) fprintf(stdout, "payload: %s\n", payload);
            }
          } else {
            fprintf(stdout, "Failure detected when attempting to copy file: %s\n", entry->d_name);
          }
        } else { // when it is the first time, record all files
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
  sprintf(cmd, "/usr/bin/ssh surcam@surCamMaster \"find /blks/surCam/ -type f -mtime +3 | xargs rm \"");
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
  if (debug) fprintf(stdout, "returning an indicator that %s", foundSomething ? "we found something\n" : "we didn't find anything new\n");
  return foundSomething;
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      send.cc -- execute the commands to send an email
 *                    message
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */

/* ---------------------------------------------------------------------- */
void whatsnew::send() {
  lineNumber = 0;
  sendMail = curl_easy_init();
  curl_easy_setopt(sendMail, CURLOPT_URL, url);
  curl_easy_setopt(sendMail, CURLOPT_USERPWD,  credentials);
  curl_easy_setopt(sendMail, CURLOPT_USE_SSL, CURLUSESSL_ALL);
  curl_easy_setopt(sendMail, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
  curl_easy_setopt(sendMail, CURLOPT_MAIL_FROM, source_destination);
  struct curl_slist *list;
  list = curl_slist_append(NULL, source_destination);
  curl_easy_setopt(sendMail, CURLOPT_MAIL_RCPT, list);
  curl_easy_setopt(sendMail, CURLOPT_READFUNCTION, sendInfo);
  curl_easy_setopt(sendMail, CURLOPT_READDATA, & lineNumber);
  curl_easy_setopt(sendMail, CURLOPT_UPLOAD, 1L);
  if (debug) {
    fprintf(stdout, "current content of source_destination: %s\n", source_destination);
    fprintf(stdout, "current content of credentials: %s\n", credentials);
    curl_easy_setopt(sendMail, CURLOPT_VERBOSE, 1L);
  }
  CURLcode result = curl_easy_perform(sendMail);
  if (result) {
    fprintf(stderr, "Curlcode: %d\n", result);
  }
  curl_slist_free_all(list);
  curl_easy_cleanup(sendMail);
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      whatsnew - constructor
 *
 *      Copyright (C) 2020
 *          Mark Broihier
 *
 */

/* ---------------------------------------------------------------------- */

whatsnew::whatsnew() {
  if (debug) fprintf(stdout, "%s %s %s \n", url, source_destination, credentials);
  curl_version_info_data * versionData;
  versionData = curl_version_info(CURLVERSION_NOW);
  if (versionData->version_num < 0x074000) {  // this code wants 7.64.0 or greater
    fprintf(stderr, "This curl version is not new enough - detected version is: %8.8x\n", versionData->version_num);
    exit(1);
  }
  if (debug) {
    fprintf(stdout, "This curl version is: %8.8x\n", versionData->version_num);
  }
}
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/*
 *      whatsnew - destructor
 *
 *      Copyright (C) 2020 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */

whatsnew::~whatsnew() {
}
/* ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
  lineNumber = 0;
  whatsnew whatsnewInstance;
  int c;

  memset(payload, 0, sizeof(payload));
  memcpy(payload, "test", sizeof("test"));

  fprintf(stdout, "Whats New VERSION %s\n", STR_VALUE(whatsnew_VERSION_NUMBER));
  if (argc > 1) {
    fprintf(stderr, USAGE_STR, argv[0]);
    return -2;
  }

  int doneProcessing = 0;
  while ((c = getopt_long(argc, argv, "h", longOpts, NULL)) >= 0) {
    switch (c) {
      case 'h': {
        fprintf(stderr, USAGE_STR, argv[0]);
        return -2;
      }
      default:
        break;
    }
  }

  if (whatsnewInstance.getNewFile(true)) {
    whatsnewInstance.send();
  }
  fprintf(stdout, "Entering main processing loop\n");
  while (!doneProcessing) {
    sleep(pollRate);
    if (whatsnewInstance.getNewFile()) {
      whatsnewInstance.send();
    }
    fprintf(stdout, "Cycle completed - status: %d\n", doneProcessing);
  }

  return 0;
}
/* ---------------------------------------------------------------------- */
