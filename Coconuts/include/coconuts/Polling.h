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
#ifndef POLLING_H
#define POLLING_H

namespace Coconuts
{
    class Polling
    {
    public:
        /* Key */
        inline static bool IsKeyPressed(int keyCode)
        {
            return s_Instance->IsKeyPressedImpl(keyCode);
        }
        
        /* Mouse */
        inline static bool IsMouseButtonPressed(int keyCode)
        {
            return s_Instance->IsMouseButtonPressedImpl(keyCode);
        }
        
        inline static double GetMouseX()
        {
            return s_Instance->GetMouseXImpl();
        }
        
        inline static double GetMouseY()
        {
            return s_Instance->GetMouseYImpl();
        }
        
    protected:
        /* Platform dependent implementations */
        virtual bool IsKeyPressedImpl(int keyCode) = 0;
        
        virtual bool IsMouseButtonPressedImpl(int keyCode) = 0;
        virtual double GetMouseXImpl() = 0;
        virtual double GetMouseYImpl() = 0;
        
    private:
        static Polling* s_Instance;
    };
    
}

#endif /* POLLING_H */

