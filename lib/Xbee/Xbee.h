class Xbee {
public:
    static int const Rx = 11;
    static int const Tx = 10;
    static int const EPin = 10;

    Xbee();
    void send(char*);
    void listen(char*);
};
