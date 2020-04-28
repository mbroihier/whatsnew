# What's New

This repository contains code that implements a program that sends email messages to an IMAP server.  The messages are sent when there is new information.  This repository is a template and the example message used here is an update to the public IP address of the host that this code is running on.  Users of this repository can update the get method (in this case, getIP), to be any kind of update that they want to distribute as email.

The IMAP server and mailbox can be configured in the CMakeLists.txt file.  Do not commit a modified version of this file to your cloned copy of this repository.  It will contain authentication information for accessing an email account.


Installation (assuming a Debian/Raspbian buster or better Linux distribution)

  1)  sudo apt-get update
  2)  sudo apt-get upgrade
  3)  sudo apt-get install git
  4)  sudo apt-get build-essentials
  5)  sudo apt-get cmake
  6)  git clone https://github.com/mbroihier/whatsnew
      - cd whatsnew
      - mkdir build
      - vi CMakeLists.txt and modify parameters discussed above
      - cd build
      - cmake ..
      - make
  7)  if you want whatsnew to run as a service
      - vi whatsnew.service - modify to point to your local paths
      - vi whatsnewWrapper - modify to point to your local paths
  8)  sudo cp -p whatsnew.service /lib/system.d/service/
  9)  sudo systemctl enable whatsnew.service
 10)  sudo systemctl start whatsnew.service

