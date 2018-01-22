#ifndef __SIMPLEOBJECT_H__
#define __SIMPLEOBJECT_H__

class SimpleObject : public GameObject
{
public:
    SimpleObject(GameObjectType::GameObjectType type, vec2 pos, vec2 scale);
    ~SimpleObject();

protected:
    virtual void UpdateCurrent(float deltatime) override;
};


#endif
