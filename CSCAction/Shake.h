#pragma once
#include "../CommonDefinition.h"

NS_CSC_BEGIN
class Shake : public ActionInterval
{
public:
    Shake();
    
    // Create the action with a time and a strength (same in x and y)
    // @param d duration of shake
    // @param strength distance of shake
    static Shake* create(float d, float strength);
    // Create the action with a time and strengths (different in x and y)
    static Shake* create(float d, float strength_x, float strength_y);
    bool initWithDuration(float d, float strength_x, float strength_y);
    
    // override of virtual function of base class (must override)
    virtual Shake* clone() const override;
    virtual Shake* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    virtual void stop(void) override;
    
protected:
     // Initial position of the shaked node
     float _initial_x, _initial_y;
     // Strength of the action
     float _strength_x, _strength_y;
};
NS_CSC_END