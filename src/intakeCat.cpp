#include "vex.h"
#include "robot-config.h"
#include "intakeCat.h"

using namespace vex;

bool autoArming = false;
int lockCount = 0;


void intakeSpin(bool reversed) { // both spin
    intake.spin(directionType::fwd, intakePow * (reversed ? -0.5 : 0.5), percentUnits::pct);
    // intake2.spin(directionType::fwd, intakePow * (reversed ? -1 : 1), percentUnits::pct);
    intakeRoller.spin(directionType::fwd, intakePow * (reversed ? -1 : 1), percentUnits::pct);

}

void intakeSpinPow( double pow){
    bool reversed = false;
    intake.spin(directionType::fwd, intakePow * (reversed ? -pow: pow), percentUnits::pct);
    // intake2.spin(directionType::fwd, intakePow * (reversed ? -1 : 1), percentUnits::pct);
    intakeRoller.spin(directionType::fwd, intakePow * (reversed ? -1 : 1), percentUnits::pct);

}

void rollerSpinOnly(double pow){
    bool reversed = false;
    intakeRoller.spin(directionType::fwd, intakePow * (reversed ? -pow : pow), percentUnits::pct);

}
void lockSpin(double pow){
    lock.spin(directionType::fwd, pow, velocityUnits::pct);

}

void lockStake(double lockDeg, double unlockDeg){ //lock is degree 96
    if (lockCount%2 ==0){
        lock.spinToPosition(lockDeg, degrees,true);
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("locked  ");
    }
    else{
        lock.spinToPosition(unlockDeg, degrees,true);  
        Controller1.Screen.setCursor(1,1);
        Controller1.Screen.print("unlocked");
    }   
    lockCount++;
}
void unlockStake(double deg){ //303 degree unlock?
    lock.spinToPosition(deg, degrees,true);
    // Controller1.Screen.setCursor(1,1);
    // Controller1.Screen.print("unlocked");
}




void intakeStop() {
    intake.stop();
    intake2.stop();
    intakeRoller.stop();
}

void catapultLower() {
    catapultA.spin(directionType::fwd, catPow, percentUnits::pct);
    catapultB.spin(directionType::fwd, catPow, percentUnits::pct);
}

void catapultRaise() {
    catapultA.spin(directionType::fwd, catPow * -1, percentUnits::pct);
    catapultB.spin(directionType::fwd, catPow * -1, percentUnits::pct);
}

void catapultStop() {
    catapultA.stop();
    catapultB.stop();
}

void catapultArm() {
    autoArming = true;
    catapultLower();
    waitUntil(catInPosArmed() || !autoArming);
    catapultStop();
    autoArming = false;
}

void catapultLaunch() {
    catapultLower();
    waitUntil(catapultRot.velocity(velocityUnits::dps) > 2); // needs to be tuned
    catapultStop();
}

bool catInPosArmed() {
    double setPos = 174; //197.57, 197.75

    double currPos = catapultRot.angle(rotationUnits::deg);
    return (currPos <= setPos);
}

/*bool catInPosShoot() {
    double posA1 = 248, posA2 = 255;

    double currPos = catapultRot.angle(rotationUnits::deg);
    //modf(360, &currPos);

    return (currPos > posA1 && currPos < posA2);
}*/

double velo;
double lastVelo;
double catAccel = 0;

/** Updates the accelleration value for the catapult
 * @param time
 * the amount of time elapsed since last update in seconds
 */
void updateCatAccel(double time) {
    velo = catapultRot.velocity(velocityUnits::dps);
    catAccel = (velo - lastVelo) / time; // time in seconds
    lastVelo = velo;
}

double getCatAccel() {
    return catAccel;
}

void stopAutoArming() {
    autoArming = false;
}