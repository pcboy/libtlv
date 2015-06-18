/*
 *          DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                  Version 2, December 2004
 *
 *  Copyright (C) 2004 Sam Hocevar
 *  14 rue de Plaisance, 75014 Paris, France
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 *
 *  David Hagege <david.hagege@gmail.com>
 */

#include <alloca.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <tlv.h>
#include <tlv/misc.h>

#define INT32(a,b,c,d) ((uint32_t)((0xFF&a) \
      | ((0xFF&b)<<8) \
      | ((0xFF&c)<<16) \
      | ((0xFF&d)<<24)))

void tlv_free(tlv *p)
{
  free(p->val);
  free(p);
  return;
}

tlv *tlv_copy(tlv *src)
{
  tlv *res = calloc(sizeof(tlv), 1);
  res->val = calloc(sizeof(char), src->len);
  memcpy(res->val, src->val, src->len);
  res->len = src->len;
  res->typ = src->typ;

  return res;
}

uint32_t tlv_getType(char *data, size_t buflen)
{
  return buflen > sizeof(tlv) ?
    ntohl(INT32(data[0], data[1], data[2], data[3])) : 0;
}

uint32_t tlv_getLen(char *data, size_t buflen)
{
  return buflen > sizeof(tlv) ?
    ntohl(INT32(data[4], data[5], data[6], data[7])) : 0;
}

tlv *tlv_getNext(char **data, size_t *buflen)
{
  tlv *res;

  if (*buflen < sizeof(tlv) ||
      !ntohl(INT32((*data)[0], (*data)[1], (*data)[2], (*data)[3]))) {
    return NULL;
  }
  if ((res = tlv_get(*data, *buflen))) {
    *data += MIN(res->len + sizeof(uint32_t)*2, *buflen);
    *buflen -= MIN(res->len + sizeof(uint32_t)*2, *buflen);
  }
  return res;
}

tlv *tlv_get(char *data, size_t buflen)
{
  if (buflen < sizeof(tlv) ||
      !ntohl(INT32(data[0], data[1], data[2], data[3]))) {
    fprintf(stderr, "!typ or abnormal packet\n");
    return NULL;
  }

  tlv *res = calloc(sizeof(struct tlv), 1);
  res->val = calloc(sizeof(char),
      MIN(buflen, ntohl(INT32(data[4], data[5], data[6], data[7]))));

  memcpy(res, data, sizeof(uint32_t)*2);
  res->typ = ntohl(res->typ);
  res->len = ntohl(res->len);
  memcpy(res->val, data+sizeof(uint32_t)*2, MIN(res->len, buflen));
  return res;
}

ssize_t tlv_send(size_t sock, uint32_t typ, uint32_t len, char *val)
{
  char *data;
  tlv msg;
  ssize_t res;

  data = (len > 8192 ? malloc(sizeof(uint32_t)*2+sizeof(char)*len)
      : alloca(sizeof(uint32_t)*2+sizeof(char)*len));
  msg.typ = htonl(typ);
  msg.len = htonl(len);
  memcpy(data, &msg, sizeof(uint32_t)*2);
  memcpy(data+sizeof(uint32_t)*2, val, len);
  res = send(sock, data, sizeof(uint32_t) * 2 + len, 0);
  if (len > 8192)
    free(data);
  return res;
}
