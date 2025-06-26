#include "HitCheck.h"
#include <cmath>
#include <DxLib.h>

std::string HitCheck::CheckHitToPart(const Player& target, const VECTOR& attackPos, float attackRadius)
{
    const std::vector<SphereCollder>& spheres = target.hitSpheres;
    VECTOR targetPos = target.GetTransform().position;

    for (const SphereCollder& col : spheres) 
    {
        VECTOR worldCenter = col.GetWorldCenter(targetPos);
        VECTOR diff = attackPos - worldCenter;

        float distSp = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
        float sumRadius = attackRadius + col.radius;
        float sumRadiusSp = sumRadius * sumRadius;

        VECTOR top = VAdd(worldCenter, VGet(0, 0, 0));
        VECTOR down = VAdd(worldCenter, VGet(0, -0.1f, 0));
        DrawSphere3D(top, 20.0f, 20, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);

        if (distSp <= sumRadiusSp) { return col.partName; }
    }

    return "";
}
