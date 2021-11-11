// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "benchmark/benchmark.h"
#include "timers.h"

#include <cstdlib>

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "check.h"
#include "string_util.h"

namespace benchmark {
namespace internal {
extern std::map<std::string, std::string>* global_context;
}

BenchmarkReporter::BenchmarkReporter()
    : output_stream_(&std::cout), error_stream_(&std::cerr) {}

BenchmarkReporter::~BenchmarkReporter() {}

void BenchmarkReporter::PrintBasicContext(std::ostream *out,
                                          Context const &context) {
  BM_CHECK(out) << "cannot be null";
  auto &Out = *out;

  Out << LocalDateTimeString() << "\n";

  if (context.executable_name)
    Out << "Running " << context.executable_name << "\n";

  if (internal::global_context != nullptr) {
    for (const auto& kv: *internal::global_context) {
      Out << kv.first << ": " << kv.second << "\n";
    }
  }

#ifndef NDEBUG
  Out << "***WARNING*** Library was built as DEBUG. Timings may be "
         "affected.\n";
#endif
}

// No initializer because it's already initialized to NULL.
const char *BenchmarkReporter::Context::executable_name;

BenchmarkReporter::Context::Context()
    : sys_info(SystemInfo::Get()) {}

std::string BenchmarkReporter::Run::benchmark_name() const {
  std::string name = run_name.str();
  if (run_type == RT_Aggregate) {
    name += "_" + aggregate_name;
  }
  return name;
}

double BenchmarkReporter::Run::GetAdjustedRealTime() const {
  double new_time = real_accumulated_time * GetTimeUnitMultiplier(time_unit);
  if (iterations != 0) new_time /= static_cast<double>(iterations);
  return new_time;
}

double BenchmarkReporter::Run::GetAdjustedCPUTime() const {
  double new_time = cpu_accumulated_time * GetTimeUnitMultiplier(time_unit);
  if (iterations != 0) new_time /= static_cast<double>(iterations);
  return new_time;
}

}  // end namespace benchmark
