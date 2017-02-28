#include "AFMotor.h"

float MachineWidthMM = 724;
float MachineHeightMM = 800;

float lastX = MachineWidthMM / 2;
float lastY = 150;

float L1 = 381;
float L2 = 381;

float StepsPerRev = 800.00;

float MMPerRev = 84;
float MMPerStep = MMPerRev / StepsPerRev;

float BandLengthMM = 20;

AF_Stepper motora(StepsPerRev, 1);
AF_Stepper motorb(StepsPerRev, 2);

const int stepType = INTERLEAVE;

void setup()
{
  Serial.begin(57600);           // set up Serial library at 9600 bps
  Serial.println("Rob’s Straight Line - Send '1' and press enter in Terminal");
}

void loop() {
  while (Serial.available() > 0) {
    char ch = Serial.read();
    if(ch == '1') {
      DrawLine(500, 500);
      DrawLine(200, 500);
      DrawLine(350, 150);
    }
  }
}

void DrawLine(int destX, int destY)
{
  float targetX = destX;
  float targetY = destY;

  float h = lastX;
  float k = lastY;
  float r = targetX-lastX;
  float s = targetY-lastY;
  float w = MachineWidthMM;

  float lineLen = sqrt( (targetX-lastX)*(targetX-lastX) + (targetY-lastY)*(targetY-lastY) );

  int numBands = int(lineLen / BandLengthMM);
  for (int bandIdx = 0; bandIdx < numBands; bandIdx++)
  {
    float tAtBandEnd = (bandIdx + 1) * 1.0 / numBands;
    float L1AtBandEnd = ComputeL1(h, r, k, s, tAtBandEnd);
    float L2AtBandEnd = ComputeL2(L1AtBandEnd, h, r, tAtBandEnd, w);

    float L1Dist = L1AtBandEnd - L1;
    float L2Dist = L2AtBandEnd - L2;

    long L1Accum = 0;
    long L2Accum = 0;

    long L1Steps = round(abs(L1Dist) / MMPerStep);
    long L2Steps = round(abs(L2Dist) / MMPerStep);
    long numSteps = max(L1Steps, L2Steps);

    for (int stepIdx = 0; stepIdx < numSteps; stepIdx++)
    {
      L1Accum += L1Steps;
      if (L1Accum > numSteps)
      {
        L1Accum -= numSteps;
        L1 += L1Dist > 0 ? MMPerStep : -MMPerStep;
        if (L1Dist > 0)
          motora.onestep(FORWARD, stepType);
        else
          motora.onestep(BACKWARD, stepType);
      }

      L2Accum += L2Steps;
      if (L2Accum > numSteps)
      {
        L2Accum -= numSteps;
        L2 += L2Dist > 0 ? MMPerStep : -MMPerStep;
        if (L2Dist > 0)
          motorb.onestep(FORWARD, stepType);
        else
          motorb.onestep(BACKWARD, stepType);
      }
      delay(1);
    }
    lastX = targetX;
    lastY = targetY;
  }
}

float ComputeL1(float h, float r, float k, float s, float t)
{
  float L1Sq = h*h + k*k + 2*h*r*t + 2*k*s*t + (r*r + s*s)*t*t;
  return sqrt(L1Sq);
}

float ComputeL2(float L1, float h, float r, float t, float w)
{
  float L2Sq = L1*L1 - 2*h*w - 2*r*t*w + w*w;
  return sqrt(L2Sq);
}
