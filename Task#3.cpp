#include <iostream>
using namespace std;

struct PrintRequest
{
    int id;
    int arrivalTime;
    int processingTime;
};
class CircularQueue
{
private:
    PrintRequest *data;
    int capacity;
    int frontIndex;
    int count;
public:
    CircularQueue(int size = 2)
    {
        capacity = size;
        frontIndex = 0;
        count = 0;
        data = new PrintRequest[capacity];
    }
    ~CircularQueue()
    {
        delete[] data;
    }
    bool isEmpty()
    {
        return count == 0;
    }
    bool isFull()
    {
        return count == capacity;
    }
    PrintRequest front()
    {
        PrintRequest empty = {-1, 0, 0};
        if (isEmpty())
            return empty;

        return data[frontIndex];
    }
    void resize(int new_capacity)
    {
        if (new_capacity < 2)
            new_capacity = 2;

        if (new_capacity < count)
            new_capacity = count;

        PrintRequest *newData = new PrintRequest[new_capacity];

        for (int i = 0; i < count; i++)
            newData[i] = data[(frontIndex + i) % capacity];

        delete[] data;
        data = newData;
        capacity = new_capacity;
        frontIndex = 0;
        cout << "Capacity changed to " << capacity << endl;
    }
    void enqueue(PrintRequest x)
    {
        if (isFull())
            resize(capacity * 2);

        int rear = (frontIndex + count) % capacity;
        data[rear] = x;
        count++;
        cout << "Added request " << x.id << endl;
    }
    PrintRequest dequeue()
    {
        PrintRequest empty = {-1, 0, 0};

        if (isEmpty())
        {
            cout << "Queue is empty." << endl;
            return empty;
        }
        PrintRequest removed = data[frontIndex];

        frontIndex = (frontIndex + 1) % capacity;
        count--;
        if (capacity > 2 && count <= capacity / 4)
            resize(capacity / 2);
        return removed;
    }
    void displayQueue()
    {
        cout << "Queue: ";

        for (int i = 0; i < count; i++)
        {
            int index = (frontIndex + i) % capacity;
            cout << data[index].id << " ";
        }
        cout << endl;
    }
};
int main()
{
    CircularQueue q;
    int n;
    cout << "How many print requests? ";
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        PrintRequest r;
        cout << "Enter request ID, arrival time, processing time: ";
        cin >> r.id >> r.arrivalTime >> r.processingTime;

        q.enqueue(r);
        q.displayQueue();
    }
    int currentTime = 0;
    int totalWaitingTime = 0;
    int processed = 0;

    while (!q.isEmpty())
    {
        PrintRequest next = q.front();
        cout << "\nNext request: " << next.id << endl;
        PrintRequest r = q.dequeue();

        cout << "Processed request: " << r.id << endl;
        q.displayQueue();
        // Wait until the request has arrived
        if (currentTime < r.arrivalTime)
            currentTime = r.arrivalTime;

        int waitingTime = currentTime - r.arrivalTime;
        totalWaitingTime += waitingTime;
        cout << "Waiting time: " << waitingTime << endl;

        currentTime += r.processingTime;
        processed++;
    }
    if (processed > 0)
    {
        cout << "\nAverage waiting time: "
             << (double)totalWaitingTime / processed << endl;
    }
    return 0;
}