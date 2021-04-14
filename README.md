# cpp_helloevent
A program in c++ that starts a thread "Boss" that will give orders "events" to three other threads "Workers". The "Worker" threads will wait for Boss orders, by using a condition variable or a queue, and perform task when it arrives.

Worker uses a condition variable
WorkerWithQueue uses a condition variable and a queue
WorkerWithHelpQueue uses an external queue (This is probably the easiest to use)

### Create build directory:
mkdir build

### Compile this with:
g++ helloevent.cpp -o ./build/helloevent

### Execute this with:
./build/helloevent

