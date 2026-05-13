#pragma once

class DebugWindow {
    bool m_isVisible = true;
protected:
    virtual void OnRender() = 0;
public:
    virtual ~DebugWindow() = default;
    
    inline void SetVisibility(bool visibility) {m_isVisible = visibility;}
    inline bool IsVisible() const {return m_isVisible;}

    void Render() {
        if(m_isVisible) {
            this->OnRender();
        }
    }
};