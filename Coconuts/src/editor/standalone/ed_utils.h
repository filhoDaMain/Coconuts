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
#ifndef ED_UTILS_H
#define ED_UTILS_H

#include <glm/glm.hpp>
#include <string>
#include <coconuts/graphics/Texture.h>
#include <coconuts/AssetManager.h>
#include <coconuts/ecs/Serializer.h>

namespace Coconuts{
namespace utils
{
    void DrawTableVec2(const std::string& label, glm::vec2& values, float colWidth = 100.0f);
    void DrawTableFloat(const std::string& label, const std::string& param, float& values, bool positive = false, float colWidth = 100.0f);
    void DrawTableTextButton(const std::string& label, const std::string& text, float colWidth = 100.0f);
    void DrawTableImage(const std::string& label, Texture2D& texture, const glm::vec4& tintColor = glm::vec4(1.0f), uint8_t divider = 2, float colWidth = 100.0f);
    void DrawTableImage(const std::string& label, Texture2D& spriteSheet, AssetManager::SpriteSelector& selector, const glm::vec4& tintColor = glm::vec4(1.0f), uint8_t divider = 2, float colWidth = 100.0f);
    
    class SaveState
    {
    public:
        static bool SaveCCNProjFile(const std::string& filePath);
        static bool StoreCCNProjFilePath(const std::string& path) { s_FilePath = path; return true; }
        static std::string GetCCNProjFilePath() { return s_FilePath; }
        
    private:
        static std::string s_FilePath;
    };
}    
}

#endif /* ED_UTILS_H */

