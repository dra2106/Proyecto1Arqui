#ifndef COLLISION_ARM_H
#define COLLISION_ARM_H

extern "C" {
    bool check_collision_arm(int y1, int x1, int h1, int w1,
                            int y2, int x2, int h2, int w2);
}

#endif // COLLISION_ARM_H
