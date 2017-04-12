/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "arrow/storage.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <arrow/utf8.h>

#include "StorageEEPROM.h"

int check_mgc() {
  int *c = flash_read();
//  if ( *c != FLASH_MAGIC_NUMBER ) {
//    return 0;
//  }
  return 1;
}

int restore_gateway_info(arrow_gateway_t *gateway) {
  printf("restore gateway info\r\n");
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_read();
    if ( utf8check(mem->gateway_hid) && strlen(mem->gateway_hid) > 0 ) {
      arrow_gateway_add_hid(gateway, mem->gateway_hid);
      return 0;
    }
  }
  return -1;
}

void save_gateway_info(const arrow_gateway_t *gateway) {
  printf("new registration\r\n");
  flash_mem_t mem;
  memcpy(&mem, flash_read(), sizeof(flash_mem_t));
  strcpy(mem.gateway_hid, gateway->hid);
  flash_write(&mem, sizeof(flash_mem_t));
}

int restore_device_info(arrow_device_t *device) {
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_read();
    if ( !utf8check(mem->device_hid) || strlen(mem->device_hid) == 0 ) {
      return -1;
    }
    arrow_device_set_hid(device, mem->device_hid);
  #if defined(__IBM__)
    if ( !utf8check(mem->device_eid) || strlen(mem->device_eid) == 0 ) {
      return -1;
    }
    arrow_device_set_eid(device, mem->device_eid);
  #endif
    return 0;
  }
  return -1;
}

void save_device_info(arrow_device_t *device) {
  flash_mem_t mem;
  memcpy(&mem, flash_read(), sizeof(flash_mem_t));
  strcpy(mem.device_hid, device->hid);
#if defined(__IBM__)
  strcpy(mem.device_eid, device->eid);
#endif
  flash_write(&mem, sizeof(flash_mem_t));
}

void save_wifi_setting(const char *ssid, const char *pass, int sec) {
  flash_mem_t mem;
  memcpy(&mem, flash_read(), sizeof(flash_mem_t));
  strcpy(mem.ssid, ssid);
  strcpy(mem.pass, pass);
  mem.sec = sec;
  flash_write(&mem, sizeof(flash_mem_t));
}

int restore_wifi_setting(char *ssid, char *pass, int *sec) {
  if (check_mgc()) {
    flash_mem_t *mem = (flash_mem_t *)flash_read();
    strcpy(ssid, mem->ssid);
    strcpy(pass, mem->pass);
    *sec = mem->sec;
    return 0;
  }
  return -1;
}
