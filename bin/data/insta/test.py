#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Use text editor to edit the script and type in valid Instagram username/password
import sys
import time
print sys.argv[1:]

from InstagramAPI import InstagramAPI

InstagramAPI = InstagramAPI(sys.argv[1], sys.argv[2])
InstagramAPI.login() # login
InstagramAPI.uploadPhoto("C:\\Users\\Quasar\\Documents\\of_v0.9.0_vs_release\\apps\\myApps\\funshot\\bin\\data\\sc1.jpg")
