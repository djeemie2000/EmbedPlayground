#pragma once

#include "Ticker.h"

template<class SourceType, class RendererType>
class CRenderManager
{
public:
    CRenderManager()
     : m_Ticker()
     , m_Source(0)
     , m_Renderer(0)
    {}

    void Start(int SamplingFrequency, SourceType* Source, RendererType* Renderer)
    {
        if(Source && Renderer)
        {
            m_Source = Source;
            m_Renderer = Renderer;

            timestamp_t PeriodMicroSeconds = 1000000.0f/SamplingFrequency;
            m_Ticker.attach_us(this, &CRenderManager<SourceType, RendererType>::OnTick, PeriodMicroSeconds);
        }
    }

    void Stop()
    {
        m_Ticker.detach();
    }

    void OnTick()
    {
        m_Renderer->Render(m_Source->Render());
    }

private:
    Ticker m_Ticker;
    SourceType* m_Source;
    RendererType* m_Renderer;
};

