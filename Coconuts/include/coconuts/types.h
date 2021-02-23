/*
 * Copyright 2020 Andre Temprilho
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
#ifndef TYPES_H
#define TYPES_H

#include <functional>

#define BIT_MASK(x) (1 << (x))

#define BIND_EVENT_FUNCTION(fn)     std::bind(&fn, this, std::placeholders::_1)

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

#endif /* TYPES_H */

