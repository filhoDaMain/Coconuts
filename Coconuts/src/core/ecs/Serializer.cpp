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

#include <yaml-cpp/yaml.h>
#include <coconuts/ecs/Serializer.h>
#include <coconuts/ecs/components/OrthoCameraComponent.h>
#include <coconuts/ecs/components/TagComponent.h>
#include <coconuts/ecs/components/TransformComponent.h>
#include <coconuts/ecs/components/SpriteComponent.h>
#include <coconuts/ecs/components/BehaviorComponent.h>

namespace Coconuts
{
    
    Serializer::Serializer(const std::shared_ptr<Scene> scene)
    : m_Scene(scene)
    {
    }
    
    static void SerializeComponent(YAML::Emitter& out, TagComponent& component)
    {
        out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginSeq;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "tag" << YAML::Value << component.tag;
        }
        out << YAML::EndMap;
        out << YAML::EndSeq;
    }
    
    static void SerializeComponent(YAML::Emitter& out, OrthoCameraComponent& component)
    {
        out << YAML::Key << "OrthoCameraComponent" << YAML::Value << YAML::BeginSeq;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "isActive" << YAML::LongBool << true;
        }
        out << YAML::EndMap;
        out << YAML::EndSeq;
    }
    
    static void SerializeComponent(YAML::Emitter& out, TransformComponent& component)
    {
        out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginSeq;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "position" << YAML::BeginSeq << component.position.x << component.position.y << YAML::EndSeq;
        }
        out << YAML::EndMap;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "size" << YAML::BeginSeq << component.size.x << component.size.y << YAML::EndSeq;
        }
        out << YAML::EndMap;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "rotationRadians" << YAML::Value << component.rotationRadians;
        }
        out << YAML::EndMap;
        out << YAML::EndSeq;
    }
    
    static void SerializeComponent(YAML::Emitter& out, SpriteComponent& component)
    {
        out << YAML::Key << "SpriteComponent" << YAML::Value << YAML::BeginSeq;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "spriteLogicalName" << YAML::Value << component.spriteLogicalName;
        }
        out << YAML::EndMap;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "tintColor" << YAML::BeginSeq;
            {
                out << component.tintColor.r;
                out << component.tintColor.g;
                out << component.tintColor.b;
                out << component.tintColor.a;
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;
        out << YAML::EndSeq;
    }
    
    static void SerializeComponent(YAML::Emitter& out, BehaviorComponent& component)
    {
        
    }
    
    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::Key << "<Entity>";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "ID" << YAML::Hex << entity.GetId();
            
            //TagComponent
            if (entity.HasComponent<TagComponent>())
            {
                SerializeComponent(out, entity.GetComponent<TagComponent>());
            }
            
            //OrthoCameraComponent
            if (entity.HasComponent<OrthoCameraComponent>())
            {
                SerializeComponent(out, entity.GetComponent<OrthoCameraComponent>());
            }
            
            //TransformComponent
            if (entity.HasComponent<TransformComponent>())
            {
                SerializeComponent(out, entity.GetComponent<TransformComponent>());
            }
            
            //SpriteComponent
            if (entity.HasComponent<SpriteComponent>())
            {
                SerializeComponent(out, entity.GetComponent<SpriteComponent>());
            }
            
            //BehaviorComponent
            if (entity.HasComponent<BehaviorComponent>())
            {
                SerializeComponent(out, entity.GetComponent<BehaviorComponent>());
            }
        }
        out << YAML::EndMap;
    }
    
    std::string Serializer::Serialize()
    {
        YAML::Emitter out;
        
        out << YAML::BeginMap;
        {
            out << YAML::Key << "<Scene>";
            out << YAML::BeginMap;
            {
                out << YAML::Key << "ID" << YAML::Hex << 0x01;
                out << YAML::Key << "Name" << YAML::Value << "Untitled";
                
                //Entities
                std::vector<Entity> all = m_Scene->GetAllEntities();
                for (Entity entity : all)
                {
                    out << YAML::Newline << YAML::Newline;
                    SerializeEntity(out, entity);
                }
            }
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
        

        
        std::string serialized(out.c_str());
        return serialized;
    }
    
}