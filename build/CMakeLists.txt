cmake_minimum_required(VERSION 2.8)
PROJECT(libtlv)
SET(SRCS
    ../src/tlv.c
    )
INCLUDE_DIRECTORIES(../inc/)
ADD_LIBRARY(libtlv ${SRCS})
