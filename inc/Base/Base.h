#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <chrono>
#include <typeinfo>
#include <map>
#include <concepts>
#include <atomic>
#include <set>
#include <sstream>
#include <fstream>

#include "platform.h"
#include "log.h"
#include "memory/object.h"
#include "memory/ref_ptr.h"
#include "memory/spy_ptr.h"
#include "interface.h"
#include "interfaces/create.h"
#include "interfaces/type_info.h"
#include "interfaces/singleton.h"
#include "io/read.h"
#include "io/write.h"