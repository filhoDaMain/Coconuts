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
#include <coconuts/Logger.h>
#include <coconuts/ECS.h>
#include <coconuts/AssetManager.h>
#include <coconuts/cameras/OrthographicCamera.h>

namespace Coconuts
{
    
    Serializer::Serializer(std::shared_ptr<Scene>& scene)
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
    
    static void SerializeComponent(YAML::Emitter& out, EventHandlerComponent& component)
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
            
            //EventHandlerComponent
            if (entity.HasComponent<EventHandlerComponent>())
            {
                SerializeComponent(out, entity.GetComponent<EventHandlerComponent>());
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
       
        /* Output */
        std::string serialized(out.c_str());
        return serialized;
    }
    
    
    
    static bool DeserializeComponent(YAML::Node& component_node, TagComponent& component)
    {
        std::string tag = component_node["tag"].as<std::string>();
        component.tag = tag;
        
        LOG_TRACE("* TagComponent");
        LOG_TRACE("  tag = {}", tag);
        
        return true;
    }
    
    static bool DeserializeComponent(YAML::Node& component_node, OrthoCameraComponent& component)
    {
        float ar = component_node["aspectRatio"].as<float>();
        float zoom = component_node["zoomLevel"].as<float>();
        float mvspeed = component_node["mooveSpeed"].as<float>();
        bool halt = component_node["halt"].as<bool>();
        
        auto bgcolor = component_node["backgroundColor"];
        float color_r = bgcolor[0].as<float>();
        float color_g = bgcolor[1].as<float>();
        float color_b = bgcolor[2].as<float>();
        float color_a = bgcolor[3].as<float>();
        
        /* Update output */
        component.aspectRatio = ar;
        component.zoomLevel = zoom;
        component.mooveSpeed = mvspeed;
        component.halt = halt;
        component.backgroundColor = {color_r, color_g, color_b, color_a};
        
        /* Reconstruct camera */
        component.camera.~OrthographicCamera();
        new (&(component.camera)) OrthographicCamera(-ar * zoom, ar * zoom, -zoom, zoom);
        
        LOG_TRACE("* OrthoCameraComponent");
        LOG_TRACE("  aspectRatio = {}", ar);
        LOG_TRACE("  zoomLevel = {}", zoom);
        LOG_TRACE("  mooveSpeed = {}", mvspeed);
        LOG_TRACE("  halt = {}", halt);
        LOG_TRACE("  backgroundColor = [ {}, {}, {}, {} ]", color_r, color_g, color_b, color_a);
        
        return true;
    }
    
    static bool DeserializeComponent(YAML::Node& component_node, TransformComponent& component)
    {
        auto position_node = component_node["position"];
        float pos_x = position_node[0].as<float>();
        float pos_y = position_node[1].as<float>();
        
        auto size_node = component_node["size"];
        float size_x = size_node[0].as<float>();
        float size_y = size_node[1].as<float>();
        
        float rotationRadians = component_node["rotationRadians"].as<float>();
        
        /* Update output */
        component.position.x = pos_x;
        component.position.y = pos_y;
        component.size.x = size_x;
        component.size.y = size_y;
        component.rotationRadians = rotationRadians;
        
        LOG_TRACE("* TransformComponent");
        LOG_TRACE("  position = [ {}, {} ]", pos_x, pos_y);
        LOG_TRACE("  size = [ {}, {} ]", size_x, size_y);
        LOG_TRACE("  rotationRadians = {}", rotationRadians);
        
        return true;
    }
    
    static bool DeserializeComponent(YAML::Node& component_node, SpriteComponent& component)
    {
        std::string spriteLogicalName = component_node["spriteLogicalName"].as<std::string>();
        
        auto tintcolor_node = component_node["tintColor"];
        float color_r = tintcolor_node[0].as<float>();
        float color_g = tintcolor_node[1].as<float>();
        float color_b = tintcolor_node[2].as<float>();
        float color_a = tintcolor_node[3].as<float>();
        
        /* Update output */
        component.spriteLogicalName = spriteLogicalName;
        component.tintColor = {color_r, color_g, color_b, color_a};
        component.tilingFactor = 1.0f;  //default
        component.sprite = AssetManager::GetSprite(spriteLogicalName);
        
        LOG_TRACE("* SpriteComponent");
        LOG_TRACE("  spriteLogicalName = {}", spriteLogicalName);
        LOG_TRACE("  tintColor = [ {}, {}, {}, {} ]", color_r, color_g, color_b, color_a);
        LOG_TRACE("  tilingFactor = {} hardcoded", 1.0f);
        
        return true;
    }
    
    static bool DeserializeComponent(YAML::Node& component_node, BehaviorComponent& component)
    {
        //TODO
        
        LOG_TRACE("* BehaviorComponent");
        LOG_TRACE("  //TODO  (detected but not deserialized)");
        
        return true;
    }
    
    static bool DeserializeComponent(YAML::Node& component_node, EventHandlerComponent& component)
    {
        //TODO
        
        LOG_TRACE("* EventHandlerComponent");
        LOG_TRACE("  //TODO  (detected but not deserialized)");
        
        return true;
    }
    
    static void DeserializeEntity(YAML::Node& entity_node, std::shared_ptr<Scene>& scene)
    {
        LOG_TRACE("Parsing <Entity> ...");
        
        uint64_t id = entity_node["ID"].as<uint64_t>(); //not used
        TagComponent tagComponent;  //mandatory
        
        //TagComponent
        auto tag_node = entity_node["TagComponent"];
        if (tag_node)
        {
            DeserializeComponent(tag_node, tagComponent);
        }
        
        /* Create Entity in current scene being parsed */
        /* ******************************************* */
        Entity entity(scene.get(), tagComponent.tag);
        /* ------------------------------------------- */
        
        
        //OrthoCameraComponent
        auto orthocamera_node = entity_node["OrthoCameraComponent"];
        if (orthocamera_node)
        {
            OrthoCameraComponent orthoCameraComponent;
            DeserializeComponent(orthocamera_node, orthoCameraComponent);
            
            /* First, delete this scene's default Camera if any was created */
            scene->DeleteDefaultSceneCamera();
            
            /* Add newly created Camera */
            entity.AddComponent<OrthoCameraComponent>() = orthoCameraComponent;
            
            /* Add the default Camera Nav System and Event Handler */
            //TODO Add these through (de)serialization of the respective components
            entity.AddComponent<BehaviorComponent>().AddBehavior<CameraNavSystem>(entity);
            entity.AddComponent<EventHandlerComponent>().AddHandler<CameraEventHandler>(entity);
        }
        
        //TransformComponent
        auto transform_node = entity_node["TransformComponent"];
        if (transform_node)
        {
            TransformComponent transformComponent;
            DeserializeComponent(transform_node, transformComponent);
            entity.AddComponent<TransformComponent>() = transformComponent;
        }
        
        //SpriteComponent
        auto sprite_node = entity_node["SpriteComponent"];
        if (sprite_node)
        {
            SpriteComponent spriteComponent;
            DeserializeComponent(sprite_node, spriteComponent);
            entity.AddComponent<SpriteComponent>() = spriteComponent;
        }
        
        //BehaviorComponent
        auto behavior_node = entity_node["BehaviorComponent"];
        if (behavior_node)
        {
            BehaviorComponent behaviorComponent;
            DeserializeComponent(behavior_node, behaviorComponent); //TODO
#if 0   //TODO
            entity.AddComponent<BehaviorComponent>() = behaviorComponent;
#endif
        }
        
        //EventHandlerComponent
        auto eventhandler_node = entity_node["EventHandlerComponent"];
        if (eventhandler_node)
        {
            EventHandlerComponent eventHandlerComponent;
            DeserializeComponent(eventhandler_node, eventHandlerComponent); //TODO
#if 0   //TODO
            entity.AddComponent<EventHandlerComponent>() = eventHandlerComponent;
#endif
        }
    }
    
    bool Serializer::Deserialize(std::string& conf)
    {
        YAML::Node root = YAML::Load(conf);
        
        auto scene_node = root["<Scene>"];
        if (scene_node)
        {
            uint32_t id = scene_node["ID"].as<uint32_t>();
            std::string name = scene_node["Name"].as<std::string>();
            
            LOG_TRACE("Parsing <Scene> ...");
            LOG_TRACE("* ID: {}", id);
            LOG_TRACE("* Name: {}", name);
            
            auto entities_list = scene_node["Entities List"];
            if (entities_list)
            {
                for (auto entity : entities_list)
                {
                    auto entity_node = entity["<Entity>"];
                    if (entity_node)
                    {
                        DeserializeEntity(entity_node, m_Scene);
                    }
                }
            }
        }
        
        return true;
    }
    
}