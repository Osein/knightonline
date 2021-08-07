#pragma once

#ifndef PCH_H
#define PCH_H

#define N3FORMAT_VER_UNKN 0x00000000
#define N3FORMAT_VER_1068 0x00000001 // (1<<0)
#define N3FORMAT_VER_1264 0x00000002 // (1<<1)
#define N3FORMAT_VER_1298 0x00000004 // (1<<2)
#define N3FORMAT_VER_2062 0x00000008 // (1<<3)
#define N3FORMAT_VER_CURR 0x40000000 // NOTE: not even going to attempting this right now
#define N3FORMAT_VER_HERO 0x80000000 // NOTE: Hero Online formatting

static const int N3FORMAT_VER_DEFAULT = N3FORMAT_VER_1264;//N3FORMAT_VER_1068;

#include <d3dx9.h>
#include <DxErr.h>
#include <Windows.h>
#include <dinput.h>

#include <iostream>
#include <string>

#endif //PCH_H
