#pragma once

#define FOR_EACH_BEGIN(__CONTAINER__)                                          \
  for (auto it = __CONTAINER__.Begin(); it != __CONTAINER__.End(); ++it) {

#define FOR_EACH_END }

#define RFOR_EACH_BEGIN(__CONTAINER__)                                         \
  for (auto it = __CONTAINER__.RBegin(); it != __CONTAINER__.REnd(); ++it) {

#define RFOR_EACH_END }