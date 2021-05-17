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
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "tag" << YAML::Value << component.tag;
        }
        out << YAML::EndMap;
    }
    
    static void SerializeComponent(YAML::Emitter& out, OrthoCameraComponent& component)
    {
        out << YAML::Key << "OrthoCameraComponent";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "aspectRatio" << YAML::Value << component.aspectRatio;
            out << YAML::Key << "zoomLevel" << YAML::Value << component.zoomLevel;
            out << YAML::Key << "mooveSpeed" << YAML::Value << component.mooveSpeed;
            out << YAML::Key << "halt" << YAML::LongBool << component.halt;
            out << YAML::Key << "backgroundColor" << YAML::Flow << YAML::BeginSeq;
            {
                out << component.backgroundColor.r;
                out << component.backgroundColor.g;
                out << component.backgroundColor.b;
                out << component.backgroundColor.a;
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;
    }
    
    static void SerializeComponent(YAML::Emitter& out, TransformComponent& component)
    {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "position" << YAML::Flow << YAML::BeginSeq << component.position.x << component.position.y << YAML::EndSeq;
            out << YAML::Key << "size" << YAML::Flow << YAML::BeginSeq << component.size.x << component.size.y << YAML::EndSeq;
            out << YAML::Key << "rotationRadians" << YAML::Value << component.rotationRadians;
        }
        out << YAML::EndMap;
    }
    
    static void SerializeComponent(YAML::Emitter& out, SpriteComponent& component)
    {
        out << YAML::Key << "SpriteComponent";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "spriteLogicalName" << YAML::Value << component.spriteLogicalName;
            out << YAML::Key << "tintColor" << YAML::Flow << YAML::BeginSeq;
            {
                out << component.tintColor.r;
                out << component.tintColor.g;
                out << component.tintColor.b;
                out << component.tintColor.a;
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;
    }
    
    static void SerializeComponent(YAML::Emitter& out, BehaviorComponent& component)
    {
        //TODO
    }
    
    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap;
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
                out << YAML::Key << "Entities List" << YAML::Value << YAML::BeginSeq;
                std::vector<Entity> all = m_Scene->GetAllEntities();
                for (Entity entity : all)
                {
                    SerializeEntity(out, entity);
                }
            }
            out << YAML::EndSeq;
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
        

        
        std::string serialized(out.c_str());
        return serialized;
    }
    
}