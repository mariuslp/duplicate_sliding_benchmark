#pragma once

#include <memory>
#include <vector>
#include <random>
#include <functional>
#include <chrono>

#include "logger.h"
#include "stream.h"
#include "statistics.h"

#include "filter_asymptotic.h"
#include "filter_a2.h"
#include "filter_stablebloom.h"
#include "filter_bdb.h"
#include "filter_cuckoo.h"
#include "filter_sq.h"
#include "filter_qht.h"
#include "filter_qht_compact.h"
#include "filter_qhtd_compact.h"
#include "filter_qqhtd_compact.h"
#include "filter_queue.h"

/** Runs tests */
void run_tests();
