#ifndef __CIRCLECOLLIDER_H__
#define __CIRCLECOLLIDER_H__

class CircleCollider : public Collider
{
public:
    CircleCollider(const GameObject* gObj);
    virtual ~CircleCollider();

    // GETTERS
    float GetSize() const;

private:
    float m_size;
};

#endif