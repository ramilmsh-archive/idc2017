class RFID {
public:
    static int const Rx = 51;
    static int const Tx = 0;
    static int const EPin = 0;

    RFID();
    bool detect();
};
