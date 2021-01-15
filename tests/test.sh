#!/usr/bin/env bash


echo "This script opens 2 terminals for 2 server's clients. First you need to login: \"login <username>\". Commands: ping, clients. Timeout 20s"

# to install xfce4-terminal:
#
# sudo apt update
# sudo apt install xfce4-terminal

xfce4-terminal -e "sh testuser1.sh" --window -e "sh testuser2.sh"
