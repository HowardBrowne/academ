#pragma once

namespace ofxEdsdk {
    class RateTimer {
    protected:
        float lastTick, averagePeriod, smoothing;
        bool secondTick;
    public:
        RateTimer() :
        smoothing(.9) {
            reset();
        }
        void reset() {
            lastTick = 0, averagePeriod = 0, secondTick = false;
        }
        void setSmoothing(float smoothing) {
            this->smoothing = smoothing;
        }
        float getFrameRate() {
            return averagePeriod == 0 ? 0 : 1 / averagePeriod;
        }
        void tick() {
            float curTick = ofGetElapsedTimef();
            if(lastTick == 0) {
                secondTick = true;
            } else {
                float curDiff = curTick - lastTick;;
                if(secondTick) {
                    averagePeriod = curDiff;
                    secondTick = false;
                } else {
                    averagePeriod = ofLerp(curDiff, averagePeriod, smoothing);
                }
            }
            lastTick = curTick;
        }
    };
}