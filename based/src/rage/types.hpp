#pragma once
#include <cstdint>
#include <minwindef.h>
#include <utility>
#include "vector.hpp"

using Void = void;
using Any = int;
using Hash = std::uint32_t;
using Entity = std::int32_t;
using Player = std::int32_t;
using FireId = std::int32_t;
using Interior = std::int32_t;
using Ped = Entity;
using Vehicle = Entity;
using Cam = std::int32_t;
using Object = Entity;
using Pickup = Object;
using Blip = std::int32_t;
using Camera = Entity;
using ScrHandle = Entity;
using Vector3 = rage::scrVector;
