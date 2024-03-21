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
#include <any>
#include <sstream>
#include <fstream>

#include "platform.h"
#include "log.h"
#include "object.h"
#include "memory/base_ptr.h"
#include "memory/ref_ptr.h"
#include "memory/spy_ptr.h"
#include "interface.h"
#include "interfaces/create.h"
#include "interfaces/type_info.h"
#include "interfaces/singleton.h"
#include "io/Read.h"
#include "io/Write.h"