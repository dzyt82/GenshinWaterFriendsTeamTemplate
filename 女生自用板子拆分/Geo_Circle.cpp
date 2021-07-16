#include "Geo_Base.cpp"
#include "Geo_Vector2.cpp"
#include "Geo_Line2.cpp"

namespace Geometry
{
    /* https://www.luogu.com.cn/record/51674409 模板题需要用long double */
    struct Circle
    {
        Vector2 center;
        FLOAT_ radius;
        Circle(Vector2 c, FLOAT_ r) : center(c), radius(r) {}
        Circle(FLOAT_ x, FLOAT_ y, FLOAT_ r) : center(x, y), radius(r) {}
        Circle(Vector2 a, Vector2 b, Vector2 c)
        {
            Vector2 p1 = Vector2::LerpUnclamped(a, b, 0.5);
            Vector2 v1 = b - a;
            swap(v1.x, v1.y);
            v1.x = -v1.x;
            Vector2 p2 = Vector2::LerpUnclamped(b, c, 0.5);
            Vector2 v2 = c - b;
            swap(v2.x, v2.y);
            v2.x = -v2.x;

            center = Line2::Intersect(Line2(p1, v1, false), Line2(p2, v2, false));

            radius = (center - a).magnitude();
        }
        std::pair<Vector2, Vector2> intersect_points(Line2 l)
        {
            FLOAT_ k = l.k();
            // 特判
            if (isnan(k))
            {
                FLOAT_ x = -l.C / l.A;
                FLOAT_ rhs = pow(radius, 2) - pow(x - center.x, 2);
                if (rhs < 0)
                    return make_pair(Vector2(nan(""), nan("")), Vector2(nan(""), nan("")));
                else
                {
                    rhs = sqrt(rhs);
                    return make_pair(Vector2(x, rhs + radius), Vector2(x, -rhs + radius));
                }
            }
            FLOAT_ lb = l.b();
            FLOAT_ a = k * k + 1;
            FLOAT_ b = 2 * k * (lb - center.y) - 2 * center.x;
            FLOAT_ c = pow(lb - center.y, 2) + pow(center.x, 2) - pow(radius, 2);
            FLOAT_ x1, x2;
            std::tie(x1, x2) = solveQuadraticEquation(a, b, c);
            if (isnan(x1))
            {
                return make_pair(Vector2(nan(""), nan("")), Vector2(nan(""), nan("")));
            }
            else
            {
                return make_pair(Vector2(x1, l.y(x1)), Vector2(x2, l.y(x2)));
            }
        }
        std::pair<Vector2, Vector2> intersect_points(Circle cir)
        {
            FLOAT_ dist = (cir.center - center).magnitude();
            if (abs(cir.radius - radius) <= dist and dist <= cir.radius + radius)
            {
                FLOAT_ AA = 2 * (cir.center.x - center.x);
                FLOAT_ BB = 2 * (cir.center.y - center.y);
                FLOAT_ CC = pow(center.x, 2) - pow(cir.center.x, 2) + pow(center.y, 2) - pow(cir.center.y, 2) + pow(cir.radius, 2) - pow(radius, 2);
                Line2 lcl(AA, BB, CC);
                return intersect_points(lcl);
            }
            else // 相离或包含
                return make_pair(Vector2(nan(""), nan("")), Vector2(nan(""), nan("")));
        }

        FLOAT_ area() { return pi * radius * radius; }

        bool is_outside(Vector2 p)
        {
            return (p - center).magnitude() > radius;
        }
        bool is_inside(Vector2 p)
        {
            return intereps((p - center).magnitude() - radius) < 0;
        }
    };


}