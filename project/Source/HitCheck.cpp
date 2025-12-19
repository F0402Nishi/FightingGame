#include "HitCheck.h"
#include <cmath>
#include <DxLib.h>
#include "Character.h"
#include "Object3D.h"

std::string HitCheck::CheckHitToPart(const Character& target, const VECTOR& attackPos, float attackRadius)
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

        if (distSp <= sumRadiusSp) { return col.partName; }
    }

    return "";
}
