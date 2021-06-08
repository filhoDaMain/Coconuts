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
    
    namespace {
    namespace Parser
    {
        namespace ROOT
        {
            constexpr auto ROOT_NODE_SCENEMANAGER = "<SceneManager>";
            constexpr auto KEY_SEQ_NODE_SCENESLIST = "Scenes List";
            
            namespace SCENE
            {
                constexpr auto CLASS_NODE_SCENE = "<Scene>";
                constexpr auto KEY_UI32_ID = "ID";
                constexpr auto KEY_STR_NAME = "Name";
                constexpr auto KEY_SEQ_NODE_ENTITIESLIST = "Entities List";

                namespace ENTITY
                {
                    constexpr auto CLASS_NODE_ENTITY = "<Entity>";
                    constexpr auto KEY_UI32_ID = "ID";

                    namespace TAGCOMPONENT
                    {
                        constexpr auto CMP_NODE_TAGCOMPONENT = "TagComponent";
                        constexpr auto KEY_STR_TAG = "tag";
                    }

                    namespace ORTHOCAMERACOMPONENT
                    {
                        constexpr auto CMP_NODE_ORTHOCAMERACOMPONENT = "OrthoCameraComponent";
                        constexpr auto KEY_FLOAT_ASPECTRATIO = "aspectRatio";
                        constexpr auto KEY_FLOAT_ZOOMLEVEL = "zoomLevel";
                        constexpr auto KEY_FLOAT_MOOVESPEED = "mooveSpeed";
                        constexpr auto KEY_BOOL_HALT = "halt";
                        constexpr auto KEY_SEQ_FLOAT4_BGCOLOR = "backgroundColor";
                    }

                    namespace TRANSFORMCOMPONENT
                    {
                        constexpr auto CMP_NODE_TRANSFORMCOMPONENT = "TransformComponent";
                        constexpr auto KEY_SEQ_FLOAT2_POSITION = "position";
                        constexpr auto KEY_SEQ_FLOAT2_SIZE = "size";
                        constexpr auto KEY_FLOAT_ROTATIONRADIANS = "rotationRadians";
                    }

                    namespace SPRITECOMPONENT
                    {
                        constexpr auto CMP_NODE_SPRITECOMPONENT = "SpriteComponent";
                        constexpr auto KEY_STR_SPRITELOGICALNAME = "spriteLogicalName";
                        constexpr auto KEY_SEQ_FLOAT4_TINTCOLOR = "tintColor";
                    }

                    namespace BEHAVIORCOMPONENT
                    {
                        constexpr auto CMP_NODE_BEHAVIORCOMPONENT = "BehaviorComponent";
                        //TODO
                    }

                    namespace EVENTHANDLERCOMPONENT
                    {
                        constexpr auto CMP_NODE_EVENTHANDLERCOMPONENT = "EventHandlerComponent";
                        //TODO
                    }
                }
            }
        }
    }
    } //namespace
    
    Serializer::Serializer(std::shared_ptr<Scene>& scene)
    : m_Scene(scene)
    {
    }
    
    static void SerializeComponent(YAML::Emitter& out, TagComponent& component)
    {
        using namespace Parser::ROOT::SCENE::ENTITY::TAGCOMPONENT;
        
        out << YAML::Key << CMP_NODE_TAGCOMPONENT;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_STR_TAG << YAML::Value << component.tag;
        }
        out << YAML::EndMap;
    }
    
    static void SerializeComponent(YAML::Emitter& out, OrthoCameraComponent& component)
    {
        using namespace Parser::ROOT::SCENE::ENTITY::ORTHOCAMERACOMPONENT;
        
        out << YAML::Key << CMP_NODE_ORTHOCAMERACOMPONENT;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_FLOAT_ASPECTRATIO << YAML::Value << component.aspectRatio;
            out << YAML::Key << KEY_FLOAT_ZOOMLEVEL << YAML::Value << component.zoomLevel;
            out << YAML::Key << KEY_FLOAT_MOOVESPEED << YAML::Value << component.mooveSpeed;
            out << YAML::Key << KEY_BOOL_HALT << YAML::LongBool << component.halt;
            out << YAML::Key << KEY_SEQ_FLOAT4_BGCOLOR << YAML::Flow << YAML::BeginSeq;
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
        using namespace Parser::ROOT::SCENE::ENTITY::TRANSFORMCOMPONENT;
        
        out << YAML::Key << CMP_NODE_TRANSFORMCOMPONENT;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_SEQ_FLOAT2_POSITION << YAML::Flow << YAML::BeginSeq << component.position.x << component.position.y << YAML::EndSeq;
            out << YAML::Key << KEY_SEQ_FLOAT2_SIZE << YAML::Flow << YAML::BeginSeq << component.size.x << component.size.y << YAML::EndSeq;
            out << YAML::Key << KEY_FLOAT_ROTATIONRADIANS << YAML::Value << component.rotationRadians;
        }
        out << YAML::EndMap;
    }
    
    static void SerializeComponent(YAML::Emitter& out, SpriteComponent& component)
    {
        using namespace Parser::ROOT::SCENE::ENTITY::SPRITECOMPONENT;
        
        out << YAML::Key << CMP_NODE_SPRITECOMPONENT;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_STR_SPRITELOGICALNAME << YAML::Value << component.spriteLogicalName;
            out << YAML::Key << KEY_SEQ_FLOAT4_TINTCOLOR << YAML::Flow << YAML::BeginSeq;
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
        using namespace Parser::ROOT::SCENE::ENTITY;
        
        out << YAML::BeginMap;
        out << YAML::Key << CLASS_NODE_ENTITY;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_UI32_ID << YAML::Hex << entity.GetId();
            
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
    
    static void SerializeScene(YAML::Emitter& out, const std::shared_ptr<Scene>& scene)
    {
        using namespace Parser::ROOT::SCENE;
        
        out << YAML::BeginMap;
        out << YAML::Key << CLASS_NODE_SCENE;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_UI32_ID << YAML::Hex << 0x01;
            out << YAML::Key << KEY_STR_NAME << YAML::Value << "Untitled";
            
            //Entities
            out << YAML::Key << KEY_SEQ_NODE_ENTITIESLIST << YAML::Value << YAML::BeginSeq;
            std::vector<Entity> all = scene->GetAllEntities();
            for (Entity entity : all)
            {
                SerializeEntity(out, entity);
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;
        out << YAML::EndMap;
    }
    
    std::string Serializer::Serialize()
    {
        using namespace Parser::ROOT;
        
        YAML::Emitter out;
        
        out << YAML::BeginMap;
        {
            out << YAML::Key << ROOT_NODE_SCENEMANAGER;
            out << YAML::BeginMap;
            {
                //Scene
                using namespace Parser::ROOT::SCENE;
                out << YAML::Key << KEY_SEQ_NODE_SCENESLIST << YAML::Value << YAML::BeginSeq;
                uint16_t k;
                for (k = 0; k < 1 /* //TODO - Hardcoded */; k++)
                {
                    //TODO - Multiple Scene instantiation
                    SerializeScene(out, m_Scene);
                }
                out << YAML::EndSeq;
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;
       
        /* Output */
        std::string serialized(out.c_str());
        return serialized;
    }
    
    
    
    static bool DeserializeComponent(YAML::Node& component_node, TagComponent& component)
    {
        using namespace Parser::ROOT::SCENE::ENTITY::TAGCOMPONENT;
        
        std::string tag = component_node[KEY_STR_TAG].as<std::string>();
        component.tag = tag;
        
        LOG_TRACE("* TagComponent");
        LOG_TRACE("  tag = {}", tag);
        
        return true;
    }
    
    static bool DeserializeComponent(YAML::Node& component_node, OrthoCameraComponent& component)
    {
        using namespace Parser::ROOT::SCENE::ENTITY::ORTHOCAMERACOMPONENT;
        
        float ar = component_node[KEY_FLOAT_ASPECTRATIO].as<float>();
        float zoom = component_node[KEY_FLOAT_ZOOMLEVEL].as<float>();
        float mvspeed = component_node[KEY_FLOAT_MOOVESPEED].as<float>();
        bool halt = component_node[KEY_BOOL_HALT].as<bool>();
        
        auto bgcolor = component_node[KEY_SEQ_FLOAT4_BGCOLOR];
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
        using namespace Parser::ROOT::SCENE::ENTITY::TRANSFORMCOMPONENT;
        
        auto position_node = component_node[KEY_SEQ_FLOAT2_POSITION];
        float pos_x = position_node[0].as<float>();
        float pos_y = position_node[1].as<float>();
        
        auto size_node = component_node[KEY_SEQ_FLOAT2_SIZE];
        float size_x = size_node[0].as<float>();
        float size_y = size_node[1].as<float>();
        
        float rotationRadians = component_node[KEY_FLOAT_ROTATIONRADIANS].as<float>();
        
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
        using namespace Parser::ROOT::SCENE::ENTITY::SPRITECOMPONENT;
        
        std::string spriteLogicalName = component_node[KEY_STR_SPRITELOGICALNAME].as<std::string>();
        
        auto tintcolor_node = component_node[KEY_SEQ_FLOAT4_TINTCOLOR];
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
        using namespace Parser::ROOT::SCENE;
        
        LOG_TRACE("Parsing <Entity> ...");
        
        uint64_t id = entity_node[ENTITY::KEY_UI32_ID].as<uint64_t>(); //not used
        TagComponent tagComponent;  //mandatory
        
        //TagComponent
        auto tag_node = entity_node[ENTITY::TAGCOMPONENT::CMP_NODE_TAGCOMPONENT];
        if (tag_node)
        {
            DeserializeComponent(tag_node, tagComponent);
        }
        
        /* Create Entity in current scene being parsed */
        /* ******************************************* */
        Entity entity(scene.get(), tagComponent.tag);
        /* ------------------------------------------- */
        
        
        //OrthoCameraComponent
        auto orthocamera_node = entity_node[ENTITY::ORTHOCAMERACOMPONENT::CMP_NODE_ORTHOCAMERACOMPONENT];
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
        auto transform_node = entity_node[ENTITY::TRANSFORMCOMPONENT::CMP_NODE_TRANSFORMCOMPONENT];
        if (transform_node)
        {
            TransformComponent transformComponent;
            DeserializeComponent(transform_node, transformComponent);
            entity.AddComponent<TransformComponent>() = transformComponent;
        }
        
        //SpriteComponent
        auto sprite_node = entity_node[ENTITY::SPRITECOMPONENT::CMP_NODE_SPRITECOMPONENT];
        if (sprite_node)
        {
            SpriteComponent spriteComponent;
            DeserializeComponent(sprite_node, spriteComponent);
            entity.AddComponent<SpriteComponent>() = spriteComponent;
        }
        
        //BehaviorComponent
        auto behavior_node = entity_node[ENTITY::BEHAVIORCOMPONENT::CMP_NODE_BEHAVIORCOMPONENT];
        if (behavior_node)
        {
            BehaviorComponent behaviorComponent;
            DeserializeComponent(behavior_node, behaviorComponent); //TODO
#if 0   //TODO
            entity.AddComponent<BehaviorComponent>() = behaviorComponent;
#endif
        }
        
        //EventHandlerComponent
        auto eventhandler_node = entity_node[ENTITY::EVENTHANDLERCOMPONENT::CMP_NODE_EVENTHANDLERCOMPONENT];
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
        using namespace Parser::ROOT::SCENE;
        
        YAML::Node root = YAML::Load(conf);
        
        auto scene_node = root[CLASS_NODE_SCENE];
        if (scene_node)
        {
            uint32_t id = scene_node[KEY_UI32_ID].as<uint32_t>();
            std::string name = scene_node[KEY_STR_NAME].as<std::string>();
            
            LOG_TRACE("Parsing <Scene> ...");
            LOG_TRACE("* ID: {}", id);
            LOG_TRACE("* Name: {}", name);
            
            auto entities_list = scene_node[Parser::ROOT::SCENE::KEY_SEQ_NODE_ENTITIESLIST];
            if (entities_list)
            {
                for (auto entity : entities_list)
                {
                    using namespace Parser::ROOT::SCENE::ENTITY;
                    auto entity_node = entity[CLASS_NODE_ENTITY];
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