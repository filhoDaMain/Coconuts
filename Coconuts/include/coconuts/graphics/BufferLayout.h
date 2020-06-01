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
#ifndef BUFFERLAYOUT_H
#define BUFFERLAYOUT_H

#include <vector>
#include <string>
#include <stdint.h>
#include <initializer_list>
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    enum class ShaderDataType : uint8_t
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };
    
    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch(type)
        {
            case ShaderDataType::None:      return 0;
            
            case ShaderDataType::Float:     return (sizeof(float));
                
            case ShaderDataType::Float2:    return (2*sizeof(float));
                
            case ShaderDataType::Float3:    return (3*sizeof(float));
                
            case ShaderDataType::Float4:    return (4*sizeof(float));
                
            case ShaderDataType::Mat3:      return (3*3*sizeof(float));
                
            case ShaderDataType::Mat4:      return (4*4*sizeof(float));
                
            case ShaderDataType::Int:       return (sizeof(int));
                
            case ShaderDataType::Int2:      return (2*sizeof(int));
                
            case ShaderDataType::Int3:      return (3*sizeof(int));
                
            case ShaderDataType::Int4:      return (4*sizeof(int));
                
            case ShaderDataType::Bool:      return (sizeof(bool));
        }
        
        LOG_ERROR("Unknown ShaderDataType {}", type);
        return 0;
    }
    
    struct BufferElement
    {
        ShaderDataType type;
        std::string name;
        uint32_t size;
        uint32_t offset;
        bool normalized;
        
        BufferElement() {}
        
        BufferElement(ShaderDataType _type, const std::string& _name, bool _normalzed = false)
            :   type(_type), name(_name), size(ShaderDataTypeSize(_type)), offset(0), normalized(_normalzed)
        { 
        }
        
        uint32_t GetComponentCount() const
        {
            switch(type)
            {       
                case ShaderDataType::None:      return 0;
                
                case ShaderDataType::Float:     return 1;
                
                case ShaderDataType::Float2:    return 2;
                
                case ShaderDataType::Float3:    return 3;
                
                case ShaderDataType::Float4:    return 4;
                
                case ShaderDataType::Mat3:      return (3*3);
                
                case ShaderDataType::Mat4:      return (4*4);
                
                case ShaderDataType::Int:       return 1;
                
                case ShaderDataType::Int2:      return 2;
                
                case ShaderDataType::Int3:      return 3;
                
                case ShaderDataType::Int4:      return 4;
                
                case ShaderDataType::Bool:      return 1;
            }
        
            LOG_ERROR("Unknown ShaderDataType {}", type);
            return 0;
        }
    };
    
    class BufferLayout
    {
    public:
        BufferLayout() {}
        
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            :   m_Elements(elements)
        {
                ComputeOffsetAndStride();
        }
        
        inline uint32_t GetStride() const { return m_Stride; }    
            
        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
        
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
        
    private:
        void ComputeOffsetAndStride()
        {
            uint32_t offset = 0;
            m_Stride = 0;
            
            for (auto& elem : m_Elements)
            {
                elem.offset = offset;
                offset += elem.size;
                m_Stride += elem.size;
            }
        }
        
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };
    
}

#endif /* BUFFERLAYOUT_H */

