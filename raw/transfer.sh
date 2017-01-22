#! /bin/bash

#This file should transfer everything new from the MIDAS computer and then unzip it.

rsync -ravpt york@134.158.196.38:/TapeData/201701/* .
gunzip *.gz