#pragma once

#define REPLAYER_VERSION_MAJOR 0
#define REPLAYER_VERSION_MINOR 7
#define REPLAYER_VERSION_PATCH 0

#define VER_STRINGIZE(x) #x
#define VER_STR(x) VER_STRINGIZE(x)

#define REPLAYER_VERSION_STRING VER_STR(REPLAYER_VERSION_MAJOR) ", " VER_STR(REPLAYER_VERSION_MINOR) ", " VER_STR(REPLAYER_VERSION_PATCH)

#define REPLAYER_COPYRIGHT "Copyright � 2023 Arnaud N�ny"
