#ifndef MOTION_PROFILE_H
#define MOTION_PROFILE_H

class MotionProfile {
public:
    MotionProfile(float terminalVx = 15.0f, float terminalVy = 15.0f)
        : terminalVelocityX(terminalVx), terminalVelocityY(terminalVy) {}
    virtual ~MotionProfile() = default;

    float getTerminalVelocityX() const { return terminalVelocityX; }
    float getTerminalVelocityY() const { return terminalVelocityY; }
    void setTerminalVelocityX(float v) { terminalVelocityX = v; }
    void setTerminalVelocityY(float v) { terminalVelocityY = v; }

protected:
    float terminalVelocityX;
    float terminalVelocityY;
};

#endif // MOTION_PROFILE_H
