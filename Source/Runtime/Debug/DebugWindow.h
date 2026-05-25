#pragma once

class DebugWindow {
    bool m_isVisible = true;
protected:
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
public:
    virtual ~DebugWindow() = default;
    
    inline void SetVisibility(bool visibility) {m_isVisible = visibility;}
    inline bool IsVisible() const {return m_isVisible;}

    void Render() {
        this->OnUpdate();
        if(m_isVisible) {
            this->OnRender();
        }
    }
};