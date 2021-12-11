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
#ifndef EXPMOVINGAVG_H
#define EXPMOVINGAVG_H

#include <atomic>
#include <limits>
#include <cmath>
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    /* Exponential Moving Average algorithm */
    
    
    // Smoothing factor represented as Nom/Denom
    template <uint32_t Nom, uint32_t Denom>
    class ExpMovingAvg
    {
    public:
        explicit ExpMovingAvg() : m_Average(std::numeric_limits<float>::quiet_NaN())
        {
            if (Nom > Denom)
            {
                LOG_ERROR("Smoothing factor of Exponential Moving Average must be ]0, 1]");
                LOG_ERROR("User specified factor: {} / {}", Nom, Denom);
                
                m_Factor = 0.5f;
                LOG_ERROR("Default factor will be used instead: {}", m_Factor);
            }
            else
            {
                m_Factor = static_cast<float>(Nom/Denom);
            }
            
            m_Complement = static_cast<float>(1.0f - m_Factor);
        }
        
        ~ExpMovingAvg() = default;
        
        void Count(float value)
        {
            // 1st iteration
            if (std::isnan(static_cast<float>(m_Average.load())))
            {
                m_Average = value;
            }
            
            else
            {
                m_Average = m_Factor * value + m_Complement * m_Average;
                
                if (m_Average < 0.001)
                {
                    m_Average = 0.0f;
                }
            }
        }
        
        float GetAverage() const
        {
            return static_cast<float>(m_Average.load());
        }
        
    private:
        std::atomic<float>  m_Average;      // current iteration average value
        float               m_Factor;       // smoothing factor     (alpha)
        float               m_Complement;   // factor's complement  (1-alpha)
    };
    
}

#endif /* EXPMOVINGAVG_H */

