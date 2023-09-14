//V1.0 (c) 2023 Juan M. Uribe
#pragma once
#include "ListLib.h"

#define TIMERHANDLER xTaskCreatePinnedToCore(timerHandler, "timerHandler", 1024, NULL, 1, &timerHandlerTask, 1)

enum timerType {TON, TOFF};

struct timer {
  timerType type; //timer type
  bool in; //timer input
  uint32_t pt; //timer preset time
  uint32_t et; //timer elapsed time
  bool q; //timer output
};

class genericTimer {
  private:
    struct timer t;
    uint32_t lastTime;
  public:
    genericTimer(timerType type);
    genericTimer(timerType type, uint32_t preset);
    void updateTimer();
    void preset(uint32_t time);
    bool q();
    uint32_t et();
    void tin(bool timerIn);
    bool tin();
};

genericTimer::genericTimer(timerType type) {
  t.type = type;
  t.pt = 10000;
}

genericTimer::genericTimer(timerType type, uint32_t preset) {
  t.type = type;
  t.pt = preset;
}

void genericTimer::updateTimer() {
  switch (t.type) {
    case TON: {
        if (!t.in)
        {
          t.q = false;
          t.et = t.pt;
        }
        t.q = (t.in && (t.et == 0));
        if (millis() >= lastTime)
        {
          lastTime = millis();
        }
        if (t.in && t.et > 0)
          t.et--;
      }
      break;
    case TOFF: {
        t.q = (!t.in && (t.et > 0));
        if (millis() >= lastTime)
        {
          lastTime = millis();
        }
        if (!t.in && t.et > 0)
          t.et--;
        if (t.in)
        {
          t.q = true;
          t.et = t.pt;
        }
      }
      break;
  }
}

void genericTimer::preset(uint32_t time) {
  t.pt = time;
}

bool genericTimer::q() {
  return t.q;
}

uint32_t genericTimer::et() {
  return t.et;
}

void genericTimer::tin(bool timerIn) {
  t.in = timerIn;
}

bool genericTimer::tin() {
  return t.in;
}

List<genericTimer *> timerList;

class espPlcTimer: public genericTimer {
  private:

  public:
    espPlcTimer(timerType type);
    espPlcTimer(timerType type, uint32_t preset);
};

espPlcTimer::espPlcTimer(timerType type): genericTimer(type) {
  timerList.Add(this);
}

espPlcTimer::espPlcTimer(timerType type, uint32_t preset):
  genericTimer(type, preset) {
  timerList.Add(this);
}


// ESPPlcTimers updaters
TaskHandle_t timerHandlerTask;

void timerHandler(void *pvParameters)
{
  (void)pvParameters;
  for (;;)
  {
    for (int i=0; i<timerList.Count(); i++)
      timerList[i]->updateTimer();
  }
}
