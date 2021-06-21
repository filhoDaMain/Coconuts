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
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

namespace Coconuts
{
    
    class FileSystem
    {
    public:
        virtual static std::string GetCurrDirPath() = 0;
        virtual static std::string GetRuntimeConfDirPath() = 0;
    };
    
}

#endif /* FILESYSTEM_H */

