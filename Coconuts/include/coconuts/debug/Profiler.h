/*
 * Copyright 2021 Andre Temprilho
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef PROFILER_H
#define PROFILER_H

#include <coconuts/debug/TimeProfiler.h>

// Debugging Enabled
#ifdef COCONUTS_DEBUG_TIMEPROFILER
#define __CCNCORE_PROFILER_LINE__(name, file, func, line) Coconuts::Profiler::InstrumentationTimer timer##line(name, file, func, line);
#define __CCNCORE_PROFILER_SCOPE__(name) __CCNCORE_PROFILER_LINE__(name, __FILE__, __func__, __LINE__)
// Uses function name as scope name
#define __CCNCORE_PROFILER_FUNC__ __CCNCORE_PROFILER_SCOPE__(__func__)

// Debugging Disabled
#else
#define __CCNCORE_PROFILER_LINE__(name, file, func, line)
#define __CCNCORE_PROFILER_SCOPE__(name)
#define __CCNCORE_PROFILER_FUNC__

#endif  // COCONUTS_DEBUG_TIMEPROFILER

#endif /* PROFILER_H */

