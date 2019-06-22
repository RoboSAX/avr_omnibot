#!/bin/bash

###############################################################################
#                                                                             #
# .robolib.sh                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.2.1                                                              #
# Date   : 09.02.18                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# Source code was taken from:                                                 #
#   https://github.com/RoboSAX/avr_spielfeld                                  #
#                                                                             #
# See also:                                                                   #
#   https://github.com/RoboSAX/avr_omnibot                                    #
###############################################################################



### download
echo ""
echo "### Downloading robolib"
echo ""
if [ -d robolib ]; then
    echo "robolib already downloaded"
else
    git clone https://github.com/RoboAG/avr_robolib robolib
    if [ $? -ne 0 ]; then return -1; exit -1; fi
fi



### install
echo ""
echo "### Configuring robolib"
echo ""
cd robolib
make install_prerequisites
if [ $? -ne 0 ]; then return -1; exit -1; fi



### source
echo ""
echo "### Sourcing robolib"
echo ""
. scripts/bashrc.sh
if [ $? -ne 0 ]; then return -1; exit -1; fi
echo "  ROBOLIB_MAKEFILE = \"$ROBOLIB_MAKEFILE\""


