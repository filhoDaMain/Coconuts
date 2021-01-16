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
#ifndef STATISTICS_H
#define STATISTICS_H

#include <coconuts/graphics/Renderer2D.h>

namespace Coconuts {
namespace Panels
{
 
    class Statistics
    {
    public:
        Statistics() = default;
        void Draw(Renderer2DStatistics& stats);
    };
    
}
}

#endif /* STATISTICS_H */

