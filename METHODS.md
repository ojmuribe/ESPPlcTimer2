# METHODS

EspPlcTimers2 is based on the EspPlcTimer class which is a derived class of the genericTimer class



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

    class espPlcTimer: public genericTimer {
      private:
    
      public:
        espPlcTimer(timerType type);
        espPlcTimer(timerType type, uint32_t preset);
    };

The timers are declared using the constructors of the espPlcTimer class:

    espPlcTimer t1(TON, 20000); //TON timer, 20s
    espPlcTimer t2(TOFF); // TOFF timer, 10s

This causes an implicit call to the constructors of the genericTimer class

The genericTimer class provides all its public methods.

The update method is used implicitly in the FREE RTOS update function. Therefore, its use is not necessary in the implementation of the program.

## preset

    void preset(uint32_t time);

Load the timer with a new preset time. time is specified in ms.

## q

    bool q();

Returns a boolean that is the current state of the timer output

## et

    uint32_t et();

Returns a 32-bit integer with the current count of the timer. Time is specified in ms. 

The count is done backwards, from the preload value to zero.

## tin

tin is the timer activation input.

tin has a set method and a get method.

### Set method: 

counter activation input.

    void tin(bool timerIn);

Takes a boolean that is used to activate the counter.

### get method: 

    bool tin();

returns a boolean that is the actual state of the counter activation input.

