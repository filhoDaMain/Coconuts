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
#ifndef FONTS_H
#define FONTS_H

#include <string>

/* Fonts directory relative to Coconuts root directory */
const std::string __FONT_DIRECTORY_REL_PATH = "/src/editor/standalone/fonts/";

/* Full path to Fonts directory */
const std::string __FONT_DIRECTORY = COCONUTS_BUILDTREE_ROOTDIR + __FONT_DIRECTORY_REL_PATH;


namespace Coconuts {
namespace Fonts
{
    
    namespace Lato
    {
        const std::string Regular = __FONT_DIRECTORY + "Lato" + "/" + "Lato" + "-Regular.ttf"; 
    }
    
}
}

#endif /* FONTS_H */

