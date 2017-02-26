#include <Stepper.h>

#define STEPS 200
#define SPEED 60
#define STEP_SIZE 5
#define PER_SERVO_STEP 10
#define MAX_STEP 800
#define POT_MAX 4095
#define THRESHOLD 15
#define MAX_ANGLE 25
#define BL 0
#define BR 1
#define FL 2
#define FR 3

int dir = 1;
int pot = A6;
int bl1 = A0;
int bl2 = A1;
int bl3 = A2;
int bl4 = A3;
int br1 = D0;
int br2 = D1;
int br3 = D2;
int br4 = D3;
int fl1 = A4;
int fl2 = A5;
int fl3 = RX;
int fl4 = TX;
int fr1 = D4;
int fr2 = D5;
int fr3 = D6;
int fr4 = D7;
int angle = 0;

Stepper bl(STEPS, bl1, bl2, bl3, bl4);
Stepper br(STEPS, br1, br2, br3, br4);
Stepper fl(STEPS, fl1, fl2, fl3, fl4);
Stepper fr(STEPS, fr1, fr2, fr3, fr4);

// bl, br, fl, fr
int curHeight[] = {0,0,0,0};
int targetHeight[] = {0,0,0,0};
int curPot;

void setup()
{
    pinMode(pot, INPUT);
    bl.setSpeed(SPEED);
    br.setSpeed(SPEED);
    fl.setSpeed(SPEED);
    fr.setSpeed(SPEED);
    Particle.function("suggest",move);
    curPot = (((float) analogRead(pot)) / POT_MAX) * MAX_STEP;
    for (int i = 0; i < 4; i++) {
        targetHeight[i] = curPot;
    }
}


void loop()
{
    int newPot = (analogRead(pot) * MAX_STEP)/POT_MAX;
    int diff = newPot - curPot; 
    if (abs(diff) <THRESHOLD) {
        diff = 0;
    } else {
         curPot = newPot;
    }
    for (int i = 0; i < 4; i++) {
        targetHeight[i] += diff;
    }
    while (true) {
        int changed = 0;
        for (int i = 0; i < 4; i++) {
            if(abs(targetHeight[i] - curHeight[i]) > 5) {
                changed = 1;
                int totalStep = targetHeight[i]-curHeight[i];
                int curStep = abs(totalStep) > PER_SERVO_STEP ? PER_SERVO_STEP : totalStep;
                if (totalStep < 0 && curStep > 0)  curStep *= -1;
                switch(i) {
                    case BL:
                        bl.step(curStep*dir);
                        break;
                    case BR:
                        br.step(curStep*dir);
                        break;
                    case FL:
                        fl.step(curStep*dir);
                        break;
                    case FR:
                        fr.step(curStep*dir);
                        break;
                }
                curHeight[i] += curStep;
            }
        }
        if (changed == 0)  break;
    }
}

int move(String command) {

    // up
    if (command=="1" && angle <= MAX_ANGLE) {
        targetHeight[FL] += STEP_SIZE;
        targetHeight[FR] += STEP_SIZE;
        targetHeight[BL] -= STEP_SIZE;
        targetHeight[BR] -= STEP_SIZE;
        if (targetHeight[FL] > MAX_STEP) {
            targetHeight[FL] = MAX_STEP;
        }
        if (targetHeight[FR] > MAX_STEP) {
            targetHeight[FR] = MAX_STEP;
        }
        if (targetHeight[BL] < 0) {
            targetHeight[BL] = 0;
        }
        if (targetHeight[BR] < 0) {
            targetHeight[BR] = 0;
        }
        angle++;
        return 1;
    }
    //down
    else if (command=="-1" && angle >= -MAX_ANGLE) {
        targetHeight[BL] += STEP_SIZE;
        targetHeight[BR] += STEP_SIZE;
        targetHeight[FL] -= STEP_SIZE;
        targetHeight[FR] -= STEP_SIZE;
        if (targetHeight[BL] > MAX_STEP) {
            targetHeight[BL] = MAX_STEP;
        }
        if (targetHeight[BR] > MAX_STEP) {
            targetHeight[BR] = MAX_STEP;
        }
        if (targetHeight[FL] < 0) {
            targetHeight[FL] = 0;
        }
        if (targetHeight[FR] < 0) {
            targetHeight[FR] = 0;
        }
        angle--;
        return -1;
    }
    //no op
    else {
        return 0;
    }
    
}
