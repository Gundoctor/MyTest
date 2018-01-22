#ifndef __COLLISIONEVENT_H__
#define __COLLISIONEVENT_H__

class GameObject;

class CollisionEvent : public Event
{
public:
    CollisionEvent(const GameObject* object1, const GameObject* object2);
    virtual ~CollisionEvent();

    // Getters
    const GameObject* GetObject1() const;
    const GameObject* GetObject2() const;

private:
    const GameObject* m_object1;
    const GameObject* m_object2;

};

#endif