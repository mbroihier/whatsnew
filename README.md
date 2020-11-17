# What's New

This repository contains code that implements a program that sends email messages to an IMAP server.  The messages are sent to notify a user that there is something new.  This repository has two example messages that can be sent. The default message is to send the public IP address of the host whenever it changes or once a day.  The other message is a file name, sent whenever a new file appears in a monitored directory.  This second example will also copy the file to a remote server and remove files from that server that have been there longer than a configured period.

Users of this repository can derive other classes from the whatsnew class to perform the task they are interested in.  The method "task" must be defined by the derived class, a getInstance method must be defined, the constructor and destructor need to be private, and the copy/assign constructors must be deleted.  The derived class is a singleton.  If the users of this repository create a derived class using one of the two examples, the only required change to the whatsnew.cc source is modifying the creation of the instance object to match the class written.

The IMAP server and mailbox can be configured in the CMakeLists.txt file.  Do not commit a modified version of this file to your cloned copy of this repository.  It will contain authentication information for accessing an email account.


Installation (assuming a Debian/Raspbian buster or better Linux distribution)

  1)  sudo apt-get update
  2)  sudo apt-get upgrade
  3)  sudo apt-get install git
  4)  sudo apt-get install build-essential
  5)  sudo apt-get install cmake
  6)  sudo apt-get install libcurl4-gnutls-dev
  7)  sudo apt-get install dnsutils
  8)  git clone https://github.com/mbroihier/whatsnew
      - cd whatsnew
      - mkdir build
      - vi CMakeLists.txt and modify parameters discussed above
      - cd build
      - cmake ..
      - make
  9)  if you want whatsnew to run as a service
      - vi whatsnew.service - modify to point to your local paths
      - vi whatsnewWrapper - modify to point to your local paths
 10)  sudo cp -p whatsnew.service /lib/systemd/system/
 11)  sudo systemctl enable whatsnew.service
 12)  sudo systemctl start whatsnew.service

