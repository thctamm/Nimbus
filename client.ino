#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 200
#define SPEED 60
#define STEP_SIZE 50
#define MAX_STEP 10000
#define BL 0
#define BR 1
#define FL 2
#define FR 3

int pot = 0;
int bl1 = 
int bl2 =
int bl3 =
int bl4 =
int br1 = 
int br2 =
int br3 =
int br4 =
int fl1 = 
int fl2 =
int fl3 =
int fl4 =
int fr1 = 
int fr2 =
int fr3 =
int fr4 =

Stepper bl(STEPS, bl1, bl2, bl3, bl4);
Stepper br(STEPS, br1, br2, br3, br4);
Stepper fl(STEPS, fl1, fl2, fl3, fl4);
Stepper fr(STEPS, fr1, fr2, fr3, fr4);

// bl, br, fl, fr
int curHeight[] = {0,0,0,0};
int targetHeight[] = {0,0,0,0};

void setup()
{
    
    bl.setSpeed(SPEED);
    br.setSpeed(SPEED);
    fl.setSpeed(SPEED);
    fr.setSpeed(SPEED);
    Particle.function("suggest",move);

}


void loop()
{
    for (int i = 0; i < 4; i++) {
        if(targetHeight[i] != curHeight[i]) {
            switch(i) {
                case BL:
                    bl.steps(targetHeight[i]-curHeight[i])
                    break;
                case BR:
                    br.steps(targetHeight[i]-curHeight[i])
                    break;
                case FL:
                    fl.steps(targetHeight[i]-curHeight[i])
                    break;
                case FR:
                    fr.steps(targetHeight[i]-curHeight[i])
                    break;
            }
        
        }
    }

}

int move(String command) {

    // up
    if (command=="1") {
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
        return 1;
    }
    //down
    else if (command=="-1") {
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
        return -1;
    }
    //no op
    else {
        return 0;
    }
}
